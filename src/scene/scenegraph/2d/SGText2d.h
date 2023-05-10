#ifndef __SG_TEXT_2D_H__
#define __SG_TEXT_2D_H__

struct SGText2d{
	SGNode 		*	sg_node;
	TextBox		*	textbox;
	void 		*	data;
};

SGText2d 	* 	SGText2d_New(Scene *_scene,Entity *_entity);
void	   		SGText2d_Delete(SGText2d *_this);

#endif
