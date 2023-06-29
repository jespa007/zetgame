#include "_zg_gui_.h"
#include "assets/image/button1.png.c"

typedef struct{
	ZG_List *on_click_events;
	Icon	icon;
	struct{
		uint32_t  relative_time,total_time;
		ZG_Color4f	color;
	}auto_click_on_over;
	bool 	mouse_collide;
	ZG_Texture 	*texture;
	ZG_TextBox 	*textbox;

}ZG_GUIButtonData;

ZG_Texture *g_default_texture_button = NULL;

static void ZG_GUIButton_Draw(void *gui_button);
static void ZG_GUIButton_PostUpdate( void *gui_button);

void ZG_GUIButton_Init(void){
	if(g_default_texture_button != NULL)
	{
		ZG_LOG_WARNINGF("ZG_GUIButton already initialized");
		return;
	}

	g_default_texture_button=ZG_Texture_NewFromMemory(button1_png,button1_png_len);
}

void ZG_GUIButton_DeInit(void){
	if(g_default_texture_button == NULL)
	{
		ZG_LOG_WARNINGF("ZG_GUIButton not initialized");
		return;
	}

	ZG_Texture_Delete(g_default_texture_button);
}

ZG_GUIButton * ZG_GUIButton_New(int x, int y, uint16_t width, uint16_t height){
	ZG_GUIButton * button = ZG_NEW(ZG_GUIButton);
	// SETUP WIDGET
	button->widget = ZG_GUIWidget_New( x,  y,  width,  height);
	button->widget->type=ZG_GUI_WIDGET_TYPE_BUTTON;
	button->widget->gui_ptr=button;

	ZG_GUIWidget_SetDrawFunction(button->widget,(ZG_CallbackWidgetUpdate){
		.ptr_function=ZG_GUIButton_Draw
		,.calling_widget=button
	});

	ZG_GUIWidget_SetPostUpdateFunction(button->widget,(ZG_CallbackWidgetUpdate){
		.ptr_function=ZG_GUIButton_PostUpdate
		,.calling_widget=button
	});


	//SETUP BUTTON
	ZG_GUIButtonData *data = ZG_NEW(ZG_GUIButtonData);
	data->textbox=ZG_TextBox_New();
	data->on_click_events=ZG_List_New();
	button->data=data;



	return button;
}

/*
static void  ZG_GUIButton_PostUpdateWidget(void *unused, void *gui_button){

	ZG_GUIButton *button=gui_button;
	ZG_GUIButtonData *data = button->data;
	ZG_Vector2i ini_pos=ZG_GUIWidget_GetPosition(button->widget,ZG_GUI_WIDGET_POSITION_WORLD);
	ZG_Vector2i dimensions=ZG_GUIWidget_GetDimensions(button->widget);

	ZG_Vector2i end_pos=ZG_Vector2i_New(
			 ini_pos.x+dimensions.x
			,ini_pos.y+dimensions.y
			);
	ZG_Vector2i mouse_pos=ZG_Input_GetMousePosition();

	if(ZG_Input_IsLeftButtonPressed() && Vector2i_PointRectCollision(mouse_pos,ini_pos,end_pos)){
		ZG_LOG_INFO("clicked");
		// handle on even click
		for(unsigned i=0; i < data->on_click_events->count; i++){
			ZG_Callback *cf=data->on_click_events->items[i];
			cf->ptr_function(NULL,cf->user_data);
		}
	}

	return NULL;
}*/
void ZG_GUIButton_Reset(ZG_GUIButton *_this){
	ZG_GUIButtonData *data = _this->data;
	data->auto_click_on_over.total_time=0;
}


void ZG_GUIButton_SetupClickOnOver(ZG_GUIButton *_this, uint32_t time_ms, ZG_Color4f color){
	ZG_GUIButtonData *data = _this->data;
	data->auto_click_on_over.relative_time=time_ms;
	data->auto_click_on_over.color=color;
}


