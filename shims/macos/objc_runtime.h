/*
 * objc_runtime.h - Objective-C runtime wrapper for Cosmopolitan
 *
 * Provides pure C access to Objective-C runtime via dlopen/dlsym.
 * This allows cosmocc-compiled code to call Cocoa/Metal on macOS.
 *
 * Usage:
 *   if (objc_init() < 0) { error; }
 *   id app = objc_msg(objc_class("NSApplication"), "sharedApplication");
 *   objc_msg(app, "run");
 */

#ifndef OBJC_RUNTIME_H
#define OBJC_RUNTIME_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ── Objective-C Types ─────────────────────────────────────────────────────── */

typedef void* id;           /* Object pointer */
typedef void* Class;        /* Class pointer */
typedef void* SEL;          /* Selector (method name) */
typedef void* IMP;          /* Implementation pointer */
typedef signed char BOOL;   /* Objective-C BOOL (signed char) */

/* Common Cocoa types (opaque pointers) */
typedef id NSApplication;
typedef id NSWindow;
typedef id NSView;
typedef id NSOpenGLContext;
typedef id NSOpenGLView;
typedef id NSOpenGLPixelFormat;
typedef id NSAutoreleasePool;
typedef id NSString;
typedef id NSArray;
typedef id NSDictionary;
typedef id NSEvent;
typedef id NSMenu;
typedef id NSMenuItem;
typedef id NSNotification;
typedef id NSBundle;
typedef id NSScreen;
typedef id NSCursor;
typedef id NSTrackingArea;
typedef id NSImage;
typedef id MTLDevice;
typedef id CAMetalLayer;

/* Cocoa geometry types */
typedef struct { double x, y; } CGPoint;
typedef struct { double width, height; } CGSize;
typedef struct { CGPoint origin; CGSize size; } CGRect;
typedef struct { double x, y; } NSPoint;
typedef struct { double width, height; } NSSize;
typedef struct { NSPoint origin; NSSize size; } NSRect;
typedef uint64_t NSUInteger;
typedef int64_t NSInteger;

/* NSWindow style masks */
enum {
    NSWindowStyleMaskBorderless     = 0,
    NSWindowStyleMaskTitled         = 1 << 0,
    NSWindowStyleMaskClosable       = 1 << 1,
    NSWindowStyleMaskMiniaturizable = 1 << 2,
    NSWindowStyleMaskResizable      = 1 << 3,
    NSWindowStyleMaskFullScreen     = 1 << 14,
};

/* NSOpenGLPixelFormat attributes */
enum {
    NSOpenGLPFADoubleBuffer       = 5,
    NSOpenGLPFAColorSize          = 8,
    NSOpenGLPFAAlphaSize          = 11,
    NSOpenGLPFADepthSize          = 12,
    NSOpenGLPFAStencilSize        = 13,
    NSOpenGLPFAAccumSize          = 14,
    NSOpenGLPFAMinimumPolicy      = 51,
    NSOpenGLPFASampleBuffers      = 55,
    NSOpenGLPFASamples            = 56,
    NSOpenGLPFAOpenGLProfile      = 99,
    NSOpenGLProfileVersionLegacy  = 0x1000,
    NSOpenGLProfileVersion3_2Core = 0x3200,
    NSOpenGLProfileVersion4_1Core = 0x4100,
};

/* NSBackingStoreType */
enum {
    NSBackingStoreBuffered = 2,
};

/* NSEvent types */
enum {
    NSEventTypeLeftMouseDown      = 1,
    NSEventTypeLeftMouseUp        = 2,
    NSEventTypeRightMouseDown     = 3,
    NSEventTypeRightMouseUp       = 4,
    NSEventTypeMouseMoved         = 5,
    NSEventTypeLeftMouseDragged   = 6,
    NSEventTypeRightMouseDragged  = 7,
    NSEventTypeMouseEntered       = 8,
    NSEventTypeMouseExited        = 9,
    NSEventTypeKeyDown            = 10,
    NSEventTypeKeyUp              = 11,
    NSEventTypeFlagsChanged       = 12,
    NSEventTypeScrollWheel        = 22,
    NSEventTypeOtherMouseDown     = 25,
    NSEventTypeOtherMouseUp       = 26,
    NSEventTypeOtherMouseDragged  = 27,
};

/* NSEventModifierFlags */
enum {
    NSEventModifierFlagCapsLock   = 1 << 16,
    NSEventModifierFlagShift      = 1 << 17,
    NSEventModifierFlagControl    = 1 << 18,
    NSEventModifierFlagOption     = 1 << 19,
    NSEventModifierFlagCommand    = 1 << 20,
};

/* ── Function Pointer Types ────────────────────────────────────────────────── */

