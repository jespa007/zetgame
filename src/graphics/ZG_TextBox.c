#include "zetgame.h"
#include "@zg_graphics.h"

typedef enum{
	ZG_TBRT_TOKEN_TYPE_WORD,
	ZG_TBRT_TOKEN_TYPE_FORMAT
}ZG_TBRT_TokenType;


typedef struct{
	void *word; 		// allocated text (char or char type in function last text set)
	uint16_t word_width;

}ZG_TBRT_TokenWord;

typedef struct{
	ZG_TTFont *font;
	ZG_Color4f color;
}ZG_TBRT_Format;

typedef struct{
	ZG_List * tbrt_tokens; // list of word or format
	uint16_t total_word_width;
	float space_width;
}ZG_TBRT_TokenLine;

typedef struct{
	ZG_TBRT_Format text_format;
}ZG_TBRT_TokenFormat;

// parse inline html stuff
typedef struct{
	void * token_data;
	ZG_TBRT_TokenType tbrt_token_type;
}ZG_TBRT_Token;


typedef struct{
	ZG_List	*		token_lines; // each line a list of tb_tokens word, end line, format, etc
	ZG_BoundingBox		bounding_box; // bounding total rendered text
}ZG_TBRenderText;


typedef struct{
	ZG_TTFont 				*		font; // init font
	void 				*		text; // text to show, from reserved memory
	ZG_CharType					char_type;
	ZG_VerticalAlignment 			vertical_alignment;
	ZG_HorizontalAlignment 		horizontal_alignment;
	ZG_Vector2i					dimensions;
	ZG_TBRenderText				render_text;
	ZG_Color4f						border_color;
	int							border_tickness;
}ZG_TextBoxData;

ZG_TextBox *ZG_TextBox_New(void){
	ZG_TextBox *textbox = ZG_NEW(ZG_TextBox);
	ZG_TextBoxData *data=ZG_NEW(ZG_TextBoxData);
	textbox->data=data;
	//data->shape2d=Shape2d_New();
	data->font=ZG_TTFont_New();
	ZG_TextBox_SetText(textbox,"");
	return textbox;
}
//---------------------------------------------------------------------------------------------------------------------------------------
ZG_TBRT_Token *ZG_TextBox_RT_NewTokenWord(
	char *word
	,uint16_t word_width
){
	ZG_TBRT_Token *token=ZG_NEW(ZG_TBRT_Token);
	ZG_TBRT_TokenWord *token_word=ZG_NEW(ZG_TBRT_TokenWord);
	token_word->word_width=word_width;
	token_word->word=word;

	token->tbrt_token_type=ZG_TBRT_TOKEN_TYPE_WORD;
	token->token_data=token_word;
	return token;
}

ZG_TBRT_TokenLine *ZG_TextBox_RT_NewLine(ZG_TextBoxData *data){

	ZG_TBRT_TokenLine *tbrt_token_line=ZG_NEW(ZG_TBRT_TokenLine);
	tbrt_token_line->tbrt_tokens=ZG_List_New();

	ZG_List_Add(data->render_text.token_lines,tbrt_token_line);
	tbrt_token_line->space_width=ZG_TTFont_GetSpaceWidth(data->font);
	return tbrt_token_line;
}

void ZG_TextBox_RT_Delete(ZG_TextBoxData *data){

	if(data->render_text.token_lines!=NULL){
		for(unsigned i=0; i < data->render_text.token_lines->count; i++){
			ZG_TBRT_TokenLine *token_line=data->render_text.token_lines->items[i];
			for(unsigned j=0; j < token_line->tbrt_tokens->count; j++){
				ZG_TBRT_Token *_token=token_line->tbrt_tokens->items[j];
				if(_token->token_data != NULL){
					if(_token->tbrt_token_type == ZG_TBRT_TOKEN_TYPE_WORD){
						ZG_TBRT_TokenWord *token_word = _token->token_data;
						ZG_FREE(token_word->word);
					}
					ZG_FREE(_token->token_data);
				}
				ZG_FREE(_token);

			}
			ZG_List_Delete(token_line->tbrt_tokens);
			ZG_FREE(token_line);
		}

		ZG_List_Delete(data->render_text.token_lines);
		data->render_text.token_lines=NULL;
	}
}

