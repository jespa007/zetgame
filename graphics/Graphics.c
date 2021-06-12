#include "zg_graphics.h"

typedef struct
{
	int 	pixels_width, pixels_height;
	float 	physical_width, physical_height;
	char 	monitor_model[1024];
} AdapterInfo;

typedef struct{
	 uint8_t 		bytes_per_pixel;
	 uint16_t     width,height;
	 Vector2f 	scale;
	 float		one_over_width
	 	 	 	,one_over_height
				,aspect_ratio
				,one_over_aspect_ratio;

	 float 		fps;

	 //ViewPort  * view_port;

	 Color4f 			background_color;
	 SDL_Window		* 	sdl_window;
	 SDL_Renderer 	*	sdl_renderer;
	 SDL_Surface 	*	sdl_window_surface;

	 bool stated_render;
	 bool screenshoot_request;
	 void * graphics_data;
	 GraphicsApi graphics_api;
	 uint32_t 	start_ms;
	 uint32_t 	elapsed_seconds;
	 List	*adapters;
	 List   *capture_screen_callbacks;
	 int 	n_screenshoot;

	Geometry *geometry_quad_default;

}GraphicsVars;


GraphicsVars *g_graphics_vars=NULL;



#include "Graphics_GL.c"
#include "Graphics_Win32.c"

static void Graphics_PrintAdapterInformation(void);

bool Graphics_Init(
		int start_posx,int start_posy	// start position
		, uint16_t _width, uint16_t _height // logical resolution
		//, uint16_t _window_width, uint16_t _window_height // screen resolution
		,GraphicsApi _video_context
		,const char *caption
		,uint16_t properties
	) {

	uint16_t _window_width=_width;
	uint16_t _window_height=_height;

	if(g_graphics_vars != NULL){
		Log_Error("Graphics already initialized");
		return false;
	}

	g_graphics_vars = NEW(GraphicsVars);
	g_graphics_vars->capture_screen_callbacks=List_New();

	uint32_t video_flags = 0;
	bool ok=false;

	if (SDL_Init(0 ) < 0) {
		Log_Error("Unable to init SDL: %s\n", SDL_GetError());
		return false;
	}

	g_graphics_vars->graphics_api = _video_context;
	g_graphics_vars->sdl_window = NULL;

	if (SDL_WasInit(SDL_INIT_VIDEO) != SDL_INIT_VIDEO) {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
			Log_Error("Unable to init video subsystem: %s", SDL_GetError());
			return false;
		}
	}

	if(properties & MSK_GRAPHIC_PROPERTY_DESKTOP){
		SDL_DisplayMode current;
		if(SDL_GetCurrentDisplayMode(0, &current) != 0){
			Log_Error("Unable to get display mode: %s", SDL_GetError());
			return false;
		}
		_window_width=current.w;
		_window_height=current.h;
	}


	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		// set video_flags as opengl
		video_flags|=SDL_WINDOW_OPENGL;
		break;
	}

	// main window is always created at the main screen...
	g_graphics_vars->sdl_window = SDL_CreateWindow(
			caption
			, start_posx
			, start_posy
			, _window_width
			, _window_height
			, video_flags);
	if (!g_graphics_vars->sdl_window) {
		Log_Error("Unable to create window: %s", SDL_GetError());
		return false;
	}

	g_graphics_vars->sdl_renderer = SDL_GetRenderer(g_graphics_vars->sdl_window);
	g_graphics_vars->sdl_window_surface = SDL_GetWindowSurface(g_graphics_vars->sdl_window);
	g_graphics_vars->bytes_per_pixel = g_graphics_vars->sdl_window_surface->format->BytesPerPixel;
	g_graphics_vars->width = _width;
	g_graphics_vars->height = _height;
	g_graphics_vars->one_over_width=1.0f/(g_graphics_vars->width);
	g_graphics_vars->one_over_height=1.0f/(g_graphics_vars->height);
	g_graphics_vars->aspect_ratio=((float)_width/(float)_height);
	g_graphics_vars->one_over_aspect_ratio=1.0f/g_graphics_vars->aspect_ratio;
	g_graphics_vars->scale = Vector2f_New2f(1,1);

	// set logical res if different from screen res
	if(
			_width  != _window_width
		|| 	_height != _window_height
	){

		g_graphics_vars->scale = Vector2f_New2f(
				(float)_width/(float)_window_width
				,(float)_height/(float)_window_height
		);
	}

	Log_Info("Created main window %ix%i (%ibpp)", _window_width,_window_height, g_graphics_vars->sdl_window_surface->format->BitsPerPixel);
	Log_Info("SDL version: %02i.%02i.%02i",SDL_MAJOR_VERSION,SDL_MINOR_VERSION,SDL_PATCHLEVEL);

	//Input_SetupCursors();
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		ok=Graphics_GL_Init();
		break;
	}

	// init gl vars
	g_graphics_vars->geometry_quad_default=Geometry_NewQuad(GEOMETRY_TEXTURE);

	// adapter list
	g_graphics_vars->adapters=NULL;

