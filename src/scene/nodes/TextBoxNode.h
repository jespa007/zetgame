#ifndef __TEXTBOX_NODE_H__
#define __TEXTBOX_NODE_H__

struct TextBoxNode{
	ECTransform				*	ec_transform;
	ECTransformAnimation	*	ec_transform_animation;
	ECMaterial				*	ec_material;
	ECMaterialAnimation		*	ec_material_animation;
	ECTextBoxRenderer 		*	ec_textbox_renderer;
	void 					*	data;
};

TextBoxNode * 	TextBoxNode_New(Entity *_entity);
void	   		TextBoxNode_Delete(TextBoxNode *_this);

#endif


