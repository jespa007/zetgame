#ifndef __SGTEXTBOX_H__
#define __SGTEXTBOX_H__

typedef struct SGTextBox2d SGTextBox2d;
struct SGTextBox2d{
	SGNode *node;
	void *	data;
};

SGTextBox2d * 	SGTextBox2d_New(void);
void			SGTextBox2d_SetDimensions(SGTextBox2d *_this,uint16_t width, uint16_t height);
void 			SGTextBox2d_SetText(SGTextBox2d *_this, const char *str, ...);
void 			SGTextBox2d_SetWText(SGTextBox2d *_this, const wchar_t *str, ...);
void	   		SGTextBox2d_Delete(SGTextBox2d *_this);

#endif


