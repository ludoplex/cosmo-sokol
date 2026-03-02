/*
 * objc_runtime.c - Objective-C runtime wrapper for Cosmopolitan
 *
 * Implements objc_runtime.h using cosmo_dlopen to load libobjc.dylib
 * and provide pure C access to Objective-C runtime functions.
 *
 * This is the SAME pattern as shims/linux/x11.c - dlopen + function pointers.
 */

#include "objc_runtime.h"
#include <cosmo.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

/* ── Global Function Pointers ──────────────────────────────────────────────── */

objc_msgSend_t       objc_msgSend_fn = NULL;
objc_msgSend_fpret_t objc_msgSend_fpret_fn = NULL;
objc_msgSend_stret_t objc_msgSend_stret_fn = NULL;
objc_getClass_t      objc_getClass_fn = NULL;
objc_allocateClassPair_t objc_allocateClassPair_fn = NULL;
objc_registerClassPair_t objc_registerClassPair_fn = NULL;
sel_registerName_t   sel_registerName_fn = NULL;
sel_getName_t        sel_getName_fn = NULL;
class_addMethod_t    class_addMethod_fn = NULL;
class_addIvar_t      class_addIvar_fn = NULL;
class_replaceMethod_t class_replaceMethod_fn = NULL;
objc_getAssociatedObject_t objc_getAssociatedObject_fn = NULL;
objc_setAssociatedObject_t objc_setAssociatedObject_fn = NULL;

/* ── Static State ──────────────────────────────────────────────────────────── */

static void* libobjc = NULL;
static void* foundation = NULL;
static bool initialized = false;

/* ── Helper to load a symbol ───────────────────────────────────────────────── */

#define LOAD_SYM(lib, name) \
    do { \
        name##_fn = (name##_t)cosmo_dltramp(cosmo_dlsym(lib, #name)); \
        if (!name##_fn) { \
            fprintf(stderr, "objc_runtime: failed to load %s\n", #name); \
            return -1; \
        } \
    } while (0)

/* ── Initialization ────────────────────────────────────────────────────────── */

int objc_init(void) {
    if (initialized) {
        return 0;
    }

    if (!IsXnu()) {
        /* Not on macOS - this is expected, not an error */
        return -1;
    }

    /* Load libobjc.dylib */
    libobjc = cosmo_dlopen("/usr/lib/libobjc.dylib", RTLD_NOW | RTLD_GLOBAL);
    if (!libobjc) {
        fprintf(stderr, "objc_runtime: failed to load /usr/lib/libobjc.dylib\n");
        return -1;
    }

    /* Load core runtime functions */
    LOAD_SYM(libobjc, objc_msgSend);
    LOAD_SYM(libobjc, objc_getClass);
    LOAD_SYM(libobjc, sel_registerName);
    LOAD_SYM(libobjc, sel_getName);
    LOAD_SYM(libobjc, objc_allocateClassPair);
    LOAD_SYM(libobjc, objc_registerClassPair);
    LOAD_SYM(libobjc, class_addMethod);
    LOAD_SYM(libobjc, class_addIvar);
    LOAD_SYM(libobjc, class_replaceMethod);
    LOAD_SYM(libobjc, objc_getAssociatedObject);
    LOAD_SYM(libobjc, objc_setAssociatedObject);

    /* objc_msgSend_fpret is the same as objc_msgSend on arm64, different on x86_64 */
    objc_msgSend_fpret_fn = (objc_msgSend_fpret_t)cosmo_dltramp(
        cosmo_dlsym(libobjc, "objc_msgSend_fpret"));
    if (!objc_msgSend_fpret_fn) {
        /* Fallback to regular objc_msgSend for arm64 */
        objc_msgSend_fpret_fn = (objc_msgSend_fpret_t)objc_msgSend_fn;
    }

    /* objc_msgSend_stret is only needed on x86_64 for struct returns */
    objc_msgSend_stret_fn = (objc_msgSend_stret_t)cosmo_dltramp(
        cosmo_dlsym(libobjc, "objc_msgSend_stret"));
    /* Note: arm64 doesn't use stret, but we keep the pointer for compatibility */

    initialized = true;
    return 0;
}

bool objc_is_initialized(void) {
    return initialized;
}

/* ── NSString Helpers ──────────────────────────────────────────────────────── */

id objc_nsstring(const char *cstr) {
    if (!cstr) return NULL;
    Class NSString_class = objc_getClass_fn("NSString");
    if (!NSString_class) return NULL;

    /* [NSString stringWithUTF8String:cstr] */
    SEL sel = sel_registerName_fn("stringWithUTF8String:");
    return objc_msgSend_fn((id)NSString_class, sel, cstr);
}

const char* objc_cstring(id nsstring) {
    if (!nsstring) return NULL;
    SEL sel = sel_registerName_fn("UTF8String");
    return (const char*)objc_msgSend_fn(nsstring, sel);
}

/* ── Class Registration ────────────────────────────────────────────────────── */

Class objc_create_class(const char *name, const char *superclass_name) {
    if (!initialized) return NULL;

    Class superclass = objc_getClass_fn(superclass_name);
    if (!superclass) {
        fprintf(stderr, "objc_runtime: superclass '%s' not found\n", superclass_name);
        return NULL;
    }

    Class cls = objc_allocateClassPair_fn(superclass, name, 0);
    if (!cls) {
        fprintf(stderr, "objc_runtime: failed to allocate class '%s'\n", name);
        return NULL;
    }

    objc_registerClassPair_fn(cls);
    return cls;
}

bool objc_add_method(Class cls, const char *sel_name, void *imp, const char *types) {
    if (!initialized || !cls || !sel_name || !imp) return false;

    SEL sel = sel_registerName_fn(sel_name);
    return class_addMethod_fn(cls, sel, (IMP)imp, types);
}

/* ── Framework Loading ─────────────────────────────────────────────────────── */

void* objc_load_framework(const char *name) {
    if (!name) return NULL;

    char path[512];

    /* Try system framework path first */
    snprintf(path, sizeof(path),
             "/System/Library/Frameworks/%s.framework/%s", name, name);
    void *handle = cosmo_dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if (handle) return handle;

    /* Try user framework path */
    snprintf(path, sizeof(path),
             "/Library/Frameworks/%s.framework/%s", name, name);
    handle = cosmo_dlopen(path, RTLD_NOW | RTLD_GLOBAL);
    if (handle) return handle;

    fprintf(stderr, "objc_runtime: failed to load framework '%s'\n", name);
    return NULL;
}
