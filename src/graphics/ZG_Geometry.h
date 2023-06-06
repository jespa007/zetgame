#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

// Meshes are constrained by these values
#define ZG_VERTEX_COORDS_LEN 	3	// XYZ
#define ZG_TEXTURE_COORDS_LEN 	2	// UV
#define ZG_COLOR_COORDS_LEN 	3	// RGB
#define ZG_NORMAL_COORDS_LEN 	3	// XYZ

#define N_TRIANGLES_FROM_N_VERTEXS(v)	((v)-2)
#define N_INDICES_TRIANGLES_FROM_N_VERTEXS(v)	(N_TRIANGLES_FROM_N_VERTEXS(v)*3)


#define 	ZG_N_VERTEXS_QUAD	4


#define		ZG_GEOMETRY_PROPERTY_COLOR		(0x1 << 1) // Color ARGB
#define		ZG_GEOMETRY_PROPERTY_TEXTURE	(0x1 << 2) // TEXTURE UV 2D
#define		ZG_GEOMETRY_PROPERTY_NORMAL 	(0x1 << 3)  // NORMALS


typedef struct ZG_Geometry ZG_Geometry;


typedef enum{
	ZG_GEOMETRY_TYPE_POINTS=0,
	GEOMETRY_TYPE_TRIANGLES,
	ZG_GEOMETRY_TYPE_LINE_LOOP,
	ZG_GEOMETRY_TYPE_TRIANGLE_STRIP
}GeometryType;

struct ZG_Geometry{
	//vars
	GeometryType geometry_type;
	size_t n_vertexs;
	size_t index_length;
	uint32_t properties;
	void * data;
};



ZG_Geometry	* 	ZG_Geometry_GetDefaultPoint(void);
ZG_Geometry	* 	ZG_Geometry_GetDefaultCircle(void); // it returns a circle of 1 radius
ZG_Geometry	* 	ZG_Geometry_GetDefaultRectangle(void); // it returns a quad of 1 by 1
ZG_Geometry	* 	ZG_Geometry_GetDefaultRectangleFilled(void); // it returns a quad of 1 by 1
ZG_Geometry	* 	ZG_Geometry_GetDefaultRectangleTextured(void); // it returns a quad of 1 by 1
ZG_Geometry	* 	ZG_Geometry_New(GeometryType _geometry_type,size_t _n_indexs, size_t _n_vertexs, uint32_t _properties);
ZG_Geometry	* 	ZG_Geometry_NewRectangle(uint32_t properties);
ZG_Geometry	* 	ZG_Geometry_NewRectangleFilled(uint32_t _properties);
ZG_Geometry	* 	ZG_Geometry_NewPoints(size_t _n_point, uint32_t _properties);
/**
 * _smooth: 1-N
 */
ZG_Geometry	* 	ZG_Geometry_NewCircle(uint16_t _divisions_per_quadrant,uint32_t properties);

void 			ZG_Geometry_SetIndices(ZG_Geometry *_geometry,const short *_indices,size_t _indices_len);
void 			ZG_Geometry_SetMeshVertex(ZG_Geometry *geometry,const float *mesh_vertexs,size_t mesh_vertexs_len);
void 			ZG_Geometry_SetMeshTexture(ZG_Geometry *geometry,const float *mesh_texure_vertexs,size_t mesh_texture_vertexs_len);
void 			ZG_Geometry_SetMeshColor(ZG_Geometry *geometry,const float *mesh_color_vertexs,size_t mesh_color_vertexs_len);
void 			ZG_Geometry_SetMeshNormal(ZG_Geometry *geometry,const float *mesh_normal_vertexs,size_t mesh_normal_vertexs_len);
void 			ZG_Geometry_Draw(ZG_Geometry *geometry);

void			ZG_Geometry_Delete(ZG_Geometry *geometry);

void			ZG_Geometry_DeInit(void);

#endif
