#include "zg_tilemap.h"


typedef struct{
	MapString 			* 	tilemaps;	// it saves its layers
	TextureManager   	*   texture_manager;

}TilemapManagerData;





void TilemapManager_OnDeleteTilemap(MapStringNode *node){
	Tilemap *tm=(Tilemap *)node->val;

	if(tm != NULL){
		Tilemap_Delete(tm);
	}


}
void TilemapManager_OnDeleteTexture(MapStringNode *node){
	Texture *texture=(Texture *)node->val;

	if(texture != NULL){
		Texture_Delete(texture);
	}

}


// MEMBERS
TilemapManager *TilemapManager_New(TextureManager	* _texture_manager){
	TilemapManager *tmm=NEW(TilemapManager);
	TilemapManagerData *data=NEW(TilemapManagerData);

	data->tilemaps = MapString_New();//new std::map<std::string,TTFont *>();
	data->texture_manager = _texture_manager;

	data->tilemaps->on_delete=TilemapManager_OnDeleteTilemap;

	tmm->data=data;

	return tmm;
}

bool TilemapManager_LoadFromMemory(
		TilemapManager *_this
		,const char *_path
		,uint8_t *_json_buf
		,size_t _json_buf_len
	){

	short *tiles=NULL;
	char filename[MAX_PATH]={0};
	bool ok=false;

	Texture *texture;
	TilemapManagerData *data=_this->data;

	// first read tilesets...
	cJSON * root = cJSON_ParseWithLength((char *)_json_buf,_json_buf_len);
	cJSON *layers,*layer,*width,*height,*tilesets,*tileset;
	cJSON *tile,*tilemap_data,*layer_name,*tilemap_width,*tilemap_height,*x,*y;
	cJSON *firstgid,*image,*margin,*tilecount,*tilewidth,*tileheight,*tilesets_tiles;
	List *tile_animations=NULL;

	cJSONAttribute tilemap_attr[]={
			 {"layers",&layers}
			,{"tilewidth",&tilewidth}
			,{"tileheight",&tileheight}
			,{"width",&width}
			,{"height",&height}
			,{"tilesets",&tilesets}
	};

	cJSONAttribute layer_attr[]={
			{"data",&tilemap_data}
			,{"name",&layer_name}
			,{"width",&tilemap_width}
			,{"height",&tilemap_height}

			,{"x",&x}
			,{"y",&y}
	};

	cJSONAttribute tileset_attr[]={
			{"firstgid",&firstgid}
			,{"image",&image}
			,{"margin",&margin}
			,{"tilecount",&tilecount}
			,{"tilewidth",&tilewidth}
			,{"tileheight",&tileheight}
			,{"tiles",&tilesets_tiles}
	};

	for(unsigned i=0; i < ARRAY_SIZE(tilemap_attr); i++){
		if((*tilemap_attr[i].value = cJSON_GetObjectItem(root,tilemap_attr[i].name))== NULL){
			Log_Error("JsonParse: Cannot get '%s'",tilemap_attr[i].name);
			return false;
		}
	}

	size_t tileset_data_len=cJSON_GetArraySize(tilesets);

	if(tileset_data_len == 0){
		Log_Error("JsonParse: there's no tilesets");
		return false;
	}

	cJSON_ArrayForEach(layer, layers) {

		bool tileset_found=false;

		for(unsigned i=0; i < ARRAY_SIZE(tilemap_attr); i++){
			if((*layer_attr[i].value = cJSON_GetObjectItem(layer,layer_attr[i].name)) == NULL){
				Log_Error("JsonParse: Cannot get '%s' in 'layer'",layer_attr[i].name);
				return false;
			}
		}

		if(MapString_GetValue(data->tilemaps,layer_name->valuestring,NULL)!=NULL){
			Log_Warning("JsonParse: layer '%s' already added in manager",layer_name->valuestring);
			continue;
		}

		size_t tilemap_data_len=cJSON_GetArraySize(tilemap_data);
		size_t tilemap_dim= tilemap_width->valueint*tilemap_height->valueint;

		if(tilemap_data_len == 0){
			Log_Warning("JsonParse: empty tilemap");
			continue;
		}

		if(tilemap_data_len != tilemap_dim){
			Log_Error("JsonParse: tilemap_data length doesn't match tilemap_width * tilemap_height (%i != %i)",tilemap_data_len,tilemap_dim);
			return false;
		}

		tile=cJSON_GetArrayItem(tilemap_data,0);

		// search tileset...
		cJSON_ArrayForEach(tileset, tilesets) {
			for(unsigned i=0; i < ARRAY_SIZE(tileset_attr); i++){
				if((*tileset_attr[i].value = cJSON_GetObjectItem(tileset,tileset_attr[i].name))== NULL){
					Log_Error("JsonParse: Cannot get '%s' attribute in 'tilesets'",tileset_attr[i].name);
					return false;
				}
			}

			if(firstgid->valueint <= tile->valueint && (tile->valueint < (tilecount->valueint))){
				tileset_found=true;
				break;
			}

		}

		if(tileset_found == false){
			Log_Error("JsonParse layer '%s': tileset not found",layer_name->valuestring);
			return false;
		}

		if(tileset_found == false){
			Log_Error("JsonParse layer '%s': firstgid >= tilecount (%i >= %i)'",layer_name->valuestring,firstgid->valueint,tilecount->valueint);
			return false;
		}


		tiles=malloc(sizeof(short)*tilemap_data_len);
		int idx_tile=0;


		// load the array of tile array
		cJSON_ArrayForEach(tile, tilemap_data) {
			short idx_tile_block=tile->valueint;
			// check tile and
			if(firstgid->valueint <= idx_tile_block && idx_tile_block <= tilecount->valueint){
				*(tiles+idx_tile)=idx_tile_block;
			}else{
				Log_Error("JsonParse data layer '%s': tile at position %i out of bounds (min:%i max:%i)'"
						,layer_name->valuestring
						,idx_tile_block
						,firstgid->valueint
						,tilecount->valueint);
				goto tmm_load_error;
			}

			idx_tile++;
		}

		sprintf(filename,"%s/%s",_path,image->valuestring);
		if((texture=TextureManager_Get(data->texture_manager,filename)) == NULL){
			goto tmm_load_error;
		}

		// check for animations...
		size_t tilesets_tiles_len=cJSON_GetArraySize(tilesets_tiles);
		if(tilesets_tiles_len > 0){ // setup frames...
			cJSON *tilesets_tile=NULL;
			cJSON *tilesets_tile_animations=NULL;
			cJSON *tilesets_tile_animation=NULL;
			cJSON *tilesets_tile_animation_tileid=NULL;
			cJSON *tilesets_tile_animation_duration=NULL;

			// load image ...
			SDL_Surface *image=SDL_LoadImageFromFile(filename,0,0);

			if(image == NULL){
				goto tmm_load_error;
			}


			cJSON_ArrayForEach(tilesets_tile, tilesets_tiles) {

				if((tilesets_tile_animations = cJSON_GetObjectItem(tilesets_tile,"animation")) != NULL){

					if(tile_animations == NULL){
						tile_animations=List_New();
					}

					TileAnimation *tile_animation=NEW(TileAnimation);
					tile_animation->frames=List_New();

					List_Add(tile_animations,tile_animation);

					size_t tilesets_tiles_animation_len=cJSON_GetArraySize(tilesets_tile_animations);
					// resrve
					if(tilesets_tiles_animation_len > 0){

						cJSON_ArrayForEach(tilesets_tile_animation, tilesets_tile_animations) {
							if((tilesets_tile_animation_duration = cJSON_GetObjectItem(tilesets_tile_animation,"duration")) == NULL){
								Log_Error("JsonParse data tilesets->animation->duration not found");
								continue;
							}

							if((tilesets_tile_animation_tileid = cJSON_GetObjectItem(tilesets_tile_animation,"tileid")) == NULL){
								Log_Error("JsonParse data tilesets->animation->tileid not found");
								continue;
							}

							TileAnimationFrame *tileset_animation_frame=NEW(TileAnimationFrame);

							tileset_animation_frame->duration=tilesets_tile_animation_duration->valueint;
							tileset_animation_frame->tile_id=tilesets_tile_animation_tileid->valueint;

							// get offset uv
							int tile_y=tileset_animation_frame->tile_id/tilemap_width->valueint;
							int tile_x=tileset_animation_frame->tile_id%tilemap_width->valueint;

							tileset_animation_frame->image=SDL_Crop(image,(SDL_Rect){tile_x,tile_y,tilemap_width->valueint,tilemap_height->valueint});

							Log_Info("Loaded tile: %i duration: %i OK",tileset_animation_frame->tile_id,tileset_animation_frame->duration);

							List_Add(tile_animation->frames,tileset_animation_frame);


						}
					}
				}
			}

			SDL_FreeSurface(image);
		}

		// load texture...
		Tilemap *tm=Tilemap_New(
				tiles
				,tilemap_width->valueint
				, tilemap_height->valueint
				, tilewidth->valueint
				, tileheight->valueint
				, texture
				,tile_animations);

		if(tm != NULL){
			MapString_SetValue(data->tilemaps,layer_name->valuestring,tm);
			ok=true;
		}
	}


tmm_load_error:

	if(tiles) free(tiles);

	return ok;

}

