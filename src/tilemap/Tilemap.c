#include "zg_tilemap.h"




typedef struct{
	//size_t 		width, height; 	// in tiles...
	//size_t 		tile_height, tile_width; // in pixels ...
	//short 		*tiles; // map tile ...
	Geometry 	*geometry;

	//short  		*indexs;
	//float	 	*mesh_vertexs;
	//float 		*mesh_texture;
	Texture 	*texture;
	Tilesets	*tilesets;
}TilemapData;


Tilemap *Tilemap_New(
		short *_tiles
		, size_t _width
		, size_t _height
		, size_t _tile_width
		, size_t _tile_height
		, Texture *_texture
		,Tilesets *_tilesets
){
	Tilemap *tm=NEW(Tilemap);
	Geometry *geometry=NULL;
	//short *tiles=NULL;
	TilemapData *data=NEW(TilemapData);
	tm->data=data;
	data->tilesets=_tilesets;

	data->texture=_texture;

	int offset_mesh=0;
	int offset_indices=0;
	int offset_ptr_indices=0;
	float start_tilemap_x3d= -ViewPort_ScreenToWorldWidth((float)(_width*_tile_width))/2.0f;
	float tilemap_y3d= ViewPort_ScreenToWorldHeight((float)(_height*_tile_height))/2.0f;
	float inc_tilemap_dxdx3d = ViewPort_ScreenToWorldWidth((float)(_tile_width));
	float inc_tilemap_dydy3d = ViewPort_ScreenToWorldHeight((float)(_tile_height));


	float inc_tile_dxdu = (float)(_tile_width) / (float)_texture->width;
	float inc_tile_dydv = (float)(_tile_height) /(float)_texture->height;

	// calculate one_texel_over_width to do an offset in texture to avoid weird solid border between tiles
	float one_texel_over_width = 1.0f/(float)_texture->width;
	float one_texel_over_height = 1.0f/(float)_texture->height;

	int tile_count_x = _tilesets->tile_count_x;

	int n_vertexs = _width*_height*3*2; // *3*2 because each tile is done by 2 triangles (each one by 3 vertexes)
	int n_indices = GEOMETRY_INDICES_FROM_N_VERTEXS(n_vertexs);
	int mesh_vertexs_len=n_vertexs*3; // *3 because it has 3d coords (xyz)
	int mesh_texture_len=n_vertexs*2; // *2 because it has 2d coords (uv)

	short *indexs = malloc(sizeof(short)*n_indices);
	float *mesh_vertexs=malloc(sizeof(float)*mesh_vertexs_len);
	float *mesh_texture=malloc(sizeof(float)*mesh_texture_len);

	geometry=data->geometry = 	Geometry_New(
			n_vertexs,
			  GEOMETRY_PROPERTY_TEXTURE
	);

	// mesh vertexs
	for(int y = 0; y < (int)_height; y++)
	{
		float tilemap_x3d=start_tilemap_x3d;
		for(int x = 0; x < (int)_width; x++) // new quad...
		{
			// configure indices
			*(indexs+offset_ptr_indices+0)=(offset_indices+0);
			*(indexs+offset_ptr_indices+1)=(offset_indices+1);
			*(indexs+offset_ptr_indices+2)=(offset_indices+2);
			*(indexs+offset_ptr_indices+3)=(offset_indices+0);
			*(indexs+offset_ptr_indices+4)=(offset_indices+2);
			*(indexs+offset_ptr_indices+5)=(offset_indices+3);

			// configure 4 vertexes

			*(mesh_vertexs+offset_mesh+0)=tilemap_x3d;
			*(mesh_vertexs+offset_mesh+1)=tilemap_y3d-inc_tilemap_dydy3d;
			*(mesh_vertexs+offset_mesh+2)=0.0f;

			*(mesh_vertexs+offset_mesh+3 )=tilemap_x3d;
			*(mesh_vertexs+offset_mesh+4 )=tilemap_y3d;
			*(mesh_vertexs+offset_mesh+5 )=0.0f;

			*(mesh_vertexs+offset_mesh+6 )=tilemap_x3d+inc_tilemap_dxdx3d;
			*(mesh_vertexs+offset_mesh+7 )=tilemap_y3d;
			*(mesh_vertexs+offset_mesh+8 )=0.0f;

			*(mesh_vertexs+offset_mesh+9 )=tilemap_x3d+inc_tilemap_dxdx3d;
			*(mesh_vertexs+offset_mesh+10)=tilemap_y3d-inc_tilemap_dydy3d;
			*(mesh_vertexs+offset_mesh+11)=0.0f;

			tilemap_x3d+=inc_tilemap_dxdx3d;
			offset_mesh+=4*3;
			offset_ptr_indices+=6; // 6 per quad
			offset_indices+=4; // 3 per quad

		}

		tilemap_y3d-=inc_tilemap_dydy3d;
	}

	// mesh texture
	offset_mesh=0;
	short *tile_it=_tiles;
	for(int y = 0; y < (int)_height; y++)
	{
		for(int x = 0; x < (int)_width; x++,tile_it++) // new quad...
		{
			// +- pixel is to avoid render next pixel tile...
			short tile_xy = (*tile_it)-1;//*(_tiles+y*_width+x)-1;
			short tile_x=(tile_xy % tile_count_x);
			short tile_y=(tile_xy / tile_count_x);

			float u = (_tilesets->tile_margin+tile_x*(_tile_width+_tilesets->tile_spacing))*one_texel_over_width;
			float v = (_tilesets->tile_margin+tile_y*(_tile_height+_tilesets->tile_spacing))*one_texel_over_height;

			*(mesh_texture+offset_mesh+0 )=u;
			*(mesh_texture+offset_mesh+1 )=v+inc_tile_dydv;

			*(mesh_texture+offset_mesh+2 )=u;
			*(mesh_texture+offset_mesh+3 )=v;

			*(mesh_texture+offset_mesh+4 )=u+inc_tile_dxdu;
			*(mesh_texture+offset_mesh+5 )=v;

			*(mesh_texture+offset_mesh+6 )=u+inc_tile_dxdu;
			*(mesh_texture+offset_mesh+7 )=v+inc_tile_dydv;

			offset_mesh+=4*2;
		}
	}

	Geometry_SetIndices(geometry,indexs,n_indices);
	Geometry_SetMeshVertex(geometry,mesh_vertexs,mesh_vertexs_len);
	Geometry_SetMeshTexture(geometry,mesh_texture,mesh_texture_len);

	free(mesh_vertexs);
	free(mesh_texture);
	free(indexs);

	return tm;
}