void ZG_TextBox_RT_Init(ZG_TextBoxData *data){
	ZG_TextBox_RT_Delete(data);
	data->render_text.token_lines=ZG_List_New();
}

void ZG_TextBox_RT_Build(ZG_TextBox *_this){
	ZG_TextBoxData *data=_this->data;
	unsigned long ch_space=(unsigned long)' ';
	size_t sizeof_char=sizeof(char);
	int ascender=ZG_TTFont_GetAscender(data->font);
	if(data->char_type==ZG_CHAR_TYPE_WCHAR){
		sizeof_char=sizeof(wchar_t);
		ch_space=(unsigned long)L' ';
	}

	data->render_text.bounding_box=ZG_BoundingBox_New4f(
		 FLT_MAX
		,FLT_MAX
		,-FLT_MAX
		,-FLT_MAX
	);

	ZG_BoundingBox *bb_render=&data->render_text.bounding_box;

	float inv_sizeof_char=1.0f/sizeof_char;

	// reset current render text data...
	ZG_TextBox_RT_Init(data);

	if(data->char_type==ZG_CHAR_TYPE_WCHAR){
		if(ZG_String_WIsNullOrEmpty((wchar_t *)data->text)) return;
	}
	else{
		if(ZG_String_IsNullOrEmpty(data->text)) return;
	}

	ZG_List *lines=NULL;

	// split text by lines with <br> token
	if(data->char_type==ZG_CHAR_TYPE_WCHAR){
		lines=ZG_String_WSplitString(data->text,L"<br>"); // get lines ...
	}
	else{
		lines=ZG_String_SplitString(data->text,"<br>"); // get lines ...
	}

	ZG_TBRT_TokenLine *tbrt_token_line=NULL;//data->render_text.tb_tokens;

	uint16_t word_width=0;
	ZG_BoundingBox bb_word;//=ZG_TTFont_GetBoundingBox(data->font,);
	uint16_t space_width=ZG_TTFont_GetSpaceWidth(data->font);
	int y=0;

	// for each line
	for(unsigned i=0; i < lines->count; i++){
		void *text_line=lines->items[i];
		tbrt_token_line=ZG_TextBox_RT_NewLine(data);

		unsigned long ch=0;
		uint16_t total_space_width=0;

		do{

			void *word=NULL;
			void *word_ini=text_line;
			void *word_end=text_line;
			size_t word_len=0;
			bool end_char=false;

			// process word
			do{

				ch=ZG_String_GetChar(text_line,data->char_type);
				if(!(ch == 0 || ch == ch_space)){
					ZG_String_Advance(&text_line,data->char_type);
				}else{
					end_char=true;
				}
			}while(!end_char);

			word_end=text_line;

			// skip chars...
			if(ch!=0){
				end_char=false;

				do{
					ch=ZG_String_GetChar(text_line,data->char_type);
					if((ch == 0 || ch == ch_space)){
						ZG_String_Advance(&text_line,data->char_type);
					}else{
						end_char=true;
					}
				}while(!end_char);
			}


			word_len=((intptr_t)word_end-(intptr_t)word_ini)*inv_sizeof_char; // div size of char or wchar
			word=malloc(word_len+sizeof_char);
			memset(word,0,word_len*inv_sizeof_char+sizeof_char);
			memcpy(word,word_ini,word_len*inv_sizeof_char);

			// get length rendered word...
			if(data->char_type==ZG_CHAR_TYPE_WCHAR){
				bb_word=ZG_TTFont_WGetBoundingBoxN(data->font,word_ini,word_len);
				//word_width=ZG_TTFont_WGetWidthN(data->font,word_ini,word_len);
			}else{
				bb_word=ZG_TTFont_GetBoundingBoxN(data->font,word_ini,word_len);
				//word_width=ZG_TTFont_GetWidthN(data->font,word_ini,word_len);
			}

			word_width=bb_word.maxx-bb_word.minx;
			bb_render->minx=MIN(bb_render->minx,bb_word.minx);
			bb_render->miny=MIN(bb_render->miny,bb_word.miny);
			bb_render->maxy=MAX(bb_render->maxy,y+bb_word.maxy);

			if(((tbrt_token_line->total_word_width+word_width+total_space_width)>data->dimensions.x) // if current word width + space width overpasses x bound...
					&& (tbrt_token_line->total_word_width > 0) // ... and there's some word
				){

				// if line exceeds max dimension, create new line..
				if(data->horizontal_alignment == ZG_HORIZONTAL_ALIGNMENT_JUSTIFY){// && (token_line->tbrt_tokens->count>2)){
					tbrt_token_line->space_width=data->dimensions.x-tbrt_token_line->total_word_width;
					if(tbrt_token_line->tbrt_tokens->count>2){
						tbrt_token_line->space_width/=(tbrt_token_line->tbrt_tokens->count-1);
					}

					tbrt_token_line->space_width=MAX(tbrt_token_line->space_width,ZG_TTFont_GetSpaceWidth(data->font));

					// update max x and y intervals
					bb_render->maxx=MAX(bb_render->maxx,tbrt_token_line->total_word_width+tbrt_token_line->space_width*(tbrt_token_line->tbrt_tokens->count-1));
				}

				tbrt_token_line=ZG_TextBox_RT_NewLine(data);
				total_space_width=0;
				y+=ascender;
			}else{
				// update max x and y intervals
				//bb_render->maxx=MAX(bb_render->maxx,tbrt_token_line->total_word_width+tbrt_token_line->space_width*(tbrt_token_line->tbrt_tokens->count-1));
				bb_render->maxx=MAX(bb_render->maxx,tbrt_token_line->total_word_width+word_width+total_space_width);
			}

			// add word token...
			ZG_List_Add(tbrt_token_line->tbrt_tokens,ZG_TextBox_RT_NewTokenWord(word,word_width));

			// update words width ...
			tbrt_token_line->total_word_width+=word_width;

			// ... and spaces width
			total_space_width+=space_width;


		}while(ch!=0); // not end line...

		y+=ascender;
	}

	ZG_List_DeleteAndFreeAllItems(lines);
}

