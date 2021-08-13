#include "zg_gui.h"

#define DEFAULT_WINDOW_WIDTH 			150
#define DEFAULT_WINDOW_CAPTION_HEIGHT 	32
#define DEFAULT_WINDOW_CONTENT_HEIHGT 	300


typedef struct{

	GUIFrame		*	frame_content,
					*	frame_caption;

	GUILabel		* 	label_caption;
	GUIButton		*	button_close;

	bool			visible_caption;
	bool			start_dragging;
	Vector2i 		start_mouse_position;

}GUIWindowData;


void GUIWidget_AttachWidgetBase(GUIWidget *_this, GUIWidget *widget);
//void GUIWindow_OnMousePressDown();

void GUIWindow_OnMouseButtonUp(MouseEvent * mouse_event,void *gui_window);
void GUIWindow_OnMouseButtonDown(MouseEvent * mouse_event,void *gui_window);
void GUIWindow_OnMouseMotion(MouseEvent * mouse_event,void *gui_window);
void GUIWindow_OnSetWidth(void *gui_window,uint16_t width);
void GUIWindow_OnSetHeight(void *gui_window,uint16_t width);
void GUIWindow_AttachChild(void *gui_window, GUIWidget * widget_to_attach);

GUIWindow * GUIWindow_New(int x, int y, uint16_t width, uint16_t height){

	GUIWindow *window = NEW(GUIWindow);
	GUIWindowData *data = NEW(GUIWindowData);
	window->data=data;

	data->visible_caption=true;

	window->widget=GUIWidget_New(x, y, width, height);

	// CAPTION
	data->frame_caption=GUIFrame_New(0,0,width,DEFAULT_WINDOW_CAPTION_HEIGHT);
	data->frame_caption->widget->background_color=Color4f_FromRGB(0,128,255);

	data->label_caption=GUILabel_New(0,0,width,DEFAULT_WINDOW_CAPTION_HEIGHT);
	Textbox_SetText(data->label_caption->widget->textbox,"Window");


	data->button_close=GUIButton_New(
			width-(DEFAULT_WINDOW_CAPTION_HEIGHT)
			,(DEFAULT_WINDOW_CAPTION_HEIGHT>>1)-((DEFAULT_WINDOW_CAPTION_HEIGHT*0.75f)*0.5f)
			,DEFAULT_WINDOW_CAPTION_HEIGHT*0.75
			,DEFAULT_WINDOW_CAPTION_HEIGHT*0.75
			);

	Textbox_SetText(data->button_close->widget->textbox,"");
	GUIButton_SetIcon(data->button_close,IconManager_GetIconDefault(DEFAULT_ICON_CLOSE_BIG));


	// CONTENT
	data->frame_content=GUIFrame_New(0,DEFAULT_WINDOW_CAPTION_HEIGHT,width,height);



	// SETUP CAPTION
	GUIWidget_AttachWidget(data->frame_caption->widget,data->label_caption->widget);
	GUIWidget_AttachWidget(data->frame_caption->widget,data->button_close->widget);

	// SETUP WINDOW
	GUIWidget_AttachWidget(window->widget,data->frame_caption->widget);
	GUIWidget_AttachWidget(window->widget,data->frame_content->widget);


	data->visible_caption=true;
	data->start_dragging=false;



	Input_AddEventOnMouseButtonDown((CallbackMouseEvent){
		.ptr_function=GUIWindow_OnMouseButtonDown
		,.user_data=window
	});

	Input_AddEventOnMouseButtonUp((CallbackMouseEvent){
		.ptr_function=GUIWindow_OnMouseButtonUp
		,.user_data=window
	});

	Input_AddEventOnMouseMotion((CallbackMouseEvent){
		.ptr_function=GUIWindow_OnMouseMotion
		,.user_data=window
	});

	GUIWidget_OnSetWidth(window->widget,(CallbackWidgetOnSetDimension){
	   .ptr_function=GUIWindow_OnSetWidth
	  ,.calling_widget=window
	});
	GUIWidget_OnSetHeight(window->widget,(CallbackWidgetOnSetDimension){
	   .ptr_function=GUIWindow_OnSetHeight
	  ,.calling_widget=window
	});

	GUIWidget_AttachWidgetFunctionOverride(window->widget,(CallbackWidgetAttachWidget){
	   .ptr_function=GUIWindow_AttachChild
	  ,.calling_widget=window
	});


	return window;

}

