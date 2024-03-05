#include "zetgame.h"

// sending geometry quads...

#define MAX_PARTICLES	100000

/*class tParticlePosition{
public:
	float	pos[3];

	bool operator < (const tParticlePosition & pp){

		return true;
	}

	tParticlePosition &  operator = (const tParticlePosition & pp){

		pos[0] = pp.pos[0];
		pos[1] = pp.pos[1];
		pos[2] = pp.pos[2];
		return *this;
	}
};*/

bool vboInited=false;

    short indicesBuffer[]=	// indexs
	{
		0,1,2,2,3,0
	};

	float vertexBuffer[]= // 3D
	{
		   -1,  1, 0.0f,     // top left
		    1,  1, 0.0f,    // bottom right
		    1, -1, 0.0f,    // top right
		    -1,-1, 0.0f    // bottom left
	};



float	pos[MAX_PARTICLES][4];
float	vel[MAX_PARTICLES][3];
float	col[MAX_PARTICLES][4];

int			numParticles		= 100000;
int			adder				= 20000;
float		currentRot			= 0;

GLuint		vao;
GLuint		vboVertexId;
GLuint		vboIndicesId;
GLuint vertBufID;
GLuint vaoId;


//--------------------------------------------------------------
void particles_setup(){
	//ofBackground(0,0,0);

	/*for(int i=0; i<MAX_PARTICLES; i++) {
		pos[i].pos[0] = unifRand(-1, 1);
		pos[i].pos[1] = unifRand(-1, 1);
		pos[i].pos[2] = unifRand(-1, 1);
		vel[i][0] = unifRand(-1, 1);
		vel[i][1] = unifRand(-1, 1);
		vel[i][2] = unifRand(-1, 1);

		col[i][0] = 1;//ofRandom(0, 1);
		col[i][1] = 1;//ofRandom(0, 1);
		col[i][2] = 1;//ofRandom(0, 1);
		col[i][3] = 1;//ofRandom(0, 1);
	}*/
	 //glGenVertexArrays(1,&vaoId);
	// glBindVertexArray(vaoId);

	 // Create a new Vertex Buffer Object in memory and select it (bind)
	 // A VBO is a collection of Vectors which in this case resemble the location of each vertex.
	 glGenBuffers(1,&vboVertexId);
	 glBindBuffer(GL_ARRAY_BUFFER, vboVertexId);
	 glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
	 // Put the VBO in the attributes list at index 0
	// glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	 // Deselect (bind to 0) the VBO
	 //glBindBuffer(GL_ARRAY_BUFFER, 0);
	 // Deselect (bind to 0) the VAO
	// glBindVertexArray(0);
	 // Create a new VBO for the indices and select it (bind)
	 glGenBuffers(1, &vboIndicesId);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesId);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBuffer),indicesBuffer, GL_STATIC_DRAW);
	 // Deselect (bind to 0) the VBO
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // generating index VBO
  /*  glGenBuffers(1, &indicesBufID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesData.size()*sizeof(int), &indicesData[0], GL_STREAM_DRAW);*/

	vboInited = true;
	//image.loadImage("grad.png");
	//tex = image.getMSATexture();
}
/*
bool sortByDistance(const tParticlePosition &lhs, const tParticlePosition &rhs)
{
	// PRE: Camera is at point 0,0
    return lhs.pos[2] < rhs.pos[2];
}

*/

void particle_unload(){
	glDeleteBuffers(1, &vboVertexId);
	 glDeleteBuffers(1, &vboIndicesId);

}
//--------------------------------------------------------------
void particles_update(){


	currentRot += 0.2;

	/*float r = sin(ZG_System_GetTicks()*0.5f) * 0.5f + 0.5f;
	float g = cos(ZG_System_GetTicks()*0.5f) * 0.5f + 0.5f;
	float b = sin(ZG_System_GetTicks()*0.5f) * 0.5f + 0.5f;
	for(int i=0; i<numParticles; i++) {
		pos[i].pos[0] += vel[i][0];
		pos[i].pos[1] += vel[i][1];
		pos[i].pos[2] += vel[i][2];

	col[i][0] = r;
		col[i][1] = g;
		col[i][2] = b;
	}*/

	// Update vertices in the VBO, first bind the VBO
	/*glBindBuffer(GL_ARRAY_BUFFER, vboId);

	// Apply and update vertex data
	for (int i = 0; i < 1; i++) {
	    //TexturedVertex vertex = vertices[i];

	    // Define offset
	    //float offsetX = (float) (cos(Math.PI * random()) * 0.1);
	    //float offsetY = (float) (sin(Math.PI * random()) * 0.1);

	    // Offset the vertex position
	    //float[] xyz = vertex.getXYZ();
	    //vertex.setXYZ(xyz[0] + offsetX, xyz[1] + offsetY, xyz[2]);

	    // Put the new data in a ByteBuffer (in the view of a FloatBuffer)
	   // FloatBuffer vertexFloatBuffer = vertexByteBuffer.asFloatBuffer();
	   // vertexFloatBuffer.rewind();
	   // vertexFloatBuffer.put(vertex.getElements());
	   // vertexFloatBuffer.flip();

	    glBufferSubData(GL_ARRAY_BUFFER, i * sizeof(float)*4,
	    		quadVertices);

	    // Restore the vertex data
	    //vertex.setXYZ(xyz[0], xyz[1], xyz[2]);
	}

	// And of course unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);*/


}

