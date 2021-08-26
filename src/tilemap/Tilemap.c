#include "game/zg_game.h"

typedef struct{
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
		CParserVarString<_CONST_CHAR("name")> m_name;
		CParserVarString<_CONST_CHAR("value")> m_value;
	}tPropertyTilemap;



	typedef struct{
			CParserVarString<_CONST_CHAR("name")> m_name;
			CParserVarNumber<_CONST_CHAR("opacity")> m_opacity;
			CParserVarString<_CONST_CHAR("type")>	m_type;
			CParserVarNumber<_CONST_CHAR("x")>		m_x;
			CParserVarNumber<_CONST_CHAR("y")>		m_y;
			CParserVarNumber<_CONST_CHAR("width")>	m_width;
			CParserVarNumber<_CONST_CHAR("height")>	m_height;
			CParserVarBoolean<_CONST_CHAR("visible")>	m_visible;
			CParserVarArrayNumber<_CONST_CHAR("data")>	m_data;
	}tPropLayer;

	typedef struct{
		CParserVarString<_CONST_CHAR("image")> 		m_image;
		CParserVarNumber<_CONST_CHAR("imagewidth")> 	m_imagewidth;
		CParserVarNumber<_CONST_CHAR("imageheight")> 	m_imageheight;
		CParserVarNumber<_CONST_CHAR("tilewidth")>	m_tilewidth;
		CParserVarNumber<_CONST_CHAR("tileheight")>	m_tileheight;
		CParserVarNumber<_CONST_CHAR("firstgid")> 	m_firstgid;
		CParserVarNumber<_CONST_CHAR("spacing")>		m_spacing;
		CParserVarNumber<_CONST_CHAR("margin")>		m_margin;
		CParserVarString<_CONST_CHAR("name")>			m_name;
		CParserVarPropertyGroup <tPropertyTilemap,_CONST_CHAR("properties")>  m_properties;
	}tPropTileSet;


	typedef struct{
			CParserVarNumber<_CONST_CHAR("width")>		m_width;
			CParserVarNumber<_CONST_CHAR("height")>		m_height;
			CParserVarNumber<_CONST_CHAR("tilewidth")>	m_tilewidth;
			CParserVarNumber<_CONST_CHAR("tileheight")>	m_tileheight;
			CParserVarNumber<_CONST_CHAR("version")>		m_version;
			CParserVarString<_CONST_CHAR("orientation")>	m_orientation;

			CParserVarPropertyGroup <tPropertyTilemap,_CONST_CHAR("properties")> m_properties;

			CParserVarArrayPropertyGroup<tPropLayer,_CONST_CHAR("layers")>		m_layers;
			CParserVarArrayPropertyGroup<tPropTileSet,_CONST_CHAR("tilesets")> 	m_tilesets;


	}tTiled;

	CParserVarPropertyGroup <tTiled> * m_mainStructTiled;


public:
}TilemapData;

Tilemap *Tilemap_New(){
	//m_tilemapAppearance = NULL;
	m_tilemapGeometry = NULL;
	m_textureTileMap=NULL;
}



