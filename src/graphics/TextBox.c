#include "../zetgame.h"
#include "zg_graphics.h"

//#define TBRT_NEXT_CHAR(text,ch,sizechar) while(text!=NULL && *text!=0 && *text!=ch) text++
//#define TBRT_SKIP_SPACES(text,sizechar) 	while(*text==' ' && *text!=0) text++

typedef enum{
	TBRT_TOKEN_TYPE_WORD,
	TBRT_TOKEN_TYPE_FORMAT
}TBRT_TokenType;


typedef struct{
	void *word; 		// allocated text (char or char type in function last text set)
	uint16_t word_width;

}TBRT_TokenWord;

typedef struct{
	TTFont *font;
	Color4f color;
}TBRT_Format;

typedef struct{
	List * tbrt_tokens; // list of word or format
	uint16_t total_width;
}TBRT_TokenLine;

typedef struct{
	TBRT_Format text_format;
}TBRT_TokenFormat;

// parse inline html stuff
typedef struct{
	void * token_data;
	TBRT_TokenType tbrt_token_type;
}TBRT_Token;


typedef struct{
	List	*token_lines; // each line a list of tb_tokens word, end line, format, etc
}TBRenderText;


typedef struct{
	//Shape2d 	*	shape2d;
	TTFont 		*	font; // init font
	void 		*	text; // text to show, from reserved memory
	CharType		char_type;
	VerticalAlign 	vertical_align;
	TextAlign 		text_align;
	Vector2i		dimensions;
	TBRenderText	render_text;

}TextboxData;


TextBox *Textbox_New(void){
	TextBox *textbox = ZG_NEW(TextBox);
	TextboxData *data=ZG_NEW(TextboxData);
	textbox->data=data;
	//data->shape2d=Shape2d_New();
	data->font=TTFontManager_GetEmbeddedFont();
	Textbox_SetText(textbox,"Hello World");
	return textbox;
}

//---------------------------------------------------------------------------------------------------------------------------------------
TBRT_Token *Textbox_RT_NewTokenWord(
		char *word
		,uint16_t word_width
		){
	TBRT_Token *token=ZG_NEW(TBRT_Token);
	TBRT_TokenWord *token_word=ZG_NEW(TBRT_TokenWord);
	token_word->word_width=word_width;
	token_word->word=word;

	token->tbrt_token_type=TBRT_TOKEN_TYPE_WORD;
	token->token_data=token_word;
	return token;
}

TBRT_TokenLine *Textbox_RT_NewLine(TextboxData *data){

	TBRT_TokenLine *tbrt_token_line=ZG_NEW(TBRT_TokenLine);
	tbrt_token_line->tbrt_tokens=List_New();

	List_Add(data->render_text.token_lines,tbrt_token_line);
	return tbrt_token_line;
}


void Textbox_RT_Delete(TextboxData *data){

	if(data->render_text.token_lines!=NULL){
		for(unsigned i=0; i < data->render_text.token_lines->count; i++){
			TBRT_TokenLine *token_line=data->render_text.token_lines->items[i];
			for(unsigned j=0; j < token_line->tbrt_tokens->count; j++){
				TBRT_Token *_token=token_line->tbrt_tokens->items[j];
				if(_token->token_data != NULL){
					if(_token->tbrt_token_type == TBRT_TOKEN_TYPE_WORD){
						TBRT_TokenWord *token_word = _token->token_data;
						ZG_FREE(token_word->word);
					}
					ZG_FREE(_token->token_data);
				}
				ZG_FREE(_token);

			}
			List_Delete(token_line->tbrt_tokens);
			ZG_FREE(token_line);
		}

		List_Delete(data->render_text.token_lines);
		data->render_text.token_lines=NULL;

	}

}


void Textbox_RT_Init(TextboxData *data){

	Textbox_RT_Delete(data);

	data->render_text.token_lines=List_New();
}

