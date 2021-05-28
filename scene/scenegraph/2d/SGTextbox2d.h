#ifndef __SGTEXTBOX_H__
#define __SGTEXTBOX_H__

typedef struct SGTextbox2d SGTextbox2d;
struct SGTextbox2d{
	SGNode *node;
	void *	data;
};

SGTextbox2d * 	SGTextbox2d_New(void);
void			SGTextbox2d_SetDimensions(SGTextbox2d *_this,uint16_t width, uint16_t height);
void 			SGTextbox2d_SetText(SGTextbox2d *_this, const char *str, ...);
void 			SGTextbox2d_SetWText(SGTextbox2d *_this, const wchar_t *str, ...);
void	   		SGTextbox2d_Delete(SGTextbox2d *_this);

#endif


