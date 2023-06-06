#include "zg_graphics.h"

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

	 //ViewPort  * view_port;

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

	ZG_Geometry *geometry_rectangle_default;
	ZG_Appearance *appearance_rectangle_default;
	Material *material_rectangle_default;
	SDL_Rect *rect_display;
	int num_displays;
	int active_display;
	bool fullscreen;
	ZG_ProjectionMode projection_mode;

}GraphicsVars;


GraphicsVars *g_graphics_vars=NULL;




#include "ZG_Graphics_GL.c"
#include "ZG_Graphics_Win32.c"

static void ZG_Graphics_PrintAdapterInformation(void);

bool ZG_Graphics_Init(
		int start_posx,int start_posy	// start position
		, uint16_t _width, uint16_t _height // logical resolution
		//, uint16_t _window_width, uint16_t _window_height // screen resolution
		,GraphicsApi _video_context
		,const char *_caption
		,uint16_t _properties
	) {

	uint16_t _window_width=_width;
	uint16_t _window_height=_height;

	if(g_graphics_vars != NULL){
		Log_ErrorF("Graphics already initialized");
		return false;
	}

	g_graphics_vars = ZG_NEW(GraphicsVars);
	g_graphics_vars->capture_screen_callbacks=List_New();

	uint32_t video_flags = 0;


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

	g_graphics_vars->num_displays=SDL_GetNumVideoDisplays();
	if(g_graphics_vars->num_displays < 0){
		Log_Error("SDL_GetNumVideoDisplays error : %s\n", SDL_GetError());
		return false;
	}
	g_graphics_vars->rect_display=(SDL_Rect *)malloc(sizeof(SDL_Rect)*g_graphics_vars->num_displays);
	for(int i=0 ; i < g_graphics_vars->num_displays; i++){
		if(SDL_GetDisplayBounds(i, &g_graphics_vars->rect_display[i])!=0){
			Log_Error("SDL_GetDisplayBounds error : %s\n", SDL_GetError());
			return false;
		}
	}

	if(_properties & MSK_GRAPHIC_PROPERTY_FULLSCREEN){

		/*SDL_DisplayMode current;
		if(SDL_GetCurrentDisplayMode(0, &current) != 0){
			Log_Error("Unable to get display mode: %s", SDL_GetError());
			return false;
		}
		_window_width=current.w;
		_window_height=current.h;*/
		g_graphics_vars->fullscreen=true;
	}



	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
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
	g_graphics_vars->scale=Vector2f_New(1,1);


	Log_Info("Created main window %ix%i (%ibpp)", _window_width,_window_height, g_graphics_vars->sdl_window_surface->format->BitsPerPixel);
	Log_Info("SDL version: %02i.%02i.%02i",SDL_MAJOR_VERSION,SDL_MINOR_VERSION,SDL_PATCHLEVEL);

	//Input_SetupCursors();
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		if(ZG_Graphics_GL_Init()==false){
			return false;
		}
		break;
	}

	// select 2d model view as default...
	ZG_Graphics_SetProjectionMode(PROJECTION_MODE_ORTHO);


	// adapter list
	g_graphics_vars->adapters=NULL;

#ifdef _WIN32
	g_graphics_vars->adapters=ZG_Graphics_Win32_GetListAdapters();
#endif

	ZG_Graphics_PrintAdapterInformation();

	TTFont_Init();
	IconManager_Init();
	ViewPort_Init(_width,_height);

	// created default rectangle/s for drawing
	g_graphics_vars->geometry_rectangle_default=Geometry_NewRectangleFilled(GEOMETRY_PROPERTY_TEXTURE);
	g_graphics_vars->appearance_rectangle_default=Appearance_New();
	g_graphics_vars->material_rectangle_default=Material_New(0);
	g_graphics_vars->appearance_rectangle_default->material=g_graphics_vars->material_rectangle_default;

	ZG_Graphics_SetFullscreen(g_graphics_vars->fullscreen);


	return true;
}

Vector2f ZG_Graphics_GetScale(){
	return g_graphics_vars->scale;
}

void ZG_Graphics_SetProjectionMode(PROJECTION_MODE _projection_mode){
	//Input_SetupCursors();
	g_graphics_vars->projection_mode=_projection_mode;

	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		ZG_Graphics_GL_SetProjectionMode(_projection_mode);
		break;

	}
}

void ZG_Graphics_SetCameraTransform(Transform *transform){
	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			ZG_Graphics_GL_SetCameraTransform(transform);
			break;
	}
}

