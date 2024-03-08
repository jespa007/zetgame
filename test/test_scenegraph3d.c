#include "zetgame.h"
#if 0
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
	unsigned int *indices=malloc(sizeof(unsigned int)*(stack_count-1)*sector_count*6);
	unsigned int *line_indices=malloc(sizeof(unsigned int)*(stack_count)*sector_count*4);
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

	FILE *fp_vertices=fopen("vertices.txt","wt+");
	FILE *fp_normals=fopen("normals.txt","wt+");
	FILE *fp_textures=fopen("textures.txt","wt+");
	FILE *fp_indices=fopen("indices.txt","wt+");

	for(int i=0; i < n_indices;i++){
		fprintf(fp_indices," %i",indices[i]);
	}

	for(int i=0; i < n_vertices;i++){
		 if((i % 3) == 0 && i!=0){
		            fprintf(fp_vertices,"\n");
		        }
		fprintf(fp_vertices," %0.4f",vertices[i]);
	}

	for(int i=0; i < n_tex_coords;i++){
		fprintf(fp_textures," %0.4f",tex_coords[i]);
	}

	for(int i=0; i < n_normals;i++){
        if((i % 3) == 0 && i!=0){
            fprintf(fp_normals,"\n");
        }
		fprintf(fp_normals," %0.4f",normals[i]);
	}

	fclose(fp_indices);
	fclose(fp_vertices);
	fclose(fp_normals);
	fclose(fp_textures);

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
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


// Texture loading function
GLuint loadTexture(const char *filename) {
    SDL_Surface *surface;
    GLuint textureID;
    surface = SDL_LoadBMP(filename);
    if (!surface) {
        printf("Error: Texture loading failed\n");
        return 0;
    }
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    SDL_FreeSurface(surface);
    return textureID;
}

// Function to create a textured sphere VBO
void createTexturedSphereVBO(
		GLfloat radius
		, GLint slices
		, GLint stacks
		, GLuint *vboID
		, GLuint *texCoordVBOID
		, GLuint *indexVBOID
) {
    int numVertices = (slices + 1) * (stacks + 1);
    int numIndices = slices * stacks * 6;
    GLfloat *vertices = malloc(numVertices * 3 * sizeof(GLfloat));
    GLfloat *texCoords = malloc(numVertices * 2 * sizeof(GLfloat));
    GLuint *indices = malloc(numIndices * sizeof(GLuint));

    int vertexIndex = 0;
    int texCoordIndex = 0;

    for (int i = 0; i <= slices; i++) {
        float phi = ((float)i / slices) * PI * 2.0;
        for (int j = 0; j <= stacks; j++) {
            float theta = ((float)j / stacks) * PI;

            float x = cos(phi) * sin(theta);
            float y = cos(theta);
            float z = sin(phi) * sin(theta);

            vertices[vertexIndex++] = radius * x;
            vertices[vertexIndex++] = radius * y;
            vertices[vertexIndex++] = radius * z;

            texCoords[texCoordIndex++] = (float)i / slices;
            texCoords[texCoordIndex++] = (float)j / stacks;
        }
    }

    int index = 0;
    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            indices[index++] = (i * (stacks + 1)) + j;
            indices[index++] = ((i + 1) * (stacks + 1)) + j;
            indices[index++] = ((i + 1) * (stacks + 1)) + (j + 1);

            indices[index++] = (i * (stacks + 1)) + j;
            indices[index++] = ((i + 1) * (stacks + 1)) + (j + 1);
            indices[index++] = (i * (stacks + 1)) + (j + 1);
        }
    }

    glGenBuffers(1, vboID);
    glBindBuffer(GL_ARRAY_BUFFER, *vboID);
    glBufferData(GL_ARRAY_BUFFER, numVertices * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, texCoordVBOID);
    glBindBuffer(GL_ARRAY_BUFFER, *texCoordVBOID);
    glBufferData(GL_ARRAY_BUFFER, numVertices * 2 * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

    glGenBuffers(1, indexVBOID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexVBOID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    free(vertices);
    free(texCoords);
    free(indices);
}

// Function to draw a textured sphere using VBO
void drawTexturedSphereVBO(GLuint vboID, GLuint texCoordVBOID, GLuint indexVBOID, GLuint textureID, GLint slices, GLint stacks) {
    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordVBOID);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, 0);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBOID);

    for (int i = 0; i < slices; i++) {
        glDrawElements(GL_TRIANGLES, stacks * 6, GL_UNSIGNED_INT, (void *)(i * stacks * 6 * sizeof(GLuint)));
    }

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// Function to draw a simple triangle
void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f);   // Red
    glVertex3f(0.0f, 1.0f, 0.0f);  // Top vertex
    glColor3f(0.0f, 1.0f, 0.0f);   // Green
    glVertex3f(-1.0f, -1.0f, 0.0f); // Bottom left vertex
    glColor3f(0.0f, 0.0f, 1.0f);   // Blue
    glVertex3f(1.0f, -1.0f, 0.0f);  // Bottom right vertex
    glEnd();
}
/*
// Function to draw a textured sphere
void drawTexturedSphere(GLfloat radius, GLint slices, GLint stacks, GLuint textureID) {
    float phi, theta;
    float x, y, z;
    float s, t;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    for (int i = 0; i < slices; i++) {
        for (int j = 0; j < stacks; j++) {
            phi = ((i * 2 * PI) / slices);
            theta = ((j * PI) / stacks);

            x = cos(phi) * sin(theta);
            y = cos(theta);
            z = sin(phi) * sin(theta);

            s = (float)i / slices;
            t = (float)j / stacks;


            glTexCoord2f(s, t);
            glVertex3f(radius * x, radius * y, radius * z);


            phi = (((i + 1) * 2 * PI) / slices);

            x = cos(phi) * sin(theta);
            y = cos(theta);
            z = sin(phi) * sin(theta);

            s = (float)(i + 1) / slices;
            t = (float)j / stacks;


            glTexCoord2f(s, t);
            glVertex3f(radius * x, radius * y, radius * z);


            theta = (((j + 1) * PI) / stacks);

            x = cos(phi) * sin(theta);
            y = cos(theta);
            z = sin(phi) * sin(theta);

            s = (float)(i + 1) / slices;
            t = (float)(j + 1) / stacks;


            glTexCoord2f(s, t);
            glVertex3f(radius * x, radius * y, radius * z);


            phi = ((i * 2 * PI) / slices);

            x = cos(phi) * sin(theta);
            y = cos(theta);
            z = sin(phi) * sin(theta);

            s = (float)i / slices;
            t = (float)(j + 1) / stacks;


            glTexCoord2f(s, t);
            glVertex3f(radius * x, radius * y, radius * z);

        }
    }
    glEnd();
}*/