enum PARTICLE_METHOD_TYPE{
	VBA_METHOD=0,
	VBO_METHOD=1,
	NORMAL_METHOD
};

//--------------------------------------------------------------
void particles_draw(CAppearance *app){
	glPushMatrix();

	glTranslatef(0, 0, 0.0);
	//glRotatef(currentRot, 0, 1, 0);

	//app->apply();


    //glBindVertexArray(quadVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    //glBindVertexArray(0);
	//glVertexPointer(3, GL_FLOAT, 0, quadVertices);
	//glDrawArrays(GL_QUADS, 0, 4);

	 // Bind to the VAO that has all the information about the vertices
    // activating VBO
	// Bind to the VAO that has all the information about the vertices
	//glBindVertexArray(vaoId);
	//glEnableVertexAttribArray(0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertexId);

	glVertexPointer(3, GL_FLOAT, 0, 0L);		// 3 float * size(float)

	// Bind to the index VBO that has all the information about the order of the vertices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesId);

	// Draw the vertices

	glDrawElements(GL_TRIANGLES, sizeof(indicesBuffer)/sizeof(indicesBuffer[0]), GL_UNSIGNED_SHORT, 0);


	// Put everything back to default (deselect)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);


	//app->restore_attribs();


	glPopMatrix();

}


int main(int argc, char * argv[]){


	if(!CEngine::setup( argc, argv))
			return -1;


	CEngine *m_engine = CEngine::getInstance();
	//GLuint vboId=GL_INVALID_VALUE;
	struct{
		float x,y,z;
		float size;
	}Particle[100];

	CTexture *text;
	CImageObject *img;



	CVideo *v = CVideo::getInstance();




	//char *str = ;

	//print_info_cr("\nShading language %s",glGetString ( GL_SHADING_LANGUAGE_VERSION ));
	// let's compile and link...


	//GLint vv=GL_INVALID_VALUE;

	//loadShaders();
	CShader *s;


	particles_setup();

	CAppearance *app = NEW_APPEARANCE();

	img=NEW_IMAGE();
	img->load("data/trail.png");

	text=NEW_TEXTURE();
	text->setImage(img);

	glDisable(GL_LIGHTING);
	glEnable(GL_PROGRAM_POINT_SIZE);

	app->setAppearanceAttribs(CAppearance::NO_LIGHT | CAppearance::TRANSPARENCY | CAppearance::INTENSITY );
	app->setTexture(text);

	do{

		ZG_INPUT->updateInput();

		v->startRender();

		if(ZG_INPUT->key[JEMEKey::JEMEK_F5]){
			s=v->getShaderManager()->GetShaderFromFile("data/point_sprite.vs","data/point_sprite.fs");

			if(s==NULL) {
				print_error_cr("cannot load shader point");
			}else{
				glUseProgram(s->GetProgram());
			}

			//loadShaders();
		}

		// the hell starts here...
		float attenuation[3] = {0.0f, 1.0f, 0.0f};
		//glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, attenuation);

		//glPointParameterf(GL_POINT_SIZE_MIN, 1.0f);
		//glPointParameterf(GL_POINT_SIZE_MAX, 128.0f);
		//if(my_program != GL_INVALID_ENUM)
		//	my_vec3_location = glGetAttribLocationARB(my_program, );


		/*//vv = glGetUniformLocationARB(my_program, "float_from_shader");
		if(vv==GL_INVALID_VALUE){

		}*/
		glPushMatrix();
		glTranslatef(0, 0, -1);

		particles_update();
		particles_draw(app);


		/*glEnable(GL_POINT_SPRITE);
		glDisable(GL_LIGHTING);


		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);


		glPointSize(20);


	   glBegin(GL_POINTS);

	  // glColor3f(((rand()%256))/256.0f,((rand()%256))/256.0f,((rand()%256))/256.0f);

	   for(int i = 0; i < 1; i++){
	   	   glVertex3f(0,0,0);//(200-(rand()%100))*0.01f,(200-(rand()%100))*0.01f,0);
	   }
	   glEnd();
		glDisable(GL_POINT_SPRITE);
*/



		glPopMatrix();
		v->endRender();



	}while(!ZG_INPUT->key[JEMEKey::JEMEK_ESCAPE]);
		particle_unload();


	  m_engine->unloadProject();
	  m_engine->destroy();

	return 0;
}
