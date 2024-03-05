#include "zetgame.h"


#define MAX_PARTICLES	100000

class tParticlePosition{
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
};

tParticlePosition	pos[MAX_PARTICLES];
float	vel[MAX_PARTICLES][3];
float	col[MAX_PARTICLES][4];
float	siz[MAX_PARTICLES][3];


float		currentRot			= 0;

bool		vboInited			= false;
int			pointSize			= 16;

GLuint		vbo[3];




//--------------------------------------------------------------
void particles_setup(){
	//ofBackground(0,0,0);

	for(int i=0; i<MAX_PARTICLES; i++) {
		pos[i].pos[0] = unifRand(-1, 1);
		pos[i].pos[1] = unifRand(-1, 1);
		pos[i].pos[2] = unifRand(-1, 1);
		vel[i][0] = unifRand(-1, 1)*0.1f;
		vel[i][1] = unifRand(-1, 1)*0.1f;
		vel[i][2] = unifRand(-1, 1)*0.1f;

		col[i][0] = 1;//ofRandom(0, 1);
		col[i][1] = 1;//ofRandom(0, 1);
		col[i][2] = 1;//ofRandom(0, 1);
		col[i][3] = 1;//ofRandom(0, 1);

		siz[i][0]=1;
		siz[i][1]=0;
		siz[i][2]=0;
	}


	glGenBuffers(3, &vbo[0]);

	// vbo for vertex positions
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pos), &pos[0].pos, GL_STREAM_DRAW);
	//
	//	vbo for vertex colors
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(col), col, GL_STREAM_DRAW);

	// vbo for vertex normals (but in fact we'll use for change its sizes)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(siz), siz, GL_STREAM_DRAW);


	vboInited = true;
	//image.loadImage("grad.png");
	//tex = image.getMSATexture();
}

bool sortByDistance(const tParticlePosition &lhs, const tParticlePosition &rhs)
{
	// PRE: Camera is at point 0,0
    return lhs.pos[2] < rhs.pos[2];
}


//--------------------------------------------------------------
void particles_update(){


	//currentRot += 0.2;

	float r = sin(ZG_System_GetTicks()*0.001f)*0.5f+0.5f;// * 0.5f + 0.5f;
	float g = cos(ZG_System_GetTicks()*0.001f) * 0.5f + 0.5f;
	float b = sin(ZG_System_GetTicks()*0.001f) * 0.5f + 0.5f;
	float incv=1.0f/(float)MAX_PARTICLES;
	float dvdf=0;
	for(int i=0; i<MAX_PARTICLES; i++) {
		pos[i].pos[0] += vel[i][0];
		pos[i].pos[1] += vel[i][1];
		pos[i].pos[2] += vel[i][2];

		// we only modify the first coordinate (x) for size...
		siz[i][0]=r*2;
		siz[i][1]=r*360;



		col[i][0] = r;
		col[i][1] = 1-g;
		col[i][2] = b;
	}

	//std::sort(begin(pos),end(pos),sortByDistance);

}


bool enable_depth_mask = true;

enum PARTICLE_METHOD_TYPE{
	VBA_METHOD=0,
	VBO_METHOD=1,
	NORMAL_METHOD
};

