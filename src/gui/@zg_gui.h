#ifndef __ZG_GUI_H__
#define __ZG_GUI_H__

#include "input/@zg_input.h"
#include "graphics/@zg_graphics.h"

#include "ZG_GUIWidget.h"
#include "ZG_GUIFrame.h"
#include "ZG_GUIButton.h"
#include "ZG_GUITextBox.h"
#include "ZG_GUITexture.h"
#include "ZG_GUIWindow.h"
//#include "ZG_GUIWindowManager.h"


void ZG_GUI_Init(void);


/*void GUI_Begin(ZG_GUIWindow * window);
void GUI_End(void);*/

void ZG_GUI_DeInit(void);


		/*void begin(){
			CZG_GUIWindow *window=ZG_GET_ACTIVE_GUI_WINDOW;

			if(window != NULL) {
				window->update();
			}
		}

		void end(){
			//----------------------
			// restore canvas + draw gui mouse ...

			glViewport(0,0,CVideoDriverOpenGL::getInstance()->getWidth(),CVideoDriverOpenGL::getInstance()->getHeight());

			ZG_GET_ACTIVE_RENDER_CONTEXT->setMode(CSGRender::ORTHO_MODE);
			glDisable(GL_LIGHTING);


			CInput::ZG_Cursor cursor =ZG_INPUT->getCursor();



			cursor = CZG_GUIWindow::getIconCollisionCursor();
			CPoint2i *m_normalizedMouseCoordinates=ZG_INPUT->getNormalizedMouseCoordinates();

			switch(cursor){
			default:
			case CInput::ZG_Cursor::ZG_CURSOR_ARROW:
					CGUIIconManager::drawDefaultIcon(
							m_normalizedMouseCoordinates->x-16*0.5f,
							m_normalizedMouseCoordinates->y-16*0.5f,16,16,
								DefaultIcon(CGUIIconManager::ICON_MOUSE_ID),CColor3f(1,1,1));
					break;
			case CInput::ZG_Cursor::ZG_CURSOR_SIZEWE:
				CGUIIconManager::drawDefaultIcon(
						m_normalizedMouseCoordinates->x-((16)*0.5f),
						m_normalizedMouseCoordinates->y-16*0.5f,16,16,
							DefaultIcon(CGUIIconManager::ICON_SIZE_WE_ID),CColor3f(1,1,1));
				break;
			case CInput::ZG_Cursor::ZG_CURSOR_OPENED_HAND:
				CGUIIconManager::drawDefaultIcon(
						m_normalizedMouseCoordinates->x-((16)*0.5f),
						m_normalizedMouseCoordinates->y-16*0.5f,16,16,
							DefaultIcon(CGUIIconManager::ICON_MOUSE_OPENED_HAND),CColor3f(1,1,1));
				break;
			case CInput::ZG_Cursor::ZG_CURSOR_CLOSED_HAND:
				CGUIIconManager::drawDefaultIcon(
						m_normalizedMouseCoordinates->x-((16)*0.5f),
						m_normalizedMouseCoordinates->y-16*0.5f,16,16,
							DefaultIcon(CGUIIconManager::ICON_MOUSE_CLOSED_HAND),CColor3f(1,1,1));
				break;
			case CInput::ZG_Cursor::ZG_CURSOR_HAND:
				CGUIIconManager::drawDefaultIcon(
						m_normalizedMouseCoordinates->x-16*0.5f,
						m_normalizedMouseCoordinates->y-16*0.5f,16,16,
							DefaultIcon(CGUIIconManager::ICON_HAND_ID),CColor3f(1,1,1));
				break;

			};
		}*/


#endif
