#pragma once


#include "CParticle.h"


typedef struct
{

	ParserVarString<ZJZJ_CONST_CHAR("id")> 		 					m_id;
	ParserVarString<ZJZJ_CONST_CHAR("attribs")> 			 			m_attribs;
	ParserVarString<ZJZJ_CONST_CHAR("appearanceAttribs")> 			 	m_appearanceAttribs;
	ParserVarNumber<ZJZJ_CONST_CHAR("angle")> 			 				m_angle;
	CParserVarNumberArray<ZJZJ_CONST_CHAR("pointRef")>					m_pointRef;
	ParserVarString<ZJZJ_CONST_CHAR("imageParticle")> 		 			m_imageParticle;
	CParserVarNumberArray<ZJZJ_CONST_CHAR("colorParticle")> 		 		m_colorParticle;


	//--- BASE PARTICLES
	ParserVarNumber<ZJZJ_CONST_CHAR("percentLifeParticles")> 			m_percentLife;			// Base life to die particle.
	ParserVarNumber<ZJZJ_CONST_CHAR("percentNumberParticles")> 			m_percentNumber;		// Base Number particles to be generated.
	ParserVarNumber<ZJZJ_CONST_CHAR("percentSizeParticles")> 		 	m_percentSize;			// Base  Size of every particle.
	ParserVarNumber<ZJZJ_CONST_CHAR("percentVelocityParticles")> 		m_percentVelocity;		// Base Velocity of particle.			// Base Visibilty.
	ParserVarNumber<ZJZJ_CONST_CHAR("percentWeightParticles")> 			m_percentWeight;		// Base Weight of particle
	ParserVarNumber<ZJZJ_CONST_CHAR("percentSpinParticles")> 			m_percentSpin;			// Base Spin particle
	ParserVarNumber<ZJZJ_CONST_CHAR("percentMotionRandomParticles")> 	m_percentMotionRandom;	// Base Motion random. I don't known this user, still?
	ParserVarNumber<ZJZJ_CONST_CHAR("percentBounceParticles")>			m_percentBounce;		// Base Bounce (Salto in spanish). I don't known her user, still?


	//--- VARIATIONS PARTICLES
	ParserVarNumber<ZJZJ_CONST_CHAR("percentLifeVariation")>			m_percentLifeVariation;		//PERCENT_LIFE_VARIATION,		// Life variation.   --> life_particle = LIFE*PERCENT_LIFE + rand()%LIFE_VARIATION;
	ParserVarNumber<ZJZJ_CONST_CHAR("percentNumberVariation")>			m_percentNumberVariation;	// Number variation. --> number_particles = NUMBER*PERCENT_NUMBER + rand()%NUMBER_VARIATION;
	ParserVarNumber<ZJZJ_CONST_CHAR("percentSizeVariation")>			m_percentSizeVariation;     // Size variation.   --> size_particle = SIZE*PERCENT_NUMBER + rand()%SIZE_VARIATION;
	ParserVarNumber<ZJZJ_CONST_CHAR("percentVelocityVariation")>		m_percentVelocityVariation; // Velocity 		 --> velocity_particle = VELOCITY*PERCENT_VELOCITY + rand()%VELOCITY_VARIATION;
	ParserVarNumber<ZJZJ_CONST_CHAR("percentWeightVariation")>			m_percentWeightVariation;	// Weight			 --> weight = WEIGHT*PERCENT_WEIGHT + rand()%VELOCITY_VARIATION;
	ParserVarNumber<ZJZJ_CONST_CHAR("percentSpinVariation")>			m_percentSpinVariation;			// Spin				 --> spin	= SPIN*PERCENT_SPIN	+ rand()%SPIN_VARIATION;
	ParserVarNumber<ZJZJ_CONST_CHAR("percentMotionRandomVariation")>	m_percentMotionRandomVariation; // Motion ?!?!
	ParserVarNumber<ZJZJ_CONST_CHAR("percentBounceVariation")>			m_percentBounceVariation;		// Bounce ?!?!?

	//ParserVarNumber<ZJZJ_CONST_CHAR("percentEmissionAngle")>			m_percentEmissionAngle;		// Emission angle ?!?!?
	//ParserVarNumber<ZJZJ_CONST_CHAR("percentEmissionRange")>			m_percentEmissionRange;		// Emission range ?!?!?

	//---- OTHERS

	ParserVarNumber<ZJZJ_CONST_CHAR("percentVisibilityParticles")>		m_percentVisibility;



	//--- BASE ATTRIBUTES PER PARTICLE


	ParserVarNumber<ZJZJ_CONST_CHAR("lifeParticle")> 					m_lifeParticle;		// Real life particle to die.

	// interpolators ?!?!?!
	CParserVarNumberArray<ZJZJ_CONST_CHAR("velocityOverLife")>		m_intVelocity;
	CParserVarNumberArray<ZJZJ_CONST_CHAR("sizeOverLife")>			m_intSize;
	CParserVarNumberArray<ZJZJ_CONST_CHAR("lifeOverLife")>			m_intLife;
	CParserVarNumberArray<ZJZJ_CONST_CHAR("weightOverLife")>			m_intWeight;
	CParserVarNumberArray<ZJZJ_CONST_CHAR("bounceOverLife")>			m_intBounce;
	CParserVarNumberArray<ZJZJ_CONST_CHAR("motionRandomOverLife")>	m_intMotionRandom;
	CParserVarNumberArray<ZJZJ_CONST_CHAR("colorOverLife")>			m_intColor;
	CParserVarNumberArray<ZJZJ_CONST_CHAR("alphaOverLife")>			m_intAlpha;

}tEmitterInfo;


