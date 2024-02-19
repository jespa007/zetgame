#include "@zg_graphics.h"

typedef struct
{
	int 	pixels_width, pixels_height;
	float 	physical_width, physical_height;
	char 	monitor_model[1024];
} ZG_AdapterInfo;

typedef struct{
	 uint8_t 		bytes_per_pixel;
	 uint16_t     width,height;
	 ZG_Vector2f 	scale;
	 float		one_over_width
	 	 	 	,one_over_height
				,aspect_ratio
				,one_over_aspect_ratio;

	 float 		fps;

	 //ZG_ViewPort  * view_port;

	 ZG_Color4f 			background_color;
	 SDL_Window		* 	sdl_window;
	 SDL_Renderer 	*	sdl_renderer;
	 SDL_Surface 	*	sdl_window_surface;

	 bool stated_render;
	 bool screenshoot_request;
	 void * graphics_data;
	 ZG_GraphicsApi graphics_api;
	 uint32_t 	start_ms;
	 uint32_t 	elapsed_seconds;
	 ZG_List	*adapters;
	 ZG_List   *capture_screen_callbacks;
	 int 	n_screenshoot;

	ZG_Geometry *default_geometry_textured_rectangle2d;
	ZG_Appearance *default_appearance_rectangle2d;
	ZG_Material *default_material_rectangle2d;
	SDL_Rect *rect_display;
	int num_displays;
	int active_display;
	bool fullscreen;
	ZG_ProjectionMode projection_mode;

}ZG_GraphicsVars;


ZG_GraphicsVars *g_graphics_vars=NULL;

#include "ZG_Graphics_GL.c"
#include "ZG_Graphics_Win32.c"

static void ZG_Graphics_PrintAdapterInformation(void);

