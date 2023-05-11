#include "scene/zg_scene.h"


typedef struct{
		void *data;
}TextBoxNodeData;


TextBoxNode * TextBoxNode_New(Entity *_entity){
	TextBoxNode *textbox_node = ZG_NEW(TextBoxNode);


	textbox_node->ec_transform=_entity->components[EC_TRANSFORM];
	textbox_node->ec_transform_animation=_entity->components[EC_TRANSFORM_ANIMATION];
	textbox_node->ec_material=_entity->components[EC_MATERIAL];
	textbox_node->ec_material_animation=_entity->components[EC_MATERIAL_ANIMATION];

	textbox_node->ec_textbox_renderer=_entity->components[EC_TEXTBOX_RENDERER];

	TextBoxNodeData *data = ZG_NEW(TextBoxNodeData);
	textbox_node->data=data;


	return textbox_node;

}

void	   TextBoxNode_Delete(TextBoxNode *_this){
	if(_this == NULL) return;
	TextBoxNodeData * data=_this->data;

	TransformNode_Delete(_this->sg_node);

	ZG_FREE(data);
	ZG_FREE(_this);
}
