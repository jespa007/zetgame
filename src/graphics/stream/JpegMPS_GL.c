bool JpegMPS_GL_SetupGfx(JpegMPSData *data){

	if(FRAMEBUFFER_OBJECT_EXTENSION_ENABLED){

		if(VERTEX_BUFFER_OBJECT_EXTENSION_ENABLED){
			// create PBO_COUNT pixel buffer objects, you need to delete them when program exits.
			glGenBuffers(PBO_COUNT, data->pboIds);
			// reserve memory for back and front system buffer
			for(int i = 0; i < PBO_COUNT; i++){
				//downsampleData[i] = (unsigned char *)malloc(STREAM_SIZE);
				//processed_jpeg[i]=NULL;//(uint8_t *)malloc(STREAM_WIDTH*STREAM_HEIGHT*STREAM_CHANNELS);
				// glBufferDataARB with NULL pointer reserves only memory space.
				glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, data->pboIds[i]);
				glBufferData(GL_PIXEL_PACK_BUFFER_ARB, STREAM_SIZE, 0, GL_STREAM_READ_ARB);
			}

			glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);

			// create frame buffer ...
			if(glGenFrameBufferExt(&data->fbo,STREAM_WIDTH,STREAM_HEIGHT)){
				return true;
			}else{
				Log_Error("Cannot create glGenFrameBufferExt");
			}
		}
		else{
			Log_Error("This Video card doesn't support Pixel Buffer Object extension");
		}
	}else{
		Log_Error("This video card doesn't support Frame Buffer Object");
	}

	return false;

}

void JpegMPS_GL_CaptureFrame(JpegMPSData *data){

	//blit backbuffer to downsampled buffer
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, data->fbo.id);
	glBlitFramebuffer(0, 0, Graphics_GetWidth(), Graphics_GetHeight(), 0, 0, STREAM_WIDTH, STREAM_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void JpegMPS_GL_FetchFrame(JpegMPSData *data){


	// bind downsampled data...
	glBindFramebuffer(GL_FRAMEBUFFER, data->fbo.id);

	//--------------
	// Method 1: bind downsampled buffer for reading
	//glReadPixels(0, 0, STREAM_WIDTH, STREAM_HEIGHT,  GL_BGRA, GL_UNSIGNED_BYTE, downsampleData[n_write]);

	//--------------
	// Method 3: using pbo
	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, data->pboIds[data->index]);
	//read from framebuffer to PBO asynchronously

	glReadPixels(0, 0, STREAM_WIDTH, STREAM_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, 0);
	//now read other PBO which should be ready

	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, data->pboIds[data->nextIndex]);
	//map buffer so we can access it
	unsigned char *gpu_data = (unsigned char *)glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
	if (gpu_data) {
		//ok. we have the data now available and could process it or copy it somewhere
		//...
		// copy vertically...

		// user function
		JpegMPS_SaveFrame(data,gpu_data);
		//if(on_data != NULL){
		//	on_data.callback(gpu_data,on_data.user_data);
		//}

		//unmap the buffer again
		//downsampleData = nullptr;
		if(!glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB)){
			Log_Error("Cannot unmapbuffer");
		}
	}
	else{
		Log_Error("Error 0x%X",glGetError());
	}

	glBindBuffer(GL_PIXEL_PACK_BUFFER_ARB, 0);


	//back to conventional pixel operation
	//glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);

	// end method 3
	//--------------

	//unbind downsampled data ...
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void JpegMPS_Delete_GL(JpegMPSData *data){
	if(FRAMEBUFFER_OBJECT_EXTENSION_ENABLED){
		glDeleteBuffers(PBO_COUNT, data->pboIds);

		/*for(int i = 0; i < PBO_COUNT; i++){
			if(processed_jpeg[i]!=NULL){
				free(processed_jpeg[i]);
				processed_jpeg[i]=NULL;
			}
		}*/
	}

	if(data->init){

		glDeleteFrameBufferExt(&data->fbo);
	}
}
