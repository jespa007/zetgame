#include "screen/zg_screen.h"

// https://stackoverflow.com/questions/17397724/point-sprites-for-particle-system/17400234

#define 	MAX_TOTAL_PARTICLES			7000

const char *particle_vertex_shader=
"#version 130\n"
"\n"
"// In your shader declare\n"
"uniform vec2 WindowSize;\n"
"varying float vRotation;\n"
"// Constants (tweakable):\n"
"\n"
"void main(void) {\n"
"\n"
"	vRotation = (gl_Normal.y*3.141592654)/(180.0);\n"
"\n"
"	gl_PointSize =  (WindowSize.y / -(gl_ModelViewMatrix * gl_Vertex).z)*gl_Normal.x;\n"
"	gl_Position = ftransform();\n"
"	gl_FrontColor = gl_Color;\n"
"\n"
"}\n";


const char *particle_fragment_shader=
"#version 130\n"
"\n"
"uniform sampler2D tex;\n"
"varying float vRotation;\n"
"void main(void)\n"
"{\n"
" 	float mid = 0.5;\n"
"    vec2 rotated = vec2(cos(vRotation) * (gl_PointCoord.x - mid)*1.2 + sin(vRotation) * (gl_PointCoord.y - mid)*1.2 + mid,\n"
"                        cos(vRotation) * (gl_PointCoord.y - mid)*1.2 - sin(vRotation) * (gl_PointCoord.x - mid)*1.2 + mid);\n"
"    vec4 rotatedTexture=texture2D(tex, rotated);\n"
"    gl_FragColor =  gl_Color * rotatedTexture;\n"
"}\n";


#define  	FPS_PARTICLES				DESIRED_FPS //60 //one_over_fps
#define 	FACTOR_PARTICLES_PER_FRAME 	100

float APERTURE_ANGLE_TAIL=0.3f;
float ONE_OVER_FPS = 1.0f/(float)FPS_PARTICLES;
CShader 		*CParticleEmitter::m_shader=NULL;

typedef CParticle *PointerParticle;

CParticleEmitter::CParticleEmitter(){
	PERCENT_VELOCITY_PARTICLES=0;
	timer_waitNextParticle=0;
	PERCENT_BOUNCE_VARIATION=0;
	PERCENT_MOTION_RAND_PARTICLES=0;
	PERCENT_SPIN_PARTICLES=0;
	PERCENT_WEIGHT_VARIATION=0;
	WAIT_TIME_MILLIS_TO_CREATE_NEW_PARTICLE=0;
	WAIT_TIME_MILLIS_TO_OUT_ANOTHER_PARTICLE=0;
	PERCENT_SPIN_VARIATION=0;
	PERCENT_LIFE_VARIATION=0;
	old_frame=0;
	PERCENT_LIFE_PARTICLES=0;
	PERCENT_WEIGHT_PARTICLES=0;
	timer_createNewParticle=0;
	PERCENT_EMISSION_ANGLE=0;
	PERCENT_VELOCITY_VARIATION=0;
	PARTICLES_PER_FRAME=0;
	PERCENT_EMISSION_RANGE=0;
	PERCENT_BOUNCE_PARTICLES=0;
	PERCENT_SIZE_VARIATION=0;
	PERCENT_VISIBILITY_PARTICLES=0;
	PERCENT_NUMBER_VARIATION=0;
	PERCENT_SIZE_PARTICLES=0;
	PERCENT_NUMBER_PARTICLES=0;
	PERCENT_MOTION_RAND_VARIATION=0;
	MAX_PARTICLES=0;

	actual_frame=0;
	m_system_particle=NULL;
	m_geomParticle=NULL;
	m_appParticle=NULL;


	particle_siz_rot=NULL;
	particle_pos=NULL;
	particle_col=NULL;
	m_particle=NULL;

	emitter_attribs=0;

	vbo[0]=GL_INVALID_VALUE;
	vbo[1]=GL_INVALID_VALUE;
	vbo[2]=GL_INVALID_VALUE;
}

