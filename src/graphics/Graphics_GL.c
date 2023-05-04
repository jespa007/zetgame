#include "zg_graphics.h"

//bool screen_shoot_request;
uint32_t start_ticks;

#define GRAPHICS_GL_DISABLE_VSYNCH

//static SDL_GLContext * g_sdl_gl_context = NULL;


bool Graphics_GL_Init(void){

	// Attach the OpenGL context to our window
// Only creates opengl context in Windows environment. In linux is already created
#ifdef _WIN32
	SDL_GLContext *g_sdl_gl_context = SDL_GL_CreateContext(g_graphics_vars->sdl_window);
	if(g_sdl_gl_context == NULL){
		Log_Error("Cannot create gl context:%s",SDL_GetError());
		return false;
	}

	/*if(SDL_GL_MakeCurrent(g_graphics_vars->sdl_window,
			g_sdl_gl_context)!=0){
		Log_Error("Cannot make current context:%s",SDL_GetError());
		return false;
	}*/
#endif

	// Disable vsync (Because it takes lot of high CPU)
	SDL_GL_SetSwapInterval(
#ifdef GRAPHICS_GL_DISABLE_VSYNCH
			0
#else
			1
#endif
	);

	//SDL_GL_SetSwapInterval(1);

	// ini opengl core functions...
	glExtraIni();

	Log_Info("OpenGL %s Shading language:%s", glGetString(GL_VERSION),glGetString ( GL_SHADING_LANGUAGE_VERSION ));

	// return to modelview matrix
	glShadeModel(GL_SMOOTH);                    // shading mathod: _GL_SMOOTH or _GL_FLAT

	// enable /disable features
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glDepthMask(TRUE);
	glEnable(GL_DEPTH_TEST);

	// we will create own custom z-buffer
	glDisable(GL_LIGHTING);

	 /** To avoid see triangles back side must configure OpenGL  _GL_CULL_FACE with only _GL_BACK and FrontFace ClockWise orientation
	  * https://www.khronos.org/opengl/wiki/Face_Culling
	  */
	 glEnable(GL_CULL_FACE);
	 /** glCullFace specifies whether front- or back-facing facets are culled (as specified by mode) when facet culling is enabled
	  * OpenGL checks all the faces that are front facing towards the viewer and renders those while discarding all the faces that are back facing
	  * , saving us a lot of fragment shader calls. We do need to tell OpenGL which of the faces we use are actually the front faces and which faces
	  * are the back faces. OpenGL uses a clever trick for this by analyzing the winding order of the vertex data.
	  */
	 glCullFace(GL_BACK); // we want to (cull or erase) the back faces
	 /**
	  * Facet culling is initially disabled. To enable and disable facet culling, call the glEnable and glDisable commands with the argument GL_CULL_FACE.
	  * Facets include triangles, quadrilaterals, polygons, and rectangles.
	  * glFrontFace specifies which of the clockwise and counterclockwise facets are front-facing and back-facing. See glFrontFace.
	  */
	 glFrontFace(GL_CW); // we stablish front face side in ClockWise order

	glClearDepth(1.0f);                         // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);


	//---- BEGIN POINT SPRITES SETUP FOR SYSTEM PARTICLE (Don't touch these values, everything works fine now!)
    /*GLfloat psr[2];
	GLfloat pda[3] = { 1.0f, 0.0f, 0.01f }; // defaults are (1.0, 0.0, 0.0)

	glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, psr);

	// Requires OpenGL 1.4
	glPointParameterf(GL_POINT_SIZE_MIN, psr[0]);
	glPointParameterf(GL_POINT_SIZE_MAX, psr[1]);
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, pda);

	glPointSize(psr[1]);
	Log_Info("Point sizes (%f,%f) ",psr[0],psr[1]);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SMOOTH);*/
	//---- END POINT SPRITES SETUP FOR SYSTEM PARTICLE

	return true;

}

void Graphics_GL_SetCameraTransform(Transform *transform){
	Vector3f rotate,translate;
	Quaternion q_inv;
	Matrix4f m_inv;

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	translate = transform->translate;
	rotate = transform->rotate;

	// rotate negate...
	q_inv=Quaternion_FromEulerV3f(
			(Vector3f){
				.x=-rotate.x
				,.y=-rotate.y
				,.z=-rotate.z
			}
	);
	m_inv=Quaternion_ToMatrix4f(q_inv);


	glLoadMatrixf(&m_inv.e11);

	glTranslatef(-translate.x, -translate.y,-translate.z);
}

void Graphics_GL_SetProjectionMode(PROJECTION_MODE projection_mode){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, Graphics_GetWindowWidth(), Graphics_GetWindowHeight());


	switch(projection_mode){
	case PROJECTION_MODE_ORTHO:
		glOrtho(-Graphics_GetAspectRatio(), Graphics_GetAspectRatio(), -1, 1, -1, 1);
		break;
	case PROJECTION_MODE_PERSPECTIVE:

		glPerspective(90.0f, //90.0f,  // field of view
				Graphics_GetAspectRatio(), // shape of viewport rectangle
				.01f,         // Min Z: how far from eye position does view start
				500.0f);       // max Z: how far from eye position does view extend

		break;
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}


void Graphics_GL_SetColor4f(float _r, float _g, float _b, float _a){
	glColor4f(_r, _g, _b, _a);
}

void Graphics_GL_SetLineThickness( uint8_t _thickness){
	glLineWidth(_thickness); // set line width
}

void Graphics_GL_SetPointSize( uint8_t _point_size){
	glPointSize(_point_size); // set line width
}

void Graphics_GL_ClearScreen(Color4f color) { // start render and clear background...
	glClearColor(color.r,color.g,color.b,color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

//begin/end
void Graphics_GL_StartRender(void) { // start render and clear background...
	glClearColor(g_graphics_vars->background_color.r,g_graphics_vars->background_color.g,g_graphics_vars->background_color.b,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	start_ticks=SDL_GetTicks();

}

void Graphics_GL_EndRender(void) {
	float diff = (float)(SDL_GetTicks()-start_ticks);
	int to_16 = (16-diff);

	if(diff == 0)
		diff=1;

	g_graphics_vars->fps=1000.0f/diff;

	// Swap buffers
	// wait vsync and swap buffers... (you must get over 60FPS)
	SDL_GL_SwapWindow(g_graphics_vars->sdl_window);


#ifdef GRAPHICS_GL_DISABLE_VSYNCH
	// because we had disabled vsync we have to wait for 60 FPS...
	if(to_16 > 0){
		SDL_Delay(to_16);
	}
#endif
}

SDL_Surface * Graphics_GL_ScreenShoot(void){

	ViewPort *view_port=ViewPort_GetCurrent();
	SDL_Surface * srf_screenshoot = SDL_CreateRGBSurface(SDL_SWSURFACE, view_port->width, view_port->height, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);

	glReadPixels(0, 0, view_port->width, view_port->height, GL_RGB, GL_UNSIGNED_BYTE, srf_screenshoot->pixels);

	return srf_screenshoot;

}


void Graphics_GL_DeInit(void) {

	GLenum e=glGetError();
	if(e!=GL_NO_ERROR){
		Log_Warning("OpenGL (%x): There's some OGL problems. Enable debug to check file/line",e);
	}else{
		Log_InfoF("OpenGL: OK");
	}

}


