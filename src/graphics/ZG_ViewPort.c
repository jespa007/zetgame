#include "_zg_graphics_.h"

#include "ZG_ViewPort_GL.c"

#define ZG_STACK_VIEW_PORTS_MAX 25

typedef struct{
	ZG_ViewPort 		view_ports[ZG_STACK_VIEW_PORTS_MAX];
	ZG_ViewPort 		current_viewport;
	ZG_ViewPort 		first_viewport;
	int					stk_viewport;

}ZG_ViewPortVars;


static ZG_ViewPortVars * g_viewport_vars=NULL;

static ZG_ViewPort 	ZG_ViewPort_New(int x ,int y,uint16_t _width, uint16_t _height, ZG_ProjectionMode projection_mode);
static float 		ZG_ViewPort_ScreenToWorldPosX_BuiltIn(ZG_ViewPort *view_port,int x);
static float 		ZG_ViewPort_ScreenToWorldPosY_BuiltIn(ZG_ViewPort *view_port,int y);


void 		ZG_ViewPort_Init(uint16_t _width, uint16_t _height){
	if(g_viewport_vars!=NULL){
		ZG_Log_WarningF("ZG_ViewPort status already initialized");
	}

	g_viewport_vars=ZG_NEW(ZG_ViewPortVars);
	g_viewport_vars->first_viewport=g_viewport_vars->current_viewport=ZG_ViewPort_New(0,0,_width,_height,ZG_PROJECTION_MODE_ORTHO);

}


//--------
// PRIVATE
static ZG_ViewPort  	ZG_ViewPort_New(int x, int y,uint16_t _width, uint16_t _height, ZG_ProjectionMode projection_mode){
	ZG_ViewPort view_port;

	memset(&view_port,0,sizeof(view_port));

	view_port.x=x;
	view_port.y=y;
	view_port.width = _width;
	view_port.height = _height;
	view_port.projection_mode=projection_mode;
	view_port.one_over_width = 1.0f/(float)_width;
	view_port.one_over_height = 1.0f/(float)_height;
	//view_port.aspect_ratio = g_viewport_vars->aspect_ratio;//(float)Graphics_Width()/(float)Graphics_Height(); //640.0f/480.0f;//((float) (_width) / (float) (_height));
	view_port.projection_origin=(ZG_Vector3f){
			.x=ZG_ViewPort_ScreenToWorldPosX_BuiltIn(&view_port,0)
			,.y=ZG_ViewPort_ScreenToWorldPosY_BuiltIn(&view_port,0)
			,.z=0
	};

	return view_port;
}


void 		ZG_ViewPort_SetView(ZG_ViewPort *  view_port){

	g_viewport_vars->current_viewport=*view_port;


	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_ViewPort_GL_SetView(view_port);
		break;
	default:
		break;

	}
}

//--------
// PUBLIC
void ZG_ViewPort_RestoreView(ZG_ViewPort * view_port){

	switch(ZG_Graphics_GetGraphicsApi()){
	case ZG_GRAPHICS_API_GL:
		ZG_ViewPort_GL_RestoreView(view_port);
		break;
	default:
		break;

	}

	g_viewport_vars->current_viewport=*view_port;
}



ZG_ViewPort *		ZG_ViewPort_Push(int x, int y,uint16_t width, uint16_t height, ZG_ProjectionMode projection_mode){
	// PRE: Init should be initialized as well as opengl context
	ZG_ViewPort * view_port=NULL;
	// set glview...
	if(g_viewport_vars->stk_viewport<ZG_STACK_VIEW_PORTS_MAX){

		view_port=&g_viewport_vars->view_ports[g_viewport_vars->stk_viewport++];
		*view_port=ZG_ViewPort_New(x,y,width,height, projection_mode);

		ZG_ViewPort_SetView(view_port);

	}else{
		ZG_Log_Error("Max view ports reached (Max: %i)",ZG_STACK_VIEW_PORTS_MAX);
	}

	return view_port;

}


ZG_ViewPort * 	ZG_ViewPort_GetCurrent(void){
	return &g_viewport_vars->current_viewport;
}