#ifdef _WIN32
	g_graphics_vars->adapters=Graphics_Win32_GetListAdapters();
#endif

	Graphics_PrintAdapterInformation();

	TTFont_Init();
	Texture_Init();
	IconManager_Init();
	ViewPort_Init(_width,_height);

	return ok;
}

Vector2f Graphics_GetScale(){
	return g_graphics_vars->scale;
}

void Graphics_SetProjectionMode(PROJECTION_MODE projection_mode){
	//Input_SetupCursors();
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_SetProjectionMode(projection_mode);
		break;

	}
}

void Graphics_SetCameraTransform(Transform *transform){
	switch(Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			Graphics_GL_SetCameraTransform(transform);
			break;
	}
}

GraphicsApi Graphics_GetGraphicsApi(void){
	return g_graphics_vars->graphics_api;
}


void Graphics_PrintVideoInfo(void){
	// Declare display mode structure to be filled in.
	SDL_DisplayMode current;

	int n_displays = SDL_GetNumVideoDisplays();
	for( int i = 0 ; i < n_displays ; i++ )
	{
		SDL_Rect rect_display;
	    SDL_GetDisplayBounds( i, &rect_display);
	    SDL_GetCurrentDisplayMode(i, &current);

	    Log_Info("* Detected %i monitor [%i %i %i %i %i]",
	    		i+1,
				rect_display.x,
				rect_display.y,
				rect_display.w,
				rect_display.h,
	    		current.format

	    		);
	}
}

unsigned Graphics_GetNumMonitors(void){
	return SDL_GetNumVideoDisplays();
}

float CGraphics_GetAdapterPhysicalWidth(unsigned idx_monitor){
	if(idx_monitor >= g_graphics_vars->adapters->count){
		Log_Error("%i monitor out of bounds. (Max monitors:%i)",g_graphics_vars->adapters->count);
		return 0;
	}

	return ((AdapterInfo *)g_graphics_vars->adapters->items[idx_monitor])->physical_width;


}

float CGraphics_GetAdapterPhysicalHeight(unsigned idx_monitor){
	if(idx_monitor >= g_graphics_vars->adapters->count){
		Log_Error("%i monitor out of bounds. (Max monitors:%i)",g_graphics_vars->adapters->count);
		return 0;
	}

	return ((AdapterInfo *)g_graphics_vars->adapters->items[idx_monitor])->physical_height;
}

const char *CGraphics_GetAdapterMonitorModel(unsigned idx_monitor){
	if(idx_monitor >= g_graphics_vars->adapters->count){
		Log_Error("%i monitor out of bounds. (Max monitors:%i)",g_graphics_vars->adapters->count);
		return 0;
	}

	return ((AdapterInfo *)g_graphics_vars->adapters->items[idx_monitor])->monitor_model;
}

static void Graphics_PrintAdapterInformation(void){


	if( g_graphics_vars->adapters == NULL || g_graphics_vars->adapters->count==0){
		Log_Info("No adapters information");
		return;
	}

	for(unsigned i = 0;i < g_graphics_vars->adapters->count; i++){
		AdapterInfo *adapter=(void *)g_graphics_vars->adapters->items[i];
		Log_Info("--------------------------------------");
		Log_Info("MonitorModel: %s",adapter->monitor_model);
		Log_Info("Width: %i",adapter->pixels_width);
		Log_Info("Height: %i",adapter->pixels_height);
		Log_Info("PhysicalWidth: %f",adapter->physical_width);
		Log_Info("PhysicalHeight: %f",adapter->physical_height);

	}
	Log_Info("--------------------------------------");
}



/// Queries the Screen to see if it's set to Fullscreen or Not
/// @return SDL_FALSE if windowed, SDL_TRUE if fullscreen
bool Graphics_IsFullScreen(void) {

	uint32_t flags = SDL_GetWindowFlags(g_graphics_vars->sdl_window);

	if (flags & SDL_WINDOW_FULLSCREEN)
		return SDL_TRUE; // return SDL_TRUE if fullscreen

	return false; // Return SDL_FALSE if windowed
}