bool ZG_Graphics_Init(
		int start_posx,int start_posy	// start position
		, uint16_t _width, uint16_t _height // logical resolution
		//, uint16_t _window_width, uint16_t _window_height // screen resolution
		,ZG_GraphicsApi _video_context
		,const char *_caption
		,uint16_t _properties
	) {

	uint16_t _window_width=_width;
	uint16_t _window_height=_height;

	if(g_graphics_vars != NULL){
		ZG_LOG_ERRORF("Graphics already initialized");
		return false;
	}

	g_graphics_vars = ZG_NEW(ZG_GraphicsVars);
	g_graphics_vars->capture_screen_callbacks=ZG_List_New();

	uint32_t video_flags = 0;


	if (SDL_Init(0 ) < 0) {
		ZG_LOG_ERROR("Unable to init SDL: %s\n", SDL_GetError());
		return false;
	}

	g_graphics_vars->graphics_api = _video_context;
	g_graphics_vars->sdl_window = NULL;

	if (SDL_WasInit(SDL_INIT_VIDEO) != SDL_INIT_VIDEO) {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
			ZG_LOG_ERROR("Unable to init video subsystem: %s", SDL_GetError());
			return false;
		}
	}

	g_graphics_vars->num_displays=SDL_GetNumVideoDisplays();
	if(g_graphics_vars->num_displays < 0){
		ZG_LOG_ERROR("SDL_GetNumVideoDisplays error : %s\n", SDL_GetError());
		return false;
	}
	g_graphics_vars->rect_display=(SDL_Rect *)malloc(sizeof(SDL_Rect)*g_graphics_vars->num_displays);
	for(int i=0 ; i < g_graphics_vars->num_displays; i++){
		if(SDL_GetDisplayBounds(i, &g_graphics_vars->rect_display[i])!=0){
			ZG_LOG_ERROR("SDL_GetDisplayBounds error : %s\n", SDL_GetError());
			return false;
		}
	}

	if(_properties & ZG_MSK_GRAPHIC_PROPERTY_FULLSCREEN){

		/*SDL_DisplayMode current;
		if(SDL_GetCurrentDisplayMode(0, &current) != 0){
			ZG_LOG_ERROR("Unable to get display mode: %s", SDL_GetError());
			return false;
		}
		_window_width=current.w;
		_window_height=current.h;*/
		g_graphics_vars->fullscreen=true;
	}



	switch(g_graphics_vars->graphics_api){
	case ZG_GRAPHICS_API_GL:
		// set video_flags as opengl
		video_flags|=SDL_WINDOW_OPENGL;
		break;
	}

	// main window is always created at the main screen...
	g_graphics_vars->sdl_window = SDL_CreateWindow(
			_caption
			,g_graphics_vars->fullscreen?g_graphics_vars->rect_display[g_graphics_vars->active_display].x:g_graphics_vars->rect_display[g_graphics_vars->active_display].x+(g_graphics_vars->rect_display[g_graphics_vars->active_display].w>>1)
			,g_graphics_vars->fullscreen?g_graphics_vars->rect_display[g_graphics_vars->active_display].y-1:g_graphics_vars->rect_display[g_graphics_vars->active_display].y+(g_graphics_vars->rect_display[g_graphics_vars->active_display].h>>1)
			,g_graphics_vars->fullscreen?g_graphics_vars->rect_display[g_graphics_vars->active_display].w:_width
			,g_graphics_vars->fullscreen?g_graphics_vars->rect_display[g_graphics_vars->active_display].h:_height
			, video_flags);
	if (!g_graphics_vars->sdl_window) {
		ZG_LOG_ERROR("Unable to create window: %s", SDL_GetError());
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
	g_graphics_vars->scale=ZG_Vector2f_New(1,1);


	ZG_LOG_INFO("Created main window %ix%i (%ibpp)", _window_width,_window_height, g_graphics_vars->sdl_window_surface->format->BitsPerPixel);
	ZG_LOG_INFO("SDL version: %02i.%02i.%02i",SDL_MAJOR_VERSION,SDL_MINOR_VERSION,SDL_PATCHLEVEL);

	//ZG_Input_SetupCursors();
	switch(g_graphics_vars->graphics_api){
	case ZG_GRAPHICS_API_GL:
		if(ZG_Graphics_GL_Init()==false){
			return false;
		}
		break;
	}

	// select 2d model view as default...
	ZG_Graphics_SetProjectionMode(ZG_PROJECTION_MODE_ORTHO);


	// adapter list
	g_graphics_vars->adapters=NULL;

#ifdef _WIN32
	g_graphics_vars->adapters=ZG_Graphics_Win32_GetListAdapters();
#endif

	ZG_Graphics_PrintAdapterInformation();

	ZG_TTFontManager_Init();
	ZG_IconManager_Init();
	ZG_ViewPort_Init(_width,_height);

	// created default rectangle/s for drawing
	g_graphics_vars->default_geometry_textured_rectangle2d=ZG_Geometry_NewTexturedRectangle2d();
	g_graphics_vars->default_appearance_rectangle2d=ZG_Appearance_New();
	g_graphics_vars->default_material_rectangle2d=ZG_Material_New(0);
	g_graphics_vars->default_appearance_rectangle2d->material=g_graphics_vars->default_material_rectangle2d;

	ZG_Graphics_SetFullscreen(g_graphics_vars->fullscreen);


	return true;
}

ZG_Vector2f ZG_Graphics_GetScale(){
	return g_graphics_vars->scale;
}

void ZG_Graphics_SetProjectionMode(ZG_ProjectionMode _projection_mode){
	//ZG_Input_SetupCursors();
	g_graphics_vars->projection_mode=_projection_mode;

	switch(g_graphics_vars->graphics_api){
	case ZG_GRAPHICS_API_GL:
		ZG_Graphics_GL_SetProjectionMode(_projection_mode);
		break;

	}
}

void ZG_Graphics_SetCameraTransform(ZG_Transform *transform){
	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Graphics_GL_SetCameraTransform(transform);
			break;
	}
}

