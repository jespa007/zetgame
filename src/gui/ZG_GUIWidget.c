#include "@zg_gui.h"


typedef struct{
	ZG_List 				*widgets;
	ZG_GUIWidget 			*parent;
	ZG_Vector2i 			position_local, position_world, position_screen;
	ZG_Vector2i 			dimensions;
	ZG_Color4f				color,background_color;
	float 				opacity;
	bool 				is_enabled;

	ZG_GUIWindow *window;

	// events
	ZG_CallbackWidgetUpdate post_update,draw,update;
	ZG_CallbackWidgetOnSetDimension on_set_width;
	ZG_CallbackWidgetOnSetDimension on_set_height;
	ZG_CallbackWidgetAttachWidget attach_widget;


}ZG_GUIWidgetData;

ZG_GUIWidget  * ZG_GUIWidget_New(int x, int y, uint16_t width, uint16_t height){
	ZG_GUIWidget *widget=ZG_NEW(ZG_GUIWidget);
	ZG_GUIWidgetData *data=ZG_NEW(ZG_GUIWidgetData);
	data->widgets = ZG_List_New();
	widget->data=data;

	data->is_enabled=true;
	data->color=ZG_COLOR4F_WHITE;
	data->background_color=ZG_COLOR4F_WHITE;
	data->opacity=ZG_ALPHA_VALUE_SOLID;

	data->position_local.x=x;
	data->position_local.y=y;

	data->dimensions.x=width;
	data->dimensions.y=height;

	return widget;
}

void ZG_GUIWidget_SetWindow(ZG_GUIWidget *_this,ZG_GUIWindow *_window){
	ZG_GUIWidgetData *data=_this->data;
	if(data->window!=NULL){
		ZG_LOG_ERRORF("widget already has a window");
		return;
	}

	data->window=_window;
}

void ZG_GUIWidget_SetPositionX(ZG_GUIWidget *_this,int x){
	ZG_GUIWidgetData *data=_this->data;
	data->position_local.x=x;
}

void ZG_GUIWidget_SetPositionY(ZG_GUIWidget *_this,int y){
	ZG_GUIWidgetData *data=_this->data;
	data->position_local.y=y;
}

void ZG_GUIWidget_SetPosition2i(ZG_GUIWidget *_this,int x, int y){
	ZG_GUIWidget_SetPositionX(_this,x);
	ZG_GUIWidget_SetPositionY(_this,y);
}

void ZG_GUIWidget_SetPositionAbsolute2i(ZG_GUIWidget *_this,int x,int y){
	ZG_GUIWidgetData *data=_this->data;
	data->position_world.x=x;
	data->position_world.y=y;
}

void ZG_GUIWidget_SetWidth(ZG_GUIWidget *_this,uint16_t width){
	ZG_GUIWidgetData *data=_this->data;
	bool modify_width=data->dimensions.x != width;

	data->dimensions.x = width;

	if(modify_width && data->on_set_width.ptr_function!=NULL){
		data->on_set_width.ptr_function(data->on_set_width.calling_widget,width);
	}
}

void ZG_GUIWidget_SetHeight(ZG_GUIWidget *_this,uint16_t height){
	ZG_GUIWidgetData *data=_this->data;
	bool modify_height=data->dimensions.y != height;

	data->dimensions.y = height;

	if(modify_height && data->on_set_height.ptr_function!=NULL){
		data->on_set_height.ptr_function(data->on_set_height.calling_widget,height);
	}
}

void ZG_GUIWidget_SetDimensions(ZG_GUIWidget *_this,uint16_t width, uint16_t height){
	ZG_GUIWidget_SetHeight(_this,height);
	ZG_GUIWidget_SetWidth(_this,width);
}

ZG_Color4f ZG_GUIWidget_GetBackgroundColor4f(ZG_GUIWidget *_this){
	ZG_GUIWidgetData *data=_this->data;
	return data->background_color;
}

void 		ZG_GUIWidget_SetBackgroundColor4f(ZG_GUIWidget *_this,ZG_Color4f _color){
	ZG_GUIWidgetData *data=_this->data;
	data->background_color=_color;
}

