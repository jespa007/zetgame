#include <stdio.h>
#include <stdlib.h>
#include <bcm_host.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

int main() {
    // Initialize Broadcom host library
    bcm_host_init();

    // Open a display on the default screen
    DISPMANX_DISPLAY_HANDLE_T display = vc_dispmanx_display_open(0);

    // Set up the display configuration
    DISPMANX_MODEINFO_T mode_info;
    vc_dispmanx_display_get_info(display, &mode_info);
    printf("Size: %dx%d, Transform: %d\n", mode_info.width, mode_info.height, mode_info.transform);

    // Create an EGL display
    EGLDisplay egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    // Initialize EGL
    eglInitialize(egl_display, NULL, NULL);

    // Create an EGL surface
    EGLConfig egl_config;
    EGLint num_configs;

    EGLint attributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    eglChooseConfig(egl_display, attributes, &egl_config, 1, &num_configs);

    // Create a DispmanX window
    DISPMANX_ELEMENT_HANDLE_T element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display = vc_dispmanx_display_open(0);
    DISPMANX_UPDATE_HANDLE_T update = vc_dispmanx_update_start(0);
    VC_RECT_T dst_rect = {0, 0, mode_info.width, mode_info.height};
    VC_RECT_T src_rect = {0, 0, mode_info.width << 16, mode_info.height << 16};

    VC_DISPMANX_ALPHA_T alpha = {DISPMANX_FLAGS_ALPHA_FROM_SOURCE, 255, 0};

    element = vc_dispmanx_element_add(
		update
		, dispman_display
		, 0
		, &dst_rect
		, 0
		, &src_rect
		, DISPMANX_PROTECTION_NONE
		, &alpha
		, 0
		, DISPMANX_NO_ROTATE
	);

    vc_dispmanx_update_submit_sync(update);

    EGLSurface egl_surface = eglCreateWindowSurface(
		egl_display
		, egl_config
		, (EGLNativeWindowType)element
		, NULL
	);

    // Set the current rendering context
    eglMakeCurrent(
		egl_display
		, egl_surface
		, egl_surface
		, eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, NULL)
	);

    // OpenGL ES code
    glClearColor(0.2f, 0.4f, 0.8f, 1.0f); // Set clear color to blue
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw a rectangle using two triangles
    GLfloat vertices[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        0.5f, 0.5f,

        -0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(0);

    // Swap buffers
    eglSwapBuffers(egl_display, egl_surface);

    // Wait for a few seconds
    sleep(5);

    // Clean up
    eglMakeCurrent(
		egl_display
		, EGL_NO_SURFACE
		, EGL_NO_SURFACE
		, EGL_NO_CONTEXT
	);

    eglDestroySurface(egl_display, egl_surface);

    eglTerminate(egl_display);

    vc_dispmanx_element_remove(dispman_display, element);

    vc_dispmanx_display_close(dispman_display);

    bcm_host_deinit();

    return 0;
}