void ZG_Graphics_SetLineThickness(uint8_t _thickness){
	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Graphics_GL_SetLineThickness(_thickness);
			break;
	}
}

void ZG_Graphics_SetPointSize(uint8_t _point_size){
	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Graphics_GL_SetPointSize(_point_size);
			break;
	}
}

void ZG_Graphics_SetColor4f(float _r, float _g, float _b, float _a){
		switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case ZG_GRAPHICS_API_GL:
			ZG_Graphics_GL_SetColor4f(_r,_g,_b,_a);
			break;
	}
}

ZG_GraphicsApi ZG_Graphics_GetGraphicsApi(void){
	return g_graphics_vars->graphics_api;
}


void ZG_Graphics_PrintVideoInfo(void){
	// Declare display mode structure to be filled in.
	SDL_DisplayMode current;

	int n_displays = SDL_GetNumVideoDisplays();
	for( int i = 0 ; i < n_displays ; i++ )
	{
		SDL_Rect rect_display;
	    SDL_GetDisplayBounds( i, &rect_display);
	    SDL_GetCurrentDisplayMode(i, &current);

	    ZG_LOG_INFO("* Detected %i monitor [%i %i %i %i %i]",
	    		i+1,
				rect_display.x,
				rect_display.y,
				rect_display.w,
				rect_display.h,
	    		current.format

	    		);
	}
}

unsigned ZG_Graphics_GetNumMonitors(void){
	return SDL_GetNumVideoDisplays();
}

float ZG_Graphics_GetAdapterPhysicalWidth(unsigned idx_monitor){
	if(idx_monitor >= g_graphics_vars->adapters->count){
		ZG_LOG_ERROR("%i monitor out of bounds. (Max monitors:%i)",g_graphics_vars->adapters->count);
		return 0;
	}

	return ((ZG_AdapterInfo *)g_graphics_vars->adapters->items[idx_monitor])->physical_width;
}

float ZG_Graphics_GetAdapterPhysicalHeight(unsigned idx_monitor){
	if(idx_monitor >= g_graphics_vars->adapters->count){
		ZG_LOG_ERROR("%i monitor out of bounds. (Max monitors:%i)",g_graphics_vars->adapters->count);
		return 0;
	}

	return ((ZG_AdapterInfo *)g_graphics_vars->adapters->items[idx_monitor])->physical_height;
}

const char *ZG_Graphics_GetAdapterMonitorModel(unsigned idx_monitor){
	if(idx_monitor >= g_graphics_vars->adapters->count){
		ZG_LOG_ERROR("%i monitor out of bounds. (Max monitors:%i)",g_graphics_vars->adapters->count);
		return 0;
	}

	return ((ZG_AdapterInfo *)g_graphics_vars->adapters->items[idx_monitor])->monitor_model;
}

static void ZG_Graphics_PrintAdapterInformation(void){


	if( g_graphics_vars->adapters == NULL || g_graphics_vars->adapters->count==0){
		ZG_LOG_INFOF("No adapters information");
		return;
	}

	for(unsigned i = 0;i < g_graphics_vars->adapters->count; i++){
		ZG_AdapterInfo *adapter=(void *)g_graphics_vars->adapters->items[i];
		ZG_LOG_INFOF("--------------------------------------");
		ZG_LOG_INFO("MonitorModel: %s",adapter->monitor_model);
		ZG_LOG_INFO("Width: %i",adapter->pixels_width);
		ZG_LOG_INFO("Height: %i",adapter->pixels_height);
		ZG_LOG_INFO("PhysicalWidth: %f",adapter->physical_width);
		ZG_LOG_INFO("PhysicalHeight: %f",adapter->physical_height);

	}
	ZG_LOG_INFOF("--------------------------------------");
}



