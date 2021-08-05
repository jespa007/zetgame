#include "zg_graphics.h"

#include "Geometry_GL.c"

static Geometry * g_geometry_default=NULL;

Geometry	* Geometry_Default(void){
	if(g_geometry_default == NULL){
		g_geometry_default=Geometry_NewQuad(GEOMETRY_TEXTURE);
	}

	return g_geometry_default;
}

Geometry	* Geometry_New(size_t n_vertexs,uint32_t properties){

	if(n_vertexs < 2){
		Log_Error("Number of vertex should be greather than 2");
		return NULL;
	}

	Geometry *geometry=NEW(Geometry);
	memset(geometry, 0,sizeof(Geometry));
	geometry->n_vertexs=n_vertexs;
	geometry->properties=properties;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_New(geometry,n_vertexs, properties);
		break;
	}


	return geometry;
}

Geometry	* Geometry_NewQuad(uint32_t properties){

	Geometry *geometry=NULL;

	uint16_t indices[N_INDICES(N_VERTEX_QUAD)]={
			0,1,2,
			0,2,3
	};

	// A quarter of screen as size...
	float mesh_vertex[N_VERTEX_QUAD*VERTEX_COORDS_LEN]={
		   -0.25f,-0.25f,0.0f,   // bottom left
		   -0.25f,+0.25f,0.0f,   // top left
		   +0.25f,+0.25f,0.0f,   // top right
		   +0.25f,-0.25f,0.0f    // bottom right
	};
	float mesh_texture_coords[N_VERTEX_QUAD*TEXTURE_COORDS_LEN]={
		   0.0f,  1.0f,   // bottom left
		   0.0f,  0.0f,   // top left
		   1.0f,  0.0f,   // top right
		   1.0f,  1.0f    // bottom right
	};


	geometry=Geometry_New(N_VERTEX_QUAD,properties);

	if(geometry){ // setup indexes...

		Geometry_SetIndices(geometry,(BufferShort){
			.data=indices
			,.len=N_INDICES(N_VERTEX_QUAD)
		});

		Geometry_SetMeshVertex(geometry,(BufferFloat){
			.data=mesh_vertex
			,.len=N_VERTEX_QUAD*VERTEX_COORDS_LEN
		});

		if(properties & GEOMETRY_TEXTURE){
			Geometry_SetMeshTexture(geometry,(BufferFloat){
				.data=mesh_texture_coords
				,.len=N_VERTEX_QUAD*TEXTURE_COORDS_LEN
			});
		}
	}

	return geometry;
}

void 			Geometry_SetIndices(Geometry *geometry,BufferShort indices){

	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetIndices(geometry,indices);
		break;
	}
}

void 			Geometry_SetMeshVertex(Geometry *geometry,BufferFloat mesh_vertex){

	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetMeshVertex(geometry,mesh_vertex);
		break;
	}
}

void 			Geometry_SetMeshTexture(Geometry *geometry,BufferFloat mesh_texture){

	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetMeshTexture(geometry,mesh_texture);
		break;
	}
}

void 			Geometry_SetMeshColor(Geometry *geometry,BufferFloat mesh_color){

	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetMeshColor(geometry,mesh_color);
		break;
	}
}

void 			Geometry_SetMeshNormal(Geometry *geometry,BufferFloat mesh_normal){
	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetMeshNormal(geometry,mesh_normal);
		break;
	}
}

void 			Geometry_Draw(Geometry *geometry){
	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_Draw(geometry);
		break;
	}
}

void	Geometry_Delete(Geometry *_this){
	if(_this == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_DeInit(_this);
		break;
	}

	FREE(_this);

}

void			Geometry_DeInit(void){
	if(g_geometry_default != NULL){
		Geometry_Delete(g_geometry_default);
	}

	g_geometry_default=NULL;
}
