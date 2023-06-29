#ifndef _ZG_ECTEXTBOX_RENDER_H__
#define _ZG_ECTEXTBOX_RENDER_H__

typedef struct ZG_ECTextBoxRenderer ZG_ECTextBoxRenderer;

struct ZG_ECTextBoxRenderer{
	ZG_EComponentHeader header;
	ZG_TextBox 		*	textbox;
	void 			*	data;
};

ZG_ComponentList ZG_ECTextBoxRenderer_RequiredComponents(void);
void ZG_ECTextBoxRenderer_OnCreate(void *_this,ZG_ComponentId _id);
void ZG_ECTextBoxRenderer_OnUpdate(void *_this);
void ZG_ECTextBoxRenderer_OnDestroy(void *_this);






#endif