class CParticleSystem;
class CParticleEmitter: public CDrawableObject{


	uint32_t 			emitter_attribs;
	int 			MAX_PARTICLES;
	CInterpolator1d m_intVelocity,
					m_intLife,
					m_intWeight,
					m_intSize,
					m_intMotionRandom,
					m_intBounce,
					m_intAlpha;
	CInterpolator3d m_intColor;

	float	*particle_pos;
	unsigned char	*particle_col;
	float	*particle_siz_rot;

	CVector3f m_pointRef;//.set(_pointRef);

	//tEmitterData *m_data;



	int    	actual_frame,
				old_frame;

	int			WAIT_TIME_MILLIS_TO_OUT_ANOTHER_PARTICLE,
		        WAIT_TIME_MILLIS_TO_CREATE_NEW_PARTICLE;


	uint32_t timer_waitNextParticle;
	uint32_t timer_createNewParticle;
	int PARTICLES_PER_FRAME;

	CParticleSystem *m_system_particle;


	CParticle		**m_particle;

	GLuint		vbo[3];



public:
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//
	// Static
	//
	CParticleEmitter();

	/*bool load(
		uint32_t _attribs,
		uint32_t _ap_attribs,
		uint32_t _angle,
		const CVector3f  & _pointRef,
		//CVector3f color,
		const string & texture_name,
		//-------- MAIN PARAMS
		float _percentLife, // life inf millis (more life more tail)...
		float _percentNumParticles, // num_ particles...
		float _percentSize, // size...
		float _percentVelocity, // velocity
		float _percentWeight, // weight (from -1 to 1)
		float _percentSpin, // spin  (from -1 to 1)
		float _pecentMotionRandom, // motion rand (from 0 to 1)
		float _percentBounce, // bounce ?!?!? ... (from 0 to 1)
		float _percentVisibility, // Visibility  (from 0 to 1)
		//---------- VARIATIONS  (all from 0 to 1)
		float _varLife, // life
		float _varNumParticles, // number particles variation
		float _varSize, // size variations
		float _varVelocity, // velocity variations // contant over time.
		float _varWeight,  // weight var
		float _varSpin,  // spin var
		float _varRandomMotion, // motion rand
		float _varBounce,  // bounce var
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

	);*/



	

	//
	//
	//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

	CGeometry		*m_geomParticle;
	CAppearance		*m_appParticle;
	static CShader 		*m_shader;

	CVector3f     m_relative_position,
				  m_random_start_particle,
			      m_thePosition;


	// static data
	//tEmitterInfo *m_peData;

	// setup variables ...
	float   PERCENT_SIZE_PARTICLES,			// Base  Size of every particle.
			PERCENT_MOTION_RAND_PARTICLES,	// Base Motion random. I don't known this user, still?
			PERCENT_LIFE_PARTICLES,			// Base life to die particle.
			PERCENT_NUMBER_PARTICLES,		// Base Number particles to be generated.
			PERCENT_WEIGHT_PARTICLES,		// Base Weight of particle
			PERCENT_SPIN_PARTICLES,			// Base Spin particle

