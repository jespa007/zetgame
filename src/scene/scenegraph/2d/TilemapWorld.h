#pragma once

//#include "zetgine_core.h"

#define NEW_TILEMAP_WORLD 			CTilemapWorld::newTilemapWorld
#define GET_TILEMAP_WORLD			CTilemapWorld::get



//import java.awt.*;
//----------------------------------------------------------------
class CTilemapWorld: public CDrawableObject2d /*CSprite*/ {

	static vector<CTilemapWorld *> m_vTilemapWorldContainer;
	/*public class tInfoTile{
		tRectangle	clip;
	};*/

	//tRectangle	clip_tile_texture[][];
	float       ** UVpoint;
	CTilemap * tilemap_data;

public:

	

	static CTilemapWorld * newTilemapWorld(
			const string & image_name,
			const FLOAT_VECTOR & tilemap_data,
			uint32_t nTilesX,
			uint32_t nTilesY,
			uint32_t tileWidthTile,
			uint32_t tileHeight
	);



	

	enum
	{

			TILEMAP_FLOOR_MODE7	= 0x1 << 0,
			TILEMAP_CEIL_MODE7	= 0x1 << 1,
			TILEMAP_INFINITE 	= 0x1 << 2
	};




	vector <CDrawableObject *> subtilemap3D;
	CDrawableObject ** tilemap3D;

	//CTilemapWorldData tilemap_data = NULL;


	int *** subtilemap;
	int ** tilemap;

	int nFrames, nVertexs;
	int nTilesX, nTilesY;

	//tTexture texture;
	//DrawableObject frame_shape;

	vector<CTilemapWorld> tile;// = new vector<CTilemapWorld>();
	bool map_created,
	tilemap_configured;
	int stripVertexCounts;
	float inc_subTileMapX3D,inc_subTileMapY3D;







	float width3d, heightransform;

	int tilemap_properties;

public:

	CTilemapWorld();

	float getWidth3D();
	float getHeight3D();


	CTilemapWorld(CTilemap * t, int properties);


	virtual void setScene(CScene * scn);

	//--------------------------------------------------------

	 bool isMode7();





	// Generate the object by reference (by-default)
	 virtual void updateChilds();
	//----------------------------------------------------------------------------------------------------------------------------
	 virtual void unload();

};
