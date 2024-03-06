#ifndef __ZG_GEOMETRY_H__
#define __ZG_GEOMETRY_H__

// Meshes are constrained by these values
#define 	ZG_VERTEX_COORDS_LEN 	3	// XYZ
#define 	ZG_TEXTURE_COORDS_LEN 	2	// UV
#define 	ZG_COLOR_COORDS_LEN 	3	// RGB
#define 	ZG_NORMAL_COORDS_LEN 	3	// XYZ

#define 	ZG_N_TRIANGLES_FROM_N_VERTEXS(v)			((v)-2)
#define 	ZG_N_INDICES_TRIANGLES_FROM_N_VERTEXS(v)	(ZG_N_TRIANGLES_FROM_N_VERTEXS(v)*3)


#define 	ZG_N_VERTEXS_QUAD	4


#define		ZG_GEOMETRY_PROPERTY_COLOR		(0x1 << 1) // Color ARGB
#define		ZG_GEOMETRY_PROPERTY_TEXTURE	(0x1 << 2) // TEXTURE UV 2D
#define		ZG_GEOMETRY_PROPERTY_NORMAL 	(0x1 << 3)  // NORMALS


typedef struct ZG_Geometry ZG_Geometry;


typedef enum{
	ZG_GEOMETRY_TYPE_POINTS=0,
	ZG_GEOMETRY_TYPE_TRIANGLES,
	ZG_GEOMETRY_TYPE_LINE_LOOP,
	ZG_GEOMETRY_TYPE_LINES,
	// To see com to configure triangle strip show the following link https://www.learnopengles.com/tag/triangle-strips/
	ZG_GEOMETRY_TYPE_TRIANGLE_STRIP
}ZG_GeometryType;

struct ZG_Geometry{
	//vars
	ZG_GeometryType geometry_type;
	size_t indices_length;
	uint32_t properties;
	void * data;
};



ZG_Geometry	* 	ZG_Geometry_GetDefaultPoint(void);
ZG_Geometry	* 	ZG_Geometry_GetDefaultCircle2d(void); // it returns a circle of 1 radius
ZG_Geometry	* 	ZG_Geometry_GetDefaultRectangle2d(void); // it returns a quad of 1 by 1
ZG_Geometry	* 	ZG_Geometry_GetDefaultFilledRectangle2d(void); // it returns a quad of 1 by 1
ZG_Geometry	* 	ZG_Geometry_GetDefaultTexturedRectangle2d(void); // it returns a quad of 1 by 1
ZG_Geometry	* 	ZG_Geometry_NewRectangle2d(void);
ZG_Geometry	* 	ZG_Geometry_NewFilledRectangle2d(void);
ZG_Geometry	* 	ZG_Geometry_NewTexturedRectangle2d(void);
ZG_Geometry	* 	ZG_Geometry_NewPoints(void);
/**
 * _smooth: 1-N
 */
ZG_Geometry	* 	ZG_Geometry_NewCircle2d(uint16_t _divisions_per_quadrant);


ZG_Geometry	* 	ZG_Geometry_New(ZG_GeometryType _geometry_type);
void 			ZG_Geometry_SetIndices(ZG_Geometry *_this,const uint32_t *_indices,size_t _indices_len);
void 			ZG_Geometry_SetMeshVertex(ZG_Geometry *_this,const float *_vertexs,size_t _vertexs_len);
void 			ZG_Geometry_SetMeshTexture(ZG_Geometry *_this,const float *_vertexs,size_t _vertexs_len);
void 			ZG_Geometry_SetMeshColor(ZG_Geometry *_this,const float *_vertexs,size_t _vertexs_len);
void 			ZG_Geometry_SetMeshNormal(ZG_Geometry *_this,const float *_vertexs,size_t _vertexs_len);
void 			ZG_Geometry_Draw(ZG_Geometry *_this);

void			ZG_Geometry_Delete(ZG_Geometry *_this);

void			ZG_Geometry_DeInit(void);

#endif
