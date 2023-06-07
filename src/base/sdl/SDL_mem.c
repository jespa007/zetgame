
#ifdef __MEMMANAGER__

#undef  	SDL_CreateRGBSurface
#undef		SDL_CreateRGBSurfaceFrom
#undef		SDL_ConvertSurface
#undef		SDL_FreeSurface

#define MAX_CHECK_SURFACES	1000
#define KEY_NOT_FOUND		-1

typedef  struct{
	uintptr_t pointer; //this is the element to  be ordered.
	const char *file;
	int line;
}SDL_SurfacePointerTrackInfo;


static bool g_sdl_mem_init=false;
static SDL_SurfacePointerTrackInfo g_allocated_surfaces[MAX_CHECK_SURFACES];
static int g_n_allocated_surfaces=0;

//----------------------------------------------------------------------------------------------------------------------------
void  MEMSDL_Init()
{
	if(g_sdl_mem_init) return;

	memset(&g_allocated_surfaces,0,sizeof(g_allocated_surfaces));
    ZG_Log_InfoF("SDLMEM init OK");
    atexit(MEMSDL_PrintStatus);

    g_sdl_mem_init=true;
}

void  MEMSDL_PrintStatus()
{
	int n_unllocated=0;
	// check unllocated srf
	for(unsigned i = 0; i < MAX_CHECK_SURFACES; i++){
		SDL_SurfacePointerTrackInfo *pti=&g_allocated_surfaces[i];
		if(pti->pointer != 0){
			//ZG_Log_Print(const  char  *file,  int  line, LogLevel level, bool with_cr, const  char  *string_text,...);
			ZG_Log_Print(pti->file,pti->line,ZG_LOG_LEVEL_WARNING," SDL surface not deallocated");
			n_unllocated++;
		}
	}

	if(n_unllocated==0){
		ZG_Log_InfoF("SDLMem: ok");
	}

}


//--------------------------------------------------------------------------------------------
// DICOTOMIC

int SDLMEM_dicotomic_search(uintptr_t key)
{
	int idx_min=0;
	int idx_max=g_n_allocated_surfaces-1;
	// continue searching while [imin,imax] is not empty
	while (idx_max >= idx_min)
	{
		// calculate the midpoint for roughly equal partition
		int idx_mid = (idx_min + idx_max ) >> 1;
		if(g_allocated_surfaces[idx_mid].pointer == key){
			// key found at index idx_mid
			return idx_mid;
		// determine which subarray to search
		}else if (g_allocated_surfaces[idx_mid].pointer < key){
			// change min index to search upper subarray
			idx_min = idx_mid + 1;
		}else{
			// change max index to search lower subarray
			idx_max = idx_mid - 1;
		}
	}
	// key was not found
	return KEY_NOT_FOUND;
}

bool SDLMEM_dicotomic_insert(const char * file, int line, uintptr_t key)
{
	if(g_n_allocated_surfaces==(MAX_CHECK_SURFACES-2)){ // array full
		ZG_Log_ErrorF("DS Error full table");
		return false;
	}

	// PRE: array is already ordered
	int size=g_n_allocated_surfaces;
	int idx_min = 0, idx_max = g_n_allocated_surfaces - 1;

	if(size > 0){

		if (key > g_allocated_surfaces[idx_max].pointer){
		  idx_min=size;
		}
		else{
		  while (idx_max > idx_min) {
			int idx_mid = (idx_min + idx_max) >> 1;
			if (g_allocated_surfaces[idx_mid].pointer > key) {
				idx_max = idx_mid;
			}
			else{
				idx_min = idx_mid + 1;
			}
		  }
		}
	}

	if(idx_min >= 0){

		if(idx_min < size){
			for (int i = g_n_allocated_surfaces-1; i >= idx_min; --i){
				g_allocated_surfaces[i+1] = g_allocated_surfaces[i];
			}
		}
		g_allocated_surfaces[idx_min].pointer = key;
		g_allocated_surfaces[idx_min].file = file;
		g_allocated_surfaces[idx_min].line = line;

		/*for(int i=0; i < (n_allocated_pointers-1);i++){
			if(	ds_pointer_array[i].pointer > ds_pointer_array[i+1].pointer){ // mierda
				int k=0;
				k++;
				LOG_ERROR("Cannot ");
			}
		}*/

		g_n_allocated_surfaces++;
		return true;
	}else{
		ZG_Log_ErrorF("Cannot insert SDL pointer");
	}
	return false;
}

bool SDLMEM_dicotomic_delete(uintptr_t key)
{
	if(g_n_allocated_surfaces==0){
		ZG_Log_ErrorF("DS Error empty table");
		return false;
	}

	int pos=SDLMEM_dicotomic_search(key);
	//printf("(pos:%i)",pos);
	if(pos != KEY_NOT_FOUND){
		//memcpy(&g_allocated_surfaces[pos],&g_allocated_surfaces[pos+1],(g_n_allocated_surfaces-pos)*sizeof(SDL_SurfacePointerTrackInfo));
		for (int i = pos; i < g_n_allocated_surfaces; i++){
			g_allocated_surfaces[i] = g_allocated_surfaces[i+1];
		}
		g_allocated_surfaces[g_n_allocated_surfaces-1].pointer=0;
		g_allocated_surfaces[g_n_allocated_surfaces-1].file=0;
		g_allocated_surfaces[g_n_allocated_surfaces-1].line=0;
		g_n_allocated_surfaces--;
		return true;
	}
	return false;

}


//----------------------------------------------------------------------------------------------------------------------------
//SDL_CreateRGBSurface
SDL_Surface		*__sdlmem__SDL_CreateRGBSurface(const  char  *file,  int  line, uint32_t  flags,  int  width,  int  height,  int  depth,  uint32_t  Rmask,  uint32_t  Gmask,  uint32_t  Bmask,  uint32_t  Amask)
{
	MEMSDL_Init();

	SDL_Surface *srf= SDL_CreateRGBSurface( flags,   width,   height,   depth,   Rmask,   Gmask,   Bmask,   Amask);
	SDLMEM_dicotomic_insert(file,line,(uintptr_t)srf);
	return srf;
}
//----------------------------------------------------------------------------------------------------------------------------
SDL_Surface		*__sdlmem__SDL_CreateRGBSurfaceFrom(const  char  *file,  int  line,  void  *pixels,  int  width,  int  height,  int  depth,  int  pitch,  uint32_t  Rmask,  uint32_t  Gmask,  uint32_t  Bmask,  uint32_t  Amask)
{
	MEMSDL_Init();

	SDL_Surface *srf=SDL_CreateRGBSurfaceFrom(pixels,   width,   height,   depth,   pitch,   Rmask,   Gmask,   Bmask,   Amask);
	SDLMEM_dicotomic_insert(file,line,(uintptr_t)srf);
	return srf;

}


SDL_Surface		*__sdlmem__SDL_ConvertSurface(const  char  *file,  int  line,  SDL_Surface  *src,  SDL_PixelFormat  *fmt,  uint32_t  flags)
{
	MEMSDL_Init();

	SDL_Surface *srf=SDL_ConvertSurface(src,  fmt,  flags);
	SDLMEM_dicotomic_insert(file,line,(uintptr_t)srf);
	return  srf;
}

void	 __sdlmem__SDL_FreeSurface(const  char  *file,  int  line, void  *srf)
{
	MEMSDL_Init();
	if(SDLMEM_dicotomic_delete((uintptr_t)srf)){
		SDL_FreeSurface(srf);
	}
	else{
		ZG_Log_Print(file,line,ZG_LOG_LEVEL_WARNING," pointer was not allocated by SDL");
	}

}

#endif