			PERCENT_BOUNCE_PARTICLES,		// Base Bounce (Salto in spanish). I don't known her user, still?

			PERCENT_VISIBILITY_PARTICLES,
		    PERCENT_VELOCITY_PARTICLES,		// Base Velocity of particle.			// Base Visibilty.
		    PERCENT_EMISSION_ANGLE,
		    PERCENT_EMISSION_RANGE;

	//---------------- VARIATIONS ------------------------------------
	float		PERCENT_LIFE_VARIATION,		// Life variation.   --> life_particle = LIFE*PERCENT_LIFE + rand()%LIFE_VARIATION;
				PERCENT_NUMBER_VARIATION,   // Number variation. --> number_particles = NUMBER*PERCENT_NUMBER + rand()%NUMBER_VARIATION;
				PERCENT_SIZE_VARIATION,     // Size variation.   --> size_particle = SIZE*PERCENT_NUMBER + rand()%SIZE_VARIATION;
				PERCENT_VELOCITY_VARIATION, // Velocity 		 --> velocity_particle = VELOCITY*PERCENT_VELOCITY + rand()%VELOCITY_VARIATION;
				PERCENT_WEIGHT_VARIATION,	// Weight			 --> weight = WEIGHT*PERCENT_WEIGHT + rand()%VELOCITY_VARIATION;
				PERCENT_SPIN_VARIATION,		// Spin				 --> spin	= SPIN*PERCENT_SPIN	+ rand()%SPIN_VARIATION;
				PERCENT_MOTION_RAND_VARIATION,  // Motion ?!?!
				PERCENT_BOUNCE_VARIATION;		// Bounce ?!?!?

	void setupInternalVars();

	bool loadFromEmitterInfo(
				CParticleSystem *  _system_particle,
				tEmitterInfo * peData
	);

	bool load(
			CParticleSystem *  _system_particle,
			uint32_t _attribs,
			uint32_t _ap_attribs,
			uint32_t _angle,
			//CVector3f color,
			const FLOAT_VECTOR  & _pointRef,
			const string & texture_name,

			//-------- MAIN PARAMS
			float _percentLife, // life inf millis (more life more tail)...
			float _percentNumParticles, // num_ particles...
			float _percentSize, // size...
			float _percentVelocity, // velocity
			float _percentWeight, // weight (from -1 to 1)
			float _percentSpin, // spin  (from -1 to 1)
			float _pecentMotionRandom, // motion rand (from 0 to 1)
			float _percentBounce, // bounce ?!?!? ... (from 0 to 1)
			float _percentVisibility, // Visibility  (from 0 to 1)
			//---------- VARIATIONS  (all from 0 to 1)
			float _varLife, // life
			float _varNumParticles, // number particles variation
			float _varSize, // size variations
			float _varVelocity, // velocity variations // contant over time.
			float _varWeight,  // weight var
			float _varSpin,  // spin var
			float _varRandomMotion, // motion rand
			float _varBounce,  // bounce var
			//---------- INTERPOLATORS (Remove setup default interpolators)
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


	);



	void setLifeOverTime(const FLOAT_VECTOR & _intLife);
	void setVelocityOverTime(const FLOAT_VECTOR & _intVelocity);
	void setSizeOverTime(const FLOAT_VECTOR & _intSize);
	void setMotionRandomOverTime(const FLOAT_VECTOR & _intMotionRandom);
	void setWeightverTime(const FLOAT_VECTOR & _intWeight);
	void setBounceOverTime(const FLOAT_VECTOR & _intBounce);


	void setColorOverTime(const FLOAT_VECTOR & _intLife);
	void setOpacityOverTime(const FLOAT_VECTOR & _intLife);


	/*CParticleEmitter(
			const string & name_emitter,
			tEmitterData *_m_data
	);*/
	CParticleSystem *getSystemParticle(){
		return m_system_particle;
	}

	void setAttributes();

	//void virtual update();
	//virtual void updateChilds();

	void updateAndDraw();

	virtual ~CParticleEmitter();

};