void GUIWindow_SetVisibleCaption(GUIWindow *_this, bool _v){
	GUIWindowData *data= _this->data;
	data->visible_caption=_v;

	Vector2i dim_content=GUIWidget_GetDimensions(data->frame_content->widget);
	Vector2i dim_caption=GUIWidget_GetDimensions(data->frame_caption->widget);


	if(_v){
		GUIWidget_SetPosition2i(_this->widget,0,DEFAULT_WINDOW_CAPTION_HEIGHT);
		GUIWidget_SetHeight(_this->widget,dim_content.y+dim_caption.y);
	}
	else{
		GUIWidget_SetPosition2i(_this->widget,0,0);
		GUIWidget_SetHeight(_this->widget,dim_content.y);
	}
}

void GUIWindow_SetColorBackground3i(GUIWindow * _this, uint8_t r, uint8_t g, uint8_t b){
	GUIWindowData *data=_this->data;
	data->frame_content->widget->background_color=Color4f_FromRGB(r,g,b);
}

void GUIWindow_OnSetWidth(void *gui_window, uint16_t width){

	UNUSUED_PARAM(width);

	GUIWindow *_this=gui_window;
	GUIWindowData *data=_this->data;

	Vector2i dimensions=GUIWidget_GetDimensions(_this->widget);
	Vector2i caption_dimensions=GUIWidget_GetDimensions(data->frame_caption->widget);


	GUIWidget_SetWidth(data->frame_caption->widget,dimensions.x);
	GUIWidget_SetWidth(data->frame_content->widget,dimensions.x);

	GUIWidget_SetPosition2i(data->button_close->widget,dimensions.x -(DEFAULT_WINDOW_CAPTION_HEIGHT),(caption_dimensions.y>>1)-(DEFAULT_WINDOW_CAPTION_HEIGHT*0.75)*0.5);

}

// the attach child is overrided to add in frame_content...
void GUIWindow_AttachChild(void *gui_window, GUIWidget * widget_to_attach){
	GUIWindow *_this=gui_window;
	GUIWindowData *data=_this->data;

	GUIWidget_AttachWidgetBase(data->frame_content->widget,widget_to_attach);
}

void GUIWindow_OnSetHeight(void *gui_window,uint16_t height){
	GUIWindow *_this=gui_window;
	GUIWindowData *data=_this->data;

	GUIWidget_SetHeight(data->frame_content->widget,height);
}


void GUIWindow_OnMouseButtonDown(MouseEvent * mouse_event, void *gui_window){

	GUIWindow *_this=gui_window;
	GUIWindowData *data=_this->data;

	if(!data->visible_caption){
		return;
	}

	//mouse_position=Input_GetMousePosition();

	if(GUIWidget_IsPointCollision(data->button_close->widget,mouse_event->position))
	{
	//	Log_Info("button collision");
		// collision with button (ignore it!)
		return;
	}

	if(GUIWidget_IsPointCollision(data->frame_caption->widget,mouse_event->position))
	{
		if(Input_IsMouseButtonPressed()) {
			Log_Info("start dragging");
			data->start_dragging = true;
			data->start_mouse_position=Input_GetMousePosition();
		}
	 }
}

void  GUIWindow_OnMouseMotion(MouseEvent * event, void *gui_window){

	UNUSUED_PARAM(event);

	 GUIWindow *_this=gui_window;
	 GUIWindowData *data=_this->data;

	if(!data->visible_caption){
		return;
	}


	if(data->start_dragging) {

		//Log_Info("moving !");

		Vector2i position = Vector2i_Add(
			GUIWidget_GetPosition(_this->widget,WIDGET_POSITION_LOCAL)
			,Vector2i_Sub(Input_GetMousePosition(),data->start_mouse_position)
		);

		GUIWidget_SetPosition2i(_this->widget,position.x,position.y);


		data->start_mouse_position=Input_GetMousePosition();

	}

	return;
}

 void GUIWindow_OnMouseButtonUp(MouseEvent * event, void *gui_window){
	 UNUSUED_PARAM(event);

	 GUIWindow *_this=gui_window;
	 GUIWindowData *data=_this->data;

	// Log_Info("mouse up");

	if(!data->visible_caption){
		return;
	}

	if(data->start_dragging) {

		data->start_dragging = false;
	}

}


void GUIWindow_Delete(GUIWindow *_this) {
	if(_this == NULL) return;
	GUIWindowData *data=_this->data;

	GUIWidget_Delete(_this->widget);
	GUIFrame_Delete(data->frame_caption);
	GUIFrame_Delete(data->frame_content);
	GUILabel_Delete(data->label_caption);
	GUIButton_Delete(data->button_close);

	FREE(data);
	FREE(_this);
}