/// Queries the Screen to see if it's set to Fullscreen or Not
/// @return SDL_FALSE if windowed, SDL_TRUE if fullscreen
/*bool ZG_Graphics_IsFullScreen(void) {

	uint32_t flags = SDL_GetWindowFlags(g_graphics_vars->sdl_window);

	if (flags & SDL_WINDOW_FULLSCREEN)
		return SDL_TRUE; // return SDL_TRUE if fullscreen

	return false; // Return SDL_FALSE if windowed
}


void ZG_Graphics_ToggleFullscreen(void)
{
	if (SDL_GetWindowFlags(g_graphics_vars->sdl_window) & SDL_WINDOW_FULLSCREEN) {

		// Swith to WINDOWED mode
		if (SDL_SetWindowFullscreen(g_graphics_vars->sdl_window, SDL_FALSE) < 0)
	  {
			ZG_LOG_ERROR("Setting windowed failed : %s",SDL_GetError());

	  }
	} else {
		// Swith to FULLSCREEN mode
		if (SDL_SetWindowFullscreen(g_graphics_vars->sdl_window, SDL_TRUE) < 0)
		{
			ZG_LOG_ERROR("Setting fullscreen failed : %s", SDL_GetError());

		}
	}
}*/
bool ZG_Graphics_IsFullscreen(void){
	return g_graphics_vars->fullscreen;
}

void ZG_Graphics_ToggleFullscreen(void){
	ZG_Graphics_SetFullscreen(!g_graphics_vars->fullscreen);
}

void 	ZG_Graphics_SetFullscreen(bool _fullscreen){

	// if request is the same
	if(g_graphics_vars->fullscreen == _fullscreen){
		return;
	}

	g_graphics_vars->scale = ZG_Vector2f_New(
		1
		,1
	);

	if(_fullscreen){
		int wx=0,wy=0;
		SDL_GetWindowPosition(g_graphics_vars->sdl_window,&wx,&wy);
		g_graphics_vars->active_display=-1;
		for(int i=0; (i < g_graphics_vars->num_displays) && (g_graphics_vars->active_display==-1);i++){
			if(g_graphics_vars->rect_display[i].x<=wx && wx<(g_graphics_vars->rect_display[i].x+g_graphics_vars->rect_display[i].w)){
				g_graphics_vars->active_display=i;
			}
		}


		//SDL_SetWindowBordered( window, SDL_FALSE );
		SDL_SetWindowFullscreen(g_graphics_vars->sdl_window,SDL_WINDOW_FULLSCREEN_DESKTOP);
		// FULLSCREEN
		//calculeScaleFactors();
		g_graphics_vars->scale = ZG_Vector2f_New(
			(float)g_graphics_vars->width/(float)g_graphics_vars->rect_display[g_graphics_vars->active_display].w
			,(float)g_graphics_vars->height/(float)g_graphics_vars->rect_display[g_graphics_vars->active_display].h
		);

		SDL_SetWindowPosition(
			g_graphics_vars->sdl_window
			, g_graphics_vars->rect_display[g_graphics_vars->active_display].x
			, g_graphics_vars->rect_display[g_graphics_vars->active_display].y-1
		);

		/*SDL_SetWindowSize(
			window
			, rect_display.w
			,rect_display.h
		);*/

	}else{
		// WINDOWED
		//SDL_SetWindowBordered( window, SDL_TRUE );
		SDL_SetWindowFullscreen(g_graphics_vars->sdl_window,0);


		SDL_SetWindowPosition(
			g_graphics_vars->sdl_window
			, g_graphics_vars->rect_display[g_graphics_vars->active_display].x+(g_graphics_vars->rect_display[g_graphics_vars->active_display].w>>1)-(g_graphics_vars->width>>1)
			,g_graphics_vars->rect_display[g_graphics_vars->active_display].y+(g_graphics_vars->rect_display[g_graphics_vars->active_display].h>>1)-(g_graphics_vars->height>>1)
		);

		SDL_SetWindowSize(
			g_graphics_vars->sdl_window
			, g_graphics_vars->width
			,g_graphics_vars->height
		);


	}

	ZG_Graphics_SetProjectionMode(g_graphics_vars->projection_mode);
	g_graphics_vars->fullscreen=_fullscreen;
}