//---------------------------------------------------------------------------------------------------------------------------------------
ZG_HorizontalAlignment ZG_TextBox_ParseTextAlign(const char *_text_align){
	ZG_HorizontalAlignment horizontal_alignment=ZG_HORIZONTAL_ALIGNMENT_LEFT; // by default
	char *str_text_align=ZG_String_ToLower(_text_align);
	if(ZG_STRCMP(str_text_align,==,"left")){
		horizontal_alignment=ZG_HORIZONTAL_ALIGNMENT_LEFT;
	}else if(ZG_STRCMP(str_text_align,==,"center")){
		horizontal_alignment=ZG_HORIZONTAL_ALIGNMENT_CENTER;
	}else if(ZG_STRCMP(str_text_align,==,"right")){
		horizontal_alignment=ZG_HORIZONTAL_ALIGNMENT_RIGHT;
	}else if(ZG_STRCMP(str_text_align,==,"justify")){
		horizontal_alignment=ZG_HORIZONTAL_ALIGNMENT_JUSTIFY;
	}else{
		ZG_LOG_ERROR("Unknow align text '%s' ",_text_align);
	}

	ZG_FREE(str_text_align);

	return horizontal_alignment;
}

ZG_VerticalAlignment ZG_TextBox_ParseVerticalAlignment(const char *_vertical_text){
	ZG_VerticalAlignment vertical_alignment=ZG_VERTICAL_ALIGNMENT_TOP; // by default
	char *str_vertical_align=ZG_String_ToLower(_vertical_text);
	if(ZG_STRCMP(str_vertical_align,==,"top")){
		vertical_alignment=ZG_VERTICAL_ALIGNMENT_TOP;
	}else if(ZG_STRCMP(str_vertical_align,==,"center")){
		vertical_alignment=ZG_VERTICAL_ALIGNMENT_CENTER;
	}else{
		ZG_LOG_ERROR("Unknow vertical text '%s' ",_vertical_text);
	}

	ZG_FREE(str_vertical_align);

	return vertical_alignment;
}

