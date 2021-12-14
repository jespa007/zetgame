#include "zg_graphics.h"

#include "Geometry_GL.c"

#define DEFAULT_CIRCLE_DIVISIONS_PER_QUADRANT 128

static Geometry * g_geometry_default_point=NULL;
static Geometry * g_geometry_default_rectangle=NULL;
static Geometry * g_geometry_default_rectangle_filled=NULL;
static Geometry * g_geometry_default_rectangle_textured=NULL;
static Geometry * g_geometry_default_circle=NULL;

Geometry	* Geometry_GetDefaultPoint(void){
	if(g_geometry_default_point == NULL){
		short index=0;
		float pos[]={0,0,0};
		g_geometry_default_point=Geometry_NewPoints(1,0);
		Geometry_SetIndices(g_geometry_default_point,&index,1);
		Geometry_SetMeshVertex(g_geometry_default_point,pos,3);
	}

	return g_geometry_default_point;
}

Geometry	* Geometry_GetDefaultRectangle(void){
	if(g_geometry_default_rectangle == NULL){
		g_geometry_default_rectangle=Geometry_NewRectangle(0);
	}

	return g_geometry_default_rectangle;
}

Geometry	* 	Geometry_GetDefaultCircle(void){
	if(g_geometry_default_circle == NULL){
		g_geometry_default_circle=Geometry_NewCircle(0,0);
	}

	return g_geometry_default_circle;
}

Geometry	* 	Geometry_GetDefaultRectangleFilled(void){
	if(g_geometry_default_rectangle_filled == NULL){
		g_geometry_default_rectangle_filled=Geometry_NewRectangleFilled(GEOMETRY_PROPERTY_COLOR);
	}

	return g_geometry_default_rectangle_filled;
}

Geometry	* 	Geometry_GetDefaultRectangleTextured(void){
	if(g_geometry_default_rectangle_textured == NULL){
		g_geometry_default_rectangle_textured=Geometry_NewRectangleFilled(GEOMETRY_PROPERTY_TEXTURE);
	}

	return g_geometry_default_rectangle_textured;
}

Geometry	* Geometry_New(GeometryType _geometry_type,size_t _index_length,size_t _n_vertexs,uint32_t _properties){

	if(_n_vertexs < 2 && _geometry_type != GEOMETRY_TYPE_POINTS){
		Log_Error("Number of vertex should be greather than 2");
		return NULL;
	}

	Geometry *geometry=NEW(Geometry);
	memset(geometry, 0,sizeof(Geometry));
	geometry->geometry_type=_geometry_type;
	geometry->n_vertexs=_n_vertexs;
	geometry->index_length=_index_length;
	geometry->properties=_properties;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_New(geometry,_properties);
		break;
	}

	return geometry;
}


Geometry	* Geometry_NewPoints(size_t _n_points, uint32_t _properties){

	Geometry *geometry=NULL;

	geometry=Geometry_New(GEOMETRY_TYPE_POINTS,_n_points,_n_points,_properties);

	return geometry;
}

Geometry	* Geometry_NewRectangle(uint32_t _properties){

	Geometry *geometry=NULL;

	short indices[]={
			 0,1
			 ,2,3
	};

	// A quarter of screen as size...
	float mesh_vertex[]={
		   -0.5f,-0.5f,0.0f,   // bottom left
		   -0.5f,+0.5f,0.0f,   // top left
		   +0.5f,+0.5f,0.0f,   // top right
		   +0.5f,-0.5f,0.0f    // bottom right
	};


	geometry=Geometry_New(GEOMETRY_TYPE_LINE_LOOP,ARRAY_SIZE(indices),N_VERTEX_QUAD,_properties);

	if(geometry){ // setup indexes...

		Geometry_SetIndices(geometry,indices,ARRAY_SIZE(indices));

		Geometry_SetMeshVertex(geometry,mesh_vertex,ARRAY_SIZE(mesh_vertex));

	}

	return geometry;
}

