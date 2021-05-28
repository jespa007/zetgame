#include "zg_graphics.h"

void Appearance_GL_Apply(Appearance * appearance){


	glPushAttrib( GL_TEXTURE_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT);

	if((appearance->properties & APPEARANCE_CULL_FACE)){
		glEnable(GL_CULL_FACE);
	}else{
		glDisable(GL_CULL_FACE);
	}


	if(appearance->properties & APPEARANCE_DEPTH_TEST){
		glEnable(GL_DEPTH_TEST);
	}else{
		glDisable(GL_DEPTH_TEST);
	}


	if(appearance->properties & APPEARANCE_LIGHT) {// doesn't matter if the object have material (o light)
		glEnable(GL_LIGHTING);
	}else{
		glDisable(GL_LIGHTING);
	}

	if(appearance->material){
		Material_Apply(appearance->material);
	}

	if(appearance->properties & APPEARANCE_TRANSPARENCY) {

		glEnable(GL_BLEND);
		if(appearance->properties & APPEARANCE_INTENSITY){
			glBlendFunc(GL_SRC_ALPHA,GL_ONE); // intensity
		}else{ // normal operation ...
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // normal
		}

		//glBlendFunc (GL_ONE, GL_ONE);
		//glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_NOTEQUAL, 0);
	}

	if(appearance->texture != NULL) {
		// _this piece of code should be inside appearance apply ?!?!?!?
		Texture_Bind(appearance->texture);


		if(appearance->transform_texture != NULL)
		{
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			Transform_Apply(appearance->transform_texture);
			// we need to take back GL_MODEL VIEW to have transform into 3d world
			glMatrixMode(GL_MODELVIEW);
		}
	}
}

void Appearance_GL_Restore(Appearance * appearance){
	if(appearance->transform_texture!=NULL)
	{
		glMatrixMode(GL_TEXTURE); // restore texture matrix ...
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		//glPopMatrix();
	}
	glPopAttrib();
}

