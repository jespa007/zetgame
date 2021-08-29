#include "zg_graphics.h"

#include "ViewPort_GL.c"

#define STACK_VIEW_PORTS_MAX 25

typedef struct{
	ViewPort 		view_ports[STACK_VIEW_PORTS_MAX];
	ViewPort 		current_viewport;
	ViewPort 		first_viewport;
	int				stk_viewport;

}ViewPortVars;


static ViewPortVars * g_viewport_vars=NULL;

static ViewPort 	ViewPort_New(int x ,int y,uint16_t _width, uint16_t _height, PROJECTION_MODE projection_mode);
static float ViewPort_ScreenToWorldPosX_BuiltIn(ViewPort *view_port,int x);
static float ViewPort_ScreenToWorldPosY_BuiltIn(ViewPort *view_port,int y);


void 		ViewPort_Init(uint16_t _width, uint16_t _height){
	if(g_viewport_vars!=NULL){
		Log_Warning("ViewPort status already initialized");
	}

	g_viewport_vars=NEW(ViewPortVars);
	g_viewport_vars->first_viewport=g_viewport_vars->current_viewport=ViewPort_New(0,0,_width,_height,PROJECTION_MODE_ORTHO);

}


//--------
// PRIVATE
static ViewPort  	ViewPort_New(int x, int y,uint16_t _width, uint16_t _height, PROJECTION_MODE projection_mode){
	ViewPort view_port;

	memset(&view_port,0,sizeof(view_port));

	view_port.x=x;
	view_port.y=y;
	view_port.width = _width;
	view_port.height = _height;
	view_port.projection_mode=projection_mode;
	view_port.one_over_width = 1.0f/(float)_width;
	view_port.one_over_height = 1.0f/(float)_height;
	//view_port.aspect_ratio = g_viewport_vars->aspect_ratio;//(float)Graphics_Width()/(float)Graphics_Height(); //640.0f/480.0f;//((float) (_width) / (float) (_height));
	view_port.projection_origin=(Vector3f){
			.x=ViewPort_ScreenToWorldPosX_BuiltIn(&view_port,0)
			,.y=ViewPort_ScreenToWorldPosY_BuiltIn(&view_port,0)
			,.z=0
	};

	return view_port;
}


void 		ViewPort_SetView(ViewPort *  view_port){

	g_viewport_vars->current_viewport=*view_port;


	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		ViewPort_SetView_GL(view_port);
		break;
	default:
		break;

	}
}

//--------
// PUBLIC
void ViewPort_RestoreView(ViewPort * view_port){

	switch(Graphics_GetGraphicsApi()){
	case GRAPHICS_API_GL:
		ViewPort_RestoreView_GL(view_port);
		break;
	default:
		break;

	}

	g_viewport_vars->current_viewport=*view_port;
}



ViewPort *		ViewPort_Push(int x, int y,uint16_t width, uint16_t height, PROJECTION_MODE projection_mode){
	// PRE: Init should be initialized as well as opengl context
	ViewPort * view_port=NULL;
	// set glview...
	if(g_viewport_vars->stk_viewport<STACK_VIEW_PORTS_MAX){

		view_port=&g_viewport_vars->view_ports[g_viewport_vars->stk_viewport++];
		*view_port=ViewPort_New(x,y,width,height, projection_mode);

		ViewPort_SetView(view_port);

	}else{
		Log_Error("Max view ports reached (Max: %i)",STACK_VIEW_PORTS_MAX);
	}

	return view_port;

}


ViewPort * 	ViewPort_GetCurrent(void){
	return &g_viewport_vars->current_viewport;
}

ViewPort * 	ViewPort_Pop(void){

	if(g_viewport_vars == NULL) return NULL;

	ViewPort *viewport=NULL;

	if(g_viewport_vars->stk_viewport>0){

		--g_viewport_vars->stk_viewport;

		if(g_viewport_vars->stk_viewport>0){

			viewport=&g_viewport_vars->view_ports[g_viewport_vars->stk_viewport-1];
		}
		else{
			viewport=&g_viewport_vars->first_viewport;
		}

		ViewPort_RestoreView(viewport);


	}else{
		Log_Error("Cannot pop view ports stack empty");
	}
	// restore glview...

	return viewport;
}



float ViewPort_ScreenToWorldPosX_BuiltIn(ViewPort * view_port,int x2d){

	return (-Graphics_GetAspectRatio() + 2.0f*(float)(x2d)*(Graphics_GetAspectRatio()*view_port->one_over_width))*1;
}

float ViewPort_ScreenToWorldPosY_BuiltIn(ViewPort * view_port,int y2d){

	return (1.0f - 2.0f*(float)(y2d)*view_port->one_over_height)*1;
}

int ViewPort_WorldToScreenPosX_BuiltIn(ViewPort * view_port,float x3d){
	return (x3d+Graphics_GetAspectRatio())*Graphics_GetOneOverAspectRatio()*0.5f*view_port->width;
}

int ViewPort_WorldToScreenPosY_BuiltIn(ViewPort * view_port,float y3d){
	return (y3d-1)*(-0.5f)*view_port->height;
}



uint16_t ViewPort_CurrentWidth(void){
	return g_viewport_vars->current_viewport.width;
}

uint16_t ViewPort_CurrentHeight(void){
	return g_viewport_vars->current_viewport.height;
}

int ViewPort_WorldToScreenPosX(float x3d){
	return ViewPort_WorldToScreenPosX_BuiltIn(&g_viewport_vars->current_viewport, x3d);
}

int ViewPort_WorldToScreenPosY(float y3d){
	return ViewPort_WorldToScreenPosY_BuiltIn(&g_viewport_vars->current_viewport, y3d);
}

Vector2i ViewPort_WorldToScreen(float x3d, float y3d){
	return (Vector2i){
		.x=ViewPort_WorldToScreenPosY(x3d)
		,.y=ViewPort_WorldToScreenPosY(y3d)
	};

}

float ViewPort_ScreenToWorldPosX(int x2d){
	return ViewPort_ScreenToWorldPosX_BuiltIn(&g_viewport_vars->current_viewport, x2d);
}

float ViewPort_ScreenToWorldPosY(int y2d){
	return ViewPort_ScreenToWorldPosY_BuiltIn(&g_viewport_vars->current_viewport, y2d);
}

Vector3f ViewPort_ScreenToWorld(int x, int y){
	Vector3f v3d=(Vector3f){
		.x=ViewPort_ScreenToWorldPosX(x)
		,.y=ViewPort_ScreenToWorldPosY(y)
		,.z=0
	};

	return v3d;
}


Vector3f ViewPort_GetProjectionOrigin(void){
	return g_viewport_vars->current_viewport.projection_origin;
}

float ViewPort_ScreenToWorldWidth(int  w){

	return 2*(w)*(Graphics_GetAspectRatio()*g_viewport_vars->current_viewport.one_over_width);
}

float ViewPort_ScreenToWorldHeight(int h){

	return 2*(h)*(g_viewport_vars->current_viewport.one_over_height);
}

Vector3f ViewPort_ScreenToWorldDimension2i(int width, int height){
	Vector3f v3d=(Vector3f){
		.x=ViewPort_ScreenToWorldWidth(width)
		,.y=ViewPort_ScreenToWorldHeight(height)
		,.z=0
	};

	return v3d;
}

void 		ViewPort_DeInit(void){
	if(g_viewport_vars == NULL) {
		Log_Error("Viewport not init");
		return;
	}


	FREE(g_viewport_vars);
	g_viewport_vars=NULL;
}
