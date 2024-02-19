#include "@zg_graphics.h"

#include "ZG_Geometry_GL.c"

#define ZG_CIRCLE_DIVISIONS_PER_QUADRANT 128

static ZG_Geometry * g_geometry_default_point=NULL;
static ZG_Geometry * g_geometry_default_rectangle=NULL;
static ZG_Geometry * g_geometry_default_rectangle_filled=NULL;
static ZG_Geometry * g_geometry_default_rectangle_textured=NULL;
static ZG_Geometry * g_geometry_default_circle=NULL;

ZG_Geometry	* ZG_Geometry_GetDefaultPoint(void){
	if(g_geometry_default_point == NULL){
		short index=0;
		float pos[]={0,0,0};
		g_geometry_default_point=ZG_Geometry_NewPoints();
		ZG_Geometry_SetIndices(g_geometry_default_point,&index,1);
		ZG_Geometry_SetMeshVertex(g_geometry_default_point,pos,3);
	}

	return g_geometry_default_point;
}

ZG_Geometry	* ZG_Geometry_GetDefaultRectangle2d(void){
	if(g_geometry_default_rectangle == NULL){
		g_geometry_default_rectangle=ZG_Geometry_NewRectangle2d();
	}

	return g_geometry_default_rectangle;
}

ZG_Geometry	* 	ZG_Geometry_GetDefaultCircle(void){
	if(g_geometry_default_circle == NULL){
		g_geometry_default_circle=ZG_Geometry_NewCircle2d(0);
	}

	return g_geometry_default_circle;
}

ZG_Geometry	* 	ZG_Geometry_GetDefaultFilledRectangle2d(void){
	if(g_geometry_default_rectangle_filled == NULL){
		g_geometry_default_rectangle_filled=ZG_Geometry_NewFilledRectangle2d();
	}

	return g_geometry_default_rectangle_filled;
}

ZG_Geometry	* 	ZG_Geometry_GetDefaultTexturedRectangle2d(void){
	if(g_geometry_default_rectangle_textured == NULL){
		g_geometry_default_rectangle_textured=ZG_Geometry_NewTexturedRectangle2d();
	}

	return g_geometry_default_rectangle_textured;
}

ZG_Geometry	* ZG_Geometry_New(ZG_GeometryType _geometry_type){

	ZG_Geometry *geometry=ZG_NEW(ZG_Geometry);
	memset(geometry, 0,sizeof(ZG_Geometry));
	geometry->geometry_type=_geometry_type;
	geometry->indices_length=0;
	//geometry->n_vertexs=_n_vertexs;
	//geometry->indices_length=_index_length;
	//geometry->properties=_properties;

	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Geometry_GL_New(geometry);
		break;
	}

	return geometry;
}


ZG_Geometry	* ZG_Geometry_NewPoints(void){

	ZG_Geometry *geometry=NULL;

	geometry=ZG_Geometry_New(ZG_GEOMETRY_TYPE_POINTS);

	return geometry;
}

ZG_Geometry	* ZG_Geometry_NewRectangle2d(void){
	short indices[]={
		0,1,2,3	// Indices of 1st and 2nd triangle. 1st triangle uses the vertexs according indices 0,1,2. 2nd triangle shares ones of the first triangle (i.e 0,1,2) + the rest one (i.e 3)		0,1,2,3
	};

	// A quarter of screen as size...
	float mesh_vertex[]={
		   -0.5f,-0.5f,0.0f,  // bottom left
		   -0.5f,+0.5f,0.0f,  // top left
		   +0.5f,+0.5f,0.0f,  // top right
		   +0.5f,-0.5f,0.0f   // bottom right
	};

	ZG_Geometry *geometry=ZG_Geometry_New(ZG_GEOMETRY_TYPE_LINE_LOOP);

	if(geometry){ // setup indexes...

		ZG_Geometry_SetIndices(geometry,indices,ZG_ARRAY_SIZE(indices));

		ZG_Geometry_SetMeshVertex(geometry,mesh_vertex,ZG_ARRAY_SIZE(mesh_vertex));

	}

	return geometry;
}


