

void ZG_ViewPort_GL_SetView(ZG_ViewPort * viewport){

	// transform gl normalized y origin on the on top...
	int yn=ZG_Graphics_GetHeight()-viewport->y-viewport->height;


	glViewport(viewport->x,yn, viewport->width, viewport->height);
	glScissor(viewport->x,yn, viewport->width, viewport->height);
	glEnable(GL_SCISSOR_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ZG_ViewPort_GL_RestoreView(ZG_ViewPort * viewport){
	// disable gl scissors and reenable if not null?
	glDisable(GL_SCISSOR_TEST);
	if(viewport!=NULL){
		ZG_ViewPort_GL_SetView(viewport);
	}
}


