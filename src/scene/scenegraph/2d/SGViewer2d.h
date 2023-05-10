#ifndef __SG_VIEWER_2D_H__
#define __SG_VIEWER_2D_H__

struct SGViewer2d{
	SGNode *sg_node;
	void *	data;
};

SGViewer2d * 	SGViewer2d_New(Scene *_scene, Entity *_entity);
void 			SGViewer2d_SetDimensions(SGViewer2d *_this,uint16_t width, uint16_t height);
void			SGViewer2d_SetTexture(SGViewer2d *_this,Texture *texture);
void	   		SGViewer2d_Delete(SGViewer2d *_this);

#endif
