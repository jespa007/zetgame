#ifndef _ZG_ECTEXTBOX_RENDER_H__
#define _ZG_ECTEXTBOX_RENDER_H__

typedef struct ZG_ECTextBoxRenderer ZG_ECTextBoxRenderer;

struct ZG_ECTextBoxRenderer{
	ZG_EComponentHeader header;
	ZG_TextBox 		*	textbox;
	void 			*	data;
};

ZG_EComponentList ZG_ECTextBoxRenderer_RequiredComponents(void);
void ZG_ECTextBoxRenderer_Setup(ZG_EComponentHeader *_this,ZG_ComponentId _id);
void ZG_ECTextBoxRenderer_Update(ZG_EComponentHeader *_this);
void ZG_ECTextBoxRenderer_Destroy(ZG_EComponentHeader *_this);






#endif
