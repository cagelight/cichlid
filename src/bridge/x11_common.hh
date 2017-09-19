#pragma once

#include "glad.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <GL/glx.h>

#include <X11/extensions/xf86dga.h>

namespace cichlid::x11 {
	
	extern bool initialized;
	extern Display * dpy;
	extern Window win;
	extern GLXFBConfig fbc;
}

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
typedef void (*glXSwapIntervalEXTProc)(Display *dpy, GLXDrawable drawable, int interval);
