#ifndef __SG_VIEWER2D_H__
#define __SG_VIEWER2D_H__

typedef struct SGViewer2d SGViewer2d;
struct SGViewer2d{
	SGNode *node;
	void *	data;
};

SGViewer2d * 	SGViewer2d_New(void);
void 		SGViewer2d_SetDimensions(SGViewer2d *_this,uint16_t width, uint16_t height);
void		SGViewer2d_SetTexture(SGViewer2d *_this,Texture *texture);
void	   	SGViewer2d_Delete(SGViewer2d *_this);

#endif