void 		ZG_GUIWidget_SetColor4f(ZG_GUIWidget *_this,ZG_Color4f _color){
	ZG_GUIWidgetData *data=_this->data;
	data->color=_color;

}

ZG_Color4f 	ZG_GUIWidget_GetColor4f(ZG_GUIWidget *_this){
	ZG_GUIWidgetData *data=_this->data;
	return data->color;
}

float 		ZG_GUIWidget_GetOpacity(ZG_GUIWidget *_this){
	ZG_GUIWidgetData *data=_this->data;
	return data->opacity;
}

void 		ZG_GUIWidget_SetOpacity(ZG_GUIWidget *_this,float  _opacity){
	ZG_GUIWidgetData *data=_this->data;
	data->opacity=_opacity;
}


void 		ZG_GUIWidget_SetEnabled(ZG_GUIWidget *_this, bool _enabled){
	ZG_GUIWidgetData *data=_this->data;
	data->is_enabled=_enabled;
}

bool ZG_GUIWidget_IsEnabled(ZG_GUIWidget *_this){
	ZG_GUIWidgetData *data=_this->data;
	return data->is_enabled;
}

ZG_Vector2i ZG_GUIWidget_GetDimensions(ZG_GUIWidget *_this){
	ZG_GUIWidgetData *data=_this->data;
	return data->dimensions;
}

void ZG_GUIWidget_AttachWidgetBase(ZG_GUIWidget *_this, ZG_GUIWidget *widget_to_attach){

	ZG_GUIWidgetData *data=_this->data;
	ZG_GUIWidgetData *data_widget=NULL;

	if(widget_to_attach==NULL){
		ZG_LOG_ERRORF("widget null");
		return;
	}

	data_widget=widget_to_attach->data;

	if(data_widget->parent!=NULL){
		ZG_LOG_ERRORF("Widget is already added. Please detach first");
		return;
	}

	ZG_List_Add(data->widgets, widget_to_attach);

	// now widget has this parent
	data_widget->parent=_this;
}


// it goes parent to parent till it founds a widget with type window
ZG_GUIWindow 		*ZG_GUIWidget_GetWindow(ZG_GUIWidget *_this){

	if(_this == NULL){
		return NULL;
	}

	ZG_GUIWidgetData *data=_this->data;
	if(_this->type == ZG_GUI_WIDGET_TYPE_WINDOW){
		// THIS IS NOT CORRECT
		return (ZG_GUIWindow 		*)_this->gui_ptr;
	}

	if(data->parent){
		return ZG_GUIWidget_GetWindow(data->parent);
	}
	return NULL;
}
/*
ZG_TTFontManager 	*ZG_GUIWidget_GetTTFontManager(ZG_GUIWidget *_this){
	ZG_GUIWindow *window=ZG_GUIWidget_GetWindow(_this);

	if(window == NULL){
		ZG_LOG_ERRORF("Cannot get ZG_TTFontManager because there's not window attached in this widget");
	}

	return ZG_GUIWindow_GetTTFontManager(window);
}
*/
ZG_TextureManager 	*ZG_GUIWidget_GetTextureManager(ZG_GUIWidget *_this){
	ZG_GUIWindow *window=ZG_GUIWidget_GetWindow(_this);

	if(window == NULL){
		ZG_LOG_ERRORF("Cannot get ZG_TextureManager because there's not window attached in this widget");
	}

	return ZG_GUIWindow_GetTextureManager(window);
}


void ZG_GUIWidget_AttachWidget(ZG_GUIWidget *_this, ZG_GUIWidget *widget_to_attach){

	ZG_GUIWidgetData *data=_this->data;

	if(data->attach_widget.ptr_function!=NULL){
		data->attach_widget.ptr_function(data->attach_widget.calling_widget,widget_to_attach);
	}else{
		ZG_GUIWidget_AttachWidgetBase(_this,widget_to_attach);
	}
}

void ZG_GUIWidget_AttachWidgetFunctionOverride(ZG_GUIWidget *_this,ZG_CallbackWidgetAttachWidget attach_widget){
	ZG_GUIWidgetData *data=_this->data;
	data->attach_widget=attach_widget;
}