void Textbox_RT_Build(TextBox *_this){
	TextboxData *data=_this->data;
	unsigned long ch_space=(unsigned long)' ';
	size_t sizeof_char=sizeof(char);
	if(data->char_type==CHAR_TYPE_WCHAR){
		sizeof_char=sizeof(wchar_t);
		ch_space=(unsigned long)L' ';
	}

	float inv_sizeof_char=1.0f/sizeof_char;

	// reset current render text data...
	Textbox_RT_Init(data);

	if(data->char_type==CHAR_TYPE_WCHAR){
		if(StrUtils_WStrIsNullOrEmpty((wchar_t *)data->text)) return;
	}
	else{
		if(StrUtils_StrIsNullOrEmpty(data->text)) return;
	}

	List *lines=NULL;

	if(data->char_type==CHAR_TYPE_WCHAR){
		lines=StrUtils_WStrSplitWStr(data->text,L"<br>"); // get lines ...
	}
	else{
		lines=StrUtils_StrSplitStr(data->text,"<br>"); // get lines ...
	}


	TBRT_TokenLine *tbrt_token_line=NULL;//data->render_text.tb_tokens;


	uint16_t word_width=0;
	uint16_t space_width=data->font->space_width;

	for(unsigned i=0; i < lines->count; i++){
		void *text_line=lines->items[i];
		tbrt_token_line=Textbox_RT_NewLine(data);
		unsigned long ch=0;
		do{

			void *word=NULL;
			void *word_ini=text_line;
			void *word_end=text_line;
			size_t word_len=0;
			bool end_char=false;


			do{

				ch=StrUtils_GetChar(text_line,data->char_type);
				if(!(ch == 0 || ch == ch_space)){
					StrUtils_Advance(&text_line,data->char_type);

				}else{
					end_char=true;
				}
			}while(!end_char);

			word_end=text_line;

			//TBRT_NEXT_CHAR(text_line,' ',data->char_type==CHAR_TYPE_WCHAR?wchar_t:char);
			//word_end=text_line;

			// skip chars...
			if(ch!=0){
				end_char=false;

				do{
					ch=StrUtils_GetChar(text_line,data->char_type);
					if((ch == 0 || ch == ch_space)){
						StrUtils_Advance(&text_line,data->char_type);
					}else{
						end_char=true;
					}
				}while(!end_char);
			}


			word_len=((intptr_t)word_end-(intptr_t)word_ini)*inv_sizeof_char; // div size of char or wchar
			word=malloc(word_len+sizeof_char);
			memset(word,0,word_len*inv_sizeof_char+sizeof_char);
			memcpy(word,word_ini,word_len*inv_sizeof_char);

			// get len word...
			if(data->char_type==CHAR_TYPE_WCHAR){
				word_width=TTFont_WGetWidthN(data->font,word_ini,word_len);
			}else{
				word_width=TTFont_GetWidthN(data->font,word_ini,word_len);
			}

			if((((tbrt_token_line->total_width+space_width)>data->dimensions.x) && tbrt_token_line->total_width > 0)){ // if line exceeds max dimension, create new line..
				tbrt_token_line=Textbox_RT_NewLine(data);
			}

			// add word token...
			List_Add(tbrt_token_line->tbrt_tokens,Textbox_RT_NewTokenWord(word,word_width));


			tbrt_token_line->total_width+=(word_width+space_width);


		}while(ch!=0); // not end line...

	}

	List_DeleteAndFreeAllItems(lines);

}

//---------------------------------------------------------------------------------------------------------------------------------------

void     Textbox_SetText(TextBox *_this,const char *in, ...){
	char out[1024]={0};
	ZG_VARGS(out,in);

	TextboxData *data=_this->data;
	if(data->text!=NULL){
		free(data->text);
	}

	data->char_type=CHAR_TYPE_CHAR;
	size_t total_mem=strlen(out)+sizeof(char); // +1 end string
	data->text=malloc(total_mem);
	memset(data->text,0,total_mem);

	strcpy(data->text,out);

	Textbox_RT_Build(_this);
}


void     Textbox_WSetText(TextBox *_this,const wchar_t *in, ...){
	wchar_t out[1024]={0};
	ZG_WVARGS(out,in);

	TextboxData *data=_this->data;
	if(data->text!=NULL){
		free(data->text);
	}

	data->char_type=CHAR_TYPE_WCHAR;
	size_t total_mem=wcslen(out)+sizeof(wchar_t);  // +1 end string
	data->text=malloc(total_mem);
	memset(data->text,0,total_mem);

	wcscpy(data->text,out);

	Textbox_RT_Build(_this);
}

void     Textbox_SetFont(TextBox *_this, TTFont *font){
	TextboxData *data=_this->data;

	if(data->font != font){
		data->font=font;
		Textbox_RT_Build(_this);
	}
}

void	 Textbox_SetTextAlign(TextBox *_this, TextAlign text_align){
	TextboxData *data=_this->data;
	data->text_align=text_align;
}

