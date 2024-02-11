#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

typedef struct{
	int x,y,w,h;
}Rectangle;

typedef struct{
	Rectangle * monitors;
	int			n_monitors;
	int 		posx,posy;
	Display		*display;

}X11DisplayDevice;

typedef struct{
	int 		posx,posy;
	Window		window;
	XEvent 		event;
	GLXContext  glc;
	X11DisplayDevice		*display_device;
}X11Window;

void MonitorInfo(X11DisplayDevice *_display_device){
	int numMonitors;
	Display	*display=_display_device->display;

	int screen = DefaultScreen(display);

	XRRScreenResources *resources = XRRGetScreenResources(display, RootWindow(display, screen));
	if (resources != NULL) {
	    numMonitors = resources->noutput;
	    printf("Number of monitors: %d\n", numMonitors);
	    _display_device->n_monitors=numMonitors;

	    _display_device->monitors=malloc(sizeof(Rectangle)*numMonitors);

	    for (int i = 0; i < numMonitors; i++) {
	        XRROutputInfo *outputInfo = XRRGetOutputInfo(display, resources, resources->outputs[i]);
	        if (outputInfo != NULL) {
	            if (outputInfo->crtc != None) {
	                XRRCrtcInfo *crtcInfo = XRRGetCrtcInfo(display, resources, outputInfo->crtc);
	                if (crtcInfo != NULL) {
	                    printf("Monitor %d: Resolution %dx%d, Offset: (%d,%d)\n",
	                            i+1, crtcInfo->width, crtcInfo->height,
	                            crtcInfo->x, crtcInfo->y);
	                    _display_device->monitors[i].x=crtcInfo->x;
	                    _display_device->monitors[i].y=crtcInfo->y;
	                    _display_device->monitors[i].w=crtcInfo->width;
	                    _display_device->monitors[i].h=crtcInfo->height;

	                    XRRFreeCrtcInfo(crtcInfo);
	                }
	            }
	            XRRFreeOutputInfo(outputInfo);
	        }
	    }
	    XRRFreeScreenResources(resources);
	}
}

void CreateDisplay(X11DisplayDevice *_display_device){
	memset(_display_device,0,sizeof(X11DisplayDevice));

    // Open a connection to the X server
	_display_device->display = XOpenDisplay(NULL);

    if (_display_device->display == NULL) {
        printf("Cannot open display\n");
        exit(1);
    }

    // Monitor information
    MonitorInfo(_display_device);
}

void DestroyDisplay(X11DisplayDevice *_display_device){

    glXMakeCurrent(_display_device->display, None, NULL);
    XCloseDisplay(_display_device->display);

}

X11Window *CreateWindow(X11DisplayDevice *_display_device){
	X11Window *x11_window = malloc(sizeof(X11Window));


	memset(x11_window,0,sizeof(X11Window));

	x11_window->display_device=_display_device;
    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo *vi;
    Colormap cmap;
    XSetWindowAttributes swa;

    // Create a window
    vi = glXChooseVisual(_display_device->display, 0, att);
    cmap = XCreateColormap(_display_device->display, RootWindow(_display_device->display, vi->screen), vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask
    		| KeyPressMask // want to receive key press events
			| PointerMotionMask; // want to receive mouse motion events
    x11_window->window = XCreateWindow(
    		_display_device->display
    		, RootWindow(_display_device->display, vi->screen)
			, 0
			, 0
			, WINDOW_WIDTH
			, WINDOW_HEIGHT
			, 0
			, vi->depth
			, InputOutput
			, vi->visual
			, CWColormap | CWEventMask
			, &swa
	);

    XMapWindow(_display_device->display, x11_window->window);

    // caption
    XStoreName(_display_device->display, x11_window->window, "OpenGL Window");

    // Create an OpenGL context
    x11_window->glc = glXCreateContext(_display_device->display, vi, NULL, GL_TRUE);
    glXMakeCurrent(_display_device->display, x11_window->window, x11_window->glc);

    printf("Created window OK\n");

    return x11_window;
}

void DestroyWindow(X11Window *_x11_window){
	XDestroyWindow(_x11_window->display_device->display, _x11_window->window);
	glXDestroyContext(_x11_window->display_device->display,_x11_window->glc);
	free(_x11_window);
}



// Function to set up OpenGL
void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set the background color to black
}

// Function to render the triangle
void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);  // Red
    glVertex3f(0.0, 1.0, 0.0);

    glColor3f(0.0, 1.0, 0.0);  // Green
    glVertex3f(-1.0, -1.0, 0.0);

    glColor3f(0.0, 0.0, 1.0);  // Blue
    glVertex3f(1.0, -1.0, 0.0);
    glEnd();

    glFlush();
}

void setFullscreen(X11Window *_window, bool _fullscreen){

	Atom wmState = XInternAtom(_window->display_device->display, "_NET_WM_STATE", False);
	Atom fullscreenAtom = XInternAtom(_window->display_device->display, "_NET_WM_STATE_FULLSCREEN", False);
	XEvent xev;

	if (_fullscreen) {
		// Add fullscreen state
		xev.xclient.type = ClientMessage;
		xev.xclient.window = _window->window;
		xev.xclient.message_type = wmState;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = 1;
		xev.xclient.data.l[1] = fullscreenAtom;
		xev.xclient.data.l[2] = 0;
		xev.xclient.data.l[3] = 0;
		xev.xclient.data.l[4] = 0;
	} else {
		// Remove fullscreen state
		xev.xclient.type = ClientMessage;
		xev.xclient.window = _window->window;
		xev.xclient.message_type = wmState;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = 0;
		xev.xclient.data.l[1] = fullscreenAtom;
		xev.xclient.data.l[2] = 0;
		xev.xclient.data.l[3] = 0;
		xev.xclient.data.l[4] = 0;
	}
	XSendEvent(_window->display_device->display, RootWindow(_window->display_device->display, DefaultScreen(_window->display_device->display)), False,
			   SubstructureNotifyMask, &xev);
	XFlush(_window->display_device->display);

}

int main() {
	X11DisplayDevice display_device;
	X11Window *window;
	bool fullscreen=false;

	CreateDisplay(&display_device);
	window=CreateWindow(&display_device);

    // Initialize OpenGL
    initOpenGL();

    // Main loop
    while (1) {
        XNextEvent(display_device.display, &window->event);

        if (window->event.type == Expose) {
            render();
            glXSwapBuffers(display_device.display, window->window);
        }

        if (window->event.type == MotionNotify){
        	printf("mouse motion\n");
        }

        if (window->event.type == KeyPress){
        	printf("keycode %i\n",window->event.xkey.keycode);
        	if(window->event.xkey.keycode == 9){
        		// ESC
        		break;
        	}

        	// switch fullscreen (F)
        	if(window->event.xkey.keycode == 75){
        		fullscreen=!fullscreen;
        		setFullscreen(window,fullscreen);
        	}
        }

    }

    // Cleanup
	DestroyWindow(window);
	DestroyDisplay(&display_device);

	printf("Deinit\n");

    return 0;
}