void ZG_GUIWidget_SetDrawFunction(ZG_GUIWidget *_this,ZG_CallbackWidgetUpdate draw){
	ZG_GUIWidgetData *data=_this->data;
	data->draw=draw;
}

void ZG_GUIWidget_SetPostUpdateFunction(ZG_GUIWidget *_this,ZG_CallbackWidgetUpdate post_update){
	ZG_GUIWidgetData *data=_this->data;
	data->post_update=post_update;
}

void ZG_GUIWidget_UpdateFunctionOverride(ZG_GUIWidget *_this,ZG_CallbackWidgetUpdate update){
	ZG_GUIWidgetData *data=_this->data;
	data->update=update;
}

void ZG_GUIWidget_OnSetHeight(ZG_GUIWidget *_this,ZG_CallbackWidgetOnSetDimension on_set_height){
	ZG_GUIWidgetData *data=_this->data;
	data->on_set_height=on_set_height;
}

void ZG_GUIWidget_OnSetWidth(ZG_GUIWidget *_this,ZG_CallbackWidgetOnSetDimension on_set_width){
	ZG_GUIWidgetData *data=_this->data;
	data->on_set_width=on_set_width;
}


bool	ZG_GUIWidget_IsPointCollision(ZG_GUIWidget *_this,ZG_Vector2i point){
	ZG_GUIWidgetData *data=_this->data;
	return Vector2i_PointRectCollision(
				 point
				,data->position_screen
				,ZG_Vector2i_Add(data->position_screen,data->dimensions)
			);
}


ZG_Vector2i ZG_GUIWidget_GetPosition(ZG_GUIWidget *_this,ZG_GUIWidgetPosition widget_pos){
	ZG_GUIWidgetData *data=_this->data;
	switch(widget_pos){
	case ZG_GUI_WIDGET_POSITION_WORLD:
		return data->position_world;
	case ZG_GUI_WIDGET_POSITION_SCREEN:
		return data->position_screen;
	default:
		break;
	}

	return data->position_local;
}


void ZG_GUIWidget_UpdateChilds(ZG_GUIWidget *_this){
	ZG_GUIWidgetData *data=_this->data;

	for(unsigned i = 0; i < data->widgets->count ; i++) {
		ZG_GUIWidget *widget=data->widgets->items[i];
		ZG_GUIWidget_Update(widget);
	}
}

void ZG_GUIWidget_UpdatePosition(ZG_GUIWidget *_this){
	ZG_GUIWidgetData *data=_this->data;
	data->position_world=data->position_local;
	data->position_screen=data->position_local;

	if(data->parent!=NULL){
		ZG_GUIWidgetData *parent_data=data->parent->data;
		data->position_world=ZG_Vector2i_Add(data->position_world,parent_data->position_world);
		data->position_screen=ZG_Vector2i_Add(data->position_screen,parent_data->position_screen);
	}
}

void ZG_GUIWidget_UpdateWidget(ZG_GUIWidget *_this){

	ZG_GUIWidgetData *data=_this->data;
	ZG_GUIWidget_UpdatePosition(_this);

	if(data->post_update.ptr_function!=NULL){
		data->post_update.ptr_function(data->post_update.calling_widget);
	}

	// add widget to render list...
	if(data->draw.ptr_function!=NULL){
		data->draw.ptr_function(data->draw.calling_widget);
	}


	ZG_GUIWidget_UpdateChilds(_this);

}

void ZG_GUIWidget_Update(ZG_GUIWidget *_this){
	ZG_GUIWidgetData *data=_this->data;
	if(data->update.ptr_function != NULL){
		data->update.ptr_function(data->update.calling_widget);
	}
	else{
		ZG_GUIWidget_UpdateWidget(_this);
	}
}

void ZG_GUIWidget_Delete(ZG_GUIWidget *_this){
	if(_this == NULL) return;

	ZG_GUIWidgetData *_data= _this->data;

	ZG_List_Delete(_data->widgets);
	ZG_FREE(_data);
	ZG_FREE(_this);
}