void ZG_Graphics_SetLineThickness(uint8_t _thickness){
	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			ZG_Graphics_GL_SetLineThickness(_thickness);
			break;
	}
}

void ZG_Graphics_SetPointSize(uint8_t _point_size){
	switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			ZG_Graphics_GL_SetPointSize(_point_size);
			break;
	}
}

void ZG_Graphics_SetColor4f(float _r, float _g, float _b, float _a){
		switch(ZG_Graphics_GetGraphicsApi()){
		default:
			break;
		case GRAPHICS_API_GL:
			ZG_Graphics_GL_SetColor4f(_r,_g,_b,_a);
			break;
	}
}

GraphicsApi ZG_Graphics_GetGraphicsApi(void){
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

unsigned ZG_Graphics_GetNumMonitors(void){
	return SDL_GetNumVideoDisplays();
}

float CZG_Graphics_GetAdapterPhysicalWidth(unsigned idx_monitor){
	if(idx_monitor >= g_graphics_vars->adapters->count){
		Log_Error("%i monitor out of bounds. (Max monitors:%i)",g_graphics_vars->adapters->count);
		return 0;
	}

	return ((AdapterInfo *)g_graphics_vars->adapters->items[idx_monitor])->physical_width;


}

float CZG_Graphics_GetAdapterPhysicalHeight(unsigned idx_monitor){
	if(idx_monitor >= g_graphics_vars->adapters->count){
		Log_Error("%i monitor out of bounds. (Max monitors:%i)",g_graphics_vars->adapters->count);
		return 0;
	}

	return ((AdapterInfo *)g_graphics_vars->adapters->items[idx_monitor])->physical_height;
}

const char *CZG_Graphics_GetAdapterMonitorModel(unsigned idx_monitor){
	if(idx_monitor >= g_graphics_vars->adapters->count){
		Log_Error("%i monitor out of bounds. (Max monitors:%i)",g_graphics_vars->adapters->count);
		return 0;
	}

	return ((AdapterInfo *)g_graphics_vars->adapters->items[idx_monitor])->monitor_model;
}

static void ZG_Graphics_PrintAdapterInformation(void){


	if( g_graphics_vars->adapters == NULL || g_graphics_vars->adapters->count==0){
		Log_InfoF("No adapters information");
		return;
	}

	for(unsigned i = 0;i < g_graphics_vars->adapters->count; i++){
		AdapterInfo *adapter=(void *)g_graphics_vars->adapters->items[i];
		Log_InfoF("--------------------------------------");
		Log_Info("MonitorModel: %s",adapter->monitor_model);
		Log_Info("Width: %i",adapter->pixels_width);
		Log_Info("Height: %i",adapter->pixels_height);
		Log_Info("PhysicalWidth: %f",adapter->physical_width);
		Log_Info("PhysicalHeight: %f",adapter->physical_height);

	}
	Log_InfoF("--------------------------------------");
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
			Log_Error("Setting windowed failed : %s",SDL_GetError());

	  }
	} else {
		// Swith to FULLSCREEN mode
		if (SDL_SetWindowFullscreen(g_graphics_vars->sdl_window, SDL_TRUE) < 0)
		{
			Log_Error("Setting fullscreen failed : %s", SDL_GetError());

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

	g_graphics_vars->scale = Vector2f_New(
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
		g_graphics_vars->scale = Vector2f_New(
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

void ZG_Graphics_SetBackgroundColor3i(Color4f c){
	g_graphics_vars->background_color = c;
}

void ZG_Graphics_ClearScreen(Color4f color){
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

void ZG_Graphics_SetBackgroundColor(Color4f color){
	g_graphics_vars->background_color=color;
}
/*
void ZG_Graphics_AddCaptureScreenCallback(Callback fun){

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

		ZG_Graphics_Print(0,ZG_Graphics_GetHeight()-30,COLOR4F_WHITE, "FPS: %.02f",FPS);
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
			case GRAPHICS_API_GL:
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
}
//---------------------------------------------------------------------------------------------------------------------------
//
// Drawing functions
//

void ZG_Graphics_Draw(Transform *transform, Geometry *geometry, Appearance *appearance){
	if(transform)	Transform_Apply(transform);
	if(appearance)  Appearance_Apply(appearance);

	if(geometry) 	Geometry_Draw(geometry);

	if(appearance) 	Appearance_Restore(appearance);
	if(transform) 	Transform_Restore(transform);
}

void ZG_Graphics_DrawPoint2f(float _x, float _y, Color4f _color, uint8_t _point_size){
	ZG_Transform t=Transform_New();
	t.translate.x=_x;
	t.translate.y=_y;

	ZG_Graphics_SetColor4f(_color.r,_color.b, _color.g, _color.a);
	ZG_Graphics_SetPointSize(_point_size);
	ZG_Transform_Apply(&t);
	ZG_Geometry_Draw(Geometry_GetDefaultPoint());
	ZG_Transform_Restore(&t);
}

void ZG_Graphics_DrawPoint2i(int _x, int _y, Color4f _color, uint8_t _point_size){
	ZG_Graphics_DrawPoint2f(
		ViewPort_ScreenToWorldPositionX(_x)
		,ViewPort_ScreenToWorldPositionY(_y)
		,_color
		,_point_size
	);
}


void ZG_Graphics_DrawRectangle4i(int _x_center, int _y_center, int _width, int _height, Color4f _color, uint8_t _thickness){


	ZG_Vector3f translate=ViewPort_ScreenToWorld(_x_center,_y_center);
	ZG_Vector3f scale=ViewPort_ScreenToWorldDimension2i(_width,_height);

	ZG_Graphics_DrawRectangle4f(translate.x,translate.y,scale.x,scale.y,_color,_thickness);
}

void ZG_Graphics_DrawRectangle4f(float _x_center, float _y_center, float _scale_x, float _scale_y, Color4f _color, uint8_t _thickness){
	ZG_Transform t=Transform_New();
	t.translate.x=_x_center;
	t.translate.y=_y_center;
	t.scale.x=_scale_x;
	t.scale.y=_scale_y;


	ZG_Graphics_SetColor4f(_color.r,_color.b, _color.g, _color.a);
	ZG_Graphics_SetLineThickness(_thickness);
	Transform_Apply(&t);
	Geometry_Draw(Geometry_GetDefaultRectangle());
	Transform_Restore(&t);
}

void ZG_Graphics_DrawRectangleFilled4i(int x, int y, uint16_t width, uint16_t height, Color4f color){

	ZG_Vector2i p1_2d=Vector2i_New(x,y);
	ZG_Vector2i p2_2d=Vector2i_New(x+width,y+height);

	Vector3f p1_3d=ViewPort_ScreenToWorld(p1_2d.x,p1_2d.y);
	Vector3f p2_3d=ViewPort_ScreenToWorld(p2_2d.x,p2_2d.y);

	ZG_Graphics_DrawRectangleFilled4f(p1_3d.x,p1_3d.y,p2_3d.x,p2_3d.y,color);
}

void ZG_Graphics_DrawRectangleFilled4f(float _x1, float _y1, float _x2, float _y2, Color4f _color){
	Transform t=Transform_New();
	float w=_x2-_x1;
	float h=_y2-_y1;
	t.translate.x=_x1+w*0.5;
	t.translate.y=_y1+h*0.5;
	t.scale.x=w;
	t.scale.y=h;

	ZG_Graphics_SetColor4f(_color.r,_color.b, _color.g, _color.a);
	Transform_Apply(&t);
	Geometry_Draw(Geometry_GetDefaultRectangleFilled());
	Transform_Restore(&t);
}

void ZG_Graphics_DrawRectangleTextured4i(int _x, int _y, uint16_t _width, uint16_t _height, Color4f _color, Texture *text, TextureRect * text_crop){

	Vector2i p1_2d=Vector2i_New(_x-(_width>>1),_y+(_height>>1));
	Vector2i p2_2d=Vector2i_New(_x+(_width>>1),_y-(_height>>1));

	Vector3f p1_3d=ViewPort_ScreenToWorld(p1_2d.x,p1_2d.y);
	Vector3f p2_3d=ViewPort_ScreenToWorld(p2_2d.x,p2_2d.y);

	ZG_Graphics_DrawRectangleTextured4f(p1_3d.x,p1_3d.y,p2_3d.x,p2_3d.y,_color,text,text_crop);
}

void ZG_Graphics_DrawRectangleTextured4f(float _x1, float _y1, float _x2, float _y2,  Color4f _color,Texture *_texture, TextureRect * _text_crop){
	Transform t=Transform_New();

	// setup transform
	float w=fabs(_x2-_x1);
	float h=fabs(_y2-_y1); // y1 > y2, that's way y1-y2
	t.translate.x=_x1+w*0.5;
	t.translate.y=_y1+h*0.5;
	t.scale.x=w;
	t.scale.y=h;

	// setup appearance
	g_graphics_vars->appearance_rectangle_default->material->color=_color;
	g_graphics_vars->appearance_rectangle_default->texture=_texture;

	Transform_Apply(&t);
	Appearance_Apply(g_graphics_vars->appearance_rectangle_default);

	// setup crop
   if(_text_crop == NULL){

		float mesh_texture[]={
			   0.0f,  1.0f,   // bottom left
			   1.0f,  1.0f,   // bottom right
			   0.0f,  0.0f,   // top left
			   1.0f,  0.0f    // top right
		};

		Geometry_SetMeshTexture(g_graphics_vars->geometry_rectangle_default,mesh_texture,ARRAY_SIZE(mesh_texture));
	}else{
		float mesh_texture[]={
				_text_crop->u1, _text_crop->v2, // bottom left
				_text_crop->u2, _text_crop->v2, // bottom right
				_text_crop->u1, _text_crop->v1, // top left
				_text_crop->u2, _text_crop->v1  // top right
		};

		Geometry_SetMeshTexture(g_graphics_vars->geometry_rectangle_default,mesh_texture,ARRAY_SIZE(mesh_texture));
	}

	Geometry_Draw(g_graphics_vars->geometry_rectangle_default);

	Appearance_Restore(g_graphics_vars->appearance_rectangle_default);
	Transform_Restore(&t);
}


void ZG_Graphics_DrawCircle3f(float _x, float _y, float _r, Color4f _color, uint8_t _thickness){

	Transform t=Transform_New();
	t.translate.x=_x;
	t.translate.y=_y;
	t.scale.x=_r;
	t.scale.y=_r;

	ZG_Graphics_SetColor4f(_color.r,_color.b, _color.g, _color.a);
	ZG_Graphics_SetLineThickness(_thickness);
	Transform_Apply(&t);
	Geometry_Draw(Geometry_GetDefaultCircle());
	Transform_Restore(&t);
}


void ZG_Graphics_DrawCircle3i(int _x, int _y, int _r, Color4f _color, uint8_t _thickness){

	ZG_Graphics_DrawCircle3f(
			ViewPort_ScreenToWorldPositionX(_x)
			,ViewPort_ScreenToWorldPositionY(_y)
			,ViewPort_ScreenToWorldWidth(_r)
			,_color
			,_thickness
	);
}

void ZG_Graphics_Print(int x, int y, Color4f color, const char *in, ...){

	char out[1024]={0};
	ZG_VARGS(out,in);

	Vector3f pos3d=ViewPort_ScreenToWorld(x,y);

	TTFont_RenderTextBegin(NULL);
	TTFont_Print(TTFont_GetEmbeddedFont(),pos3d.x,pos3d.y,color,out);
	TTFont_RenderTextEnd();
}

void ZG_Graphics_WPrint(int x, int y, Color4f color, const wchar_t *in, ...){
	wchar_t out[1024]={0};
	ZG_WVARGS(out,in);

	TTFont_RenderTextBegin(NULL);
	TTFont_WPrint(TTFont_GetEmbeddedFont(),x,y,color,out);
	TTFont_RenderTextEnd();
}
//---------------------------------------------------------------------------------------------------------------------------
void ZG_Graphics_DeInit(void) {

	if(g_graphics_vars == NULL){
		Log_ErrorF("Graphics not initialized");
		return;
	}

	IconManager_DeInit();
	TextureManager_DeInit();
	Geometry_DeInit();

	TTFont_DeInit();
	ViewPort_DeInit();


	// deinit gl vars first
	Geometry_Delete(g_graphics_vars->geometry_rectangle_default);
	Appearance_Delete(g_graphics_vars->appearance_rectangle_default);
	Material_Delete(g_graphics_vars->material_rectangle_default);

	// ...then deini gl context
	switch(g_graphics_vars->graphics_api){
	case GRAPHICS_API_GL:
		ZG_Graphics_GL_DeInit();
		break;
	}

	if(g_graphics_vars->sdl_window!=NULL){
		SDL_DestroyWindow(g_graphics_vars->sdl_window);
	}

	// Remove adapter lists
	List_DeleteAndFreeAllItems(g_graphics_vars->adapters);
	List_DeleteAndFreeAllItems(g_graphics_vars->capture_screen_callbacks);

	ZG_FREE(g_graphics_vars->rect_display);
	ZG_FREE(g_graphics_vars);
}

