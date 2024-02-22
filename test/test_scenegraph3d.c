#include "zetgame.h"

void createSphere(int radius, int sectors, int stacks){
	// clear memory of prev arrays
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / stacks;
	float sectorAngle, stackAngle;

	for(int i = 0; i <= stacks; ++i)
	{
	    stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
	    xy = radius * cosf(stackAngle);             // r * cos(u)
	    z = radius * sinf(stackAngle);              // r * sin(u)

	    // add (sectorCount+1) vertices per stack
	    // first and last vertices have same position and normal, but different tex coords
	    for(int j = 0; j <= sectors; ++j)
	    {
	        sectorAngle = j * sectorStep;           // starting from 0 to 2pi

	        // vertex position (x, y, z)
	        x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
	        y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
	        vertices.push_back(x);
	        vertices.push_back(y);
	        vertices.push_back(z);

	        // normalized vertex normal (nx, ny, nz)
	        nx = x * lengthInv;
	        ny = y * lengthInv;
	        nz = z * lengthInv;
	        normals.push_back(nx);
	        normals.push_back(ny);
	        normals.push_back(nz);

	        // vertex tex coord (s, t) range between [0, 1]
	        s = (float)j / sectors;
	        t = (float)i / sectors;
	        tex_coords.push_back(s);
	        tex_coords.push_back(t);
	    }
	}

	// generate CCW index list of sphere triangles
	// k1--k1+1
	// |  / |
	// | /  |
	// k2--k2+1
	int *indices;
	int *line_indices;
	int k1, k2;
	int n_indices=0;
	int n_line_indices=0;

	for(int i = 0; i < stacks; ++i)
	{
	    k1 = i * (sectors + 1);     // beginning of current stack
	    k2 = k1 + sectors + 1;      // beginning of next stack

	    for(int j = 0; j < sectors; ++j, ++k1, ++k2)
	    {
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
	        line_indices[n_line_indices++]=k1;
	        line_indices[n_line_indices++]=k2;
	        if(i != 0)  // horizontal lines except 1st stack, k1 => k+1
	        {
	            line_indices[n_line_indices++]=k1;
	            line_indices[n_line_indices++]=k1 + 1;
	        }
	    }
	}
}



int main(int argc, char *argv[]){
	ZG_UNUSUED_PARAM(argc);
	ZG_UNUSUED_PARAM(argv);

	ZG_Init(NULL);

	createSphere(1, 36, 18);

	do{
		// Event update
		ZG_Input_Update();

		// toggle fullscreen
		if(ZG_KP_F9){
			ZG_Graphics_ToggleFullscreen();
		}

		ZG_Graphics_BeginRender();

		ZG_Graphics_EndRender();



	}while(!ZG_KP_ESC);

	ZG_DeInit();

	return 0;
}