int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_GLContext context;
    GLuint textureID;
    int quit = 0;
    GLuint vboID, indexVBOID, texCoordVBOID;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create SDL window
    window = SDL_CreateWindow("Textured Sphere"
    		, SDL_WINDOWPOS_CENTERED
			, SDL_WINDOWPOS_CENTERED
			, WINDOW_WIDTH
			, WINDOW_HEIGHT
			, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create OpenGL context
    context = SDL_GL_CreateContext(window);
    glExtraIni();

/*	if(SDL_GL_MakeCurrent(window,context)!=0){
		ZG_LOG_ERROR("Cannot make current context:%s",SDL_GetError());
		return false;
	}

	// Disable vsync (Because it takes lot of high CPU)
	SDL_GL_SetSwapInterval(
#ifdef ZG_GRAPHICS_GL_DISABLE_VSYNCH
			0
#else
			1
#endif
	);*/



    // Initialize OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glDepthFunc(GL_LEQUAL);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glPerspective(90.0f, //90.0f,  // field of view
			(float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, // shape of viewport rectangle
			.01f,         // Min Z: how far from eye position does view start
			500.0f);       // max Z: how far from eye position does view extend


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Load texture
    textureID = loadTexture("texture.bmp");
    if (!textureID) {
        printf("Texture loading failed\n");
        return 1;
    }

    // Create textured sphere VBO
     createTexturedSphereVBO(1.0f, 30, 30, &vboID, &texCoordVBOID,&indexVBOID);

    // Main loop
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear the buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set camera
        glTranslatef(0.0f, 0.0f, -5.0f);

        // Rotate sphere
        static float angle = 0.0f;
        glRotatef(angle, 1.0f, 1.0f, 1.0f);

        // Draw textured sphere
        //drawTexturedSphere(1.0f, 30, 30, textureID);
        // Draw textured sphere using VBO
        drawTexturedSphereVBO(vboID, texCoordVBOID, indexVBOID, textureID, 30, 30);

        //drawTriangle();

        // Swap buffers
        SDL_GL_SwapWindow(window);

        // Update angle for rotation
        angle += 0.5f;

        // Cap the frame rate
        SDL_Delay(10);
    }

    // Cleanup
    glDeleteBuffers(1, &vboID);
    glDeleteBuffers(1, &indexVBOID);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
