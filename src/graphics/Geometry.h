#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

// Meshes are constrained by these values
#define VERTEX_COORDS_LEN 	3	// XYZ
#define TEXTURE_COORDS_LEN 	2	// UV
#define COLOR_COORDS_LEN 	3	// RGB
#define NORMAL_COORDS_LEN 	3	// XYZ

#define N_TRIANGLES(v)	((v)-2)
#define N_INDICES(v)	(N_TRIANGLES(v)*3)

#define N_VERTEX_QUAD	4


#define		GEOMETRY_COLOR		(0x1 << 1) // Color ARGB
#define		GEOMETRY_TEXTURE	(0x1 << 2) // TEXTURE UV 2D
#define		GEOMETRY_NORMAL 	(0x1 << 3)  // NORMALS



typedef struct Geometry Geometry;

struct Geometry{
	//vars
	void * geometry_data;
	size_t n_vertexs;
	uint32_t properties;
};

Geometry	* 	Geometry_Default(void); // it returns a quad

Geometry	* 	Geometry_New(size_t index_count, uint32_t properties);
Geometry	* 	Geometry_NewQuad(uint32_t properties);

void 			Geometry_SetIndices(Geometry *geometry,BufferShort indices);
void 			Geometry_SetMeshVertex(Geometry *geometry,BufferFloat mesh_vertex);
void 			Geometry_SetMeshTexture(Geometry *geometry,BufferFloat mesh_texture);
void 			Geometry_SetMeshColor(Geometry *geometry,BufferFloat mesh_color);
void 			Geometry_SetMeshNormal(Geometry *geometry,BufferFloat mesh_normal);
void 			Geometry_Draw(Geometry *geometry);

void			Geometry_Delete(Geometry *geometry);

void			Geometry_DeInit(void);

#endif