// main screen starts at 0,0
bool ZG_Graphics_GetScreenBounds(int nudisplay, SDL_Rect * rect){
	int totalDisplays = SDL_GetNumVideoDisplays();

	if(nudisplay >= totalDisplays){
		return false;
	}

	SDL_GetDisplayBounds( nudisplay, rect);

	return true;
}

void ZG_Graphics_PrintGraphicsInfo(void){

	  // Declare display mode structure to be filled in.
	 SDL_DisplayMode current;

	int numdisplays = SDL_GetNumVideoDisplays();

	for( int i = 0 ; i < numdisplays ; i++ )
	{
		SDL_Rect screen_rect;
		SDL_GetDisplayBounds( i, &screen_rect);
		SDL_GetCurrentDisplayMode(i, &current);

		ZG_LOG_INFO("Detected %i monitor [%i %i %i %i %i]",
				i+1,
				screen_rect.x,
				screen_rect.y,
				screen_rect.w,
				screen_rect.h,
				current.format
		);
	}
}

uint16_t ZG_Graphics_GetWindowWidth(void){
	int w,h;
	SDL_GetWindowSize(g_graphics_vars->sdl_window,&w,&h);
	return w;
}
uint16_t ZG_Graphics_GetWindowHeight(void){
	int w,h;
	SDL_GetWindowSize(g_graphics_vars->sdl_window,&w,&h);
	return h;
}


uint16_t ZG_Graphics_GetWidth(void){
	return g_graphics_vars->width;
}
uint16_t ZG_Graphics_GetHeight(void){
	return g_graphics_vars->height;
}

float ZG_Graphics_GetOneOverWidth(void){
	return g_graphics_vars->one_over_width;
}

float ZG_Graphics_GetOneOverHeight(void){
	return g_graphics_vars->one_over_height;
}

uint8_t ZG_Graphics_GetBytesPerPixel(void){
	return g_graphics_vars->bytes_per_pixel;
}

float ZG_Graphics_GetAspectRatio(void){
	if(g_graphics_vars->projection_mode==ZG_PROJECTION_MODE_ORTHO){
		return 1;
	}

	return g_graphics_vars->aspect_ratio;
}

float ZG_Graphics_GetOneOverAspectRatio(void){
	if(g_graphics_vars->projection_mode==ZG_PROJECTION_MODE_ORTHO){
		return 1;
	}

	return g_graphics_vars->one_over_aspect_ratio;
}

void ZG_Graphics_SetBackgroundColor3i(ZG_Color4f c){
	g_graphics_vars->background_color = c;
}

void ZG_Graphics_MakeScreenShot(void){
	g_graphics_vars->screenshoot_request=true;
}

void ZG_Graphics_ClearScreen(ZG_Color4f color){
	g_graphics_vars->start_ms=SDL_GetTicks();
	switch(g_graphics_vars->graphics_api){
	case ZG_GRAPHICS_API_GL:
		ZG_Graphics_GL_ClearScreen(color);
		break;
	}
}

void ZG_Graphics_BeginRender(void){
	g_graphics_vars->start_ms=SDL_GetTicks();
	switch(g_graphics_vars->graphics_api){
	case ZG_GRAPHICS_API_GL:
		ZG_Graphics_GL_StartRender();
		break;
	}
}

