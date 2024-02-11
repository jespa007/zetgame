#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <bcm_host.h>
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include <fcntl.h>
#include <linux/input.h>
#include <linux/input.h>
#include <libudev.h>

#define BLOCK_SIZE 512

typedef struct{
	DISPMANX_DISPLAY_HANDLE_T 	dispmanx_display;
	EGL_DISPMANX_WINDOW_T 		dispmanx_window;
	EGLDisplay egl_display;
	EGLSurface egl_surface;
}Window_RPI;

#define check() assert(glGetError() == 0)

typedef struct{
	 struct udev *udev;
	 struct udev_monitor *monitor;
	 int fd;
}Input_RPI;

// Define the path to the input event device
#define INPUT_EVENT_DEVICE "/dev/input/event0"

void Input_RPI_Create(Input_RPI *_input_rpi){

	memset(_input_rpi,0,sizeof(Input_RPI));
	struct udev_device *dev;
	struct udev_enumerate *enumerate;
	struct udev_list_entry *devices, *dev_list_entry;

	 // Initialize libudev context
	_input_rpi->udev = udev_new();
	if (_input_rpi->udev == NULL) {
		fprintf(stderr, "Failed to initialize libudev\n");
		exit(EXIT_FAILURE);
	}

   // Create a udev monitor
	_input_rpi->monitor = udev_monitor_new_from_netlink(_input_rpi->udev, "udev");
	if (_input_rpi->monitor == NULL) {
		fprintf(stderr, "Failed to create udev monitor\n");
		udev_unref(_input_rpi->udev);
		exit(EXIT_FAILURE);
	}

	/* create enumerate object */
	enumerate = udev_enumerate_new(_input_rpi->udev);
	if (!enumerate) {
		udev_unref(_input_rpi->udev);
		fprintf(stderr, "Cannot create enumerate context.\n");
		exit(EXIT_FAILURE);
	}

	udev_enumerate_add_match_subsystem(enumerate, "block");
	udev_enumerate_scan_devices(enumerate);

	/* fillup device list */
	devices = udev_enumerate_get_list_entry(enumerate);
	if (!devices) {
		udev_unref(_input_rpi->udev);
		fprintf(stderr, "Failed to get device list.\n");
		exit(EXIT_FAILURE);
	}

	udev_list_entry_foreach(dev_list_entry, devices) {
		const char *path, *tmp;
		unsigned long long disk_size = 0;
		unsigned short int block_size = BLOCK_SIZE;

		path = udev_list_entry_get_name(dev_list_entry);
		dev = udev_device_new_from_syspath(_input_rpi->udev, path);

		/* skip if device/disk is a partition or loop device */
		if (strncmp(udev_device_get_devtype(dev), "partition", 9) != 0 &&
		    strncmp(udev_device_get_sysname(dev), "loop", 4) != 0) {
			printf("I: DEVNODE=%s\n", udev_device_get_devnode(dev));
			printf("I: KERNEL=%s\n", udev_device_get_sysname(dev));
			printf("I: DEVPATH=%s\n", udev_device_get_devpath(dev));
			printf("I: DEVTYPE=%s\n", udev_device_get_devtype(dev));

			tmp = udev_device_get_sysattr_value(dev, "size");
			if (tmp)
				disk_size = strtoull(tmp, NULL, 10);

			tmp = udev_device_get_sysattr_value(dev, "queue/logical_block_size");
			if (tmp){
				block_size = atoi(tmp);
			}

			printf("I: DEVSIZE=");
			if (strncmp(udev_device_get_sysname(dev), "sr", 2) != 0)
				printf("%lld GB\n", (disk_size * block_size) / 1000000000);
			else
				printf("n/a\n");
		}

		/* free dev */
		udev_device_unref(dev);
	}
	/* free enumerate */
	udev_enumerate_unref(enumerate);

	// Add filter for input devices
	if(udev_monitor_filter_add_match_subsystem_devtype(_input_rpi->monitor, "input", NULL) < 0){
		fprintf(stderr, "Failed to udev_monitor_filter_add_match_subsystem_devtype\n");
		udev_unref(_input_rpi->udev);
		exit(EXIT_FAILURE);
	}

	// Enable the monitor
	if(udev_monitor_enable_receiving(_input_rpi->monitor) < 0){
		fprintf(stderr, "Failed to udev_monitor_enable_receiving\n");
		udev_unref(_input_rpi->udev);
		exit(EXIT_FAILURE);
	}

	_input_rpi->fd = udev_monitor_get_fd(_input_rpi->monitor);

	printf("libudev initialized!\n");

}

