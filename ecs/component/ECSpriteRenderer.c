#include "ecs/zg_ecs.h"


typedef struct{
	//Shape2d *shape2d;
	Geometry 		*	geometry;
	Appearance	 	*  	appearance;
	uint16_t 			width, height;
}ECSpriteRendererData;

void ECSpriteRenderer_Setup(void *_this){
	ECSpriteRenderer *ec_sprite_renderer=_this;

	ECSpriteRendererData *data=NEW(ECSpriteRendererData);

	data->appearance=Appearance_New();
	data->geometry=Geometry_NewQuad(GEOMETRY_TEXTURE); // Quad by default ?
	ECSpriteRenderer_SetDimensions(ec_sprite_renderer,100,100); // default with/height

	ec_sprite_renderer->entity=NULL;
	ec_sprite_renderer->data=data;
}

void ECSpriteRenderer_Ini(void *_this,Entity *_entity){
	ECSpriteRenderer *ec_sprite_renderer=_this;
	ec_sprite_renderer->entity=_entity;
}


void ECSpriteRenderer_SetDimensions(ECSpriteRenderer *_this,uint16_t width, uint16_t height){

	if(_this == NULL) return;

	ECSpriteRendererData * data= _this->data;
	if(!(data->width == width && data->height == height)){
		// project dimensions
		Vector3f p=ViewPort_ScreenToWorldDim2i(width>>1,height>>1);

		// setup vertexs...
		float vertexs[N_VERTEX_QUAD*VERTEX_COORDS_LEN]={
				   -p.x,-p.y,0.0f,   // bottom left
				   -p.x,+p.y,0.0f,   // top left
				   +p.x,+p.y,0.0f,   // top right
				   +p.x,-p.y,0.0f    // bottom right
		};


		//.. and set vertex to geometry
	   Geometry_SetMeshVertex(data->geometry,(BufferFloat){
			.data=vertexs
			,.len=N_VERTEX_QUAD*VERTEX_COORDS_LEN
		});

		data->width=width;
		data->height=height;

	}
}

void ECSpriteRenderer_SetAlpha(ECSpriteRenderer *_this, float _alpha){
	if(_this == NULL) return;

	ECSpriteRendererData *data=_this->data;
	data->appearance->material->color.a=_alpha;
}

void ECSpriteRenderer_SetTexture(ECSpriteRenderer *_this,Texture *texture){

	if(_this == NULL) return;

	ECSpriteRendererData * data= _this->data;
	data->appearance->texture=texture;
}


void ECSpriteRenderer_Update(void *_this){
	ECSpriteRenderer *ec_sprite_renderer=_this;
	ECTransform *ec_transform=Entity_GetComponent(ec_sprite_renderer->entity,ENTITY_COMPONENT_TRANSFORM);
	ECSpriteRendererData * data= ec_sprite_renderer->data;
	Graphics_Draw(&ec_transform->transform,data->geometry,data->appearance);
}

void ECSpriteRenderer_DeIni(void *_this){
	ECSpriteRendererData * data= ((ECSpriteRenderer *)_this)->data;

	Appearance_Delete(data->appearance);
	Geometry_Delete(data->geometry);

	free(data);
}