void	 		ZG_TextBox_SetBorderThickness(ZG_TextBox *_this, uint16_t _border_tickness){
	ZG_TextBoxData *data=_this->data;
	data->border_tickness=_border_tickness;
}

void	 		ZG_TextBox_SetBorderColor4f(ZG_TextBox *_this, ZG_Color4f _border_color){
	ZG_TextBoxData *data=_this->data;
	data->border_color=_border_color;
}

void     ZG_TextBox_SetText(ZG_TextBox *_this,const char *in, ...){
	char out[1024]={0};
	ZG_VARGS(out,in);

	ZG_TextBoxData *data=_this->data;
	if(data->text!=NULL){
		free(data->text);
	}

	data->char_type=ZG_CHAR_TYPE_CHAR;
	size_t total_mem=strlen(out)+sizeof(char); // +1 end string
	data->text=malloc(total_mem);
	memset(data->text,0,total_mem);

	strcpy(data->text,out);

	ZG_TextBox_RT_Build(_this);
}

const char *    ZG_TextBox_GetText(ZG_TextBox *_this){
	ZG_TextBoxData *data=_this->data;
	return data->text;
}


void     ZG_TextBox_WSetText(ZG_TextBox *_this,const wchar_t *in, ...){
	wchar_t out[1024]={0};
	ZG_WVARGS(out,in);

	ZG_TextBoxData *data=_this->data;
	if(data->text!=NULL){
		free(data->text);
	}

	data->char_type=ZG_CHAR_TYPE_WCHAR;
	size_t total_mem=wcslen(out)+sizeof(wchar_t);  // +1 end string
	data->text=malloc(total_mem);
	memset(data->text,0,total_mem);

	wcscpy(data->text,out);

	ZG_TextBox_RT_Build(_this);
}

void     ZG_TextBox_SetFontFile(ZG_TextBox *_this, const char *_font_file){
	ZG_TextBoxData *data=_this->data;
	ZG_TTFont_LoadFromFile(data->font,_font_file);
	ZG_TextBox_RT_Build(_this);

}

void     		ZG_TextBox_SetFontSize(ZG_TextBox *_this, uint16_t _font_size){
	ZG_TextBoxData *data=_this->data;
	ZG_TTFont_SetFontSize(data->font,_font_size);
	ZG_TextBox_RT_Build(_this);
}

uint16_t     		ZG_TextBox_GetFontSize(ZG_TextBox *_this){
	ZG_TextBoxData *data=_this->data;
	return ZG_TTFont_GetFontSize(data->font);
}


ZG_TTFont *   ZG_TextBox_GetFont(ZG_TextBox *_this){
	ZG_TextBoxData *data=_this->data;
	return data->font;
}

void	 ZG_TextBox_SetHorizontalAlignment(ZG_TextBox *_this, ZG_HorizontalAlignment horizontal_alignment){
	ZG_TextBoxData *data=_this->data;
	data->horizontal_alignment=horizontal_alignment;
}

