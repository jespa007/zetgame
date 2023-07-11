#include "ecs/_zg_ecs_.h"


typedef struct{
	//Shape2d *shape2d;
	ZG_ComponentId 			ec_geometry;
	ZG_ComponentId 			ec_material;
	ZG_Appearance	 	  	*appearance;
	uint16_t 				width, height;
}ZG_ECSpriteRendererData;

static const char * g_ec_sprite_renderer_required_components[]={
		ZG_STR_CONCAT("",ECTransform)
		,ZG_STR_CONCAT("",ECGeometry)
		,ZG_STR_CONCAT("",ECMaterial)
		,ZG_STR_CONCAT("",ECTexture)
};

//----

ZG_ComponentList ZG_ECSpriteRenderer_RequiredComponents(void){
	ZG_ComponentList cl;
	cl.components=g_ec_sprite_renderer_required_components;
	cl.n_components=ZG_ARRAY_SIZE(g_ec_sprite_renderer_required_components);

	return cl;
}

void ZG_ECSpriteRenderer_OnCreate(void *_this, ZG_Entity _entity){
	ZG_ECSpriteRenderer *ec_sprite_renderer=_this;

	//ec_sprite_renderer->header.entity=_entity;
	//ec_sprite_renderer->header.id=_id;
	//_entity->components[EC_SPRITE_RENDERER]=_this;

	ZG_ECSpriteRendererData *data=ZG_NEW(ZG_ECSpriteRendererData);

	data->ec_geometry=ZG_ENTITY_TYPE_GET_COMPONENT(ec_sprite_renderer->header.archetype,ECGeometry,_id);//_entity->components[EC_GEOMETRY];


	if(data->ec_geometry==NULL){
		ZG_LOG_ERRORF("ZG_ECSpriteRenderer_OnCreate : EntityType doesn't have geometry");
		return;
	}

	data->ec_material=ZG_ENTITY_TYPE_GET_COMPONENT(ec_sprite_renderer->header.archetype,ECMaterial,_id);//_entity->components[EC_MATERIAL];

	if(data->ec_material==NULL){
		ZG_LOG_ERRORF("ZG_ECSpriteRenderer_OnCreate : EntityType doesn't have material component");
		return;
	}


	data->appearance=ZG_Appearance_New();
	data->ec_geometry->geometry=ZG_Geometry_NewRectangleFilled(ZG_GEOMETRY_PROPERTY_TEXTURE); // Quad by default ?
	data->ec_material->material=data->appearance->material=ZG_Material_New(0); // Mat by default ?

	ec_sprite_renderer->data=data;

	ZG_ECSpriteRenderer_SetDimensions(ec_sprite_renderer,100,100); // default with/height
}


void ZG_ECSpriteRenderer_OnUpdate(void *_this, ZG_Entity _entity){
	ZG_ECSpriteRenderer *ec_sprite_renderer=_this;
	ZG_ECSpriteRendererData * data= ec_sprite_renderer->data;
	ZG_Transform *transform = NULL;
	ZG_ECTransform *ec_transform=ec_sprite_renderer->header.entity->components[EC_TRANSFORM];
	ZG_ECTexture *ec_texture=ec_sprite_renderer->header.entity->components[EC_TEXTURE];
	if(ec_transform){
		transform=&ec_transform->transform;
	}

	if(ec_texture){
		data->appearance->texture=ec_texture->texture;
	}

	Graphics_Draw(transform,data->geometry,data->appearance);
}

void ZG_ECSpriteRenderer_OnDestroy(void *_this, ZG_Entity _entity){
	ZG_ECSpriteRendererData * data= ((ZG_ECSpriteRenderer *)_this)->data;
	ZG_Appearance_Delete(data->appearance);
	ZG_FREE(data);
}

//----

void ZG_ECSpriteRenderer_SetDimensions(ZG_ECSpriteRenderer *_this,uint16_t width, uint16_t height){

	if(_this == NULL) return;

	ZG_ECSpriteRendererData * data= _this->data;
	if(!(data->width == width && data->height == height)){
		// project dimensions
		ZG_Vector3f p=ZG_ViewPort_ScreenToWorldDimension2i(width>>1,height>>1);

		// setup vertexs...
		float vertexs[ZG_N_VERTEXS_QUAD*ZG_VERTEX_COORDS_LEN]={
				   -p.x,-p.y,0.0f,  // bottom left
				   +p.x,-p.y,0.0f,  // bottom right
				   -p.x,+p.y,0.0f,  // top left
				   +p.x,+p.y,0.0f   // top right

		};


		//.. and set vertex to geometry
	   ZG_Geometry_SetMeshVertex(data->ec_geometry->geometry,vertexs,ZG_N_VERTEXS_QUAD*ZG_VERTEX_COORDS_LEN);

		data->width=width;
		data->height=height;

	}
}

void ZG_ECSpriteRenderer_SetAlpha(ZG_ECSpriteRenderer *_this, float _alpha){
	if(_this == NULL) return;

	ZG_ECSpriteRendererData *data=_this->data;
	data->appearance->material->color.a=_alpha;
}

void ZG_ECSpriteRenderer_SetTexture(ZG_ECSpriteRenderer *_this,ZG_Texture *texture){

	if(_this == NULL) return;

	ZG_ECSpriteRendererData * data= _this->data;
	data->appearance->texture=texture;
}



