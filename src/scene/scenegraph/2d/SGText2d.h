#ifndef __SG_VIEWER2D_H__
#define __SG_VIEWER2D_H__

typedef struct SGViewer2d SGViewer2d;
struct SGViewer2d{
	SGNode *sg_node;
	void *	data;
};

SGViewer2d * 	SGText2d_New(Entity *_entity);
void 			SGText2d_SetDimensions(SGViewer2d *_this,uint16_t width, uint16_t height);
void			SGText2d_SetFontFile(SGViewer2d *_this,Texture *texture);
void	   		SGText2d_Delete(SGViewer2d *_this);

#endif
