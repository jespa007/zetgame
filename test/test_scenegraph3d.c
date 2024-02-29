#include "zetgame.h"

ZG_Geometry *createSphere(int radius, int sectors, int stacks){
	// clear memory of prev arrays
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord
	int num_vertices=(sectors+1)*(stacks+1);
	float *vertices=malloc(sizeof(float)*3*num_vertices);
	float *normals=malloc(sizeof(float)*3*num_vertices);
	float *tex_coords=malloc(sizeof(float)*2*num_vertices);

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / stacks;
	float sectorAngle, stackAngle;

	int n_vertices=0;
	int n_normals=0;
	int n_tex_coords=0;

	for(int i = 0; i <= stacks; ++i){
	    stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
	    xy = radius * cosf(stackAngle);             // r * cos(u)
	    z = radius * sinf(stackAngle);              // r * sin(u)

	    // add (sectorCount+1) vertices per stack
	    // first and last vertices have same position and normal, but different tex coords
	    for(int j = 0; j <= sectors; ++j){
	        sectorAngle = j * sectorStep;           // starting from 0 to 2pi

	        // vertex position (x, y, z)
	        x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
	        y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
	        vertices[n_vertices++]=x;
	        vertices[n_vertices++]=y;
	        vertices[n_vertices++]=z;

	        // normalized vertex normal (nx, ny, nz)
	        nx = x * lengthInv;
	        ny = y * lengthInv;
	        nz = z * lengthInv;
	        normals[n_normals++]=nx;
	        normals[n_normals++]=ny;
	        normals[n_normals++]=nz;

	        // vertex tex coord (s, t) range between [0, 1]
	        s = (float)j / sectors;
	        t = (float)i / sectors;
	        tex_coords[n_tex_coords++]=s;
	        tex_coords[n_tex_coords++]=t;
	    }
	}

	//printf("v r:%i e:%i\n",3*num_vertices,n_vertices);
	//printf("n r:%i e:%i\n",3*num_vertices,n_normals);
	//printf("u r:%i e:%i\n",2*num_vertices,n_tex_coords);

	// generate CCW index list of sphere triangles
	// k1--k1+1
	// |  / |
	// | /  |
	// k2--k2+1
	uint32_t *indices=malloc(sizeof(uint32_t)*(stacks-1)*sectors*6);
	//int *line_indices;
	int k1, k2;
	int n_indices=0;
	//int n_line_indices=0;

	for(int i = 0; i < stacks; ++i){
	    k1 = i * (sectors + 1);     // beginning of current stack
	    k2 = k1 + sectors + 1;      // beginning of next stack

	    for(int j = 0; j < sectors; ++j, ++k1, ++k2){
	        // 2 triangles per sector excluding first and last stacks
	        // k1 => k2 => k1+1
	        if(i != 0)
	        {
	            indices[n_indices++]=k1;
	            indices[n_indices++]=k2;
	            indices[n_indices++]=k1 + 1;
	        }

	        // k1+1 => k2 => k2+1
	        if(i != (stacks-1))
	        {
	            indices[n_indices++]=k1 + 1;
	            indices[n_indices++]=k2;
	            indices[n_indices++]=k2 + 1;
	        }

	        // store indices for lines
	        // vertical lines for all stacks, k1 => k2
	       /* line_indices[n_line_indices++]=k1;
	        line_indices[n_line_indices++]=k2;
	        if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
	        {
	            line_indices[n_line_indices++]=k1;
	            line_indices[n_line_indices++]=k1 + 1;
	        }*/
	    }
	}

	//printf("r:%i e:%i\n",(stacks-1)*sectors*6,n_indices);

	ZG_Geometry *geometry=ZG_Geometry_New(ZG_GEOMETRY_TYPE_TRIANGLES);
	ZG_Geometry_SetIndices(geometry,indices,n_indices);
	ZG_Geometry_SetMeshVertex(geometry,vertices,n_vertices);
	ZG_Geometry_SetMeshNormal(geometry,normals,n_normals);
	ZG_Geometry_SetMeshTexture(geometry,tex_coords,n_tex_coords);

	free(indices);
	free(vertices);
	free(normals);
	free(tex_coords);

	return geometry;

}



int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL);

	ZG_Geometry *geometry=createSphere(1, 36, 18);
	ZG_Transform transform=ZG_Transform_New();
	ZG_Appearance *appearance=ZG_Appearance_New();
	ZG_Texture *texture=ZG_Texture_NewFromFile("../../../test/data/images/mars.jpg");
	ZG_TransformAnimation 			*ta=ZG_TransformAnimation_New();
	ZG_Texture_SetRepeatUV(texture,true);

	ZG_Graphics_SetProjectionMode(ZG_PROJECTION_MODE_PERSPECTIVE);

	ZG_TransformAnimation_StartTween(
		ta
		, ZG_TRANSFORM_COMPONENT_ROTATE_Y
		, ZG_EASE_LINEAR
		, 0
		, 360
		, 4000
		, ANIMATION_LOOP_REPEAT_INFINITE
	);

	transform.translate.z=-3.5;
	appearance->texture=texture;

	do{
		// Event update
		ZG_Input_Update();

		ZG_TransformAnimation_Update(ta,&transform);

		// toggle fullscreen
		if(ZG_KP_F9){
			ZG_Graphics_ToggleFullscreen();
		}

		ZG_Graphics_BeginRender();

		ZG_Graphics_Draw(&transform,geometry,appearance);

		ZG_Graphics_EndRender();



	}while(!ZG_KP_ESC);

	ZG_Appearance_Delete(appearance);
	ZG_Geometry_Delete(geometry);
	ZG_Texture_Delete(texture);
	ZG_TransformAnimation_Delete(ta);
	ZG_DeInit();

	return 0;
}
