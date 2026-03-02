/* Xcursor.h - minimal stub for cosmo-sokol */
#ifndef _XCURSOR_H_
#define _XCURSOR_H_

#include <X11/Xlib.h>

typedef unsigned long XcursorPixel;
typedef XcursorPixel XcursorARGB;
typedef unsigned int XcursorDim;
typedef unsigned int XcursorBool;
typedef unsigned int XcursorUInt;

typedef struct _XcursorImage {
    XcursorUInt version;
    XcursorDim size;
    XcursorDim width;
    XcursorDim height;
    XcursorDim xhot;
    XcursorDim yhot;
    XcursorUInt delay;
    XcursorPixel *pixels;
} XcursorImage;

typedef struct _XcursorImages {
    int nimage;
    XcursorImage **images;
    char *name;
} XcursorImages;

/* Functions are dlopen'd at runtime - declarations only */
XcursorImage *XcursorImageCreate(int width, int height);
void XcursorImageDestroy(XcursorImage *image);
XcursorImages *XcursorImagesCreate(int size);
void XcursorImagesDestroy(XcursorImages *images);
void XcursorImagesSetName(XcursorImages *images, const char *name);
Cursor XcursorImageLoadCursor(Display *dpy, const XcursorImage *image);
Cursor XcursorImagesLoadCursor(Display *dpy, const XcursorImages *images);

/* Additional functions used by sokol */
XcursorImage *XcursorLibraryLoadImage(const char *file, const char *theme, int size);
char *XcursorGetTheme(Display *dpy);
int XcursorGetDefaultSize(Display *dpy);

#endif /* _XCURSOR_H_ */
