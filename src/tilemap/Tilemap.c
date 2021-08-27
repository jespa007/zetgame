#include "zg_tilemap.h"

typedef struct{
	size_t 		width, height; 	// in tiles...
	size_t 		tile_height, tile_width; // in pixels ...
	short 		*tiles; // map tile ...
	Geometry 	*geometry;

	short  		*indexs;
	float	 	*mesh_3d;
	float 		*mesh_uv;
	Texture 	*texture;
}TilemapData;


Tilemap *Tilemap_New(short *_tiles, size_t _width, size_t _height, size_t _tile_width, size_t _tile_height, Texture *_texture){
	Tilemap *tm=NEW(Tilemap);
	Geometry *geometry=NULL;
	TilemapData *data=NEW(TilemapData);
	tm->data=data;

	data->width = _width;
	data->height =  _height;
	data->tile_width = _tile_width;
	data->tile_height = _tile_height;
	data->texture=_texture;


	short *tiles=data->tiles = malloc(sizeof(short)*_width*_height);

	int x, y,offset_mesh;
	float w_map= ViewPort_ScreenToWorldWidth((float)(_width*_tile_width))/2.0f;
	float h_map= ViewPort_ScreenToWorldHeight((float)(_height*_tile_height))/2.0f;
	float inc_dxdu = (float)(_tile_width) / (float)_texture->width;
	float inc_dydv = (float)(_tile_height) /(float)_texture->height;
	float one_pixeldxdu = 1.0f/(float)_texture->width;
	float one_pixeldydv = 1.0f/(float)_texture->height;
	int MAX_TILES_U = _texture->width/_tile_width;

	float dxdu = 1.0f/(float)_texture->width;
	float dydv = 1.0f/(float)_texture->height;

	//float one_over_
	float inc_dx = ViewPort_ScreenToWorldWidth((float)(_tile_width));// /(float)m_render->getWidth();
	float inc_dy = ViewPort_ScreenToWorldHeight((float)(_tile_height));// /(float)m_render->getWidth();


	float x_map, y_map;//, u_map, v_map;

	size_t total_vertexs = _width*_height*4; // by 4 because is quad
	size_t mesh3d_vertexs_len=total_vertexs*3;
	size_t mesh_texture_vertexs_len=total_vertexs*2;

	short *indexs = data->indexs= malloc(sizeof(short)*total_vertexs);
	float *mesh_3d = data->mesh_3d=malloc(sizeof(float)*mesh3d_vertexs_len);
	float *mesh_uv =data->mesh_uv = malloc(sizeof(float)*mesh_texture_vertexs_len);


	geometry=data->geometry = 	Geometry_New(
			  GEOMETRY_PROPERTY_TEXTURE
			| GEOMETRY_PROPERTY_QUADS
	);

	int i=0;
	//--- Create the 3D coordinates & indexs...
	for(y = 0, y_map = -h_map,offset_mesh=0; y < _height; y++, y_map+=inc_dy)
	{
		for(x = 0, x_map=-w_map; x < _width; x++,x_map+=(inc_dx), offset_mesh+=4*3) // new quad...
		{
			(*mesh_3d)[offset_mesh+0 ]=x_map;
			(*mesh_3d)[offset_mesh+1 ]=-y_map-inc_dy;
			(*mesh_3d)[offset_mesh+2 ]=0.0f;
			(*indexs)[i]=i;
			i++;

			(*mesh_3d)[offset_mesh+3 ]=x_map+inc_dx;
			(*mesh_3d)[offset_mesh+4 ]=-y_map-inc_dy;
			(*mesh_3d)[offset_mesh+5 ]=0.0f;
			(*indexs)[i]=i;
			i++;

			(*mesh_3d)[offset_mesh+6 ]=x_map+inc_dx;
			(*mesh_3d)[offset_mesh+7 ]=-y_map;
			(*mesh_3d)[offset_mesh+8 ]=0.0f;
			(*indexs)[i]=i;
			i++;

			(*mesh_3d)[offset_mesh+9 ]=x_map;
			(*mesh_3d)[offset_mesh+10]=-y_map;
			(*mesh_3d)[offset_mesh+11]=0.0f;
			(*indexs)[i]=i;
			i++;
		}
	}


	for(y = 0,offset_mesh=0; y < _height; y++)
	{
		for(x = 0; x < _width; x++, offset_mesh+=4*2) // new quad...
		{
			// +- pixel is to avoid render next pixel tile...
			int tile_uv = (*tiles)[y*_width+x]-1;//[y][x]-1;

			float startx_tile = (tile_uv % MAX_TILES_U)*_tile_width*dxdu;
			float starty_tile = 1-(tile_uv / MAX_TILES_U)*_tile_height*dydv;

			(*mesh_uv)[offset_mesh+0 ]=startx_tile+one_pixeldxdu;
			(*mesh_uv)[offset_mesh+1 ]=starty_tile-inc_dydv+one_pixeldydv;

			(*mesh_uv)[offset_mesh+2 ]=startx_tile+inc_dxdu-one_pixeldxdu;
			(*mesh_uv)[offset_mesh+3 ]=starty_tile-inc_dydv+one_pixeldydv;

			(*mesh_uv)[offset_mesh+4 ]=startx_tile+inc_dxdu-one_pixeldxdu;
			(*mesh_uv)[offset_mesh+5 ]=starty_tile-one_pixeldydv;

			(*mesh_uv)[offset_mesh+6 ]=startx_tile+one_pixeldxdu;
			(*mesh_uv)[offset_mesh+7 ]=starty_tile-one_pixeldydv;
		}
	}

	Geometry_SetIndicies(geometry,indexs);
	Geometry_SetMeshVertex(geometry,mesh3d_vertexs_len);
	Geometry_SetMeshTexture(geometry,mesh_texture_vertexs_len);

	return tm;
}


void Tilemap_Delete(Tilemap *_this){
	TilemapData *data=_this->data;

	free(data->mesh_3d);
	free(data->mesh_uv);
	free(data->indexs);
	free(data->tiles);

	Geometry_Delete(data->geometry);

}