ZG_ViewPort * 	ZG_ViewPort_Pop(void){

	if(g_viewport_vars == NULL) return NULL;

	ZG_ViewPort *viewport=NULL;

	if(g_viewport_vars->stk_viewport>0){

		--g_viewport_vars->stk_viewport;

		if(g_viewport_vars->stk_viewport>0){

			viewport=&g_viewport_vars->view_ports[g_viewport_vars->stk_viewport-1];
		}
		else{
			viewport=&g_viewport_vars->first_viewport;
		}

		ZG_ViewPort_RestoreView(viewport);


	}else{
		ZG_Log_ErrorF("Cannot pop view ports stack empty");
	}
	// restore glview...

	return viewport;
}



float ZG_ViewPort_ScreenToWorldPosX_BuiltIn(ZG_ViewPort * view_port,int x2d){

	return (-ZG_Graphics_GetAspectRatio() + 2.0f*(float)(x2d)*(ZG_Graphics_GetAspectRatio()*view_port->one_over_width))*1;
}

float ZG_ViewPort_ScreenToWorldPosY_BuiltIn(ZG_ViewPort * view_port,int y2d){

	return (1.0f - 2.0f*(float)(y2d)*view_port->one_over_height)*1;
}

int ZG_ViewPort_WorldToScreenPosX_BuiltIn(ZG_ViewPort * view_port,float x3d){
	return (x3d+ZG_Graphics_GetAspectRatio())*ZG_Graphics_GetOneOverAspectRatio()*0.5f*view_port->width;
}

int ZG_ViewPort_WorldToScreenPosY_BuiltIn(ZG_ViewPort * view_port,float y3d){
	return (y3d-1)*(-0.5f)*view_port->height;
}

uint16_t ZG_ViewPort_CurrentWidth(void){
	return g_viewport_vars->current_viewport.width;
}

uint16_t ZG_ViewPort_CurrentHeight(void){
	return g_viewport_vars->current_viewport.height;
}

int ZG_ViewPort_WorldToScreenPosX(float x3d){
	return ZG_ViewPort_WorldToScreenPosX_BuiltIn(&g_viewport_vars->current_viewport, x3d);
}

int ZG_ViewPort_WorldToScreenPosY(float y3d){
	return ZG_ViewPort_WorldToScreenPosY_BuiltIn(&g_viewport_vars->current_viewport, y3d);
}

ZG_Vector2i ZG_ViewPort_WorldToScreen(float x3d, float y3d){
	return (ZG_Vector2i){
		.x=ZG_ViewPort_WorldToScreenPosY(x3d)
		,.y=ZG_ViewPort_WorldToScreenPosY(y3d)
	};

}

float ZG_ViewPort_ScreenToWorldPositionX(int x2d){
	return ZG_ViewPort_ScreenToWorldPosX_BuiltIn(&g_viewport_vars->current_viewport, x2d);
}

float ZG_ViewPort_ScreenToWorldPositionY(int y2d){
	return ZG_ViewPort_ScreenToWorldPosY_BuiltIn(&g_viewport_vars->current_viewport, y2d);
}

ZG_Vector3f ZG_ViewPort_ScreenToWorld(int x, int y){
	ZG_Vector3f v3d=(ZG_Vector3f){
		.x=ZG_ViewPort_ScreenToWorldPositionX(x)
		,.y=ZG_ViewPort_ScreenToWorldPositionY(y)
		,.z=0
	};
	return v3d;
}

ZG_Vector3f ZG_ViewPort_GetProjectionOrigin(void){
	return g_viewport_vars->current_viewport.projection_origin;
}

float ZG_ViewPort_ScreenToWorldWidth(int  w){
	return 2*(w)*(ZG_Graphics_GetAspectRatio()*g_viewport_vars->current_viewport.one_over_width);
}

float ZG_ViewPort_ScreenToWorldHeight(int h){
	return 2*(h)*(g_viewport_vars->current_viewport.one_over_height);
}

ZG_Vector3f ZG_ViewPort_ScreenToWorldDimension2i(int width, int height){
	ZG_Vector3f v3d=(ZG_Vector3f){
		.x=ZG_ViewPort_ScreenToWorldWidth(width)
		,.y=ZG_ViewPort_ScreenToWorldHeight(height)
		,.z=0
	};
	return v3d;
}

void 		ZG_ViewPort_DeInit(void){
	if(g_viewport_vars == NULL) {
		ZG_Log_ErrorF("Viewport not init");
		return;
	}

	ZG_FREE(g_viewport_vars);
	g_viewport_vars=NULL;
}
