#include <stdio.h>
#include <GL/gl.h>
#include <Windows.h>
#include <stdbool.h>

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

const char g_szClassName[] = "myWindowClass";

typedef struct{
	int x,y,w,h;
}ZG_Rectangle;

typedef struct{
	ZG_Rectangle * monitors;
	int			n_monitors;
	int 		posx,posy;
	HINSTANCE hInstance;
	int nCmdShow;
}Win32DisplayDevice;


typedef struct{
	HDC hdc;
	HWND hwnd;
	HGLRC hrc; // context
}Win32Window;


bool request_fullscreen=false;

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        case WM_KEYDOWN:
        	switch(wParam){
        	case VK_F9:
        		request_fullscreen=true;
        		break;
        	default:
        		break;
        	}
        	break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

typedef struct{
	ZG_Rectangle *monitors;
	int	n_current_monitor;
}MonitorEnumProcUserData;

BOOL CALLBACK ZG_MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    MonitorEnumProcUserData *data=(MonitorEnumProcUserData *)dwData;

    if (GetMonitorInfo(hMonitor, &monitorInfo)) {
        // Get the dimensions (resolution) of the monitor

    	if (monitorInfo.dwFlags != DISPLAY_DEVICE_MIRRORING_DRIVER){

				int width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
				int height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

				// Get the offset of the monitor
				int offsetX = monitorInfo.rcMonitor.left;
				int offsetY = monitorInfo.rcMonitor.top;

				data->monitors[data->n_current_monitor].x=offsetX;
				data->monitors[data->n_current_monitor].y=offsetY;
				data->monitors[data->n_current_monitor].w=width;
				data->monitors[data->n_current_monitor].h=height;

				 printf("Monitor %d: Resolution %dx%d, Offset: (%d,%d)\n"
						,data->n_current_monitor
						, width
						, height
						,offsetX
						, offsetY
				);

				data->n_current_monitor++;
    	}
    }

    return TRUE; // Continue enumeration
}


void ZG_MonitorInfo(Win32DisplayDevice *_display_device){
	int numMonitors = GetSystemMetrics(SM_CMONITORS);
	_display_device->monitors=malloc(sizeof(ZG_Rectangle)*numMonitors);
	MonitorEnumProcUserData user_data={_display_device->monitors,0};

	EnumDisplayMonitors(NULL,NULL,ZG_MonitorEnumProc,(LPARAM)&user_data);

	printf("Monitor ok\n");
	// Iterate through each monitor
	/*for (int i = 0; i < numMonitors; ++i) {
		MONITORINFOEX monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFOEX);
		if (GetMonitorInfo(MonitorFromIndex(i, MONITOR_DEFAULTTOPRIMARY), &monitorInfo)) {
			// Get the dimensions (resolution) of the monitor
			int width = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
			int height = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

			// Get the offset of the monitor
			int offsetX = monitorInfo.rcMonitor.left;
			int offsetY = monitorInfo.rcMonitor.top;

			_display_device->monitors[i].x=offsetX;
			_display_device->monitors[i].y=offsetY;

			_display_device->monitors[i].w=width;
			_display_device->monitors[i].h=height;

			  printf("Monitor %d: Resolution %dx%d, Offset: (%d,%d)\n",
					i+1, width, height,
					offsetX, offsetY);
		}
	}*/
}

void ZG_CreateDisplay(Win32DisplayDevice *_display_device, HINSTANCE hInstance,int nCmdShow){
	memset(_display_device,0,sizeof(Win32DisplayDevice));

	_display_device->nCmdShow=nCmdShow;
	_display_device->hInstance=hInstance;

    // Monitor information
    ZG_MonitorInfo(_display_device);
}

void ZG_DestroyDisplay(Win32DisplayDevice *_display_device){

    //glXMakeCurrent(_display_device->display, None, NULL);
}