ZG_Geometry	* ZG_Geometry_NewFilledRectangle2d(void){

	ZG_Geometry *geometry=NULL;

	short indices[]={
		0,1,2,3	// Indices of 1st and 2nd triangle. 1st triangle uses the vertexs according indices 0,1,2. 2nd triangle shares ones of the first triangle (i.e 0,1,2) + the rest one (i.e 3)
	};

	// A quarter of screen as size...
	float mesh_vertex[]={
		   -0.5f,-0.5f,0.0f,  // bottom left
		   -0.5f,+0.5f,0.0f,  // top left
		   +0.5f,-0.5f,0.0f,  // bottom right
		   +0.5f,+0.5f,0.0f   // top right
	};

	geometry=ZG_Geometry_New(ZG_GEOMETRY_TYPE_TRIANGLE_STRIP);

	if(geometry){ // setup indices...


		ZG_Geometry_SetIndices(geometry,indices,ZG_ARRAY_SIZE(indices));
		ZG_Geometry_SetMeshVertex(geometry,mesh_vertex,ZG_ARRAY_SIZE(mesh_vertex));

	}

	return geometry;
}

ZG_Geometry	* ZG_Geometry_NewTexturedRectangle2d(void){

	ZG_Geometry *geometry=NULL;

	short indices[]={
		0,1,2,3	// Indices of 1st and 2nd triangle. 1st triangle uses the vertexs according indices 0,1,2. 2nd triangle shares ones of the first triangle (i.e 0,1,2) + the rest one (i.e 3)
	};

	// A quarter of screen as size...
	float mesh_vertex[]={
		   -0.5f,-0.5f,0.0f,  // bottom left
		   -0.5f,+0.5f,0.0f,  // top left
		   +0.5f,-0.5f,0.0f,  // bottom right
		   +0.5f,+0.5f,0.0f   // top right
	};

	geometry=ZG_Geometry_New(ZG_GEOMETRY_TYPE_TRIANGLE_STRIP);

	if(geometry){ // setup indices...

		ZG_Geometry_SetIndices(geometry,indices,ZG_ARRAY_SIZE(indices));

		ZG_Geometry_SetMeshVertex(geometry,mesh_vertex,ZG_ARRAY_SIZE(mesh_vertex));

		float mesh_texture[]={
			   0.0f,  0.0f,   // bottom left
			   0.0f,  1.0f,   // top left
			   1.0f,  0.0f,   // bottom right
			   1.0f,  1.0f    // top right
		};

		ZG_Geometry_SetMeshTexture(geometry,mesh_texture,ZG_ARRAY_SIZE(mesh_texture));

	}

	return geometry;
}

ZG_Geometry	* ZG_Geometry_NewCircle2d(uint16_t _divisions_per_quadrant){

	if(_divisions_per_quadrant < 1){
		// default circle divisions per quadrant
		_divisions_per_quadrant=ZG_CIRCLE_DIVISIONS_PER_QUADRANT;
	}

	ZG_Geometry *geometry=NULL;
	uint16_t n_vertexs = 4*_divisions_per_quadrant;
	size_t indices_length=n_vertexs;

	short *indices=malloc(indices_length*sizeof(short));

	// A quarter of screen as size...
	float *mesh_vertex=malloc(n_vertexs*sizeof(float)*ZG_VERTEX_COORDS_LEN);
	float *it_vertexs=mesh_vertex;
	short *it_indexs=indices;

	float inc_r=2*PI/(float)n_vertexs;
	int index=0;

    for (float r = 0; r < 2*PI; r+=inc_r,it_vertexs+=3,it_indexs++)
    {
    	*(it_vertexs+0)=cos(r)*0.5;
    	*(it_vertexs+1)=sin(r)*0.5;
    	*(it_vertexs+2)=0;

    	*(it_indexs+0)=index+0;
    	//*(it_indexs+1)=index+1;

    	index++;
    }

	geometry=ZG_Geometry_New(ZG_GEOMETRY_TYPE_LINE_LOOP);

	if(geometry){ // setup indexes...

		ZG_Geometry_SetIndices(geometry,indices,indices_length);

		ZG_Geometry_SetMeshVertex(geometry,mesh_vertex,n_vertexs*ZG_VERTEX_COORDS_LEN);

	}

	// we don't need anymore
	free(indices);
	free(mesh_vertex);

	return geometry;
}

