#ifndef __TEXTBOX_NODE_H__
#define __TEXTBOX_NODE_H__

struct TextBoxNode{
	ZG_ECTransform				*	ec_transform;
	ZG_ECMaterial				*	ec_material;
	ZG_ECTextBoxRenderer 		*	ec_textbox_renderer;
	void 					*	data;
};

TextBoxNode * 	TextBoxNode_New(Scene *_scene,ZG_Entity *_entity);
void	   		TextBoxNode_Delete(TextBoxNode *_this);

#endif