Win32Window *ZG_CreateWindow(Win32DisplayDevice *_display_device){
	Win32Window *win32_window = malloc(sizeof(Win32Window));

	memset(win32_window,0,sizeof(Win32Window));

    WNDCLASSEX wc;
    MSG Msg;

	RECT wr = {0, 0, 800, 600};    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = _display_device->hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    win32_window->hwnd = CreateWindowEx(
        0
        ,g_szClassName
		,"OpenGL Window"
        ,WS_OVERLAPPEDWINDOW
        ,CW_USEDEFAULT
		, CW_USEDEFAULT
		, wr.right - wr.left    // width of the window
        ,  wr.bottom - wr.top    // height of the window
        ,NULL
		, NULL
		, _display_device->hInstance
		, NULL);

    if(win32_window->hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Get the device context
    win32_window->hdc = GetDC(win32_window->hwnd);

    // Set up pixel format
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32, // Color depth
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24, // Depth buffer
        8,  // Stencil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    ShowWindow(win32_window->hwnd, _display_device->nCmdShow);
    UpdateWindow(win32_window->hwnd);

    int pixelFormat = ChoosePixelFormat(win32_window->hdc, &pfd);
    SetPixelFormat(win32_window->hdc, pixelFormat, &pfd);

    // Create OpenGL rendering context
    win32_window->hrc = wglCreateContext(win32_window->hdc);
    wglMakeCurrent(win32_window->hdc, win32_window->hrc);

    printf("Created window OK\n");

    return win32_window;
}

void ZG_DestroyWindow(Win32Window *_win32_window){
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(_win32_window->hrc);
	ReleaseDC(_win32_window->hwnd, _win32_window->hdc);
	DestroyWindow(_win32_window->hwnd);
}
/*
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) {
	HMONITOR *selectedMonitor=(HMONITOR *)dwData;
    if (*selectedMonitor == NULL) {
        // Store the handle of the first monitor found
        *selectedMonitor = hMonitor;
    }
    return TRUE; // Continue enumeration
}*/


void setFullscreen(Win32Window *_window, bool _fullscreen){

	DWORD windowStyle;

	if (_fullscreen) {

		// Modify the MonitorEnumProc callback function to store the monitor handle
		HMONITOR selectedMonitor = NULL;
		 windowStyle = GetWindowLong(_window->hwnd, GWL_STYLE);
		SetWindowLong(_window->hwnd, GWL_STYLE, windowStyle & ~WS_OVERLAPPEDWINDOW);

		 // Determine the position and size of the selected monitor
		//MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
		//if (GetMonitorInfo(selectedMonitor, &monitorInfo)) {

			SetWindowPos(
					_window->hwnd
					, HWND_TOP
					, 0 //monitorInfo.rcMonitor.left
					, 0 //monitorInfo.rcMonitor.top
			        , GetSystemMetrics(SM_CXSCREEN)//monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left
					, GetSystemMetrics(SM_CYSCREEN)//monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top
					, SWP_SHOWWINDOW
		);
		//}

	} else {
		SetWindowLong(
			_window->hwnd
			, GWL_STYLE
			, windowStyle
		);
		SetWindowPos(
			_window->hwnd
			, HWND_TOP
			, 0
			, 0
			, WINDOW_WIDTH
			, WINDOW_HEIGHT
			, SWP_SHOWWINDOW
		);

	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
	Win32DisplayDevice display_device;
	Win32Window *window;
	bool fullscreen=false;

	ZG_CreateDisplay(&display_device,hInstance,nCmdShow);
	window=ZG_CreateWindow(&display_device);


    // Enable basic OpenGL functionality
    glEnable(GL_DEPTH_TEST);



    // Step 3: The Message Loop
    // Main loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        if(request_fullscreen){
        	fullscreen=!fullscreen;
        	setFullscreen(window,fullscreen);
        	request_fullscreen=false;
        }

        // Your rendering code goes here
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-0.6f, -0.6f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.6f, -0.6f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.6f, 0.0f);
        glEnd();

        SwapBuffers(window->hdc);
    }


    // Cleanup
	ZG_DestroyWindow(window);
	ZG_DestroyDisplay(&display_device);

    return 0;
}