void 			ZG_Geometry_SetIndices(ZG_Geometry *_this,const short *_indices,size_t _indices_len){

	if(_this == NULL) return;

	if(_indices_len < 2 && _this->geometry_type != ZG_GEOMETRY_TYPE_POINTS){
		ZG_LOG_ERRORF("ZG_Geometry_SetIndices : Number of indices should be greather equal than 3");
		return;
	}
	
	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Geometry_GL_SetIndices(_this,_indices,_indices_len);
		break;
	}
	
	_this->indices_length=_indices_len;
}

void 			ZG_Geometry_SetMeshVertex(ZG_Geometry *_this,const float *_vertexs,size_t _vertexs_len){

	if(_this == NULL) return;

	if(_vertexs_len < 2 && _this->geometry_type != ZG_GEOMETRY_TYPE_POINTS){
		ZG_LOG_ERRORF("ZG_Geometry_SetMeshVertex : Number of vertex should be greather than 2");
		return;
	}

	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Geometry_GL_SetMeshVertex(_this,_vertexs,_vertexs_len);
		break;
	}
}

void 			ZG_Geometry_SetMeshTexture(ZG_Geometry *_this,const float *_vertexs,size_t _vertexs_len){

	if(_this == NULL) return;

	if(_vertexs_len < 2 && _this->geometry_type != ZG_GEOMETRY_TYPE_POINTS){
		ZG_LOG_ERRORF("ZG_Geometry_SetMeshTexture : Number of vertex should be greather than 2");
		return;
	}

	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Geometry_GL_SetMeshTexture(_this,_vertexs,_vertexs_len);
		break;
	}
}

void 			ZG_Geometry_SetMeshColor(ZG_Geometry *_this,const float *_vertexs,size_t _vertexs_len){

	if(_this == NULL) return;

	if(_vertexs_len < 2 && _this->geometry_type != ZG_GEOMETRY_TYPE_POINTS){
		ZG_LOG_ERRORF("ZG_Geometry_SetMeshColor : Number of vertex should be greather than 2");
		return;
	}

	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Geometry_GL_SetMeshColor(_this,_vertexs,_vertexs_len);
		break;
	}
}

void 			ZG_Geometry_SetMeshNormal(ZG_Geometry *_this,const float *_vertexs,size_t _vertexs_len){

	if(_this == NULL) return;

	if(_vertexs_len < 2 && _this->geometry_type != ZG_GEOMETRY_TYPE_POINTS){
		ZG_LOG_ERRORF("ZG_Geometry_SetMeshNormal : Number of vertex should be greather than 2");
		return;
	}

	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Geometry_GL_SetMeshNormal(_this,_vertexs,_vertexs_len);
		break;
	}
}

void 			ZG_Geometry_Draw(ZG_Geometry *geometry){
	if(geometry == NULL) return;

	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Geometry_GL_Draw(geometry);
		break;
	}
}

void	ZG_Geometry_Delete(ZG_Geometry *_this){
	if(_this == NULL) return;

	switch(ZG_Graphics_GetGraphicsApi()){
	default:

		break;
	case ZG_GRAPHICS_API_GL:
		ZG_Geometry_GL_DeInit(_this);
		break;
	}

	ZG_FREE(_this);

}

void			ZG_Geometry_DeInit(void){

	ZG_Geometry **default_geometries[]={
			&g_geometry_default_point
			,&g_geometry_default_rectangle
			,&g_geometry_default_rectangle_filled
			,&g_geometry_default_rectangle_textured
			,&g_geometry_default_circle
			,NULL
	};

	ZG_Geometry ***it=default_geometries;

	while(*it!=NULL){
		if(**it != NULL){
			ZG_Geometry_Delete(**it);
		}
		**it=NULL;

		it++;
	}

}
