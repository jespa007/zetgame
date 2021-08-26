#ifndef __TILEMAP_H__
#define __TILEMAP_H__


typedef struct Tilemap Tilemap;

struct Tilemap{
	void *data;
};



		//----------------------------------------------------------------------------------------------------
		//
		// Static functions ...
		//
		//static vector<CTilemap *>  * m_vTilemapContainer;

		static CTilemap * newTilemap(
				const string &	_texture_name,
				Uint32 			_app_attribs,
				int _width,
				int _height,
				int _tileheight,
				int _tilewidth, // in pixels ...
				const ShortBuffer & _tile
		);





		//
		//
		//
		//----------------------------------------------------------------------------------------------------


		float width3d, height3d;

		CTilemap();

		virtual bool load(const string & m_filename);

		bool load(

							const string &	_texture_name,
							Uint32 			_app_attribs,
							Sint32 _width,
							Sint32 _height,
							Sint32 _tileheight,
							Sint32 _tilewidth, // in pixels ...
							const ShortBuffer & _tile
		);

		//virtual void draw();

		virtual void unload();
		virtual void updateSceneGraph();

		virtual ~CTilemap();
};


#endif