// internal add p_emitter.
bool CParticleEmitter::loadFromEmitterInfo(
		CParticleSystem *  _system_particle,
		tEmitterInfo * peData
		){
	const tDefName 	particle_attribs[]={
	   {CParticle::ONE_PARTICLE,"ONE_PARTICLE"}
	   ,{CParticle::UNLIMITED_LIFE,"UNLIMITED_LIFE"}
	   ,{CParticle::KEEP_ALIGN_TO_MOTION,"KEEP_ALIGN_TO_MOTION"}
	   ,{CParticle::ATTACHED_TO_EMITTER,"ATTACHED_TO_EMITTER"}
	   ,{CParticle::RANDOM_REF_POINT,"RANDOM_REF_POINT"}
	};

	const tDefName 	app_attribs[]={
	   {CAppearance::TRANSPARENCY, "TRANSPARENCY"},
	   {CAppearance::INTENSITY, "INTENSITY"}
	};


	uint32_t  m_attribs = 0;
	uint32_t  m_app_attribs = 0;

	vector<string> list_attribs = StrUtils::Split(peData->m_attribs, '|');
	vector<string> list_app_attribs = StrUtils::Split(peData->m_appearanceAttribs, '|');
	bool found = false;
	for(unsigned i = 0; i < list_attribs.size(); i++){
		// Remove spaces
		list_attribs[i] = StrUtils::Remove(list_attribs[i],' ');
		found = false;
		for(unsigned  j = 0;j < sizeof(particle_attribs)/sizeof(particle_attribs[0]) && !found; j++){

			if(!strcmp(particle_attribs[j].name_id,list_attribs[i].c_str())){
				m_attribs|=particle_attribs[j].ID;
				found=true;

			}
		}

		if(!found)
			Log_Warning("Property \"%s\" don't reconized.",list_attribs[i].c_str());

	}

	for(unsigned i = 0; i < list_app_attribs.size(); i++){
		// Remove spaces
		list_app_attribs[i] = StrUtils::Remove(list_app_attribs[i],' ');
		found = false;
		for(unsigned  j = 0;j < sizeof(app_attribs)/sizeof(app_attribs[0]) && !found; j++){

			if(!strcmp(app_attribs[j].name_id,list_app_attribs[i].c_str())){
				m_app_attribs|=app_attribs[j].ID;
				found=true;

			}
		}

		if(!found)
			Log_Warning("Property \"%s\" don't reconized.",list_app_attribs[i].c_str());
	}

	return load(
			_system_particle,
			m_attribs,
			m_app_attribs,
			 peData->m_angle,
			 peData->m_pointRef.isParsed()?peData->m_pointRef.getStdVector():vector<float>{0,0,0},
			//CVector3f color,
			 peData->m_imageParticle ,
			//-------- MAIN PARAMS
			 peData->m_percentLife, // life inf millis (more life more tail)...
			 peData->m_percentNumber, // num_ particles...
			 peData->m_percentSize, // size...
			 peData->m_percentVelocity, // velocity
			 peData->m_percentWeight, // weight (from -1 to 1)
			 peData->m_percentSpin, // spin  (from -1 to 1)
			 peData->m_percentMotionRandom, // motion rand (from 0 to 1)
			 peData->m_percentBounce, // bounce ?!?!? ... (from 0 to 1)
			 peData->m_percentVisibility, // Visibility  (from 0 to 1)

			//---------- VARIATIONS  (all from 0 to 1)
			 peData->m_percentLifeVariation, // life
			 peData->m_percentNumberVariation, // number particles variation
			 peData->m_percentSizeVariation, // size variations
			 peData->m_percentVelocityVariation, // velocity variations // contant over time.
			 peData->m_percentWeightVariation,  // weight var
			 peData->m_percentSpinVariation,  // spin var
			 peData->m_percentMotionRandomVariation, // motion rand
			 peData->m_percentBounceVariation,  // bounce var
			//---------- INTERPOLATORS
			// controllers...
			 peData->m_intLife.isParsed()?peData->m_intLife.getStdVector():vector<float>{0,1,1,1},
			 peData->m_intVelocity.isParsed()?peData->m_intVelocity.getStdVector():vector<float>{0,1,1,1},
			 peData->m_intSize.isParsed()?peData->m_intSize.getStdVector():vector<float>{0,1,1,1},
			 peData->m_intMotionRandom.isParsed()?peData->m_intMotionRandom.getStdVector():vector<float>{0,1,1,1},
			 peData->m_intWeight.isParsed()?peData->m_intWeight.getStdVector():vector<float>{0,1,1,1},
			 peData->m_intBounce.isParsed()?peData->m_intBounce.getStdVector():vector<float>{0,1,1,1},

			// appearance ...
			 peData->m_intColor.isParsed()?peData->m_intColor.getStdVector():vector<float>{0,255,255,255,1,255,255,255},
			 peData->m_intAlpha.isParsed()?peData->m_intAlpha.getStdVector():vector<float>{0,1,1,1}
	);
}
// internal add p_emitter.
bool CParticleEmitter::load(
		CParticleSystem *  _system_particle,
		uint32_t m_attribs,
		uint32_t m_appearanceAttribs,
		uint32_t m_angle,
		const FLOAT_VECTOR  & _pointRef,
		//CVector3f color,
		const string & m_imageParticle ,
		//-------- MAIN PARAMS
		float m_percentLife, // life inf millis (more life more tail)...
		float m_percentNumParticles, // num_ particles...
		float m_percentSize, // size...
		float m_percentVelocity, // velocity
		float m_percentWeight, // weight (from -1 to 1)
		float m_percentSpin, // spin  (from -1 to 1)
		float m_percentMotionRandom, // motion rand (from 0 to 1)
		float m_percentBounce, // bounce ?!?!? ... (from 0 to 1)
		float m_percentVisibility, // Visibility  (from 0 to 1)

		//---------- VARIATIONS  (all from 0 to 1)
		float m_percentLifeVariation, // life
		float m_percentNumberVariation, // number particles variation
		float m_percentSizeVariation, // size variations
		float m_percentVelocityVariation, // velocity variations // contant over time.
		float m_percentWeightVariation,  // weight var
		float m_percentSpinVariation,  // spin var
		float m_percentMotionRandomVariation, // motion rand
		float m_percentBounceVariation,  // bounce var
		//---------- INTERPOLATORS
		// controllers...
		const FLOAT_VECTOR  & _intLife,
		const FLOAT_VECTOR  & _intVelocity,
		const FLOAT_VECTOR  & _intSize,
		const FLOAT_VECTOR  & _intMotionRandom,
		const FLOAT_VECTOR  & _intWeight,
		const FLOAT_VECTOR  & _intBounce,

		// appearance ...
		const FLOAT_VECTOR  & _intColor,
		const FLOAT_VECTOR  & _intAlpha
)
{
	m_system_particle = _system_particle;

	emitter_attribs = m_attribs;

	m_geomParticle= CGeometryIndexedArrayFactory::getDefaultPlane();
	m_appParticle = new CAppearance( m_appearanceAttribs| CAppearance::NO_LIGHT  | CAppearance::NO_DEPTH_TEST | CAppearance::NO_CULL_FACE);
	m_appParticle->setTexture(GET_TEXTURE(m_imageParticle));

	m_appearance=m_appParticle;

	// setup appearance !
	actual_frame = old_frame = 0;
	timer_waitNextParticle = 0;//new CTimer();
	timer_createNewParticle = 0;//new CTimer();

	// setup variables ...
	//	num_particles = 0;
	PERCENT_LIFE_PARTICLES 		= m_percentLife; //
	PERCENT_NUMBER_PARTICLES 	= m_percentNumParticles;
	PERCENT_SIZE_PARTICLES 		= m_percentSize*1; // jespada: scale fails : why ?!?!?
	PERCENT_VELOCITY_PARTICLES 	= m_percentVelocity;
	PERCENT_WEIGHT_PARTICLES 	= m_percentWeight;
	PERCENT_SPIN_PARTICLES 		= m_percentSpin;
	PERCENT_MOTION_RAND_PARTICLES= m_percentMotionRandom;
	PERCENT_BOUNCE_PARTICLES 	= m_percentBounce;
	PERCENT_VISIBILITY_PARTICLES = m_percentVisibility;
	//-------------- Variations ....
	PERCENT_LIFE_VARIATION  = m_percentLifeVariation;
	PERCENT_NUMBER_VARIATION  = m_percentNumberVariation*0.5f;
	PERCENT_SIZE_VARIATION  = m_percentSizeVariation*0.5f;
	PERCENT_VELOCITY_VARIATION  = m_percentVelocityVariation*2;
	PERCENT_WEIGHT_VARIATION  = m_percentWeightVariation;
	PERCENT_SPIN_VARIATION  = m_percentSpinVariation;
	PERCENT_MOTION_RAND_VARIATION  = m_percentMotionRandomVariation	*2;
	PERCENT_BOUNCE_VARIATION  = m_percentBounceVariation;

	// setup interpolators ...
	m_intVelocity 		= _intVelocity;
	m_intLife 			= _intLife;
	m_intWeight		= _intWeight;
	m_intSize 			= _intSize;
	m_intMotionRandom 	= _intMotionRandom;
	m_intBounce 	= _intBounce;

	//---- set interpolator as linear to get rigth results ...
	m_intVelocity.setInterpolatorType(CInterpolator1d::LINEAR_INTERPOLATOR);
	m_intLife.setInterpolatorType(CInterpolator1d::LINEAR_INTERPOLATOR);
	m_intWeight.setInterpolatorType(CInterpolator1d::LINEAR_INTERPOLATOR);
	m_intSize.setInterpolatorType(CInterpolator1d::LINEAR_INTERPOLATOR);
	m_intMotionRandom.setInterpolatorType(CInterpolator1d::LINEAR_INTERPOLATOR);
	m_intBounce.setInterpolatorType(CInterpolator1d::LINEAR_INTERPOLATOR);

	// normalize color...
	vector<float> v = _intColor;
	for(unsigned i = 0; i < v.size(); i++){
		if((i % 4) != 0)
			v[i] /= 256.0f;
	}

	m_intColor 		= v;
	m_intAlpha 		= _intAlpha;
	m_pointRef.set(_pointRef);

	actual_frame =old_frame = 0;

	// Vars in function of base particle system...
	PERCENT_EMISSION_ANGLE = _system_particle->BASE_EMISSION_ANGLE/360.0f;
	PERCENT_EMISSION_RANGE = _system_particle->BASE_EMISSION_RANGE/360.0f;

	if( (emitter_attribs & CParticle::ONE_PARTICLE) == CParticle::ONE_PARTICLE)
		MAX_PARTICLES = 1;
	else { // particle emitter as a trail
		float MAX_PARTICLES_PER_FRAME = (_system_particle->BASE_NUMBER_PARTICLES*(PERCENT_NUMBER_PARTICLES+PERCENT_NUMBER_VARIATION))/FPS_PARTICLES;
		MAX_PARTICLES = MIN(MAX((int)((MAX_PARTICLES_PER_FRAME)*(FACTOR_PARTICLES_PER_FRAME)),1),MAX_TOTAL_PARTICLES);

		PARTICLES_PER_FRAME = (_system_particle->BASE_NUMBER_PARTICLES*(PERCENT_NUMBER_PARTICLES))/FPS_PARTICLES;
		WAIT_TIME_MILLIS_TO_OUT_ANOTHER_PARTICLE = (int)(((float)((500.0f)/_system_particle->BASE_NUMBER_PARTICLES)*PERCENT_NUMBER_PARTICLES));

	}


	particle_pos=new float[MAX_PARTICLES*3];
	particle_col=new unsigned char[MAX_PARTICLES*4];
	particle_siz_rot=new float[MAX_PARTICLES*3];

	memset(particle_pos,0,(MAX_PARTICLES*3)*sizeof(float));
	memset(particle_col,0,(MAX_PARTICLES*4)*sizeof(char));
	memset(particle_siz_rot,0,(MAX_PARTICLES*3)*sizeof(float));

	if( (emitter_attribs & CParticle::ONE_PARTICLE) != CParticle::ONE_PARTICLE){
		// initialize its vbo...
		glGenBuffers(3, &vbo[0]);

		// vbo for vertex positions
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*MAX_PARTICLES*3, particle_pos, GL_STREAM_DRAW);
		//
		//	vbo for vertex colors
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(char)*MAX_PARTICLES*4, particle_col, GL_STREAM_DRAW);

		// vbo for vertex normals (but in fact we'll use for change its sizes)
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)* MAX_PARTICLES*3, particle_siz_rot,GL_STREAM_DRAW);

		m_shader = CVideo::getInstance()->getShaderManager()->GetShaderFromMemory("point_particle_internal",particle_vertex_shader,particle_fragment_shader);
	}

	m_particle = new PointerParticle[MAX_PARTICLES];

	float *particle_pos_it = particle_pos;
	unsigned char *particle_col_it = particle_col;
	float *particle_siz_rot_it = particle_siz_rot;

	// Setup particles...
	for(int i = 0; i < MAX_PARTICLES ; i++) {

		m_particle[i] = new CParticle(
									particle_pos_it,
									&particle_siz_rot_it[0],
									&particle_siz_rot_it[1],
									particle_col_it,
									emitter_attribs,
									m_angle,
									m_pointRef,
									this,
									&m_intVelocity,
									&m_intWeight,
									&m_intSize,
									&m_intMotionRandom,
									&m_intColor,
									&m_intAlpha

								);

		if((CParticle::UNLIMITED_LIFE & emitter_attribs) == CParticle::UNLIMITED_LIFE) {
			m_particle[i]->m_life = 1.0f;
			m_particle[i]->m_life_to_die_over_1_percent = 1.0f;
			m_particle[i]->startParticle(
								(CVector3f &  )_system_particle->getTranslate(CScreenObject::WORLD_COORDINATES), // Origin!
								vdir,
								0.0f, // weight
								(float)(PERCENT_SIZE_PARTICLES*_system_particle->BASE_SIZE*_system_particle->BASE_ZOOM), // size
								0.0f, // velocity
								0.0f, // random motion
								(float)(PERCENT_SPIN_PARTICLES)*_system_particle->BASE_SPIN_VELOCITY, // inc_spin
								1.0f, // visibility
								1.0f, // life
								10.0f // dtime
								);
		}

		// iterate buffers...
		particle_pos_it+=3;
		particle_col_it+=4;
		particle_siz_rot_it+=3;
	}

	return true;
}

