#include "zg_graphics.h"

//bool screen_shoot_request;
uint32_t start_ticks;



static SDL_GLContext * sdl_gl_context = NULL;

bool Graphics_GL_Init(void){

	// Attach the OpenGL context to our window
	sdl_gl_context = SDL_GL_CreateContext(g_graphics_vars->sdl_window);
	if(sdl_gl_context == NULL){
		Log_Error("Cannot create gl context:%s",SDL_GetError());
		return false;
	}

	// Disable vsync
	SDL_GL_SetSwapInterval(0);
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
	glEnable(GL_LIGHTING);

	 // To avoid see triangles back side must configure OpenGL  _GL_CULL_FACE with only _GL_BACK and FrontFace ClockWise orientation
	 glEnable(GL_CULL_FACE);
	 glCullFace(GL_BACK); // we want to hide back faces
	 glFrontFace(GL_CW); // we stablish front face side in ClockWise order

	glClearDepth(1.0f);                         // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);


	//---- BEGIN POINT SPRITES SETUP FOR SYSTEM PARTICLE (Don't touch these values, everything works fine now!)
	GLfloat psr[2];
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
	glDisable(GL_POINT_SMOOTH);
	//---- END POINT SPRITES SETUP FOR SYSTEM PARTICLE

	// select 2d model view as default...
	Graphics_SetProjectionMode(PROJECTION_MODE_ORTHO);
	return true;

}

void Graphics_GL_SetProjectionMode(PROJECTION_MODE projection_mode){

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

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


/*****
 * 2D Drawing functions
 */

void Graphics_GL_DrawRectangle4f(float x1, float y1, float x2, float y2, uint8_t thickness, Color4f color){

	glPushAttrib( GL_CURRENT_BIT | GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
		glColor4f(color.r, color.g,color.b, color.a);
		glLineWidth(thickness); // set line width
		glBegin(GL_LINES); // draw something with the texture on
				glVertex2f(x1, y1);
				glVertex2f(x2, y1);

				glVertex2f(x2, y1);
				glVertex2f(x2, y2);

				glVertex2f(x2, y2);
				glVertex2f(x1, y2);

				glVertex2f(x1, y2);
				glVertex2f(x1, y1);
		glEnd();
	glPopAttrib();
}


void Graphics_GL_DrawRectangleFilled4f(float x1, float y1, float x2, float y2, Color4f color){

	glPushAttrib( GL_CURRENT_BIT | GL_LIGHTING_BIT);

	glDisable(GL_LIGHTING);

	glColor4f(color.r, color.g,color.b,color.a);
	glBegin(GL_TRIANGLE_STRIP); // draw something with the texture on
			glVertex2f(x1, y1);
			glVertex2f(x2, y1);
			glVertex2f(x1, y2);
			glVertex2f(x2, y2);
	glEnd();

	glPopAttrib();
}


void Graphics_GL_DrawRectangleTextured4f(float x1, float y1, float x2, float y2, Color4f color, Texture *text, Rectanglef *text_crop ){

	glPushAttrib( GL_CURRENT_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);

	Rectanglef default_text_crop= Rectanglef_New4f(0,0,1,1);

	if(text_crop == NULL){
		text_crop=&default_text_crop;
	}

	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // normal
  	glAlphaFunc(GL_NOTEQUAL, 0);

	Texture_Bind(0);
	if(text != NULL){
		Texture_Bind(text);
	}

	glColor4f(color.r,color.g,color.b,color.a);

	glBegin(GL_TRIANGLE_STRIP); // draw something with the texture on
			glTexCoord2f(text_crop->x1, text_crop->y1);
			glVertex2f(x1, y1);

			glTexCoord2f(text_crop->x2, text_crop->y1);
			glVertex2f(x2, y1);

			glTexCoord2f(text_crop->x1, text_crop->y2);
			glVertex2f(x1,y2);

			glTexCoord2f(text_crop->x2, text_crop->y2);
			glVertex2f(x2,y2);
	glEnd();

	glPopAttrib();
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


	// because we had disabled vsync we have to wait for 60 FPS...
	if(to_16 > 0){
		SDL_Delay(to_16);
	}
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
		Log_Info("OpenGL: OK");
	}

}


