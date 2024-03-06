#include "zetgame.h"

ZG_Geometry *createSphere(int radius, int sector_count, int stack_count){
	// clear memory of prev arrays
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, length_inv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord
	int num_vertices=(sector_count+1)*(stack_count+1);
	float *vertices=malloc(sizeof(float)*3*num_vertices);
	float *normals=malloc(sizeof(float)*3*num_vertices);
	float *tex_coords=malloc(sizeof(float)*2*num_vertices);

	float sector_step = 2 * PI / sector_count;
	float stack_step = PI / stack_count;
	float sector_angle, stack_angle;

	int n_vertices=0;
	int n_normals=0;
	int n_tex_coords=0;

	for(int i = 0; i <= stack_count; ++i){
	    stack_angle = PI / 2 - i * stack_step;        // starting from pi/2 to -pi/2
	    xy = radius * cosf(stack_angle);             // r * cos(u)
	    z = radius * sinf(stack_angle);              // r * sin(u)

	    // add (sectorCount+1) vertices per stack
	    // first and last vertices have same position and normal, but different tex coords
	    for(int j = 0; j <= sector_count; ++j){
	        sector_angle = j * sector_step;           // starting from 0 to 2pi

	        // vertex position (x, y, z)
	        x = xy * cosf(sector_angle);             // r * cos(u) * cos(v)
	        y = xy * sinf(sector_angle);             // r * cos(u) * sin(v)
	        vertices[n_vertices++]=x;
	        vertices[n_vertices++]=y;
	        vertices[n_vertices++]=z;

	        // normalized vertex normal (nx, ny, nz)
	        nx = x * length_inv;
	        ny = y * length_inv;
	        nz = z * length_inv;
	        normals[n_normals++]=nx;
	        normals[n_normals++]=ny;
	        normals[n_normals++]=nz;

	        // vertex tex coord (s, t) range between [0, 1]
	        s = (float)j / sector_count;
	        t = (float)i / stack_count;
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
	uint32_t *indices=malloc(sizeof(uint32_t)*(stack_count-1)*sector_count*6);
	uint32_t *line_indices=malloc(sizeof(uint32_t)*(stack_count)*sector_count*4);
	int k1, k2;
	int n_indices=0;
	int n_line_indices=0;

	for(int i = 0; i < stack_count; ++i){
	    k1 = i * (sector_count + 1);     // beginning of current stack
	    k2 = k1 + sector_count + 1;      // beginning of next stack

	    for(int j = 0; j < sector_count; ++j, ++k1, ++k2){
	        // 2 triangles per sector excluding first and last stack_count
	        // k1 => k2 => k1+1
	        if(i != 0)
	        {
	            indices[n_indices++]=k1;
	            indices[n_indices++]=k2;
	            indices[n_indices++]=k1 + 1;
	        }

	        // k1+1 => k2 => k2+1
	        if(i != (stack_count-1))
	        {
	            indices[n_indices++]=k1 + 1;
	            indices[n_indices++]=k2;
	            indices[n_indices++]=k2 + 1;
	        }

	        // store indices for lines
	        // vertical lines for all stack_count, k1 => k2
	        line_indices[n_line_indices++]=k1;
	        line_indices[n_line_indices++]=k2;
	        if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
	        {
	            line_indices[n_line_indices++]=k1;
	            line_indices[n_line_indices++]=k1 + 1;
	        }
	    }
	}

	//printf("r:%i e:%i\n",(stack_count-1)*sector_count*6,n_indices);

	ZG_Geometry *geometry=ZG_Geometry_New(ZG_GEOMETRY_TYPE_TRIANGLES);
	ZG_Geometry_SetIndices(geometry,indices,n_indices);
	ZG_Geometry_SetMeshVertex(geometry,vertices,n_vertices);
	//ZG_Geometry_SetMeshNormal(geometry,normals,n_normals);
	ZG_Geometry_SetMeshTexture(geometry,tex_coords,n_tex_coords);

	/*ZG_Geometry *geometry=ZG_Geometry_New(ZG_GEOMETRY_TYPE_LINES);
	ZG_Geometry_SetIndices(geometry,line_indices,n_line_indices);
	ZG_Geometry_SetMeshVertex(geometry,vertices,n_vertices);*/
	//ZG_Geometry_SetMeshNormal(geometry,normals,n_normals);
	//ZG_Geometry_SetMeshTexture(geometry,tex_coords,n_tex_coords);

	free(line_indices);
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

	/*ZG_TransformAnimation_StartTween(
		ta
		, ZG_TRANSFORM_COMPONENT_ROTATE_X
		, ZG_EASE_IN_OUT_SINE
		, 0
		, 360
		, 4000
		, ANIMATION_LOOP_REPEAT_INFINITE
	);

	ZG_TransformAnimation_StartTween(
		ta
		, ZG_TRANSFORM_COMPONENT_ROTATE_Y
		, ZG_EASE_IN_OUT_SINE
		, 0
		, 360
		, 4000
		, ANIMATION_LOOP_REPEAT_INFINITE
	);

	ZG_TransformAnimation_StartTween(
		ta
		, ZG_TRANSFORM_COMPONENT_ROTATE_Z
		, ZG_EASE_IN_OUT_SINE
		, 0
		, 360
		, 4000
		, ANIMATION_LOOP_REPEAT_INFINITE
	);*/



	transform.translate.z=-5.5;
	//transform.translate.x01.5;

	appearance->texture=texture;

	do{
		// Event update
		ZG_Input_Update();

		//ZG_TransformAnimation_Update(ta,&transform);

		// toggle fullscreen
		if(ZG_KP_F9){
			ZG_Graphics_ToggleFullscreen();
		}

		if(ZG_KP_A){
			transform.translate.z+=1;
		}

		if(ZG_KP_Z){
			transform.translate.z-=1;
		}

		if(ZG_KP_LEFT){
			transform.rotate.y+=10;
		}

		if(ZG_KP_RIGHT){
			transform.rotate.y-=10;
		}

		if(ZG_KP_UP){
			transform.rotate.x+=10;
		}

		if(ZG_KP_DOWN){
			transform.rotate.x-=10;
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