void ZG_GUIButton_PostUpdate(void *gui_button){
	ZG_GUIButton *_this=gui_button;
	ZG_GUIButtonData *data = _this->data;

	if(!ZG_GUIWidget_IsEnabled(_this->widget)){
		return;
	}

	data->mouse_collide=ZG_GUIWidget_IsPointCollision(_this->widget,ZG_Input_GetMousePosition());
	bool auto_click_on_over=false;

	// manage click on over?
	if(data->auto_click_on_over.total_time > 0){
		if(data->mouse_collide){
			if(data->auto_click_on_over.total_time == 0){ // not started
				data->auto_click_on_over.total_time=data->auto_click_on_over.relative_time+SDL_GetTicks();
			}else{ // started! check elapsed time...
				if(data->auto_click_on_over.total_time < SDL_GetTicks()){ // -> do auto click
					ZG_GUIButton_Reset(_this);
					auto_click_on_over=true;
				}
			}
		}else{

			ZG_GUIButton_Reset(_this);
		}
	}

	if((ZG_Input_IsLeftButtonPressed() && data->mouse_collide) || auto_click_on_over){
		ZG_LOG_INFOF("clicked");
		// handle on even click
		ZG_MouseEvent mouse_event={0};

		mouse_event.left_press=true;
		mouse_event.position=ZG_Input_GetMousePosition();

		for(unsigned i=0; i < data->on_click_events->count; i++){
			ZG_CallbackMouseEvent *cf=data->on_click_events->items[i];
			cf->ptr_function(&mouse_event,cf->user_data);
		}
	}

}

static void  ZG_GUIButton_Draw(void *gui_button){
	ZG_GUIButton *_this=gui_button;
	ZG_GUIButtonData *data = _this->data;
	ZG_Transform transform=ZG_Transform_DefaultValues();
	ZG_Vector2i position=ZG_GUIWidget_GetPosition(_this->widget,ZG_GUI_WIDGET_POSITION_WORLD);
	ZG_Vector2i dimensions=ZG_GUIWidget_GetDimensions(_this->widget);

	position.x+=dimensions.x>>1;
	position.y+=dimensions.y>>1;

	ZG_Color4f result_font_color = ZG_GUIWidget_GetColor4f(_this->widget);
	//ZG_Color4f background_result=_this->widget->background_color;

	float alpha=1*ZG_GUIWidget_GetOpacity(_this->widget);
	if(!ZG_GUIWidget_IsEnabled(_this->widget)){
		alpha*=0.25f;
	}else if(!data->mouse_collide){
		alpha*=0.5f;
	}

	result_font_color.a=alpha;
	//background_result.a=alpha;

	ZG_Graphics_DrawRectangleTextured4i(position.x,position.y,dimensions.x,dimensions.y,ZG_COLOR4F_WHITE,g_default_texture_button,NULL);


	if(data->mouse_collide){
		if(data->auto_click_on_over.total_time>0){ // auto click enabled ?
			float v=(((float)(SDL_GetTicks()-data->auto_click_on_over.total_time))/data->auto_click_on_over.relative_time);
			if(v < 0) v=0;
			if(v > 1) v=1;

			ZG_Graphics_DrawRectangle4i(position.x+dimensions.x+5, position.y, 10, dimensions.y,data->auto_click_on_over.color, 1);
			ZG_Graphics_DrawRectangleFilled4i(position.x+dimensions.x+5, position.y+dimensions.y-(dimensions.y-1)*v, 9, (dimensions.y-1)*v, data->auto_click_on_over.color);

		}
	}

	if(data->icon.texture!=NULL){ // draw icon
		ZG_Graphics_DrawRectangleTextured4i(position.x,position.y,dimensions.x,dimensions.y,ZG_COLOR4F_WHITE, data->icon.texture, &data->icon.texture_crop);
	}

	ZG_Transform_SetPosition2i(&transform,position.x,position.y);
	ZG_TextBox_Draw(data->textbox,&transform,&result_font_color);

}

void 			ZG_GUIButton_SetText(ZG_GUIButton *_this, const char *_text_in,...){
	ZG_GUIButtonData *data = _this->data;
	char text_out[ZG_STR_MAX];
	ZG_STR_CAPTURE_VARGS(text_out,_text_in);

	ZG_TextBox_SetText(data->textbox,text_out);

}

void ZG_GUIButton_SetIcon(ZG_GUIButton *_this, Icon icon){
	ZG_GUIButtonData *data = _this->data;
	data->icon=icon;
}

void ZG_GUIButton_AddEventOnClick(ZG_GUIButton *_this,ZG_CallbackMouseEvent on_click){
	ZG_GUIButtonData *data=_this->data;
	ZG_CallbackMouseEvent *_cf=ZG_NEW(ZG_CallbackMouseEvent);
	*_cf=on_click;
	ZG_List_Add(data->on_click_events,_cf);
}

void ZG_GUIButton_Delete(ZG_GUIButton *_this){
	if(_this == NULL) return;
	ZG_GUIButtonData *data=_this->data;

	ZG_List_DeleteAndFreeAllItems(data->on_click_events);

	ZG_TextBox_Delete(data->textbox);
	ZG_Texture_Delete(data->texture);


	ZG_GUIWidget_Delete(_this->widget);

	ZG_FREE(data);
	ZG_FREE(_this);
}
