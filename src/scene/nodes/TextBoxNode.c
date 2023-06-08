#include "scene/zg_scene.h"


typedef struct{
		Scene *scene;
}TextBoxNodeData;


TextBoxNode * TextBoxNode_New(Scene *_scene,ZG_Entity *_entity){
	TextBoxNode *textbox_node = ZG_NEW(TextBoxNode);


	textbox_node->ec_transform=_entity->components[EC_TRANSFORM];
	textbox_node->ec_material=_entity->components[EC_MATERIAL];


	textbox_node->ec_textbox_renderer=_entity->components[EC_TEXTBOX_RENDERER];

	TextBoxNodeData *data = ZG_NEW(TextBoxNodeData);
	textbox_node->data=data;
	data->scene=_scene;


	return textbox_node;

}

void	   TextBoxNode_Delete(TextBoxNode *_this){
	if(_this == NULL) return;
	TextBoxNodeData * data=_this->data;

	ZG_FREE(data);
	ZG_FREE(_this);
}