bool pressed_esc=false;


void Input_RPI_Update(Input_RPI *_input_rpi){
	fd_set fds;
	struct timeval tv;
	int ret;

	FD_ZERO(&fds);
	FD_SET(_input_rpi->fd, &fds);
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	ret = select(_input_rpi->fd+1, &fds, NULL, NULL, &tv);
	if (ret > 0 && FD_ISSET(_input_rpi->fd, &fds)) {
		struct udev_device *dev = udev_monitor_receive_device(_input_rpi->monitor);
		if (dev) {
			printf("I: ACTION=%s\n", udev_device_get_action(dev));
			printf("I: DEVNAME=%s\n", udev_device_get_sysname(dev));
			printf("I: DEVPATH=%s\n", udev_device_get_devpath(dev));
			printf("I: MACADDR=%s\n", udev_device_get_sysattr_value(dev, "address"));
			printf("---\n");

			/* free dev */
			udev_device_unref(dev);
		}
	}
	// Read events from the monitor
	/*struct udev_device *dev = udev_monitor_receive_device(_input_rpi->monitor);
	if (dev) {
		const char *action = udev_device_get_action(dev);
		if (action && strcmp(action, "add") == 0) {
			printf("added \n");
			const char *devnode = udev_device_get_devnode(dev);
			if (devnode) {
				printf("devnode \n");
				int fd = open(devnode, O_RDONLY);
				if (fd >= 0) {
					printf("there's events \n");
					struct input_event ev;
					ssize_t bytes;
					while (1) {
						bytes = read(fd, &ev, sizeof(ev));
						if (bytes == sizeof(ev)) {
							if (ev.type == EV_KEY && ev.code == KEY_A) {
								if (ev.value == 1) {
									printf("'A' key pressed\n");
								} else if (ev.value == 0) {
									printf("'A' key released\n");
								}
							} else if (ev.type == EV_REL && (ev.code == REL_X || ev.code == REL_Y)) {
								printf("Mouse movement: X=%d, Y=%d\n", ev.value, ev.code);
							} else if (ev.type == EV_KEY && (ev.code == BTN_LEFT || ev.code == BTN_RIGHT || ev.code == BTN_MIDDLE)) {
								if (ev.value == 1) {
									printf("Mouse button %d pressed\n", ev.code);
								} else if (ev.value == 0) {
									printf("Mouse button %d released\n", ev.code);
								}
							}
						} else if (bytes == -1) {
							perror("Error reading input event");
							break;
						} else if (bytes == 0) {
							break;
						}
					}
					close(fd);
				}
			}
			// Cleanup
			udev_device_unref(dev);
		}
	}else{
		fprintf(stderr,"libudev NULL!\n");
	}*/

}

void Input_RPI_Destroy(Input_RPI *_input_rpi){

   // Cleanup
    udev_monitor_unref(_input_rpi->monitor);
    udev_unref(_input_rpi->udev);


}