void	 Textbox_SetVerticalAlign(TextBox *_this, VerticalAlign vertical_align){
	TextboxData *data=_this->data;
	data->vertical_align=vertical_align;
}


void	 Textbox_SetDimensions(TextBox *_this, uint16_t w, uint16_t h){
	TextboxData *data=_this->data;

	if(!(data->dimensions.x==w && data->dimensions.y==h)){
		Textbox_RT_Build(_this);
		data->dimensions.x=w;
		data->dimensions.y=h;
	}
}

void	 Textbox_Draw(TextBox *_this, Transform *transform,Color4f *color){

	TextboxData *data=_this->data;

	// TODO: pos is at center box by default, do a wat yo change render center
	//Vector2i start_pos=Vector2i_New(0,0);

	if(transform != NULL){
		Transform_Apply(transform);
		//start_pos=Vector2i_New(ViewPort_CurrentWidth()>>1,ViewPort_CurrentHeight()>>1); // this offset needed because we applying second transformation takin on center screen
	}

	int y=-(data->dimensions.y>>1); // default aligned top-left
	int x=0;
	float x_draw=0;
	float y_draw=0;
	float y_draw_inc=0;
	Vector3f dim3d=ViewPort_ScreenToWorldDimension2i(data->dimensions.x,data->dimensions.y);
	uint16_t text_total_height=data->render_text.token_lines->count*data->font->ascender;

	if(data->vertical_align == VERTICAL_ALIGN_CENTER){
		y=-(text_total_height>>1);
	}
	else if(data->vertical_align == VERTICAL_ALIGN_BOTTOM){
		y=+(data->dimensions.y>>1)-text_total_height;
	}

	y_draw=-ViewPort_ScreenToWorldHeight(y);
	y_draw_inc=-ViewPort_ScreenToWorldHeight(data->font->ascender);

	if(ZetGame_IsDebugMode()){
		Graphics_DrawRectangle4f(
				0 // x:0 translation keeps current translate
				,0// y:0 translation keeps current translate
				,dim3d.x
				,dim3d.y
				,COLOR4F_WHITE
				,2
				);
	}

	TTFont_RenderTextBegin(color);

	for(unsigned i=0; i < data->render_text.token_lines->count; i++){
		int inc_x=1;
		TBRT_TokenLine *token_line = data->render_text.token_lines->items[i];
		x=-(data->dimensions.x>>1); // default text_align_right
		if(data->text_align == TEXT_ALIGN_CENTER){
			x=-(token_line->total_width>>1);
		}else if(data->text_align == TEXT_ALIGN_RIGHT){
			x=(data->dimensions.x>>1);
			inc_x=-1;
		}

		for(unsigned w=0; w < token_line->tbrt_tokens->count; w++){

			TBRT_Token *token=NULL;
			if(inc_x > 0){
				token=token_line->tbrt_tokens->items[w];
			}
			else{
				token=token_line->tbrt_tokens->items[token_line->tbrt_tokens->count-w-1];
			}

			TBRT_TokenWord *token_word;
			switch(token->tbrt_token_type){
			default:
				break;
			case TBRT_TOKEN_TYPE_WORD:
				token_word=((TBRT_TokenWord*)token->token_data);
				if(w>0){ // render space
					//TTFont_Print(data->font,x,y,COLOR4F_WHITE," ");
					x+=(data->font->space_width)*inc_x;
				}

				x_draw=ViewPort_ScreenToWorldWidth(x+(inc_x<1?-token_word->word_width:0));

				if(data->char_type==CHAR_TYPE_WCHAR){
					TTFont_WPrint(data->font,x_draw,y_draw,COLOR4F_WHITE,token_word->word);
				}
				else{ // char by default.
					TTFont_Print(data->font,x_draw,y_draw,COLOR4F_WHITE,token_word->word);
				}
				x+=(token_word->word_width)*inc_x;
				break;

			}
		}

		y_draw+=y_draw_inc;
	}

	TTFont_RenderTextEnd();

	if(transform != NULL){
		Transform_Restore(transform);
	}

}

void Textbox_Delete(TextBox * _this){
	if(_this==NULL) return;
	TextboxData *data=_this->data;


	if(data->text!=NULL){
		free(data->text);
	}

	Textbox_RT_Delete(data);

	free(_this->data);
	free(_this);
}