//--------------------------------------------------------------
void particles_draw(CAppearance *app){
	glPushMatrix();

	glTranslatef(0.0, 0, -1);
	//glRotatef(currentRot, 0, 1, 0);

	//ofSetColor(0xFFFFFF);
	glDisable(GL_LIGHTING);

	//app->apply();
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // normal
 	glEnable(GL_TEXTURE_2D);

 	if(enable_depth_mask)
 		glDepthMask (GL_FALSE);

 	// set matrix transform texture if transform3D is set!

	glBindTexture(GL_TEXTURE_2D, app->getTexture()->getTextureHandle());

	//if((this->appearance_attributes & CAppearance::INTENSITY) ==  CAppearance::INTENSITY)
		glBlendFunc(GL_SRC_ALPHA,GL_ONE); // intensity
	//else // normal operation ...
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // normal


		//glBlendFunc (GL_ONE, GL_ONE);
		//glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);


	   	glEnable(GL_ALPHA_TEST);
	   	glAlphaFunc(GL_NOTEQUAL, 0);

	 	//glEnable(GL_TEXTURE_2D);

	 	// set matrix transform texture if transform3D is set!

   		//glBindTexture(GL_TEXTURE_2D, app->getTexture()->getTextureHandle());


	//app->apply();

	//glPointSize(pointSize);



	//text->bind();		// ofTexture needs to be hacked to only use TEXTURE2D
	//if(useSprite)
	{
		glEnable(GL_POINT_SPRITE);
		glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	}

	/*switch(method_type) {
	case VBA_METHOD:
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(3, GL_FLOAT, 0, pos);
		glColorPointer(4, GL_FLOAT, 0, col);

		glDrawArrays(GL_POINTS, 0, numParticles);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		break;


	case VBO_METHOD:*/



   float quadratic[] =  {1.0f, 0.0f, 0.01f};
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
	float maxSize = 0.0f;
	glGetFloatv(GL_POINT_SIZE_MAX_ARB, &maxSize );
	if(maxSize > 100.0f)
		maxSize = 100.0f;
	glPointSize(maxSize);

	glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, 60.0f);
	glPointParameterf(GL_POINT_SIZE_MIN, 1.0f);
	glPointParameterf(GL_POINT_SIZE_MAX, maxSize);

	if(isVBOSupported() && vboInited){
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY); // we store the size in there...

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_PARTICLES * 3 * sizeof(float), &pos[0].pos);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_PARTICLES * 4 * sizeof(float), col);
		glColorPointer(4, GL_FLOAT, 0, 0);




		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_PARTICLES * 3 * sizeof(float), siz);
		glNormalPointer(GL_FLOAT, 0, 0);




		glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);


	}else{
		glBegin(GL_POINTS);
		for(int i=0; i<MAX_PARTICLES; i++) {
			glColor4f(col[i][0], col[i][1], col[i][2], col[i][3]);
			glVertex3f(pos[i].pos[0], pos[i].pos[1], pos[i].pos[2]);
		}
		glEnd();

	}

	//if(useTexture) tex->unbind();

	//if(useSprite)
	{
		glDisable(GL_POINT_SPRITE);
	}


	//app->restore_attribs();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
 	if(enable_depth_mask)
 		glDepthMask (GL_TRUE);
	//app->restore_attribs();

	glPopMatrix();
/*	ofSetColor(0);
	ofRect(0, 0, ofGetWidth(), 30);
	ofSetColor(0xFFFFFF);

	ofDrawBitmapString( " FPS: " + ofToString(ofGetFrameRate(), 2) +
					   "  |  numParticles: " + ofToString(numParticles) +
					   "  |  useVA: " + ofToString(useVA) +
					   "  |  useVBO: " + ofToString(useVBO) +
					   "  |  useTexture: " + ofToString(useTexture) +
					   "  |  useSprite: " + ofToString(useSprite) +
					   "  |  vsync: " + ofToString(vsync) +
					   "  |  pointSize: " + ofToString(pointSize)

					   , 20, 20);*/

}


int rotZ=0;
float inc_tz=-1;
float transZ=0;
Uint32 timer1=0;
int timer2=0;
/*float camRot=-90;
float camZ=0;
float camX=-0.010;*/

float camRotY=0;
float camRotX=0;
float camZ=1;
float camX=0;
float camY=0;
float rot_y=315;

