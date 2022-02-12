#include "scene/zg_scene.h"


typedef struct{
	//Shape2d *shape2d;
	Geometry *	geometry;
	Texture	 *  texture;
	uint16_t 		width, height;
}SGViewer2dData;


SGViewer2d * SGViewer2d_New(void){
	SGViewer2d *sg_image = ZG_NEW(SGViewer2d);

	sg_image->node=SGNode_New();
	//sg_image->node->sgnode_type=SGNODE_TYPE_IMAGE;

	SGViewer2dData *data = ZG_NEW(SGViewer2dData);
	sg_image->data=data;
	data->texture=NULL;

	sg_image->node->geometry=data->geometry = Geometry_NewRectangleFilled(GEOMETRY_TEXTURE);
	//viewer_2d->node->draw=SGViewer2d_Draw();
	//viewer_2d->node->shape=data->shape2d->shape;

	//data->shape2d=Shape2d_New();
	SGViewer2d_SetDimensions(sg_image,100,100);

	return sg_image;

}

void SGViewer2d_SetDimensions(SGViewer2d *_this,uint16_t width, uint16_t height){

	SGViewer2dData * data= _this->data;
	if(!(data->width == width && data->height == height)){
		// project dimensions
		Vector3f p=ViewPort_ScreenToWorldDimension2i(width>>1,height>>1);

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

void		SGViewer2d_SetTexture(SGViewer2d *_this,Texture *texture){
	SGViewer2dData * data=_this->data;
	data->texture=texture;
}


void	   SGViewer2d_Delete(SGViewer2d *_this){
	if(_this == NULL) return;
	SGViewer2dData * data=_this->data;

	Geometry_Delete(data->geometry);
	SGNode_Delete(_this->node);

	FREE(data);
	FREE(_this);
}