void CParticleEmitter::updateAndDraw(){
	//Log_Info("hola!");
	float life,new_life;
	CScreenObject *m_scrParent = dynamic_cast<CScreenObject *>(m_parent);
	CMatrix3f *m3=NULL;
	bool started_some_particle = false;
	CParticleSystem *system_particle;
	CLuts *Luts = CLuts::getInstance();

	float	new_motion_random,
			new_weight,
			new_size,
			new_velocity,
			new_inc_spin,
			new_visibility,
			generate_particles;

	float random_start = (float)(unifRand()*PERCENT_NUMBER_VARIATION-PERCENT_NUMBER_VARIATION*0.5f)*(ONE_OVER_FPS);

	int n_particles_can_start = ((int)((PARTICLES_PER_FRAME+random_start)*(actual_frame-old_frame)*1.0f));//system_particle->number_particles_factor));

	//if(n_particles_can_start == 0) return;
	if(m_scrParent == NULL || MAX_PARTICLES == 0) {
		printError("Particle system null or this emiter don't have particles!");
		return;
	}

	system_particle = (CParticleSystem *)m_scrParent;
	m3 = m_scrParent->getMatrixRotationOposedVDir();

	generate_particles = 1.0f;//this.system_particle->activeGenerateParticles();

	float *particle_pos_it = particle_pos;
	unsigned char *particle_col_it = particle_col;
	float *particle_siz_rot_it = particle_siz_rot;

	//for(int i = 0; i < 1; i++) { //MAX_PARTICLES; i++)
	for(int i = 0; i < MAX_PARTICLES; i++) {

		life = m_particle[i]->m_life;

		// reset size ...
		particle_siz_rot_it[0]=0;

		if( life <= 0.0f && ((CParticle::ONE_PARTICLE & emitter_attribs) != CParticle::ONE_PARTICLE)) { // Particle has been died! --> particle.

			if((generate_particles != 0.0) &&
				n_particles_can_start > 0 &&
				timer_waitNextParticle<SDL_GetTicks() &&
				timer_createNewParticle<SDL_GetTicks()) {//(millis_start-millis_end) >  this.WAIT_TIME_MILLIS_TO_OUT_ANOTHER_PARTICLE)

				new_life     		=  (float)(system_particle->BASE_LIFE                           *(PERCENT_LIFE_PARTICLES        + (unifRand()*PERCENT_LIFE_VARIATION-PERCENT_LIFE_VARIATION*0.5f)));
				new_size     		=  (float)(system_particle->BASE_SIZE*system_particle->BASE_ZOOM*(PERCENT_SIZE_PARTICLES        + unifRand()*PERCENT_SIZE_VARIATION -PERCENT_SIZE_VARIATION*0.5f));
				new_weight   		=  (float)(system_particle->BASE_WEIGHT                         *(PERCENT_WEIGHT_PARTICLES      + unifRand()*PERCENT_WEIGHT_VARIATION-PERCENT_WEIGHT_VARIATION*0.5f));
				if((emitter_attribs & CParticle::KEEP_ALIGN_TO_MOTION) != CParticle::KEEP_ALIGN_TO_MOTION)
					new_velocity 		=  (float)(system_particle->BASE_VELOCITY                   *(PERCENT_VELOCITY_PARTICLES    + unifRand()*PERCENT_VELOCITY_VARIATION-PERCENT_VELOCITY_VARIATION*0.5f)); // For more tail, increase CUE () //-PERCENT_VELOCITY_VARIATION*0.5f));
				else
					new_velocity 		=  (float)(system_particle->BASE_VELOCITY                   *(PERCENT_VELOCITY_PARTICLES    + unifRand()*PERCENT_VELOCITY_VARIATION-PERCENT_VELOCITY_VARIATION*0.5f));
				new_inc_spin     	=  (float)(system_particle->BASE_SPIN_VELOCITY                  *(PERCENT_SPIN_PARTICLES        + unifRand()*PERCENT_SPIN_VARIATION-PERCENT_SPIN_VARIATION*0.5f));
				new_visibility  	=  (float)(system_particle->BASE_VISIBILITY                     *PERCENT_VISIBILITY_PARTICLES);// + unifRand()*VISIBILITY_VARIATION);
				new_motion_random  	=  (float)(system_particle->BASE_MOTION_RAND                    *(PERCENT_MOTION_RAND_PARTICLES +  unifRand()*PERCENT_MOTION_RAND_VARIATION-PERCENT_MOTION_RAND_VARIATION*0.5f));//-PERCENT_MOTION_RAND_VARIATION*0.5f));// + unifRand()*VISIBILITY_VARIATION);

				int base_angle= (int)((PERCENT_EMISSION_ANGLE + unifRand()*PERCENT_EMISSION_RANGE)*65536.0);

				if((emitter_attribs & CParticle::KEEP_ALIGN_TO_MOTION) != CParticle::KEEP_ALIGN_TO_MOTION) {
					if(m3 == NULL || !system_particle->changedMotion()) { // No vdir matrix, No tail effect

						vdir.x = Luts->Cos[(base_angle)&0x0ffff];//(int)(unifRand()*CLuts.DEGREE_360)&0x0ffff];//((float)unifRand()-0.5f);
						vdir.y = Luts->Sin[(base_angle)&0x0ffff];//(int)(unifRand()*CLuts.DEGREE_360)&0x0ffff];
						vdir.z = Luts->Cos[(int)(unifRand()*CLuts::DEGREE_360)&0x0ffff];
					}
					else {
						// Tail effect...
						// 1.- Random point on 0.3 aperture angle...
						vdir.x = Luts->Cos[(base_angle)&0x0ffff]*APERTURE_ANGLE_TAIL;//((float)unifRand()-0.5f);
						vdir.y = Luts->Sin[(base_angle)&0x0ffff]*APERTURE_ANGLE_TAIL;
						vdir.z = Luts->Cos[(int)(unifRand()*CLuts::DEGREE_360*PERCENT_EMISSION_ANGLE)&0x0ffff]*APERTURE_ANGLE_TAIL;

						// 2.- Rotate point to vdir_op that apoints...
						m3->transform(vdir);
					}

					if((emitter_attribs & CParticle::ALIGN_TO_MOTION) == CParticle::ALIGN_TO_MOTION) { // must find the angle from vdir.

						m_particle[i]->m_particle_angle_z = base_angle*CLuts::FACTOR_UNITS_LUT_TO_DEGREES;
					}

				}

				m_thePosition = system_particle->getTranslate(CScreenObject::WORLD_COORDINATES);

				m_thePosition.add(m_relative_position);//,thePosition);

				m_particle[i]->startParticle(
						m_thePosition, // Origin!
						vdir,
						new_weight,
						new_size,
						new_velocity,
						new_motion_random,
						new_inc_spin,		// Increase spin in every frame!!!!
						new_visibility,
						new_life, //(float)(unifRand()*1.5),//new_life,
						0.033f
					); // update at 1/30FPS = 0.33

				timer_waitNextParticle=SDL_GetTicks()+WAIT_TIME_MILLIS_TO_OUT_ANOTHER_PARTICLE;
				timer_createNewParticle=SDL_GetTicks()+WAIT_TIME_MILLIS_TO_CREATE_NEW_PARTICLE;

				n_particles_can_start--;
				started_some_particle= true;

				// first update...
				m_particle[i]->update();

			}
		}
		else{

			// in future recheck life over X!!!!
			if(((CParticle::ONE_PARTICLE & emitter_attribs) == CParticle::ONE_PARTICLE))	{
					if(generate_particles != 0.0){
						m_particle[i]->update();
						m_particle[i]->draw();
					}
			}
			else {// updates...
				m_particle[i]->update();

			}
		}
		// iterate ...
		particle_pos_it+=3;
		particle_col_it+=4;
		particle_siz_rot_it+=3;
	}

	// draw trail...
	if(((CParticle::ONE_PARTICLE & emitter_attribs) != CParticle::ONE_PARTICLE)

			){ // draw all point sprite particles

		if(m_shader!=NULL){
			m_shader->begin();
			// In your code load your shaders, link your program and call:
			int uniform_WindowSize = glGetUniformLocation(m_shader->GetProgram(),"WindowSize");
			glUniform2f(uniform_WindowSize, CVideo::getInstance()->getWidth(), CVideo::getInstance()->getHeight());
		}

		glPushMatrix();

		m_appearance->apply();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glDepthMask (GL_FALSE);


		{
			glEnable(GL_POINT_SPRITE);
			glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY); // we store the size in there...

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_PARTICLES * 3 * sizeof(float), particle_pos);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_PARTICLES * 4 * sizeof(char), particle_col);
		glColorPointer(4, GL_UNSIGNED_BYTE, 0, 0);


		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, MAX_PARTICLES * 3 * sizeof(float), particle_siz_rot);
		glNormalPointer(GL_FLOAT, 0, 0);

		glDrawArrays(GL_POINTS, 0, MAX_PARTICLES);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		{
			glDisable(GL_POINT_SPRITE);
		}


		glDepthMask (GL_TRUE);

		m_appearance->restore_attribs();

		glPopMatrix();

		if(m_shader!=NULL)
			m_shader->end();
	}

	//glEnd();

	if(started_some_particle)
		old_frame = actual_frame;

	actual_frame++;
	//----------

}

CParticleEmitter::~CParticleEmitter() {

	for(int i = 0; i < MAX_PARTICLES ; i++) {
		delete m_particle[i];
	}

	if(particle_pos != NULL){
		delete [] particle_pos;
	}

	if(particle_col != NULL){
		delete [] particle_col;
	}

	if(particle_siz_rot != NULL){
		delete [] particle_siz_rot;
	}

	delete [] m_particle;
	delete m_appParticle;

	glDeleteBuffers(3, &vbo[0]);
}

