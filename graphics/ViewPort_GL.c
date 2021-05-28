

void ViewPort_SetView_GL(ViewPort * viewport){

	// transform gl normalized y origin on the on top...
	int yn=Graphics_GetHeight()-viewport->y-viewport->height;


	glViewport(viewport->x,yn, viewport->width, viewport->height);
	glScissor(viewport->x,yn, viewport->width, viewport->height);
	glEnable(GL_SCISSOR_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ViewPort_RestoreView_GL(ViewPort * viewport){
	// disable gl scissors and reenable if not null?
	glDisable(GL_SCISSOR_TEST);
	if(viewport!=NULL){
		ViewPort_SetView_GL(viewport);
	}
}


