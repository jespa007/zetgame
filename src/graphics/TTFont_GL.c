/**
 * Font render using TrueTypeFont tutorials:
 * https://learnopengl.com/In-Practice/Text-Rendering
 * http://lazyfoo.net/tutorials/OpenGL/23_freetype_fonts/index.php
 */

#include "zg_graphics.h"
#include "zg_graphics.h"

typedef struct {
    GLuint texture;   // ID handle of the glyph texture
}CharacterDataGL;

void TTFont_GL_Init(void){
	// PRE: OpenGL context is already initialized!
}

void TTFont_GL_BuildChar(TTFontCharacter *_font_character, FT_Face _face){

	GLuint texture=GL_INVALID_VALUE;

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Generate texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_ALPHA,
		_face->glyph->bitmap.width,
		_face->glyph->bitmap.rows,
		0,
		GL_ALPHA,
		GL_UNSIGNED_BYTE,
		_face->glyph->bitmap.buffer
	);
	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    CharacterDataGL *character_data=ZG_NEW(CharacterDataGL);
    character_data->texture=texture;

    _font_character->data=character_data;

}


void TTFont_GL_DeleteChar(TTFontCharacter *_font_character){
	CharacterDataGL *ch_data=_font_character->data;
	glDeleteTextures(1,&ch_data->texture);
	ZG_FREE(ch_data);
}

void TTFont_GL_RenderTextBegin(Color4f *color){

	glPushAttrib(GL_CURRENT_BIT |GL_TEXTURE_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);

	if(color!=NULL){
		glColor4f(color->r,color->g,color->b,color->a);
	}

}


void TTFont_GL_RenderTextEnd(void){
	glPopAttrib();
}


void TTFont_GL_DeInit(void){

}