void ZG_Graphics_SetBackgroundColor(ZG_Color4f color){
	g_graphics_vars->background_color=color;
}
/*
void ZG_Graphics_AddCaptureScreenCallback(ZG_Callback fun){

}
*/
void ZG_Graphics_EndRender(void)
{

#ifdef __DEBUG__

	//if(g_graphics_vars->elapsed_seconds<SDL_GetTicks()){
		uint32_t diff=SDL_GetTicks()-g_graphics_vars->start_ms;
		float FPS=60.0f;
		if(diff > 16){
			FPS=1000.0f/diff;
		}

		ZG_Graphics_Print(5,ZG_Graphics_GetHeight()-5,ZG_COLOR4F_WHITE, "FPS: %.02f",FPS);
//			printf("%.2f fps\n",1000.0f/diff);


		//g_graphics_vars->elapsed_seconds=SDL_GetTicks()+1000;
	//}
#endif


	switch(g_graphics_vars->graphics_api){
	case ZG_GRAPHICS_API_GL:
		ZG_Graphics_GL_EndRender();
		break;
	}

	if(g_graphics_vars->screenshoot_request || g_graphics_vars->capture_screen_callbacks->count > 0) {

		SDL_Surface *srf_screen_shoot=NULL;
		switch(g_graphics_vars->graphics_api){
			case ZG_GRAPHICS_API_GL:
				srf_screen_shoot=ZG_Graphics_GL_ScreenShoot();
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
					ZG_Callback *c=g_graphics_vars->capture_screen_callbacks->items[i];
					if(c->ptr_function){
						c->ptr_function(srf_screen_shoot,NULL);
					}
				}

				ZG_List_ClearAndFreeAllItems(g_graphics_vars->capture_screen_callbacks);
			}

			SDL_FreeSurface(srf_screen_shoot);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
//
// Drawing functions
//

void ZG_Graphics_Draw(ZG_Transform *transform, ZG_Geometry *geometry, ZG_Appearance *appearance){
	if(transform)	ZG_Transform_Apply(transform);
	if(appearance)  ZG_Appearance_Apply(appearance);

	if(geometry) 	ZG_Geometry_Draw(geometry);

	if(appearance) 	ZG_Appearance_Restore(appearance);
	if(transform) 	ZG_Transform_Restore(transform);
}

void ZG_Graphics_DrawPoint2f(float _x, float _y, ZG_Color4f _color, uint8_t _point_size){
	ZG_Transform t=ZG_Transform_New();
	t.translate.x=_x;
	t.translate.y=_y;

	ZG_Graphics_SetColor4f(_color.r,_color.b, _color.g, _color.a);
	ZG_Graphics_SetPointSize(_point_size);
	ZG_Transform_Apply(&t);
	ZG_Geometry_Draw(ZG_Geometry_GetDefaultPoint());
	ZG_Transform_Restore(&t);
}

void ZG_Graphics_DrawPoint2i(int _x, int _y, ZG_Color4f _color, uint8_t _point_size){
	ZG_Graphics_DrawPoint2f(
		ZG_ViewPort_ScreenToWorldPositionX(_x)
		,ZG_ViewPort_ScreenToWorldPositionY(_y)
		,_color
		,_point_size
	);
}


void ZG_Graphics_DrawRectangle4i(int _center_x, int _center_y, uint16_t _width, uint16_t _height, ZG_Color4f _color, uint8_t _thickness){


	ZG_Vector3f translate=ZG_ViewPort_ScreenToWorld(_center_x,_center_y);
	ZG_Vector3f scale=ZG_ViewPort_ScreenToWorldDimension2i(_width,_height);

	ZG_Graphics_DrawRectangle4f(translate.x,translate.y,scale.x,scale.y,_color,_thickness);
}

void ZG_Graphics_DrawRectangle4f(float _center_x, float _center_y, float _scale_x, float _scale_y, ZG_Color4f _color, uint8_t _thickness){
	ZG_Transform t=ZG_Transform_New();
	t.translate.x=_center_x;
	t.translate.y=_center_y;
	t.scale.x=_scale_x;
	t.scale.y=_scale_y;


	ZG_Graphics_SetColor4f(_color.r,_color.b, _color.g, _color.a);
	ZG_Graphics_SetLineThickness(_thickness);
	ZG_Transform_Apply(&t);
	ZG_Geometry_Draw(ZG_Geometry_GetDefaultRectangle2d());
	ZG_Transform_Restore(&t);
}

void ZG_Graphics_DrawFilledRectangle4i(int _center_x, int _center_y, uint16_t _width, uint16_t _height, ZG_Color4f color){

	ZG_Vector3f translate=ZG_ViewPort_ScreenToWorld(_center_x,_center_y);
	ZG_Vector3f scale=ZG_ViewPort_ScreenToWorldDimension2i(_width,_height);

	ZG_Graphics_DrawFilledRectangle4f(translate.x,translate.y,scale.x,scale.y,color);
}

void ZG_Graphics_DrawFilledRectangle4f(float _center_x, float _center_y, float _scale_x, float _scale_y, ZG_Color4f _color){
	ZG_Transform t=ZG_Transform_New();
	t.translate.x=_center_x;
	t.translate.y=_center_y;
	t.scale.x=_scale_x;
	t.scale.y=_scale_y;

	ZG_Graphics_SetColor4f(_color.r,_color.b, _color.g, _color.a);
	ZG_Transform_Apply(&t);
	ZG_Geometry_Draw(ZG_Geometry_GetDefaultFilledRectangle2d());
	ZG_Transform_Restore(&t);
}

void ZG_Graphics_DrawTexturedRectangle4i(int _center_x, int _center_y, uint16_t _width, uint16_t _height, ZG_Color4f _color, ZG_Texture *text, ZG_TextureRect * text_crop){

	ZG_Vector3f translate=ZG_ViewPort_ScreenToWorld(_center_x,_center_y);
	ZG_Vector3f scale=ZG_ViewPort_ScreenToWorldDimension2i(_width,_height);

	ZG_Graphics_DrawTexturedRectangle4f(translate.x,translate.y,scale.x,scale.y,_color,text,text_crop);
}

void ZG_Graphics_DrawTexturedRectangle4f(float _center_x, float _center_y, float _scale_x, float _scale_y,  ZG_Color4f _color,ZG_Texture *_texture, ZG_TextureRect * _text_crop){
	ZG_Transform t=ZG_Transform_New();

	t.translate.x=_center_x;
	t.translate.y=_center_y;
	t.scale.x=_scale_x;
	t.scale.y=_scale_y;

	// setup appearance
	g_graphics_vars->default_appearance_rectangle2d->material->color=_color;
	g_graphics_vars->default_appearance_rectangle2d->texture=_texture;

	ZG_Transform_Apply(&t);
	ZG_Appearance_Apply(g_graphics_vars->default_appearance_rectangle2d);

	// setup crop
   if(_text_crop == NULL){

	    // set mesh texture according indexes
	   // Mesh texture are inverted due the fact that OpenGL considers first Y starting at the span-1 (Y is flipped)
		float mesh_texture[]={
   			   0.0f,  1.0f,   // bottom left
			   0.0f,  0.0f,   // top left
			   1.0f,  1.0f,   // bottom right
			   1.0f,  0.0f    // top right

		};

		ZG_Geometry_SetMeshTexture(g_graphics_vars->default_geometry_textured_rectangle2d,mesh_texture,ZG_ARRAY_SIZE(mesh_texture));
	}else{
		float mesh_texture[]={
				_text_crop->u1, _text_crop->v2, // bottom left
				_text_crop->u2, _text_crop->v2, // bottom right
				_text_crop->u1, _text_crop->v1, // top left
				_text_crop->u2, _text_crop->v1  // top right
		};

		ZG_Geometry_SetMeshTexture(g_graphics_vars->default_geometry_textured_rectangle2d,mesh_texture,ZG_ARRAY_SIZE(mesh_texture));
	}

	ZG_Geometry_Draw(g_graphics_vars->default_geometry_textured_rectangle2d);

	ZG_Appearance_Restore(g_graphics_vars->default_appearance_rectangle2d);
	ZG_Transform_Restore(&t);
}


void ZG_Graphics_DrawCircle4f(float _center_x, float _center_y, float _scale_x, float _scale_y, ZG_Color4f _color, uint8_t _thickness){

	ZG_Transform t=ZG_Transform_New();
	t.translate.x=_center_x;
	t.translate.y=_center_y;
	t.scale.x=_scale_x;
	t.scale.y=_scale_y;

	ZG_Graphics_SetColor4f(_color.r,_color.b, _color.g, _color.a);
	ZG_Graphics_SetLineThickness(_thickness);
	ZG_Transform_Apply(&t);
	ZG_Geometry_Draw(ZG_Geometry_GetDefaultCircle());
	ZG_Transform_Restore(&t);
}


void ZG_Graphics_DrawCircle4i(int _center_x, int _center_y, uint16_t _width, uint16_t _height,ZG_Color4f _color, uint8_t _thickness){

	ZG_Vector3f translate=ZG_ViewPort_ScreenToWorld(_center_x,_center_y);
	ZG_Vector3f scale=ZG_ViewPort_ScreenToWorldDimension2i(_width,_height);

	ZG_Graphics_DrawCircle4f(
			translate.x
			,translate.y
			,scale.x
			,scale.y
			,_color
			,_thickness
	);
}

void ZG_Graphics_Print(int x, int y, ZG_Color4f color, const char *in, ...){

	char out[1024]={0};
	ZG_VARGS(out,in);

	ZG_Vector3f pos3d=ZG_ViewPort_ScreenToWorld(x,y);

	ZG_TTFont_RenderTextBegin(NULL);
	ZG_TTFont_Print(ZG_TTFontManager_GetEmbeddedFont(),pos3d.x,pos3d.y,1,color,out);
	ZG_TTFont_RenderTextEnd();
}

void ZG_Graphics_WPrint(int x, int y, ZG_Color4f color, const wchar_t *in, ...){
	wchar_t out[1024]={0};
	ZG_WVARGS(out,in);

	ZG_TTFont_RenderTextBegin(NULL);
	ZG_TTFont_WPrint(ZG_TTFontManager_GetEmbeddedFont(),x,y,1,color,out);
	ZG_TTFont_RenderTextEnd();
}
//---------------------------------------------------------------------------------------------------------------------------
void ZG_Graphics_DeInit(void) {

	if(g_graphics_vars == NULL){
		ZG_LOG_ERRORF("Graphics not initialized");
		return;
	}

	ZG_IconManager_DeInit();
	ZG_TextureManager_DeInit();
	ZG_Geometry_DeInit();

	ZG_TTFontManager_DeInit();
	ZG_ViewPort_DeInit();


	// deinit gl vars first
	ZG_Geometry_Delete(g_graphics_vars->default_geometry_textured_rectangle2d);
	ZG_Appearance_Delete(g_graphics_vars->default_appearance_rectangle2d);
	ZG_Material_Delete(g_graphics_vars->default_material_rectangle2d);

	// ...then deini gl context
	switch(g_graphics_vars->graphics_api){
	case ZG_GRAPHICS_API_GL:
		ZG_Graphics_GL_DeInit();
		break;
	}

	if(g_graphics_vars->sdl_window!=NULL){
		SDL_DestroyWindow(g_graphics_vars->sdl_window);
	}

	// Remove adapter lists
	ZG_List_DeleteAndFreeAllItems(g_graphics_vars->adapters);
	ZG_List_DeleteAndFreeAllItems(g_graphics_vars->capture_screen_callbacks);

	ZG_FREE(g_graphics_vars->rect_display);
	ZG_FREE(g_graphics_vars);
}

