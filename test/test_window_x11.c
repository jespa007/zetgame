#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <stdio.h>
#include <stdlib.h>

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

int main() {
    Display *display;
    Window window;
    XEvent event;
    GLint att[] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
    XVisualInfo *vi;
    Colormap cmap;
    XSetWindowAttributes swa;
    GLXContext glc;

    // Open a connection to the X server
    display = XOpenDisplay(NULL);

    if (display == NULL) {
        printf("Cannot open display\n");
        exit(1);
    }

    // Create a window
    vi = glXChooseVisual(display, 0, att);
    cmap = XCreateColormap(display, RootWindow(display, vi->screen), vi->visual, AllocNone);
    swa.colormap = cmap;
    swa.event_mask = ExposureMask | KeyPressMask;
    window = XCreateWindow(display, RootWindow(display, vi->screen), 0, 0, 800, 600,
                           0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    XMapWindow(display, window);
    XStoreName(display, window, "OpenGL Window");

    // Create an OpenGL context
    glc = glXCreateContext(display, vi, NULL, GL_TRUE);
    glXMakeCurrent(display, window, glc);

    // Initialize OpenGL
    initOpenGL();

    // Main loop
    while (1) {
        XNextEvent(display, &event);

        if (event.type == Expose) {
            render();
            glXSwapBuffers(display, window);
        }

        if (event.type == KeyPress)
            break;
    }

    // Cleanup
    glXMakeCurrent(display, None, NULL);
    glXDestroyContext(display, glc);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}