void Tilemap_Update(Tilemap *_this){

	TilemapData *data=_this->data;

	if(data->tilesets != NULL){
		if(data->tilesets->animations){
			for(int j=0; j < data->tilesets->animations->count; j++){

				TileAnimation *tile_animation=data->tilesets->animations->items[j];

				if(tile_animation->time_change_frame < SDL_GetTicks()){ // change frame
					tile_animation->current_frame=(tile_animation->current_frame+1)%tile_animation->frames->count;
					TileAnimationFrame *frame=tile_animation->frames->items[tile_animation->current_frame];

					tile_animation->time_change_frame=SDL_GetTicks()+frame->duration;
					TileImage *tile_image=data->tilesets->tile_images[frame->tile_id];
					Texture_UpdateFromSurface(data->texture,tile_animation->u1,tile_animation->v1,tile_image->image);

				}
			}
		}
	}
}

void Tilemap_Draw(Tilemap *_this){
	TilemapData *data=_this->data;
	Texture_Bind(data->texture);
	Geometry_Draw(data->geometry);
}

void Tilemap_Delete(Tilemap *_this){
	TilemapData *data=_this->data;

	if(data->tilesets != NULL){
		if(data->tilesets->animations != NULL){
			for(int j=0; j < data->tilesets->animations->count; j++){
				TileAnimation *tile_animation=data->tilesets->animations->items[j];

				for(int k=0; k < tile_animation->frames->count; k++){
					TileAnimationFrame *frame=tile_animation->frames->items[k];
					FREE(frame);
				}
				List_Delete(tile_animation->frames);
				FREE(tile_animation);

			}
			List_Delete(data->tilesets->animations);
		}


		for(int j=0; j < data->tilesets->tile_count; j++){
			TileImage *tile_image=data->tilesets->tile_images[j];
			if(tile_image != NULL){
				SDL_FreeSurface(tile_image->image);
				FREE(tile_image);
			}


		}

		FREE(data->tilesets->tile_images);

		FREE(data->tilesets);
	}

	Geometry_Delete(data->geometry);

	free(data);
	free(_this);

}



