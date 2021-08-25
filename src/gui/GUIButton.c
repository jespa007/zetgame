#include "zg_gui.h"
#include "assets/image/button1.png.c"

typedef struct{
	List *on_click_events;
	Icon	icon;
	struct{
		uint32_t  relative_time,total_time;
		Color4f	color;
	}auto_click_on_over;
	bool 	mouse_collide;

}GUIButtonData;

Texture *g_default_texture_button = NULL;

static void GUIButton_Draw(void *gui_button);
static void GUIButton_PostUpdate( void *gui_button);

void GUIButton_Init(void){
	if(g_default_texture_button != NULL)
	{
		Log_Warning("GUIButton already initialized");
		return;
	}

	g_default_texture_button=Texture_NewFromMemory(button1_png,button1_png_len);
}

void GUIButton_DeInit(void){
	if(g_default_texture_button == NULL)
	{
		Log_Warning("GUIButton not initialized");
		return;
	}

	Texture_Delete(g_default_texture_button);
}

GUIButton * GUIButton_New(int x, int y, uint16_t width, uint16_t height){
	GUIButton * button = NEW(GUIButton);
	// SETUP WIDGET
	button->widget = GUIWidget_New( x,  y,  width,  height);
	button->widget->textbox=Textbox_New();
	GUIWidget_SetDrawFunction(button->widget,(CallbackWidgetUpdate){
		.ptr_function=GUIButton_Draw
		,.calling_widget=button
	});

	GUIWidget_SetPostUpdateFunction(button->widget,(CallbackWidgetUpdate){
		.ptr_function=GUIButton_PostUpdate
		,.calling_widget=button
	});


	//SETUP BUTTON
	GUIButtonData *data = NEW(GUIButtonData);
	data->on_click_events=List_New();
	button->data=data;



	return button;
}

/*
static void  GUIButton_PostUpdateWidget(void *unused, void *gui_button){

	GUIButton *button=gui_button;
	GUIButtonData *data = button->data;
	Vector2i ini_pos=GUIWidget_GetPosition(button->widget,WIDGET_POSITION_WORLD);
	Vector2i dimensions=GUIWidget_GetDimensions(button->widget);

	Vector2i end_pos=Vector2i_New(
			 ini_pos.x+dimensions.x
			,ini_pos.y+dimensions.y
			);
	Vector2i mouse_pos=Input_GetMousePosition();

	if(Input_IsMouseButtonPressed() && Vector2i_PointRectCollision(mouse_pos,ini_pos,end_pos)){
		Log_Info("clicked");
		// handle on even click
		for(unsigned i=0; i < data->on_click_events->count; i++){
			Callback *cf=data->on_click_events->items[i];
			cf->ptr_function(NULL,cf->user_data);
		}
	}

	return NULL;
}*/
void GUIButton_Reset(GUIButton *_this){
	GUIButtonData *data = _this->data;
	data->auto_click_on_over.total_time=0;
}


void GUIButton_SetupClickOnOver(GUIButton *_this, uint32_t time_ms, Color4f color){
	GUIButtonData *data = _this->data;
	data->auto_click_on_over.relative_time=time_ms;
	data->auto_click_on_over.color=color;
}


void GUIButton_PostUpdate(void *gui_button){
	GUIButton *_this=gui_button;
	GUIButtonData *data = _this->data;

	if(!_this->widget->is_enabled){
		return;
	}

	data->mouse_collide=GUIWidget_IsPointCollision(_this->widget,Input_GetMousePosition());
	bool auto_click_on_over=false;

	// manage click on over?
	if(data->auto_click_on_over.total_time > 0){
		if(data->mouse_collide){
			if(data->auto_click_on_over.total_time == 0){ // not started
				data->auto_click_on_over.total_time=data->auto_click_on_over.relative_time+SDL_GetTicks();
			}else{ // started! check elapsed time...
				if(data->auto_click_on_over.total_time < SDL_GetTicks()){ // -> do auto click
					GUIButton_Reset(_this);
					auto_click_on_over=true;
				}
			}
		}else{

			GUIButton_Reset(_this);
		}
	}

	if((Input_IsMouseButtonPressed() && data->mouse_collide) || auto_click_on_over){
		Log_Info("clicked");
		// handle on even click
		MouseEvent mouse_event={0};

		mouse_event.left_press=true;
		mouse_event.position=Input_GetMousePosition();

		for(unsigned i=0; i < data->on_click_events->count; i++){
			CallbackMouseEvent *cf=data->on_click_events->items[i];
			cf->ptr_function(&mouse_event,cf->user_data);
		}
	}

}

static void  GUIButton_Draw(void *gui_button){
	GUIButton *_this=gui_button;
	GUIButtonData *data = _this->data;
	Transform transform=Transform_DefaultValues();
	Vector2i position=GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_WORLD);
	Vector2i dimensions=GUIWidget_GetDimensions(_this->widget);

	Color4f result_font_color = _this->widget->color;
	Color4f background_result=_this->widget->background_color;

	float alpha=1*_this->widget->opacity;
	if(!_this->widget->is_enabled){
		alpha*=0.25f;
	}else if(!data->mouse_collide){
		alpha*=0.5f;
	}

	result_font_color.a=alpha;
	background_result.a=alpha;

	Graphics_DrawRectangleTexturedTranslate2i(position.x,position.y,dimensions.x,dimensions.y,COLOR4F_WHITE,g_default_texture_button,NULL);


	if(data->mouse_collide){
		if(data->auto_click_on_over.total_time>0){ // auto click enabled ?
			float v=(((float)(SDL_GetTicks()-data->auto_click_on_over.total_time))/data->auto_click_on_over.relative_time);
			if(v < 0) v=0;
			if(v > 1) v=1;

			Graphics_DrawRectangleTranslate2i(position.x+dimensions.x+5, position.y, 10, dimensions.y, 1,data->auto_click_on_over.color);
			Graphics_DrawRectangleFilledTranslate2i(position.x+dimensions.x+5, position.y+dimensions.y-(dimensions.y-1)*v, 9, (dimensions.y-1)*v, data->auto_click_on_over.color);

		}
	}

	if(data->icon.texture!=NULL){ // draw icon
		Graphics_DrawRectangleTexturedTranslate2i(position.x,position.y,dimensions.x,dimensions.y,COLOR4F_WHITE, data->icon.texture, &data->icon.texture_crop);
	}

	Transform_SetPosition2i(&transform,position.x,position.y);
	Textbox_Draw(_this->widget->textbox,&transform,&result_font_color);

}


void GUIButton_SetIcon(GUIButton *_this, Icon icon){
	GUIButtonData *data = _this->data;
	data->icon=icon;
}

void GUIButton_AddEventOnClick(GUIButton *_this,CallbackMouseEvent on_click){
	GUIButtonData *data=_this->data;
	CallbackMouseEvent *_cf=NEW(CallbackMouseEvent);
	*_cf=on_click;
	List_Add(data->on_click_events,_cf);
}

void GUIButton_Delete(GUIButton *_this){
	if(_this == NULL) return;
	GUIButtonData *data=_this->data;

	List_DeleteAndFreeAllItems(data->on_click_events);

	GUIWidget_Delete(_this->widget);

	FREE(data);
	FREE(_this);
}
