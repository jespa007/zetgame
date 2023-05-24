#ifndef __TEXTBOX_NODE_H__
#define __TEXTBOX_NODE_H__

struct TextBoxNode{
	ECTransform				*	ec_transform;
	ECMaterial				*	ec_material;
	ECTextBoxRenderer 		*	ec_textbox_renderer;
	void 					*	data;
};

TextBoxNode * 	TextBoxNode_New(Scene *_scene,Entity *_entity);
void	   		TextBoxNode_Delete(TextBoxNode *_this);

#endif


