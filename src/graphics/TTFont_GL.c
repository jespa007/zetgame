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

/*typedef struct {
	Geometry *char_geometry;
}TTFontDataGL;*/


void TTFont_GL_Init(void){
	// PRE: OpenGL context is already initialized!
}

TTFontCharacter * TTFont_GL_BuildChar(TTFont *_this,unsigned long c){
	//GLuint VAO, VBO;
	MapInt *characters= NULL;
	TTFontCharacter *new_character=NULL;
	CharacterDataGL *character_data=NULL;
	FT_Face face;

	if(_this==NULL) {
		return NULL;
	}

	TTFontData *data=_this->data;

	characters=_this->characters;
	face=data->ft_face;


    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set


	GLuint texture=GL_INVALID_VALUE;

	// Load character glyph
	if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	{
		Log_ErrorF("FREETYTPE: Failed to load Glyph");
		return NULL;
	}

	// Generate texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_ALPHA,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_ALPHA,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);
	// Set texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Now store character for later use
	new_character = ZG_NEW(TTFontCharacter);
	character_data=ZG_NEW(CharacterDataGL);
	*new_character=(TTFontCharacter){
		.character_data=character_data
		,.size=Vector2i_New(face->glyph->bitmap.width, face->glyph->bitmap.rows)
		,.bearing=Vector2i_New(face->glyph->bitmap_left, face->glyph->bitmap_top)
		,.advance=face->glyph->advance.x
	};

	character_data->texture=texture;

	MapInt_Set(characters,c,new_character);


    glBindTexture(GL_TEXTURE_2D, 0);

    return new_character;
}

void TTFont_GL_BuildChars(TTFont *font,unsigned long char_ini, unsigned long char_end){
	//GLuint VAO, VBO;
    // Load first 128 characters of ASCII set
    for (unsigned long c = char_ini; c < char_end; c++)
    {
    	TTFont_GL_BuildChar(font,c);
    }
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



void TTFont_GL_Delete(TTFont *font){

	for(unsigned i = 0; i < font->characters->list->count; i++){
		MapIntNode * node = (MapIntNode *)font->characters->list->items[i];
		TTFontCharacter *ch = (TTFontCharacter *)node->val;//font->characters->list->items[i];
		CharacterDataGL *ch_data=ch->character_data;
		glDeleteTextures(1,&ch_data->texture);

		ZG_FREE(ch_data);
		ZG_FREE(ch);
	}

}

void TTFont_GL_DeInit(void){

}


