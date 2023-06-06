#ifndef _EC_TEXTBOX_RENDER_H__
#define _EC_TEXTBOX_RENDER_H__

typedef struct ECTextBoxRenderer ECTextBoxRenderer;

struct ECTextBoxRenderer{
	EComponentHeader header;
	ZG_TextBox 		*	textbox;
	void 			*	data;
};

EComponentList ECTextBoxRenderer_RequiredComponents(void);
void ECTextBoxRenderer_Setup(void *_this,ComponentId _id);
void ECTextBoxRenderer_Update(void *_this);
void ECTextBoxRenderer_Destroy(void *_this);






#endif