bool Tilemap_Load(const string & m_filename){

	string file_to_load = m_working_dir + "/"+ m_filename;

	if(CObject::load(file_to_load)){
		// parse object ...
		CParserJson<tTiled> *m_parser = new CParserJson<tTiled>;

		if(m_parser->parseFile(file_to_load)){

			m_mainStructTiled = m_parser->getData()->at(0);

			// ok... until here... it's up to you.

			// load image ...
			//string image_name;
			//image_name = CFile::getFileName(file_to_load);
			//CTexture *text = NEW_TEXTURE(image_name);

			if(m_mainStructTiled->m_tilesets.size()>0 && m_mainStructTiled->m_layers.size() > 0){

				//image_name = CFile::getFileName(file_to_load);


				tPropLayer *layer = m_mainStructTiled->m_layers[0];
				tPropTileSet *tile = m_mainStructTiled->m_tilesets[0];
				string m_image = m_working_dir+"/"+tile->m_image.s_value;

				string image_name = CIO_Utils::getFileName(tile->m_image);
				CImageObject *img = NEW_IMAGE(image_name);
				/*char buf[400], *str;

				strcpy(buf,m_image.c_str());
				str = strrchr(buf,'.'); // first is the extension

				if(str != NULL){
					// save position...
					char *p_aux=str;
					// set end string
					*str=0;
					str = strrchr(buf,'.'); // next must be the root path
					// restore point
					*p_aux='.';
				}*/

				// this patch only works in linux... (TODO in Windows)
				/*if(str != NULL){

					m_image =str+3;
					// disables "./"
					//struct passwd *pw = getpwuid(getuid());

					const char *homedir = getenv("HOME");
					text->setWorkingDirectory(homedir);


					m_image = CStringUtils::remove(m_image, '\\');
					//string::replace(m_image,"\\",'');
					//m_image.replace(m_image.find("\\"), 1, "");

				}*/

				//CTexture *text = NEW_TEXTURE(image_name);

				int bytes_to_convert = 0;

				if(CStringUtils::endsWith(m_image,".gif")){
					bytes_to_convert = 4;
				}

				if(img->load(m_image, CImage::FLIP_Y, bytes_to_convert)){


					load(	image_name,
							0,
							layer->m_width,
							layer->m_height,
							tile->m_tilewidth,
							tile->m_tileheight,
							*layer->m_data.getShortBuffer()
					);
				}

			}



			delete m_parser;

			return true;
		}
	}

	return false;
}

