#pragma once

#include "glad.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

namespace cichlid::x11 {
	
	extern bool initialized;
	extern Display * dpy;
	extern Window win;
	extern GLXContext context;
}

#define GLX_CONTEXT_MAJOR_VERSION_ARB       0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB       0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
typedef void (*glXSwapIntervalEXTProc)(Display *dpy, GLXDrawable drawable, int interval);
