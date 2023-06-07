#include "zg_gui.h"


typedef struct{
	ZG_List 				*widgets;
	GUIWidget 			*parent;
	ZG_Vector2i 			position_local, position_world, position_screen;
	ZG_Vector2i 			dimensions;
	ZG_Color4f				color,background_color;
	float 				opacity;
	bool 				is_enabled;

	GUIWindow *window;

	// events
	CallbackWidgetUpdate post_update,draw,update;
	CallbackWidgetOnSetDimension on_set_width;
	CallbackWidgetOnSetDimension on_set_height;
	CallbackWidgetAttachWidget attach_widget;


}GUIWidgetData;

GUIWidget  * GUIWidget_New(int x, int y, uint16_t width, uint16_t height){
	GUIWidget *widget=ZG_NEW(GUIWidget);
	GUIWidgetData *data=ZG_NEW(GUIWidgetData);
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

void GUIWidget_SetWindow(GUIWidget *_this,GUIWindow *_window){
	GUIWidgetData *data=_this->data;
	if(data->window!=NULL){
		ZG_Log_ErrorF("widget already has a window");
		return;
	}

	data->window=_window;
}

void GUIWidget_SetPositionX(GUIWidget *_this,int x){
	GUIWidgetData *data=_this->data;
	data->position_local.x=x;
}

void GUIWidget_SetPositionY(GUIWidget *_this,int y){
	GUIWidgetData *data=_this->data;
	data->position_local.y=y;
}

void GUIWidget_SetPosition2i(GUIWidget *_this,int x, int y){
	GUIWidget_SetPositionX(_this,x);
	GUIWidget_SetPositionY(_this,y);
}

void GUIWidget_SetPositionAbsolute2i(GUIWidget *_this,int x,int y){
	GUIWidgetData *data=_this->data;
	data->position_world.x=x;
	data->position_world.y=y;
}

void GUIWidget_SetWidth(GUIWidget *_this,uint16_t width){
	GUIWidgetData *data=_this->data;
	bool modify_width=data->dimensions.x != width;

	data->dimensions.x = width;

	if(modify_width && data->on_set_width.ptr_function!=NULL){
		data->on_set_width.ptr_function(data->on_set_width.calling_widget,width);
	}
}

void GUIWidget_SetHeight(GUIWidget *_this,uint16_t height){
	GUIWidgetData *data=_this->data;
	bool modify_height=data->dimensions.y != height;

	data->dimensions.y = height;

	if(modify_height && data->on_set_height.ptr_function!=NULL){
		data->on_set_height.ptr_function(data->on_set_height.calling_widget,height);
	}
}

void GUIWidget_SetDimensions(GUIWidget *_this,uint16_t width, uint16_t height){
	GUIWidget_SetHeight(_this,height);
	GUIWidget_SetWidth(_this,width);
}

ZG_Color4f GUIWidget_GetBackgroundColor4f(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	return data->background_color;
}

void 		GUIWidget_SetBackgroundColor4f(GUIWidget *_this,ZG_Color4f _color){
	GUIWidgetData *data=_this->data;
	data->background_color=_color;
}

void 		GUIWidget_SetColor4f(GUIWidget *_this,ZG_Color4f _color){
	GUIWidgetData *data=_this->data;
	data->color=_color;

}

ZG_Color4f 	GUIWidget_GetColor4f(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	return data->color;
}

float 		GUIWidget_GetOpacity(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	return data->opacity;
}

void 		GUIWidget_SetOpacity(GUIWidget *_this,float  _opacity){
	GUIWidgetData *data=_this->data;
	data->opacity=_opacity;
}


void 		GUIWidget_SetEnabled(GUIWidget *_this, bool _enabled){
	GUIWidgetData *data=_this->data;
	data->is_enabled=_enabled;
}

bool GUIWidget_IsEnabled(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	return data->is_enabled;
}

ZG_Vector2i GUIWidget_GetDimensions(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	return data->dimensions;
}

void GUIWidget_AttachWidgetBase(GUIWidget *_this, GUIWidget *widget_to_attach){

	GUIWidgetData *data=_this->data;
	GUIWidgetData *data_widget=NULL;

	if(widget_to_attach==NULL){
		ZG_Log_ErrorF("widget null");
		return;
	}

	data_widget=widget_to_attach->data;

	if(data_widget->parent!=NULL){
		ZG_Log_ErrorF("Widget is already added. Please detach first");
		return;
	}

	ZG_List_Add(data->widgets, widget_to_attach);

	// now widget has this parent
	data_widget->parent=_this;
}


// it goes parent to parent till it founds a widget with type window
GUIWindow 		*GUIWidget_GetWindow(GUIWidget *_this){

	if(_this == NULL){
		return NULL;
	}

	GUIWidgetData *data=_this->data;
	if(_this->type == WIDGET_TYPE_WINDOW){
		// THIS IS NOT CORRECT
		return (GUIWindow 		*)_this->gui_ptr;
	}

	if(data->parent){
		return GUIWidget_GetWindow(data->parent);
	}
	return NULL;
}
/*
ZG_TTFontManager 	*GUIWidget_GetTTFontManager(GUIWidget *_this){
	GUIWindow *window=GUIWidget_GetWindow(_this);

	if(window == NULL){
		ZG_Log_ErrorF("Cannot get ZG_TTFontManager because there's not window attached in this widget");
	}

	return GUIWindow_GetTTFontManager(window);
}
*/
ZG_TextureManager 	*GUIWidget_GetTextureManager(GUIWidget *_this){
	GUIWindow *window=GUIWidget_GetWindow(_this);

	if(window == NULL){
		ZG_Log_ErrorF("Cannot get ZG_TextureManager because there's not window attached in this widget");
	}

	return GUIWindow_GetTextureManager(window);
}


void GUIWidget_AttachWidget(GUIWidget *_this, GUIWidget *widget_to_attach){

	GUIWidgetData *data=_this->data;

	if(data->attach_widget.ptr_function!=NULL){
		data->attach_widget.ptr_function(data->attach_widget.calling_widget,widget_to_attach);
	}else{
		GUIWidget_AttachWidgetBase(_this,widget_to_attach);
	}
}

void GUIWidget_AttachWidgetFunctionOverride(GUIWidget *_this,CallbackWidgetAttachWidget attach_widget){
	GUIWidgetData *data=_this->data;
	data->attach_widget=attach_widget;
}

void GUIWidget_SetDrawFunction(GUIWidget *_this,CallbackWidgetUpdate draw){
	GUIWidgetData *data=_this->data;
	data->draw=draw;
}

void GUIWidget_SetPostUpdateFunction(GUIWidget *_this,CallbackWidgetUpdate post_update){
	GUIWidgetData *data=_this->data;
	data->post_update=post_update;
}

void GUIWidget_UpdateFunctionOverride(GUIWidget *_this,CallbackWidgetUpdate update){
	GUIWidgetData *data=_this->data;
	data->update=update;
}

void GUIWidget_OnSetHeight(GUIWidget *_this,CallbackWidgetOnSetDimension on_set_height){
	GUIWidgetData *data=_this->data;
	data->on_set_height=on_set_height;
}

void GUIWidget_OnSetWidth(GUIWidget *_this,CallbackWidgetOnSetDimension on_set_width){
	GUIWidgetData *data=_this->data;
	data->on_set_width=on_set_width;
}


bool	GUIWidget_IsPointCollision(GUIWidget *_this,ZG_Vector2i point){
	GUIWidgetData *data=_this->data;
	return Vector2i_PointRectCollision(
				 point
				,data->position_screen
				,ZG_Vector2i_Add(data->position_screen,data->dimensions)
			);
}


ZG_Vector2i GUIWidget_GetPosition(GUIWidget *_this,WidgetPosition widget_pos){
	GUIWidgetData *data=_this->data;
	switch(widget_pos){
	case WIDGET_POSITION_WORLD:
		return data->position_world;
	case WIDGET_POSITION_SCREEN:
		return data->position_screen;
	default:
		break;
	}

	return data->position_local;
}


void GUIWidget_UpdateChilds(GUIWidget *_this){
	GUIWidgetData *data=_this->data;

	for(unsigned i = 0; i < data->widgets->count ; i++) {
		GUIWidget *widget=data->widgets->items[i];
		GUIWidget_Update(widget);
	}
}

void GUIWidget_UpdatePosition(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	data->position_world=data->position_local;
	data->position_screen=data->position_local;

	if(data->parent!=NULL){
		GUIWidgetData *parent_data=data->parent->data;
		data->position_world=ZG_Vector2i_Add(data->position_world,parent_data->position_world);
		data->position_screen=ZG_Vector2i_Add(data->position_screen,parent_data->position_screen);
	}
}

void GUIWidget_UpdateWidget(GUIWidget *_this){

	GUIWidgetData *data=_this->data;
	GUIWidget_UpdatePosition(_this);

	if(data->post_update.ptr_function!=NULL){
		data->post_update.ptr_function(data->post_update.calling_widget);
	}

	// add widget to render list...
	if(data->draw.ptr_function!=NULL){
		data->draw.ptr_function(data->draw.calling_widget);
	}


	GUIWidget_UpdateChilds(_this);

}

void GUIWidget_Update(GUIWidget *_this){
	GUIWidgetData *data=_this->data;
	if(data->update.ptr_function != NULL){
		data->update.ptr_function(data->update.calling_widget);
	}
	else{
		GUIWidget_UpdateWidget(_this);
	}
}

void GUIWidget_Delete(GUIWidget *_this){
	if(_this == NULL) return;

	GUIWidgetData *_data= _this->data;

	ZG_List_Delete(_data->widgets);
	ZG_FREE(_data);
	ZG_FREE(_this);
}


