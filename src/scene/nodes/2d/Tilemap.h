#pragma once

//#include "zetgine_core.h"
class CTilemap : public CDrawableObject2d{


		int nTilesY, nTilesX; // in tiles...
		int tileheight, tilewidth; // in pixels ...
		ShortBuffer * tile; // map tile ...
		//CAppearance *m_tilemapAppearance;
		CGeometry *m_tilemapGeometry;

		ShortBuffer  *theIndexs;
		FloatBuffer *theMesh3D;
		FloatBuffer *theMeshUV;
		CTexture 	*m_textureTileMap;

		typedef struct{
			ParserVarString<ZJZJ_CONST_CHAR("name")> m_name;
			ParserVarString<ZJZJ_CONST_CHAR("value")> m_value;
		}tPropertyTilemap;



		typedef struct{
				ParserVarString<ZJZJ_CONST_CHAR("name")> m_name;
				ParserVarNumber<ZJZJ_CONST_CHAR("opacity")> m_opacity;
				ParserVarString<ZJZJ_CONST_CHAR("type")>	m_type;
				ParserVarNumber<ZJZJ_CONST_CHAR("x")>		m_x;
				ParserVarNumber<ZJZJ_CONST_CHAR("y")>		m_y;
				ParserVarNumber<ZJZJ_CONST_CHAR("width")>	m_width;
				ParserVarNumber<ZJZJ_CONST_CHAR("height")>	m_height;
				CParserVarBoolean<ZJZJ_CONST_CHAR("visible")>	m_visible;
				CParserVarNumberArray<ZJZJ_CONST_CHAR("data")>	m_data;
		}tPropLayer;

		typedef struct{
			ParserVarString<ZJZJ_CONST_CHAR("image")> 		m_image;
			ParserVarNumber<ZJZJ_CONST_CHAR("imagewidth")> 	m_imagewidth;
			ParserVarNumber<ZJZJ_CONST_CHAR("imageheight")> 	m_imageheight;
			ParserVarNumber<ZJZJ_CONST_CHAR("tilewidth")>	m_tilewidth;
			ParserVarNumber<ZJZJ_CONST_CHAR("tileheight")>	m_tileheight;
			ParserVarNumber<ZJZJ_CONST_CHAR("firstgid")> 	m_firstgid;
			ParserVarNumber<ZJZJ_CONST_CHAR("spacing")>		m_spacing;
			ParserVarNumber<ZJZJ_CONST_CHAR("margin")>		m_margin;
			ParserVarString<ZJZJ_CONST_CHAR("name")>			m_name;
			ParserVarPropertyGroup <tPropertyTilemap,ZJZJ_CONST_CHAR("properties")>  m_properties;
		}tPropTileSet;


		typedef struct{
				ParserVarNumber<ZJZJ_CONST_CHAR("width")>		m_width;
				ParserVarNumber<ZJZJ_CONST_CHAR("height")>		m_height;
				ParserVarNumber<ZJZJ_CONST_CHAR("tilewidth")>	m_tilewidth;
				ParserVarNumber<ZJZJ_CONST_CHAR("tileheight")>	m_tileheight;
				ParserVarNumber<ZJZJ_CONST_CHAR("version")>		m_version;
				ParserVarString<ZJZJ_CONST_CHAR("orientation")>	m_orientation;

				ParserVarPropertyGroup <tPropertyTilemap,ZJZJ_CONST_CHAR("properties")> m_properties;

				CParserVarArrayPropertyGroup<tPropLayer,ZJZJ_CONST_CHAR("layers")>		m_layers;
				CParserVarArrayPropertyGroup<tPropTileSet,ZJZJ_CONST_CHAR("tilesets")> 	m_tilesets;


		}tTiled;

		ParserVarPropertyGroup <tTiled> * m_mainStructTiled;


public:


		//----------------------------------------------------------------------------------------------------
		//
		// Static functions ...
		//
		//static vector<CTilemap *>  * m_vTilemapContainer;

		static CTilemap * newTilemap(
				const string &	_texture_name,
				uint32_t 			_app_attribs,
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


		float width3d, heightransform;

		CTilemap();

		virtual bool load(const string & m_filename);

		bool load(

							const string &	_texture_name,
							uint32_t 			_app_attribs,
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
