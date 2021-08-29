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

TTFontCharacter * TTFont_GL_BuildChar(TTFont *font,unsigned long c){
	//GLuint VAO, VBO;
	MapInt *characters= NULL;
	TTFontCharacter *new_character=NULL;
	CharacterDataGL *character_data=NULL;
	FT_Face face;

	if(font==NULL) {
		return NULL;
	}

	characters=font->characters;
	face=font->ft_face;


    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set


	GLuint texture=GL_INVALID_VALUE;

	// Load character glyph
	if (FT_Load_Char(face, c, FT_LOAD_RENDER))
	{
		Log_Error("FREETYTPE: Failed to load Glyph");
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
	new_character = NEW(TTFontCharacter);
	character_data=NEW(CharacterDataGL);
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

void TTFont_GL_RenderText(TTFont *font,float x3d, float y3d, Color4f color, const void *text, CharType fftont_text){
	void *ptr=(void *)text;
	unsigned long c=0;
	//int x_ini=x;

	if(font == NULL) return;
	//TTFontDataGL * font_data=(TTFontDataGL *)font->font_data;
	glColor3f(color.r,color.g, color.b);

	//Vector3f start_pos3d=ViewPort_ScreenToWorld(x,y);


	while((c=StrUtils_GetCharAndAdvance(&ptr,fftont_text))!=0)
	{
		TTFontCharacter *ch=(TTFontCharacter *)MapInt_Get(font->characters,c);
		if(ch==NULL){ // build
			ch=TTFont_GL_BuildChar(font,c);

			if(ch==NULL){
				continue;
			}
		}

		CharacterDataGL *ch_data=ch->character_data;
		//int offset_origin=(ch->size.y - ch->bearing.y);
		//Vector2i p1_2d=Vector2i_New(ch->bearing.x	,font->ascender - ch->size.y);
		//Vector2i p2_2d=Vector2i_New(ch->size.x	,font->ascender);

		Vector3f p1_3d=ViewPort_ScreenToWorldDimension2i(ch->bearing.x,font->ascender - ch->size.y);
		Vector3f p2_3d=ViewPort_ScreenToWorldDimension2i(ch->size.x,font->ascender);

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch_data->texture);

		// Render quad
		//Geometry_Draw(font_data->char_geometry);
		glBegin(GL_TRIANGLE_STRIP); // draw something with the texture on
				glTexCoord2f(0, 0);
				glVertex2f(x3d+p1_3d.x, y3d-p1_3d.y);

				glTexCoord2f(1, 0);
				glVertex2f(x3d+p2_3d.x, y3d-p1_3d.y);

				glTexCoord2f(0, 1);
				glVertex2f(x3d+p1_3d.x,y3d-p2_3d.y);

				glTexCoord2f(1, 1);
				glVertex2f(x3d+p2_3d.x,y3d-p2_3d.y);
		glEnd();

		x3d += ViewPort_ScreenToWorldWidth(ch->advance >> 6); // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}



}


void TTFont_GL_RenderTextEnd(void){
	glPopAttrib();
}


void TTFont_GL_Print(TTFont *font,float x, float y,Color4f color,const char *str){
	TTFont_GL_RenderText(font,x,y,color,str,CHAR_TYPE_CHAR);
}

void TTFont_GL_WPrint(TTFont *font,float x, float y,Color4f color,const wchar_t *str){
	TTFont_GL_RenderText(font,x,y,color,str,CHAR_TYPE_WCHAR);
}


void TTFont_GL_Delete(TTFont *font){

	for(unsigned i = 0; i < font->characters->list->count; i++){
		MapIntNode * node = (MapIntNode *)font->characters->list->items[i];
		TTFontCharacter *ch = (TTFontCharacter *)node->val;//font->characters->list->items[i];
		CharacterDataGL *ch_data=ch->character_data;
		glDeleteTextures(1,&ch_data->texture);

		FREE(ch_data);
		FREE(ch);
	}

}

void TTFont_GL_DeInit(void){

}