void	 ZG_TextBox_SetVerticalAlignment(ZG_TextBox *_this, ZG_VerticalAlignment vertical_alignment){
	ZG_TextBoxData *data=_this->data;
	data->vertical_alignment=vertical_alignment;
}


void	 ZG_TextBox_SetDimensions(ZG_TextBox *_this, uint16_t _width, uint16_t _height){
	ZG_TextBox_SetWidth(_this,_width);
	ZG_TextBox_SetHeight(_this,_height);
}

void	 ZG_TextBox_SetWidth(ZG_TextBox *_this, uint16_t _width){
	ZG_TextBoxData *data=_this->data;

	if(data->dimensions.x!=_width){
		ZG_TextBox_RT_Build(_this);
		data->dimensions.x=_width;
	}
}

void	 ZG_TextBox_SetHeight(ZG_TextBox *_this, uint16_t _height){
	ZG_TextBoxData *data=_this->data;

	if(data->dimensions.y !=_height){
		data->dimensions.y=_height;
		ZG_TextBox_RT_Build(_this);
	}
}

void	 ZG_TextBox_Draw(ZG_TextBox *_this, ZG_Transform *transform,ZG_Color4f *_color){

	ZG_TextBoxData *data=_this->data;
	ZG_Color4f color=_color!=NULL?*_color:ZG_COLOR4F_WHITE;
	float x_draw=0;
	float y_draw=0;
	float y_draw_inc=0;
	int y=0;
	int x=0;
	uint16_t text_total_width=0;
	uint16_t text_total_height=0;
	ZG_Vector3f dim3d;
	int ascender=0;
	int space_width=0;

	// TODO: pos is at center box by default, do a wat yo change render center
	if(transform != NULL){
		ZG_Transform_Apply(transform);
	}

	text_total_width=data->render_text.bounding_box.maxx-data->render_text.bounding_box.minx;
	text_total_height=data->render_text.bounding_box.maxy-data->render_text.bounding_box.miny;

	y=-(data->dimensions.y>>1)-data->render_text.bounding_box.miny; // default aligned top-left
	x=0;

	dim3d=ZG_ViewPort_ScreenToWorldDimension2i(data->dimensions.x,data->dimensions.y);
	ascender=ZG_TTFont_GetAscender(data->font);
	space_width=ZG_TTFont_GetSpaceWidth(data->font);

	if(data->vertical_alignment == ZG_VERTICAL_ALIGNMENT_CENTER){
		y=-(text_total_height>>1)-data->render_text.bounding_box.miny;
	}
	else if(data->vertical_alignment == VERTICAL_ALIGNMENT_BOTTOM){
		y=+(data->dimensions.y>>1)-text_total_height-data->render_text.bounding_box.miny;
	}

	y_draw=-ZG_ViewPort_ScreenToWorldHeight(y);
	y_draw_inc=-ZG_ViewPort_ScreenToWorldHeight(ascender);

	if(data->border_tickness>0){
		ZG_Graphics_DrawRectangle4f(
				 0
				,0
				,dim3d.x
				,dim3d.y
				,data->border_color
				,data->border_tickness
		);
	}

	if(ZG_IsDebugMode()){
		int x_center=0;// default text_align_center
		int y_center=0;// default vertical_align_center

		// 2. Displace x in function text align and
		if(data->horizontal_alignment == ZG_HORIZONTAL_ALIGNMENT_LEFT){
			x_center=-(data->dimensions.x>>1)+(text_total_width>>1);
		}else if(data->horizontal_alignment == ZG_HORIZONTAL_ALIGNMENT_RIGHT){
			x_center=+(data->dimensions.x>>1)-(text_total_width>>1);
		}

		// 3. Displace y in function vertical align and
		if(data->vertical_alignment == ZG_VERTICAL_ALIGNMENT_TOP){
			y_center=+(data->dimensions.y>>1)-(text_total_height>>1);
		}else if(data->vertical_alignment == VERTICAL_ALIGNMENT_BOTTOM){
			y_center=-(data->dimensions.y>>1)+(text_total_height>>1);
		}

		ZG_Vector3f pos3d_center_rendered_text=ZG_ViewPort_ScreenToWorldDimension2i(x_center,y_center);
		ZG_Vector3f scale_rendered_text=ZG_ViewPort_ScreenToWorldDimension2i(text_total_width,text_total_height);

		ZG_Graphics_DrawRectangle4f(
			pos3d_center_rendered_text.x // x:0 translation keeps current translate
			,pos3d_center_rendered_text.y// y:0 translation keeps current translate
			,scale_rendered_text.x
			,scale_rendered_text.y
			,ZG_Color4f_New4f(1,0,0,0)
			,2
		);
	}

	ZG_TTFont_RenderTextBegin(&color);

	for(unsigned i=0; i < data->render_text.token_lines->count; i++){
		int inc_x=1;
		float space_per_word=space_width;
		ZG_TBRT_TokenLine *token_line = data->render_text.token_lines->items[i];
		int total_line_width=token_line->total_word_width+(token_line->tbrt_tokens->count-1)*token_line->space_width;

		 // set x as text_align left
		x=-(data->dimensions.x>>1);

		// if horizontal alignment is center ...
		if(data->horizontal_alignment == ZG_HORIZONTAL_ALIGNMENT_CENTER){
			x=-(total_line_width>>1);
		// if horizontal alignment is right ...
		}else if(data->horizontal_alignment == ZG_HORIZONTAL_ALIGNMENT_RIGHT){
			x=(data->dimensions.x>>1);
			inc_x=-1;
		// if horizontal alignment is justified and not last line ...
		}else if(data->horizontal_alignment == ZG_HORIZONTAL_ALIGNMENT_JUSTIFY && (i+1<data->render_text.token_lines->count)){
			// ... redistruibute spaces along the box dimension.x
			space_per_word=token_line->space_width;//(float)(data->dimensions.x-(total_line_width-((token_line->tbrt_tokens->count-1)*space_width)))/token_line->tbrt_tokens->count;
		}

		for(unsigned w=0; w < token_line->tbrt_tokens->count; w++){

			ZG_TBRT_Token *token=NULL;
			if(inc_x > 0){
				token=token_line->tbrt_tokens->items[w];
			}
			else{
				token=token_line->tbrt_tokens->items[token_line->tbrt_tokens->count-w-1];
			}

			ZG_TBRT_TokenWord *token_word;
			switch(token->tbrt_token_type){
			default:
				break;
			case ZG_TBRT_TOKEN_TYPE_WORD:
				token_word=((ZG_TBRT_TokenWord*)token->token_data);
				if(w>0){ // render space
					//ZG_TTFont_Print(data->font,x,y,ZG_COLOR4F_WHITE," ");
					x+=(space_per_word)*inc_x;
				}

				x_draw=ZG_ViewPort_ScreenToWorldWidth(x+(inc_x<1?-token_word->word_width:0));

				if(data->char_type==ZG_CHAR_TYPE_WCHAR){
					ZG_TTFont_WPrint(data->font,x_draw,y_draw,color,token_word->word);
				}
				else{ // char by default.
					ZG_TTFont_Print(data->font,x_draw,y_draw,color,token_word->word);
				}
				x+=(token_word->word_width)*inc_x;
				break;
			}
		}
		y_draw+=y_draw_inc;
	}

	ZG_TTFont_RenderTextEnd();

	if(transform != NULL){
		ZG_Transform_Restore(transform);
	}
}

void ZG_TextBox_Delete(ZG_TextBox * _this){
	if(_this==NULL) return;
	ZG_TextBoxData *data=_this->data;

	if(data->text!=NULL){
		free(data->text);
	}

	ZG_TextBox_RT_Delete(data);

	ZG_TTFont_Delete(data->font);

	free(_this->data);
	free(_this);
}
