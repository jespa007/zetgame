#ifndef _EC_TEXTBOX_RENDER_H__
#define _EC_TEXTBOX_RENDER_H__

typedef struct ECTextBoxRenderer ECTextBoxRenderer;

struct ECTextBoxRenderer{
	EComponentHeader header;
	TextBox 		*	textbox;
	void 			*	data;
};

EComponentList ECTextBoxRenderer_RequiredComponents(void);
void ECTextBoxRenderer_Setup(void *_this,ComponentId _id,  Entity *_entity);
void ECTextBoxRenderer_Update(void *_this);
void ECTextBoxRenderer_Destroy(void *_this);






#endif