void Graphics_ToggleFullscreen(void)
{
	if (SDL_GetWindowFlags(g_graphics_vars->sdl_window) & SDL_WINDOW_FULLSCREEN) {

		// Swith to WINDOWED mode
		if (SDL_SetWindowFullscreen(g_graphics_vars->sdl_window, SDL_FALSE) < 0)
	  {
			Log_Error("Setting windowed failed : %s",SDL_GetError());

	  }
	} else {
		// Swith to FULLSCREEN mode
		if (SDL_SetWindowFullscreen(g_graphics_vars->sdl_window, SDL_TRUE) < 0)
		{
			Log_Error("Setting fullscreen failed : %s", SDL_GetError());

		}
	}
}

// main screen starts at 0,0
bool Graphics_GetScreenBounds(int nudisplay, SDL_Rect * rect){
	int totalDisplays = SDL_GetNumVideoDisplays();

	if(nudisplay >= totalDisplays){
		return false;
	}

	SDL_GetDisplayBounds( nudisplay, rect);

	return true;
}

void Graphics_PrintGraphicsInfo(void){

	  // Declare display mode structure to be filled in.
	 SDL_DisplayMode current;

	int numdisplays = SDL_GetNumVideoDisplays();

	for( int i = 0 ; i < numdisplays ; i++ )
	{
		SDL_Rect screen_rect;
		SDL_GetDisplayBounds( i, &screen_rect);
		SDL_GetCurrentDisplayMode(i, &current);

		Log_Info("Detected %i monitor [%i %i %i %i %i]",
				i+1,
				screen_rect.x,
				screen_rect.y,
				screen_rect.w,
				screen_rect.h,
				current.format
		);
	}
}


uint16_t Graphics_GetWidth(void){
	return g_graphics_vars->width;
}
uint16_t Graphics_GetHeight(void){
	return g_graphics_vars->height;
}

float Graphics_GetOneOverWidth(void){
	return g_graphics_vars->one_over_width;
}

float Graphics_GetOneOverHeight(void){
	return g_graphics_vars->one_over_height;
}

uint8_t Graphics_GetBytesPerPixel(void){
	return g_graphics_vars->bytes_per_pixel;
}

float Graphics_GetAspectRatio(void){
	return g_graphics_vars->aspect_ratio;
}

float Graphics_GetOneOverAspectRatio(void){
	return g_graphics_vars->one_over_aspect_ratio;
}

void Graphics_SetColorBackground3i(Color4f c){
	g_graphics_vars->background_color = c;
}

void Graphics_ClearScreen(Color4f color){
	g_graphics_vars->start_ms=SDL_GetTicks();
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_ClearScreen(color);
		break;
	}
}

void Graphics_BeginRender(void){
	g_graphics_vars->start_ms=SDL_GetTicks();
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_StartRender();
		break;
	}
}

void Graphics_SetBackgroundColor(Color4f color){
	g_graphics_vars->background_color=color;
}

void Graphics_AddCaptureScreenCallback(Callback fun){

}

void Graphics_EndRender(void)
{
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_EndRender();
		break;
	}

	if(g_graphics_vars->screenshoot_request || g_graphics_vars->capture_screen_callbacks->count > 0) {

		SDL_Surface *srf_screen_shoot=NULL;
		switch(g_graphics_vars->graphics_api){
			case GRAPHICS_API_GL:
				srf_screen_shoot=Graphics_GL_ScreenShoot();
				break;

		}

		if(srf_screen_shoot != NULL){

			if(g_graphics_vars->screenshoot_request){
				char buffer[100];
				sprintf(buffer,"screenshoot_%03i.png",g_graphics_vars->n_screenshoot++);
				SDL_SavePNG(buffer,srf_screen_shoot);
				g_graphics_vars->screenshoot_request = false;
			}
			else {
				for(unsigned i=0; i < g_graphics_vars->capture_screen_callbacks->count; i++){
					Callback *c=g_graphics_vars->capture_screen_callbacks->items[i];
					if(c->ptr_function){
						c->ptr_function(srf_screen_shoot,NULL);
					}
				}

				List_ClearAndFreeAllItems(g_graphics_vars->capture_screen_callbacks);
			}

			SDL_FreeSurface(srf_screen_shoot);
		}

	}



#ifdef __SHOW_FPS__

	if(g_graphics_vars->elapsed_seconds<SDL_GetTicks()){
		uint32_t diff=SDL_GetTicks()-g_graphics_vars->start_ms;

		if(diff>0){
			printf("%.2f fps\n",1000.0f/diff);
		}

		g_graphics_vars->elapsed_seconds=SDL_GetTicks()+1000;
	}
#endif
}
//---------------------------------------------------------------------------------------------------------------------------
//
// Drawing functions
//