/* objc_msgSend variants - cast to appropriate function pointer for call */
typedef id  (*objc_msgSend_t)(id self, SEL op, ...);
typedef void (*objc_msgSend_void_t)(id self, SEL op, ...);
typedef double (*objc_msgSend_fpret_t)(id self, SEL op, ...);
typedef void (*objc_msgSend_stret_t)(void *stretAddr, id self, SEL op, ...);

/* Runtime functions */
typedef Class (*objc_getClass_t)(const char *name);
typedef Class (*objc_allocateClassPair_t)(Class superclass, const char *name, size_t extraBytes);
typedef void (*objc_registerClassPair_t)(Class cls);
typedef SEL (*sel_registerName_t)(const char *str);
typedef const char* (*sel_getName_t)(SEL sel);
typedef bool (*class_addMethod_t)(Class cls, SEL name, IMP imp, const char *types);
typedef bool (*class_addIvar_t)(Class cls, const char *name, size_t size, uint8_t alignment, const char *types);
typedef IMP (*class_replaceMethod_t)(Class cls, SEL name, IMP imp, const char *types);
typedef id (*objc_getAssociatedObject_t)(id object, const void *key);
typedef void (*objc_setAssociatedObject_t)(id object, const void *key, id value, int policy);

/* ── Global Runtime Function Pointers ──────────────────────────────────────── */

extern objc_msgSend_t       objc_msgSend_fn;
extern objc_msgSend_fpret_t objc_msgSend_fpret_fn;
extern objc_msgSend_stret_t objc_msgSend_stret_fn;
extern objc_getClass_t      objc_getClass_fn;
extern objc_allocateClassPair_t objc_allocateClassPair_fn;
extern objc_registerClassPair_t objc_registerClassPair_fn;
extern sel_registerName_t   sel_registerName_fn;
extern sel_getName_t        sel_getName_fn;
extern class_addMethod_t    class_addMethod_fn;
extern class_addIvar_t      class_addIvar_fn;
extern class_replaceMethod_t class_replaceMethod_fn;
extern objc_getAssociatedObject_t objc_getAssociatedObject_fn;
extern objc_setAssociatedObject_t objc_setAssociatedObject_fn;

/* ── Initialization ────────────────────────────────────────────────────────── */

/* Initialize the Objective-C runtime. Returns 0 on success, -1 on failure.
 * Must be called before any objc_* functions. */
int objc_init(void);

/* Check if runtime is initialized */
bool objc_is_initialized(void);

/* ── Convenience Macros ────────────────────────────────────────────────────── */

/* Get a class by name */
#define objc_class(name) objc_getClass_fn(name)

/* Get/create a selector */
#define objc_sel(name) sel_registerName_fn(name)

/* Message send - returns id */
#define objc_msg(obj, sel, ...) \
    objc_msgSend_fn((id)(obj), objc_sel(sel), ##__VA_ARGS__)

/* Message send - returns void */
#define objc_msg_void(obj, sel, ...) \
    ((objc_msgSend_void_t)objc_msgSend_fn)((id)(obj), objc_sel(sel), ##__VA_ARGS__)

/* Message send - returns double/float */
#define objc_msg_fpret(obj, sel, ...) \
    objc_msgSend_fpret_fn((id)(obj), objc_sel(sel), ##__VA_ARGS__)

/* Message send - returns struct (uses stret ABI) */
#define objc_msg_stret(ret, obj, sel, ...) \
    objc_msgSend_stret_fn((ret), (id)(obj), objc_sel(sel), ##__VA_ARGS__)

/* Alloc + init pattern */
#define objc_alloc_init(classname) \
    objc_msg(objc_msg(objc_class(classname), "alloc"), "init")

/* Create NSString from C string */
id objc_nsstring(const char *cstr);

/* Get C string from NSString (uses UTF8String) */
const char* objc_cstring(id nsstring);

/* Create autoreleasepool, call block, drain pool */
#define objc_autoreleasepool_begin() \
    id _objc_pool = objc_alloc_init("NSAutoreleasePool")

#define objc_autoreleasepool_end() \
    objc_msg_void(_objc_pool, "drain")

/* ── Class Registration ────────────────────────────────────────────────────── */

/* Create and register a new Objective-C class.
 * Returns the Class, or NULL on failure. */
Class objc_create_class(const char *name, const char *superclass);

/* Add a method to a class. imp is a C function pointer.
 * types is the Objective-C type encoding (e.g., "v@:" for void method with no args).
 * Common encodings:
 *   v = void, @ = id/object, : = SEL, i = int, f = float, d = double
 *   ^ = pointer prefix, e.g., ^v = void*, ^@ = id*
 */
bool objc_add_method(Class cls, const char *sel, void *imp, const char *types);

/* ── Framework Loading ─────────────────────────────────────────────────────── */

/* Load a framework by name (e.g., "Cocoa", "Metal", "OpenGL") */
void* objc_load_framework(const char *name);

#endif /* OBJC_RUNTIME_H */
