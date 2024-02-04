#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <unistd.h>
#include <assert.h>
#include <EGL/eglext.h>

#define WIDTH 1366
#define HEIGHT 768

// Define the vertices of the triangle
GLfloat vertices[] = {
    0.0f,  0.5f, 0.0f,  // Vertex 1 (top)
   -0.5f, -0.5f, 0.0f,  // Vertex 2 (bottom left)
    0.5f, -0.5f, 0.0f   // Vertex 3 (bottom right)
};

// Vertex shader source code
const char *vertexShaderSource =
    "attribute vec4 vPosition;\n"
    "void main() {\n"
    "  gl_Position = vPosition;\n"
    "}\n";

// Fragment shader source code
const char *fragmentShaderSource =
    "void main() {\n"
    "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n" // Red color
    "}\n";

#define check() assert(glGetError() == 0)

int main() {
	EGLBoolean result;

	 static const int MAX_DEVICES = 4;
	  EGLDeviceEXT eglDevs[MAX_DEVICES];
	  EGLint numDevices;
	  EGLDisplay eglDpy;

	  PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT =
	    (PFNEGLQUERYDEVICESEXTPROC)
	    eglGetProcAddress("eglQueryDevicesEXT");

	  eglQueryDevicesEXT(MAX_DEVICES, eglDevs, &numDevices);

	  printf("Detected %i devices\n", numDevices);

	  PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
	    (PFNEGLGETPLATFORMDISPLAYEXTPROC)
	    eglGetProcAddress("eglGetPlatformDisplayEXT");

	  eglDpy = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT,
	                                    eglDevs[1], 0);

	  printf("EGL_DEFAULT_DISPLAY:%p eglDpy:%p\n",EGL_DEFAULT_DISPLAY,eglDpy);

	// set display to 0.0 in order to establish EGL context
	//putenv("DISPLAY=:0.0");
	//unsetenv( "DISPLAY" );

    // Initialize EGL
    EGLDisplay egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert( egl_display!=EGL_NO_DISPLAY);
    check();

    result=eglInitialize(egl_display, NULL, NULL);
    assert(EGL_TRUE== result);
    check();

    printf("egl_client_api:%s\n",eglQueryString(egl_display,EGL_CLIENT_APIS));
    printf("egl_version:%s\n",eglQueryString(egl_display,EGL_VERSION));
    printf("egl_vendor:%s\n",eglQueryString(egl_display,EGL_VENDOR));
    printf("egl_vendor:%s\n",eglQueryString(egl_display,EGL_EXTENSIONS));

    // Set up EGL configuration
    EGLint configAttribs[] = {
	   EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
       EGL_BLUE_SIZE, 8,
          EGL_GREEN_SIZE, 8,
          EGL_RED_SIZE, 8,
          EGL_DEPTH_SIZE, 8,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    result=eglChooseConfig(egl_display, configAttribs, &config, 1, &numConfigs);
    assert(EGL_TRUE == result);
    check();

    // Create EGL context
    //EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    EGLContext egl_context = eglCreateContext(egl_display, config, EGL_NO_CONTEXT, NULL);
    assert(egl_context!=EGL_NO_CONTEXT);
    check();

    // Create EGL window surface
    // Create EGL pbuffer surface
	EGLint pbufferAttribs[] = {
		EGL_WIDTH, WIDTH,
		EGL_HEIGHT, HEIGHT,
		EGL_NONE,
	};
    //EGLSurface egl_surface = eglCreateWindowSurface(egl_display, config, (EGLNativeWindowType)NULL, NULL);
    EGLSurface egl_surface = eglCreatePbufferSurface(egl_display, config, pbufferAttribs);

    assert(egl_surface != EGL_NO_SURFACE);
    check();

    eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);

    // Compile and link the shaders
   /* GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Get the position attribute location
    GLint posAttrib = glGetAttribLocation(shaderProgram, "vPosition");
    glEnableVertexAttribArray(posAttrib);

    // Specify the vertex data
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, vertices);*/


    // Main loop
    bool running = true;
    while (running) {
        // Process events
        // For simplicity, we are not handling events in this example

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        /*glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers (not needed for pbuffer surfaces)*/
        //eglSwapBuffers(egl_display, egl_surface);

        // Introduce a delay to avoid consuming too much CPU
        usleep(10000); // 10 milliseconds
    }

    // Cleanup
    eglDestroySurface(egl_display, egl_surface);
    eglDestroyContext(egl_display, egl_context);
    eglTerminate(egl_display);

    return 0;
}
