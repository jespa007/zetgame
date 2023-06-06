#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

// Meshes are constrained by these values
#define VERTEX_COORDS_LEN 	3	// XYZ
#define TEXTURE_COORDS_LEN 	2	// UV
#define COLOR_COORDS_LEN 	3	// RGB
#define NORMAL_COORDS_LEN 	3	// XYZ

#define N_TRIANGLES_FROM_N_VERTEXS(v)	((v)-2)
#define N_INDICES_TRIANGLES_FROM_N_VERTEXS(v)	(N_TRIANGLES_FROM_N_VERTEXS(v)*3)


#define N_VERTEX_QUAD	4


#define		GEOMETRY_PROPERTY_COLOR		(0x1 << 1) // Color ARGB
#define		GEOMETRY_PROPERTY_TEXTURE	(0x1 << 2) // TEXTURE UV 2D
#define		GEOMETRY_PROPERTY_NORMAL 	(0x1 << 3)  // NORMALS


typedef struct Geometry Geometry;


typedef enum{
	GEOMETRY_TYPE_POINTS=0,
	GEOMETRY_TYPE_TRIANGLES,
	GEOMETRY_TYPE_LINE_LOOP,
	GEOMETRY_TYPE_TRIANGLE_STRIP
}GeometryType;

struct Geometry{
	//vars
	GeometryType geometry_type;
	size_t n_vertexs;
	size_t index_length;
	uint32_t properties;
	void * data;
};



Geometry	* 	Geometry_GetDefaultPoint(void);
Geometry	* 	Geometry_GetDefaultCircle(void); // it returns a circle of 1 radius
Geometry	* 	Geometry_GetDefaultRectangle(void); // it returns a quad of 1 by 1
Geometry	* 	Geometry_GetDefaultRectangleFilled(void); // it returns a quad of 1 by 1
Geometry	* 	Geometry_GetDefaultRectangleTextured(void); // it returns a quad of 1 by 1
Geometry	* 	Geometry_New(GeometryType _geometry_type,size_t _n_indexs, size_t _n_vertexs, uint32_t _properties);
Geometry	* 	Geometry_NewRectangle(uint32_t properties);
Geometry	* 	Geometry_NewRectangleFilled(uint32_t _properties);
Geometry	* 	Geometry_NewPoints(size_t _n_point, uint32_t _properties);
/**
 * _smooth: 1-N
 */
Geometry	* 	Geometry_NewCircle(uint16_t _divisions_per_quadrant,uint32_t properties);

void 			Geometry_SetIndices(Geometry *_geometry,const short *_indices,size_t _indices_len);
void 			Geometry_SetMeshVertex(Geometry *geometry,const float *mesh_vertexs,size_t mesh_vertexs_len);
void 			Geometry_SetMeshTexture(Geometry *geometry,const float *mesh_texure_vertexs,size_t mesh_texture_vertexs_len);
void 			Geometry_SetMeshColor(Geometry *geometry,const float *mesh_color_vertexs,size_t mesh_color_vertexs_len);
void 			Geometry_SetMeshNormal(Geometry *geometry,const float *mesh_normal_vertexs,size_t mesh_normal_vertexs_len);
void 			Geometry_Draw(Geometry *geometry);

void			Geometry_Delete(Geometry *geometry);

void			Geometry_DeInit(void);

#endif