void Window_RPI_Create(Window_RPI *_rpi_window){
	memset(_rpi_window,0,sizeof(Window_RPI));
	EGLBoolean result;
	EGLContext egl_context;
	EGLint egl_major=0;
	EGLint egl_minor=0;

	if (bcm_host_get_processor_id() == BCM_HOST_PROCESSOR_BCM2838){
	   puts("Cannot create window on the Pi4\n\n");
	   exit(0);
	}

    // Initialize Broadcom host library
    bcm_host_init();

    // Open a dispman_display on the default screen
    _rpi_window->dispmanx_display = vc_dispmanx_display_open(0);

    // Set up the dispman_display configuration
    DISPMANX_MODEINFO_T mode_info;
    vc_dispmanx_display_get_info(_rpi_window->dispmanx_display, &mode_info);


    // Create an EGL dispman_display
    _rpi_window->egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert( _rpi_window->egl_display!=EGL_NO_DISPLAY);
    check();

    // Initialize EGL
    result = eglInitialize(_rpi_window->egl_display, &egl_major, &egl_minor);
    assert(EGL_TRUE== result);
    check();

    printf("egl_client_api:%s\n",eglQueryString(_rpi_window->egl_display,EGL_CLIENT_APIS));
    printf("egl_version:%s\n",eglQueryString(_rpi_window->egl_display,EGL_VERSION));
    printf("egl_vendor:%s\n",eglQueryString(_rpi_window->egl_display,EGL_VENDOR));
    printf("egl_vendor:%s\n",eglQueryString(_rpi_window->egl_display,EGL_EXTENSIONS));

    //-----------
    // Available context configs

    // Get the number of available configurations
    EGLint num_configs;
    result = eglGetConfigs(_rpi_window->egl_display, NULL, 0, &num_configs);
    assert(EGL_TRUE == result);
    check();

    // Allocate memory for the configurations
    EGLConfig* configs = malloc(num_configs * sizeof(EGLConfig));
    result=eglGetConfigs(_rpi_window->egl_display, configs, num_configs, &num_configs);
    assert(EGL_TRUE== result);
    check();

    bool egl_opengles2_support=false;

    // Iterate through the configurations
    for (int i = 0; i < num_configs; i++) {
        EGLint renderable_type;

        // check EGL_RENDERABLE_TYPE attribute ...
        eglGetConfigAttrib(_rpi_window->egl_display, configs[i], EGL_RENDERABLE_TYPE, &renderable_type);

        // You can inspect other attributes of the configuration here
        // For example, color depth, depth buffer size, stencil buffer size, etc.
       	/*EGL_ALPHA_SIZE:Returns the number of bits of alpha stored in the color buffer.
		EGL_ALPHA_MASK_SIZE:Returns the number of bits in the alpha mask buffer.
		EGL_BIND_TO_TEXTURE_RGB:Returns EGL_TRUE if color buffers can be bound to an RGB texture, EGL_FALSE otherwise.
		EGL_BIND_TO_TEXTURE_RGBA:Returns EGL_TRUE if color buffers can be bound to an RGBA texture, EGL_FALSE otherwise.
		EGL_BLUE_SIZE:Returns the number of bits of blue stored in the color buffer.
		EGL_BUFFER_SIZE:Returns the depth of the color buffer. It is the sum of EGL_RED_SIZE, EGL_GREEN_SIZE, EGL_BLUE_SIZE, and EGL_ALPHA_SIZE.
		EGL_COLOR_BUFFER_TYPE:Returns the color buffer type. Possible types are EGL_RGB_BUFFER and EGL_LUMINANCE_BUFFER.
		EGL_CONFIG_CAVEAT:Returns the caveats for the frame buffer configuration. Possible caveat values are EGL_NONE, EGL_SLOW_CONFIG, and EGL_NON_CONFORMANT.
		EGL_CONFIG_ID:Returns the ID of the frame buffer configuration.
		EGL_CONFORMANT:Returns a bitmask indicating which client API contexts created with respect to this config are conformant.
		EGL_DEPTH_SIZE:Returns the number of bits in the depth buffer.
		EGL_GREEN_SIZE:Returns the number of bits of green stored in the color buffer.
		EGL_LEVEL:Returns the frame buffer level. Level zero is the default frame buffer. Positive levels correspond to frame buffers that overlay the default buffer and negative levels correspond to frame buffers that underlay the default buffer.
		EGL_LUMINANCE_SIZE:Returns the number of bits of luminance stored in the luminance buffer.
		EGL_MAX_PBUFFER_WIDTH:Returns the maximum width of a pixel buffer surface in pixels.
		EGL_MAX_PBUFFER_HEIGHT:Returns the maximum height of a pixel buffer surface in pixels.
		EGL_MAX_PBUFFER_PIXELS:Returns the maximum size of a pixel buffer surface in pixels.
		EGL_MAX_SWAP_INTERVAL:Returns the maximum value that can be passed to eglSwapInterval.
		EGL_MIN_SWAP_INTERVAL:Returns the minimum value that can be passed to eglSwapInterval.
		EGL_NATIVE_RENDERABLE:Returns EGL_TRUE if native rendering APIs can render into the surface, EGL_FALSE otherwise.
		EGL_NATIVE_VISUAL_ID:Returns the ID of the associated native visual.
		EGL_NATIVE_VISUAL_TYPE:Returns the type of the associated native visual.
		EGL_RED_SIZE:Returns the number of bits of red stored in the color buffer.
		EGL_RENDERABLE_TYPE:Returns a bitmask indicating the types of supported client API contexts.
		EGL_SAMPLE_BUFFERS:Returns the number of multisample buffers.
		EGL_SAMPLES:Returns the number of samples per pixel.
		EGL_STENCIL_SIZE:Returns the number of bits in the stencil buffer.
		EGL_SURFACE_TYPE:Returns a bitmask indicating the types of supported EGL surfaces.
		EGL_TRANSPARENT_TYPE:Returns the type of supported transparency. Possible transparency values are: EGL_NONE, and EGL_TRANSPARENT_RGB.
		EGL_TRANSPARENT_RED_VALUE:Returns the transparent red value.
		EGL_TRANSPARENT_GREEN_VALUE:Returns the transparent green value.
		EGL_TRANSPARENT_BLUE_VALUE:Returns the transparent blue value.
		*/
        // Check if OpenGL ES 2.0 is supported
        if (renderable_type & EGL_OPENGL_ES2_BIT) {
            // Output the supported OpenGL ES version
            printf("Configuration %d supports OpenGL ES 2.0\n", i);
            egl_opengles2_support=true;
        }
    }

    // Cleanup
    free(configs);

    //-----------
    // Create an EGL surface
    EGLConfig egl_config;

    EGLint attributes[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };


    static const EGLint context_attributes[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,
      EGL_NONE
    };


    result = eglChooseConfig(_rpi_window->egl_display, attributes, &egl_config, 1, &num_configs);
    assert(EGL_TRUE== result);
    check();

    // get an appropriate EGL frame buffer configuration
  /*  result = eglBindAPI(EGL_OPENGL_ES_API);
    assert(EGL_TRUE== result);
    check();*/

    // Create a DispmanX window
    DISPMANX_UPDATE_HANDLE_T update = vc_dispmanx_update_start(0);
    VC_RECT_T dst_rect = {0, 0, mode_info.width, mode_info.height};
    VC_RECT_T src_rect = {0, 0, mode_info.width << 16, mode_info.height << 16};

    VC_DISPMANX_ALPHA_T alpha = {DISPMANX_FLAGS_ALPHA_FROM_SOURCE, 255, 0};

    DISPMANX_ELEMENT_HANDLE_T element = vc_dispmanx_element_add(
		update
		, _rpi_window->dispmanx_display
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


    _rpi_window->dispmanx_window.element = element;
    _rpi_window->dispmanx_window.width = mode_info.width;
    _rpi_window->dispmanx_window.height = mode_info.height;

    _rpi_window->egl_surface = eglCreateWindowSurface(
		_rpi_window->egl_display
		, egl_config
		, &_rpi_window->dispmanx_window
		, NULL
	);

    assert(_rpi_window->egl_surface != EGL_NO_SURFACE);
    check();

    egl_context=eglCreateContext(
		_rpi_window->egl_display
		, egl_config
		, EGL_NO_CONTEXT
		, context_attributes
	);

    assert(egl_context!=EGL_NO_CONTEXT);
    check();

    // Set the current rendering egl_context
    eglMakeCurrent(
		_rpi_window->egl_display
		, _rpi_window->egl_surface
		, _rpi_window->egl_surface
		, egl_context
	);
}

void Window_RPI_Destroy(Window_RPI *_rpi_window){
    // Clean up
    eglMakeCurrent(_rpi_window->egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(_rpi_window->egl_display, _rpi_window->egl_surface);
    eglTerminate(_rpi_window->egl_display);
    vc_dispmanx_element_remove(_rpi_window->dispmanx_display, _rpi_window->dispmanx_window.element);
    vc_dispmanx_display_close(_rpi_window->dispmanx_display);
    bcm_host_deinit();
}


int main() {

	Window_RPI window_rpi;
	Input_RPI  input_rpi;

	Input_RPI_Create(&input_rpi);
	/*Window_RPI_Create(&window_rpi);



    // Define vertices of the triangle
    GLfloat vertices[] = {
        0.0f,  0.5f,  // Top vertex
       -0.5f, -0.5f,  // Bottom-left vertex
        0.5f, -0.5f   // Bottom-right vertex
    };

    // Create vertex buffer object (VBO)
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Create and compile the vertex shader
    const char* vertexShaderSource =
        "attribute vec2 position;\n"
        "void main() {\n"
        "   gl_Position = vec4(position, 0.0, 1.0);\n"
        "}\n";
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    const char* fragmentShaderSource =
        "void main() {\n"
        "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n" // Red color
        "}\n";
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create the shader program and link it
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindAttribLocation(shaderProgram, 0, "position");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);*/

    do{
    	// Set clear color to blue
        /*glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

    	// Specify the layout of the vertex data
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    	// Draw the triangle
    	glDrawArrays(GL_TRIANGLES, 0, 3);
    	check();

    	// Swap buffers
    	eglSwapBuffers(window_rpi.egl_display, window_rpi.egl_surface);*/

    	Input_RPI_Update(&input_rpi);

    // Wait for a few seconds
	}while(!pressed_esc);

   // Window_RPI_Destroy(&window_rpi);
    Input_RPI_Destroy(&input_rpi);

    return 0;
}
