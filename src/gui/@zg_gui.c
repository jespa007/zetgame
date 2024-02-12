#include "ZG_GUIWidget.c"
#include "ZG_GUIFrame.c"
#include "ZG_GUIButton.c"
#include "ZG_GUITextBox.c"
#include "ZG_GUITexture.c"
#include "ZG_GUIWindow.c"


ZG_GUIWindow *g_active_window=NULL;

void ZG_GUI_Init(void){
	ZG_GUIButton_Init();
}

void ZG_GUI_DeInit(void){
	ZG_GUIButton_DeInit();
}

/*
void GUI_Begin(ZG_GUIWindow * window){
	g_active_window=window;

	if(window != NULL) {
		window->update();
	}
}

void GUI_End(void){
	if(m_gui_mouse_visible)
	{
			if(mouse_image_show){
				CGraphics::drawQuadTextured(
						m_normalizedMouseCoordinates.x-mouse_image_width*0.5f
						,m_normalizedMouseCoordinates.y-mouse_image_height*0.5f
						,mouse_image_width
						,mouse_image_height
						,mouse_image_texture
						);
			}
			else{

				switch(cursor){
				default:
				case CInput::JEME_Cursor::JEME_CURSOR_ARROW:
						CGUIIconManager::drawDefaultIcon(
								m_normalizedMouseCoordinates.x-16*0.5f,
								m_normalizedMouseCoordinates.y-16*0.5f,16,16,
									DEFAULT_ICON(CGUIIconManager::ICON_MOUSE_ID),CColor3f(1,1,1));
						break;
				case CInput::JEME_Cursor::JEME_CURSOR_SIZEWE:
					CGUIIconManager::drawDefaultIcon(
							m_normalizedMouseCoordinates.x-((16)*0.5f),
							m_normalizedMouseCoordinates.y-16*0.5f,16,16,
								DEFAULT_ICON(CGUIIconManager::ICON_SIZE_WE_ID),CColor3f(1,1,1));
					break;
				case CInput::JEME_Cursor::JEME_CURSOR_OPENED_HAND:
					CGUIIconManager::drawDefaultIcon(
							m_normalizedMouseCoordinates.x-((16)*0.5f),
							m_normalizedMouseCoordinates.y-16*0.5f,16,16,
								DEFAULT_ICON(CGUIIconManager::ICON_MOUSE_OPENED_HAND),CColor3f(1,1,1));
					break;
				case CInput::JEME_Cursor::JEME_CURSOR_CLOSED_HAND:
					CGUIIconManager::drawDefaultIcon(
							m_normalizedMouseCoordinates.x-((16)*0.5f),
							m_normalizedMouseCoordinates.y-16*0.5f,16,16,
								DEFAULT_ICON(CGUIIconManager::ICON_MOUSE_CLOSED_HAND),CColor3f(1,1,1));
					break;
				case CInput::JEME_Cursor::JEME_CURSOR_HAND:
					CGUIIconManager::drawDefaultIcon(
							m_normalizedMouseCoordinates.x-16*0.5f,
							m_normalizedMouseCoordinates.y-16*0.5f,16,16,
								DEFAULT_ICON(CGUIIconManager::ICON_HAND_ID),CColor3f(1,1,1));
					break;

				};
			}
		}
}
*/
void Gui_DeInit(void){
	ZG_GUIButton_DeInit();
}