bool Tilemap_Load(
							const string &	_texture_name,
							Uint32 			_app_attribs,
							Sint32 _width,
							Sint32 _height,
							Sint32 _tileheight,
							Sint32 _tilewidth, // in pixels ...
							const ShortBuffer & _tile

		)
		{
			//CRender *m_render = CEngine::getInstance()->getRender();
			//image_ = _texture_name;
			nTilesX = _width;
			nTilesY =  _height;
			tileheight= _tileheight;
			tilewidth =  _tilewidth; // in pixels ...
			tile = new ShortBuffer(_tile);


			//CTexture *text = NULL;

			//m_appearance->setAppearanceAttribs(_app_attribs | CAppearance::TRANSPARENCY | CAppearance::NO_LIGHT | CAppearance::NO_FACE_CULL);
			m_appearance->getMaterial()->setEmissiveColor3i(255,255,255);


			if(m_appearance != NULL) {
				m_textureTileMap = new CTexture();
				m_texture = m_textureTileMap;
				m_texture->setImage(GET_IMAGE(_texture_name));

				m_appearance->setTexture(m_texture);
			}


			if(m_texture != NULL) { // ---> texture ok!

				//basicSetup();

				//---- provisional load image with properties...
				//text->getTexture().setMagFilter(Texture.BASE_LEVEL_POINT);
				//text->getTexture().setMinFilter(Texture.BASE_LEVEL_POINT);
				//------------------------------------------------

				//draw_attributes = _draw_attributes;

				int x, y,offset_mesh;

				//nTiles = _nTiles;

				//this.tile_w = tile_w;
				//this.tile_h = tile_h;


				/*tilemap = _tilemap;

				if(tilemap == NULL) // create one tilemap of 2x2
				{
					tilemap = new int[2][2];
					tilemap[0][0] = 0;
					tilemap[0][1] = 0;
					tilemap[1][0] = 0;
					tilemap[1][1] = 0;

				}*/

				/*if( _subtilemap[0].length == 1)
					if( _subtilemap[0][0].length == 1) // generates a random tilemap...
					{
						int n_rand_tiles = _subtilemap[0][0][0];
						_subtilemap = new int[1][_subtilemap[0][0][0]][_subtilemap[0][0][0]];

						for(int yt=0; yt <  n_rand_tiles; yt++)
							for(int xt=0; xt <  n_rand_tiles; xt++)
								_subtilemap[0][yt][xt] = (int)Math.floor(Math.random()*_infotiles.length);
					}


				//subtilemap = _subtilemap;

				subtilemap3D = new CDrawableObject[subtilemap.length];


				// For every subtilemap create the m_geometry + mesh UV controller...
				//for(int t = 0; t < subtilemap.length; t++)*/


					//system.print_debug("Creating subtilemap "+t+" ...");


					//nTilesWidth = tile[0].length;
					//nTilesHeight = tile.length;

					//float drw = ((float)(text->getRealWidth())/(float)CRender.getWidth())/2.0f;
					//float drh = ((float)(text->getRealHeight())/(float)CRender.getWidth())/2.0f;
					float w_map= ZG_ACTIVE_RENDER->width_to_projection((float)(nTilesX*tilewidth))/2.0f; //((float)(nTilesX*tilewidth)/(float)m_render->getWidth())/2.0f;
					float h_map= ZG_ACTIVE_RENDER->height_to_projection((float)(nTilesY*tileheight))/2.0f;//((float)(nTilesY*tileheight)/(float)m_render->getWidth())/2.0f;
					float inc_dxdu = (float)(tilewidth) / (float)m_texture->getWidth();
					float inc_dydv = (float)(tileheight) /(float)m_texture->getHeight();
					float one_pixeldxdu = 1.0f/(float)m_texture->getWidth();
					float one_pixeldydv = 1.0f/(float)m_texture->getHeight();
					int MAX_TILES_U = m_texture->getWidth()/tilewidth;
					//int MAX_TILES_V = text->getHeight()/tileheight;

					width3d =  ZG_ACTIVE_RENDER->width_to_projection((float)(nTilesX*tilewidth)); // /(float)(m_render->getWidth());
					height3d =  ZG_ACTIVE_RENDER->height_to_projection((float)(nTilesY*tileheight));// /(float)(m_render->getWidth());

					//inc_subTileMapX3D = (float)(width3d)/tilemap[0].length;
					//inc_subTileMapY3D = (float)(height3d)/tilemap.length;

					float dxdu = 1.0f/(float)m_texture->getWidth();
					float dydv = 1.0f/(float)m_texture->getHeight();

					//float one_over_

					float inc_dx = ZG_ACTIVE_RENDER->width_to_projection((float)(tilewidth));// /(float)m_render->getWidth();
					float inc_dy = ZG_ACTIVE_RENDER->height_to_projection((float)(tileheight));// /(float)m_render->getWidth();


					float x_map, y_map;//, u_map, v_map;

					int total_vx = (nTilesX);
					int total_vy = (nTilesY);

					int nVertexs = total_vx*total_vy*4; // by 4 because is quad

					theIndexs = new ShortBuffer(nVertexs);
					theMesh3D = new FloatBuffer(nVertexs*3);
					theMeshUV = new FloatBuffer(nVertexs*2);
					//float [][] theMeshUV_key = new float[_nFramesAnimationPerTile][nVertexs*2];
					//float [] theMeshUV_key = new float[nVertexs*2];
					int i=0;




					int attributes = CIndexedQuadArray::COORDINATES | CIndexedQuadArray::BY_REFERENCE| CIndexedQuadArray::TEXTURE_COORDINATE_2;


					//if(!((draw_attributes & NO_GENERATE_NORMALS) == NO_GENERATE_NORMALS))
					//	attributes|=IndexedQuadArray.NORMALS;

					m_tilemapGeometry = new CIndexedQuadArray(
																nVertexs, // num. vertexs ...
																attributes,//QuadArray.BY_REFERENCE | QuadArray.COORDINATES , // format ...
																nVertexs); // num indexs...
					m_geometry = m_tilemapGeometry;


					//--- Create the 3D coordinates & indexs...
					for(y = 0, y_map = -h_map,offset_mesh=0; y < nTilesY; y++, y_map+=inc_dy)
					{
						for(x = 0, x_map=-w_map; x < nTilesX; x++,x_map+=(inc_dx), offset_mesh+=4*3) // new quad...
						{

							(*theMesh3D)[offset_mesh+0 ]=x_map;
							(*theMesh3D)[offset_mesh+1 ]=-y_map-inc_dy;
							(*theMesh3D)[offset_mesh+2 ]=0.0f;
							(*theIndexs)[i]=i;
							i++;

							(*theMesh3D)[offset_mesh+3 ]=x_map+inc_dx;
							(*theMesh3D)[offset_mesh+4 ]=-y_map-inc_dy;
							(*theMesh3D)[offset_mesh+5 ]=0.0f;
							(*theIndexs)[i]=i;
							i++;

							(*theMesh3D)[offset_mesh+6 ]=x_map+inc_dx;
							(*theMesh3D)[offset_mesh+7 ]=-y_map;
							(*theMesh3D)[offset_mesh+8 ]=0.0f;
							(*theIndexs)[i]=i;
							i++;

							(*theMesh3D)[offset_mesh+9 ]=x_map;
							(*theMesh3D)[offset_mesh+10]=-y_map;
							(*theMesh3D)[offset_mesh+11]=0.0f;
							(*theIndexs)[i]=i;
							i++;

						}
					}

					//float ini_frame=1.0f;

					// the interpolator must be inititalized from an 1D array.
					//float [] theMeshUV_Animation1D = new float[_nFramesAnimationPerTile*nVertexs*2+_nFramesAnimationPerTile];
					//int it_meshuv = 0;

					// --- Create the UV coordinates...
					//for(f = 0; f < _nFramesAnimationPerTile; f++)
					{
						/*
						 * 	1.0f, 0,0f,
							0.0f, 1.0f,
							1,0f, 1.0f,
							0,0f,  0.0f
						 * */

						for(y = 0,offset_mesh=0; y < nTilesY; y++)
						{
							for(x = 0; x < nTilesX; x++, offset_mesh+=4*2) // new quad...
							{
								// +- pixel is to avoid render next pixel tile...
								int tile_uv = (*tile)[y*nTilesX+x]-1;//[y][x]-1;

								float startx_tile = (tile_uv % MAX_TILES_U)*tilewidth*dxdu;
								float starty_tile = 1-(tile_uv / MAX_TILES_U)*tileheight*dydv;



								(*theMeshUV)[offset_mesh+0 ]=startx_tile+one_pixeldxdu;
								(*theMeshUV)[offset_mesh+1 ]=starty_tile-inc_dydv+one_pixeldydv;



								(*theMeshUV)[offset_mesh+2 ]=startx_tile+inc_dxdu-one_pixeldxdu;
								(*theMeshUV)[offset_mesh+3 ]=starty_tile-inc_dydv+one_pixeldydv;



								(*theMeshUV)[offset_mesh+4 ]=startx_tile+inc_dxdu-one_pixeldxdu;
								(*theMeshUV)[offset_mesh+5 ]=starty_tile-one_pixeldydv;



								(*theMeshUV)[offset_mesh+6 ]=startx_tile+one_pixeldxdu;
								(*theMeshUV)[offset_mesh+7 ]=starty_tile-one_pixeldydv;

								//for(int k = 0; k < 8 ; k++)
								//	theMeshUV_Animation1D[it_meshuv++]=theMeshUV_key[f][offset_mesh+k];

							}



							//theMeshUV_Animation1D


						}

						//theMeshUV_Animation1D[it_meshuv++]=

						//ini_frame-=inc_dydv;
					}

					// copy the first key to meshUV...
					//System.arraycopy(theMeshUV_key[0], 0, theMeshUV, 0, theMeshUV.length);




					//---- set mesh 3d coordinates...
					m_geometry->setIndices(0, theIndexs);
					m_geometry->setCoordRefFloat(theMesh3D);

					//---- set mesh UV coordinates...
					//m_geometry->setTextureCoordinateIndices(0, 0, indices);
					m_geometry->setTexCoordRefFloat(0,theMeshUV);
					m_geometry->compile();


					return true;



			}
			else
			{
				print_error_cr("Cannot create tilemap \"%s\" with NULL texture!",getName().c_str());
				//system.exit(-1);
			}

			return false;
	}

/*
void CTilemap::draw(){


	drawShape(&t3d_absolute,m_tilemapGeometry, m_defaultAppearance );
}*/

void Tilemap_UpdateSceneGraph()
{
	updateSceneGraphGeometry();

	t3d_absolute.m_scale.z=1;// set z=1 to avoid buggy light...
}



void Tilemap_Delete(Tilemap *_this){

	if(theMesh3D!=NULL)
		delete theMesh3D;
	if(theMeshUV!=NULL)
		delete theMeshUV;
	if(theIndexs!=NULL)
		delete theIndexs;

	if(tile != NULL)
		delete tile;

	if(m_tilemapGeometry != NULL)
		delete m_tilemapGeometry;

	//if(m_tilemapAppearance != NULL)
	//	delete m_tilemapAppearance;


	theMesh3D=NULL;
	theMeshUV=NULL;
	theIndexs=NULL;
	tile=NULL;
	m_geometry=NULL;

	delete m_textureTileMap;

	m_textureTileMap=NULL;

}