/**
 * Load a set of Tilemap from exported files from Aseprite
 * @_this: TilemapManager object
 * @_texture_filename: Texture filename (it can be .png,jpg,etc)
 * @_json_filename: Json file generated by Aseprite
 * @_extra_json_filename: Json file where it adds some extra information per frame (for instance collider)
 */
bool TilemapManager_Load(TilemapManager *_this,const char *_json_tmx_file){

	BufferByte *_json_buf=NULL;bool ok=false;
	char *_path=NULL;


	if((_json_buf=File_Read(_json_tmx_file))!=NULL){
		ok=TilemapManager_LoadFromMemory(
				_this
				,_path=Path_GetDirectoryName(_json_tmx_file)
				,_json_buf->ptr
				,_json_buf->len
		);
	}


	if(_json_buf) BufferByte_Delete(_json_buf);
	if(_path) free(_path);


	return ok;
}

Tilemap *TilemapManager_GetTilemap(TilemapManager *_this, const char *key){
	TilemapManagerData *data=_this->data;

	return MapString_GetValue(data->tilemaps,key,NULL);
}



void  TilemapManager_Delete(TilemapManager *_this){
	TilemapManagerData 	*data=_this->data;

	MapString_Delete(data->tilemaps);
	//MapString_Delete(data->textures);

	FREE(data);
	FREE(_this);
}
