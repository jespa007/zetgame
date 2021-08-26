#pragma once

#include "ParticleEmitter.h"

//#include "ires/integratedParticleSystems.h"



typedef struct{
	// PS Base information ...
	CParserVarNumber<_CONST_CHAR("life")> 		 					m_life;
	CParserVarNumber<_CONST_CHAR("numberParticles")> 					m_numberParticles;
	CParserVarNumber<_CONST_CHAR("size")> 							m_size;
	CParserVarNumber<_CONST_CHAR("velocity")> 						m_velocity;
	CParserVarNumber<_CONST_CHAR("weight")> 							m_weight;		 	// % Weight of every particle
	CParserVarNumber<_CONST_CHAR("spinVelocity")> 					m_spinVelocity;		// % Rotate from origin to died end.
	CParserVarNumber<_CONST_CHAR("motionRandom")> 					m_motionRandom;    	// Motion rand, affects random weight, velocity, direction in curse of particle...
	CParserVarNumber<_CONST_CHAR("bounce")> 							m_bounce;		 	//  ?!?!?
	CParserVarNumber<_CONST_CHAR("zoom")> 							m_zoom;			 	// Zoom (Z increases or poligon increases ?)
	CParserVarNumber<_CONST_CHAR("visibility")> 						m_visibility;	 	// Visibility of particles
	CParserVarNumber<_CONST_CHAR("tintStrenght")> 					m_tintStrenght;  	// Tint strnght: (1.0 --> gris, 0.0f --> original color.
	CParserVarNumber<_CONST_CHAR("emissionAngle")> 					m_emissionAngle; 	// Angle emission in degres!
	CParserVarNumber<_CONST_CHAR("emissionRange")> 					m_emissionRange; 	// Emission range from 0, to 360�
	CParserVarNumber<_CONST_CHAR("widthBoundingRectangle")> 			m_widthBoundingRectangle; //
	CParserVarNumber<_CONST_CHAR("heightBoundingRectangle")> 		m_heightBoundingRectangle;



	// its Emitters
	CParserVarArrayPropertyGroup<tEmitterInfo,_CONST_CHAR( "emitter")> m_emitter;

}tParticleSystemData;

class CParticleSystem: public CDrawableObject {

	// define own integrated particle systems...
	//STAR_TRAIL_PARTICLE_SYSTEM;
	//STAR_BALL2_PARTICLE_SYSTEM;


	// internal setup needed to setup internal vars, after load parameters...
	//void setupInternalVars();




/*
	void addParticleEmitter(
			const string & name_emitter,
			Uint32 _attribs,
			CVector3f color,
			CTexture *texture,
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
			float _percentEmissionAngle, // Emission angle
			float _percentEmissionRange, // Emission range

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
			const FLOAT_VECTOR  & _intVelocity,
			const FLOAT_VECTOR  & _intLife,
			const FLOAT_VECTOR  & _intWeight,
			const FLOAT_VECTOR  & _intBounce,
			const FLOAT_VECTOR  & _intMotionRandom,
			// transforms...
			const FLOAT_VECTOR  & _scale,

			// appearance ...
			const FLOAT_VECTOR  & _intColor
	);
*/



public:

	static CParticleEmitter 		*pe_glowFlare;
	static CParticleEmitter 		*pe_bigGlow;
	static CParticleEmitter 		*pe_copyGlowFlare;
	static CParticleEmitter 		*pe_tail;
	static CParticleSystem	 		*ps_starTrail;

	static CParticleEmitter 		*pe_stars;
	static CParticleEmitter 		*pe_dots;
	static CParticleEmitter 		*pe_streaks;
	static CParticleSystem	 		*ps_starBall2;

	static void destroyStarTrailPS();
	static void destroyStarBallPS();

	static CParticleSystem *getStarTrailPS();
	static CParticleSystem *getStarBallPS();
	static void destroyDefaultPS();

 	// Static data ...
	CParserVarPropertyGroup<tParticleSystemData> *m_psData;


	// Main data (must be initialized in constructor...

	//---------------------------------------------------------------------------
	float 		 BASE_LIFE,
				 INI_BASE_NUMBER_PARTICLES,
			     BASE_NUMBER_PARTICLES,
			     BASE_SIZE,
			     BASE_VELOCITY,
			     BASE_WEIGHT,		 	// % Weight of every particle
			     BASE_SPIN_VELOCITY,	// % Rotate from origin to died end.
			     BASE_MOTION_RAND,    	// Motion rand, affects random weight, velocity, direction in curse of particle...
			     BASE_BOUNCE,		 	//  ?!?!?
			     BASE_ZOOM,			 	// Zoom (Z increases or poligon increases ?)
			     BASE_VISIBILITY,	 	// Visibility of particles
			     BASE_TINT_STRENGHT,  	// Tint strnght: (1.0 --> gris, 0.0f --> original color.
			     BASE_EMISSION_ANGLE, 	// Angle emission in degres!
			     BASE_EMISSION_RANGE, 	// Emission range from 0, to 360�
				 WIDTH_START_RECTANGLE_ACTION,
				 HEIGHT_START_RECTANGLE_ACTION;

	

	//
	//
	//------------------------------------------------------------------------------------------------------------------------------------------------

	CParticleSystem();


	// internal build ...
	void setup(
			Uint32 _life ,
			Uint32 _numberParticles,
			Uint32 _size,
			Uint32 _velocity,
			Uint32 _weight,
			Uint32 _spin,
			Uint32 _motion,
			Uint32 _bounce,
			Uint32 _zoom,
			Uint32 _visibility,
			Uint32 _tintStrenght,
			Uint32 _emissionAngle,
			Uint32 _emissionRange
	);

	virtual bool load(const string & m_filename);
	virtual bool loadData(const string & _data);


	bool addEmitter(tEmitterInfo & peEmitter);//CParticleEmitter * particle_emitter);
	virtual void draw();
	virtual void update();

	virtual void unload();


	virtual ~CParticleSystem();

};