int main(int argc, char * argv[]){


	if(!CEngine::setup( argc, argv))
			return -1;


	CEngine *m_engine = CEngine::getInstance();
	GLuint vboId=GL_INVALID_VALUE;
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
	CShader *s=NULL;


	particles_setup();

	CAppearance *app = NEW_APPEARANCE();

	img=NEW_IMAGE();
	img->load("res/images/trail.png");

	text=NEW_TEXTURE();
	text->setImage(img);

	glDisable(GL_LIGHTING);


	// setup points...
	GLfloat psr[2];
	GLfloat pda[3] = { 1.0f, 0.0f, 1.5f }; // defaults are (1.0, 0.0, 0.0)

	glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, psr);
	glPointSize(psr[1] < 100.0f ? psr[1] : 100.0f);
	// Requires OpenGL 1.4
	glPointParameterf(GL_POINT_SIZE_MIN, psr[0]);
	glPointParameterf(GL_POINT_SIZE_MAX, psr[1]);
	glPointParameterf(GL_POINT_FADE_THRESHOLD_SIZE, 60.0f);
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, pda);

	print_info_cr("Point sizes (%f,%f) ",psr[0],psr[1]);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_PROGRAM_POINT_SIZE);

	app->setAppearanceAttribs(CAppearance::NO_LIGHT | CAppearance::TRANSPARENCY);// | CAppearance::INTENSITY );
	app->setTexture(text);

	CCamera *camera = CCameraFactory::getDefaultResource();
	CRender *r=ZG_ACTIVE_RENDER;

	do{

		ZG_INPUT->updateInput();

		if(ZG_INPUT->key[JEMEKey::JEMEK_m]) // switch...
			enable_depth_mask=!enable_depth_mask;

		v->startRender();

		if(ZG_INPUT->key[JEMEKey::JEMEK_F5]){

			if(s!=NULL){
				v->getShaderManager()->erase(s);
				s=NULL;
			}

			s=v->getShaderManager()->GetShader("point_sprite");

			if(s==NULL) {
				print_error_cr("cannot load shader point");
			}else{
				glUseProgram(s->GetProgram());

				// get variable....
				/*printf("%u\n", MatrixID);
				glGetUniformLocation();
				// set variable...
				glUniformMatrix4f(obj->shader->location.modelview,  1, GL_FALSE, model_view);*/
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


		float angley= -((float)ZG_INPUT->getMouseCoordinates()->x/(float)r->getWidth()-0.5)*180;
		float anglex= -((float)ZG_INPUT->getMouseCoordinates()->y/(float)r->getHeight()-0.5)*180;


		//--------
		// camera ...
		if(ZG_INPUT->keyR[JEMEK_LEFT]){
			rot_y+=1;
		}

		if(ZG_INPUT->keyR[JEMEK_RIGHT]){
				rot_y-=1;
		}

		if(ZG_INPUT->keyR[JEMEK_DOWN]){
				camZ+=cos((angley/180.0f)*_PI)*0.5f;
				camX+=sin((angley/180.0f)*_PI)*0.5f;
				camY-=sin((anglex/180.0f)*_PI)*0.5f;
		}

		if(ZG_INPUT->keyR[JEMEK_UP]){
			camZ-=cos((angley/180.0f)*_PI)*0.5f;
			camX-=sin((angley/180.0f)*_PI)*0.5f;
			camY+=sin((anglex/180.0f)*_PI)*0.5f;
		}

		camera->setRotateY(angley);
		camera->setRotateX(anglex);
		camera->setTranslateZ(camZ);
		camera->setTranslateX(camX);
		camera->setTranslateY(camY);

		camera->update();

		//	float *aux_buff =  camera->getTransformMatrix3x3();
			float xc = -camera->getTranslateX();
			float yc = -camera->getTranslateY();
			float zc = -camera->getTranslateZ();

			//glMatrixMode(GL_MODELVIEW);
			//glLoadIdentity();


		//	glClear(GL_DEPTH_BUFFER_BIT);


			//memcpy(m_identity, aux_buff,16*sizeof(float));

			// FX part additional transforms to renderized scene ...
//				glTranslatef(100,100,0);
//				glRotatef(0,0,1,45);

			glLoadMatrixf(camera->getBufferTransformMatrix4x4());//->data_buffer);
			glTranslatef(xc, yc, zc);


		glPushMatrix();
		glTranslatef(0, 0, -1.0);

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


	  m_engine->unloadProject();
	  m_engine->destroy();

	return 0;
}