void Graphics_DrawRectangle(int x, int y, uint16_t width, uint16_t height, uint8_t thickness, Color4f color){

	Vector2i p1_2d=Vector2i_New2i(x,y);
	Vector2i p2_2d=Vector2i_New2i(x+width,y+height);

	Vector3f p1_3d=ViewPort_ScreenToWorld(p1_2d.x,p1_2d.y);
	Vector3f p2_3d=ViewPort_ScreenToWorld(p2_2d.x,p2_2d.y);


	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_DrawRectangle4f(p1_3d.x,p1_3d.y,p2_3d.x,p2_3d.y,thickness,color);
	}
}

void Graphics_DrawRectangle4f(float x1, float y1, float x2, float y2, uint8_t thickness, Color4f color){
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_DrawRectangle4f(x1,y1,x2,y2,thickness,color);
	}
}

void Graphics_DrawRectangleFilled(int x, int y, uint16_t width, uint16_t height, Color4f color){

	Vector2i p1_2d=Vector2i_New2i(x,y);
	Vector2i p2_2d=Vector2i_New2i(x+width,y+height);

	Vector3f p1_3d=ViewPort_ScreenToWorld(p1_2d.x,p1_2d.y);
	Vector3f p2_3d=ViewPort_ScreenToWorld(p2_2d.x,p2_2d.y);


	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_DrawRectangleFilled4f(p1_3d.x,p1_3d.y,p2_3d.x,p2_3d.y,color);
	}
}

void Graphics_DrawRectangleFilled4f(float x1, float y1, float x2, float y2, Color4f color){
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_DrawRectangleFilled4f(x1,y1,x2,y2,color);
	}
}

void Graphics_DrawRectangleTextured(int x, int y, uint16_t width, uint16_t height, Color4f color, Texture *text, Rectanglef * text_crop){


	Vector2i p1_2d=Vector2i_New2i(x,y);
	Vector2i p2_2d=Vector2i_New2i(x+width,y+height);

	Vector3f p1_3d=ViewPort_ScreenToWorld(p1_2d.x,p1_2d.y);
	Vector3f p2_3d=ViewPort_ScreenToWorld(p2_2d.x,p2_2d.y);

	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_DrawRectangleTextured4f(p1_3d.x,p1_3d.y,p2_3d.x,p2_3d.y,color,text,text_crop);
	}
}

void Graphics_DrawRectangleTextured4f(float x1, float y1, float x2, float y2,  Color4f color,Texture *text, Rectanglef * text_crop){
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_DrawRectangleTextured4f(x1,y1,x2,y2,color,text, text_crop);
	}
}

void Graphics_Draw(Transform *transform, Geometry *geometry, Appearance *appearance){
	if(transform)	Transform_Apply(transform);
	if(appearance)  Appearance_Apply(appearance);

	if(geometry) 	Geometry_Draw(geometry);

	if(appearance) 	Appearance_Restore(appearance);
	if(transform) 	Transform_Restore(transform);
}



void Graphics_Print(int x, int y, Color4f color, const char *in, ...){

	char out[1024]={0};
	ZG_VARGS(out,in);

	TTFont_RenderTextBegin(NULL);
	TTFont_Print(TTFont_GetDefaultFont(),x,y,color,out);
	TTFont_RenderTextEnd();

}

void Graphics_WPrint(int x, int y, Color4f color, const wchar_t *in, ...){

	wchar_t out[1024]={0};
	ZG_WVARGS(out,in);

	TTFont_RenderTextBegin(NULL);
	TTFont_WPrint(TTFont_GetDefaultFont(),x,y,color,out);
	TTFont_RenderTextEnd();
}

//---------------------------------------------------------------------------------------------------------------------------

void Graphics_DeInit(void) {

	if(g_graphics_vars == NULL){
		Log_Error("Graphics not initialized");
		return;
	}

	IconManager_DeInit();
	Texture_DeInit();
	Geometry_DeInit();

	TTFont_DeInit();
	ViewPort_DeInit();


	// deinit gl vars first
	Geometry_Delete(g_graphics_vars->geometry_quad_default);

	// ...then deini gl context
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		Graphics_GL_DeInit();
		break;
	}

	if(g_graphics_vars->sdl_window!=NULL){
		SDL_DestroyWindow(g_graphics_vars->sdl_window);
	}

	// Remove adapter lists
	List_DeleteAndFreeAllItems(g_graphics_vars->adapters);
	List_DeleteAndFreeAllItems(g_graphics_vars->capture_screen_callbacks);


	FREE(g_graphics_vars);
}

