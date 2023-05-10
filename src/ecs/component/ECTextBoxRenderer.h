#ifndef _EC_TEXTBOX_RENDER_H__
#define _EC_TEXTBOX_RENDER_H__

typedef struct ECTextBoxRenderer ECTextBoxRenderer;

struct ECTextBoxRenderer{
	EComponentHeader header;
	TextBox 		*	textbox;
	void 			*	data;
};

EComponentList ECTextBoxRenderer_RequiredComponents(void);
void ECTextBoxRenderer_Setup(void *_this, Entity *_entity);

//void ECTextBoxRenderer_SetQuadPixels(uint16 width, uint16 height);
void ECTextBoxRenderer_SetDimensions(ECTextBoxRenderer *_this,uint16_t width, uint16_t height);
void ECTextBoxRenderer_SetFontFile(ECTextBoxRenderer *_this,const char *_font_file);
void ECTextBoxRenderer_SetText(ECTextBoxRenderer *_this,const char *_font_file);

void ECTextBoxRenderer_Update(void *_this);
void ECTextBoxRenderer_Destroy(void *_this);






#endif
