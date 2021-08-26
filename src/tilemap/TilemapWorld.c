#include "game/zg_game.h"

vector<CTilemapWorld *>  CTilemapWorld::m_vTilemapWorldContainer ;



float ceil_height=0.128f;
int	NUM_TILES_SORROUND=20;

	CTilemapWorld::CTilemapWorld()
	{
		tilemap = NULL;
		tilemap_properties = 0;
		tilemap3D = NULL;
		map_created=false;
	}

	float CTilemapWorld::getWidth3D(){return width3d;}
	float CTilemapWorld::getHeight3D(){return height3d;}


	CTilemapWorld::CTilemapWorld(CTilemap * t, int properties)
	{
		tilemap_data = t;
		map_created=false;

		if(tilemap_data != NULL)
		{
			tilemap_properties = properties;
			m_geometry = tilemap_data->getGeometry();
			m_appearance = tilemap_data->getAppearance();
		}
	}




	 void CTilemapWorld::setScene(CScene * scn)
	{
		CScreenObject::setScene(scn);

		if(map_created)
		{
			if(scn != NULL)
			{
				for(unsigned i = 0; i < subtilemap3D.size(); i++)
					subtilemap3D[i]->setScene(scn);
			}
		}

	}

	//--------------------------------------------------------



	 bool CTilemapWorld::isMode7()
	{
		return ((tilemap_properties & CTilemapWorld::TILEMAP_CEIL_MODE7) == CTilemapWorld::TILEMAP_CEIL_MODE7) || ((tilemap_properties & CTilemapWorld::TILEMAP_FLOOR_MODE7) == CTilemapWorld::TILEMAP_FLOOR_MODE7);
	}





	// Generate the object by reference (by-default)
	 void CTilemapWorld::updateChilds()
	{
		 CRender *m_render = ZG_ACTIVE_RENDER;

		/*if(!isMode7())
		{
			super.updateChilds();
			return;
		}*/

		if(tilemap_data!=NULL)
		{


			int xtile_cam = 0;
			int ztile_cam = 0;

			if((tilemap_properties & CTilemapWorld::TILEMAP_INFINITE) != 0)
			{
				xtile_cam = (int)floor((float)(m_attached_scene->getCamera()->getTranslate(CScreenObject::WORLD_COORDINATES).x/tilemap_data->width3d));

				if(isMode7())
					ztile_cam = (int)floor((float)(m_attached_scene->getCamera()->getTranslate(CScreenObject::WORLD_COORDINATES).z/tilemap_data->height3d));
				else
					ztile_cam = (int)floor((float)(m_attached_scene->getCamera()->getTranslate(CScreenObject::WORLD_COORDINATES).y/tilemap_data->height3d));

			}

			// the mode7 is aligned to 0,0. The we must pos the start of painting NUM_TILES_AROUND.
			int xtile_start = xtile_cam-(NUM_TILES_SORROUND>>1);
			int ztile_start = ztile_cam-(NUM_TILES_SORROUND>>1);

			float x_ini = (xtile_start*(tilemap_data->width3d));
			float z_ini = (ztile_start*(tilemap_data->height3d));


			if((tilemap_properties & CTilemapWorld::TILEMAP_FLOOR_MODE7) != 0)
				tilemap_data->setRotate(90,0,0);
			else if((tilemap_properties & CTilemapWorld::TILEMAP_CEIL_MODE7) != 0)
				tilemap_data->setRotate(-90,0,0);
			else
				tilemap_data->setRotate( 0,0,0);

			tilemap_data->update();
			/*CCamera *camera=CCameraFactory::getDefaultResource();
			if(m_attached_scene!=NULL){
				camera = m_attached_scene->getCamera();
			}*/


	//		subtilemap3D[0].setRotation(-90,0,0);
	//		subtilemap3D[0].update();

			//subtilemap3D[0].updateController();
			for(float z3d=z_ini, z = 0; z < NUM_TILES_SORROUND; z++, z3d+=(tilemap_data->height3d))
			for(float x3d=x_ini, x = 0; x < NUM_TILES_SORROUND; x++, x3d+=(tilemap_data->width3d))
			{

				//subtilemap3D[0].update();
				if(isMode7())
					m_render->putObjectToDraw(x3d, ((tilemap_properties & CTilemapWorld::TILEMAP_CEIL_MODE7) == CTilemapWorld::TILEMAP_CEIL_MODE7) ?ceil_height:0 , z3d, tilemap_data, 0); // Z = 0, Force first object to be drawed every...
				else
					m_render->putObjectToDraw(x3d, z3d , 0, tilemap_data, 0); // Z = 0, Force first object to be drawed every...
			}
		}
		//subtilemap3D[0].update();

	}
	//----------------------------------------------------------------------------------------------------------------------------
	/*void CTilemapWorld::draw()
	{


		if(!isMode7()) // not mode 7 ...
			super.draw();


	}*/
	//--------------------------------------------------------
	 void  CTilemapWorld::unload(){
		 CObject::unload();
	 }