Geometry	* Geometry_NewRectangleFilled(uint32_t _properties){

	Geometry *geometry=NULL;

	short indices[]={
		0,1
		,2,3
	};

	// A quarter of screen as size...
	float mesh_vertex[]={
		   -0.5f,-0.5f,0.0f,  // bottom left
		   +0.5f,-0.5f,0.0f,  // bottom right
		   -0.5f,+0.5f,0.0f,  // top left
		   +0.5f,+0.5f,0.0f   // top right
	};

	geometry=Geometry_New(GEOMETRY_TYPE_TRIANGLE_STRIP,ARRAY_SIZE(indices),N_VERTEX_QUAD,_properties);

	if(geometry){ // setup indices...


		Geometry_SetIndices(geometry,indices,ARRAY_SIZE(indices));

		Geometry_SetMeshVertex(geometry,mesh_vertex,ARRAY_SIZE(mesh_vertex));

		if(_properties & GEOMETRY_PROPERTY_TEXTURE){

			float mesh_texture[]={
				   0.0f,  1.0f,   // bottom left
				   1.0f,  1.0f,   // bottom right
				   0.0f,  0.0f,   // top left
				   1.0f,  0.0f    // top right
			};

			Geometry_SetMeshTexture(geometry,mesh_texture,ARRAY_SIZE(mesh_texture));
		}
	}

	return geometry;
}

Geometry	* Geometry_NewCircle(uint16_t _divisions_per_quadrant, uint32_t _properties){

	if(_divisions_per_quadrant < 1){
		_divisions_per_quadrant=DEFAULT_CIRCLE_DIVISIONS_PER_QUADRANT;
	}

	Geometry *geometry=NULL;
	uint16_t n_vertexs = 4*_divisions_per_quadrant;
	size_t index_length=n_vertexs;

	short *indices=malloc(index_length*sizeof(short));

	// A quarter of screen as size...
	float *mesh_vertex=malloc(n_vertexs*sizeof(float)*VERTEX_COORDS_LEN);
	float *it_vertexs=mesh_vertex;
	short *it_indexs=indices;

	float inc_r=2*PI/(float)n_vertexs;
	int index=0;

    for (float r = 0; r < 2*PI; r+=inc_r,it_vertexs+=3,it_indexs++)
    {
    	*(it_vertexs+0)=cos(r);
    	*(it_vertexs+1)=sin(r);
    	*(it_vertexs+2)=0;

    	*(it_indexs+0)=index+0;
    	//*(it_indexs+1)=index+1;

    	index++;
    }

	geometry=Geometry_New(GEOMETRY_TYPE_LINE_LOOP,index_length,n_vertexs,_properties);

	if(geometry){ // setup indexes...

		Geometry_SetIndices(geometry,indices,index_length);

		Geometry_SetMeshVertex(geometry,mesh_vertex,n_vertexs*VERTEX_COORDS_LEN);

	}

	// we don't need anymore
	free(indices);
	free(mesh_vertex);

	return geometry;
}

void 			Geometry_SetIndices(Geometry *geometry,const short *indices,size_t indices_len){

	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetIndices(geometry,indices,indices_len);
		break;
	}
}

void 			Geometry_SetMeshVertex(Geometry *geometry,const float *mesh_vertexs,size_t mesh_vertexs_len){

	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetMeshVertex(geometry,mesh_vertexs,mesh_vertexs_len);
		break;
	}
}

void 			Geometry_SetMeshTexture(Geometry *geometry,const float *mesh_texure_vertexs,size_t mesh_texture_vertexs_len){

	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetMeshTexture(geometry,mesh_texure_vertexs,mesh_texture_vertexs_len);
		break;
	}
}

void 			Geometry_SetMeshColor(Geometry *geometry,const float *mesh_color_vertexs,size_t mesh_color_vertexs_len){

	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetMeshColor(geometry,mesh_color_vertexs,mesh_color_vertexs_len);
		break;
	}
}

void 			Geometry_SetMeshNormal(Geometry *geometry,const float *mesh_normal_vertexs,size_t mesh_normal_vertexs_len){
	if(geometry == NULL) return;

	switch(Graphics_GetGraphicsApi()){
	default:

		break;
	case GRAPHICS_API_GL:
		Geometry_GL_SetMeshNormal(geometry,mesh_normal_vertexs,mesh_normal_vertexs_len);
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

	Geometry **default_geometries[]={
			&g_geometry_default_point
			,&g_geometry_default_rectangle
			,&g_geometry_default_rectangle_filled
			,&g_geometry_default_rectangle_textured
			,&g_geometry_default_circle
			,NULL
	};

	Geometry ***it=default_geometries;

	while(*it!=NULL){
		if(**it != NULL){
			Geometry_Delete(**it);
		}
		**it=NULL;

		it++;
	}

}
