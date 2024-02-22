#pragma once

//#include "zg_engine.h"

	// Particle ...
class CParticleEmitter;
class CParticle: public CDrawable
{

	float *m_translate; // must by 3
	float *m_scale; //
	float *m_rotate; //Around Z
	unsigned char *m_color; // rgba

	CVector3f it_motion;
	CVector3f trans_aux;
	CGeometry *m_geometry;


	void drawTrail();
	void drawOneParticle();


	void billboardBegin();
	void billboardEnd();

public:



	enum
	{
		// PARTICLE PROPERTIES
		UNLIMITED_LIFE			=0x1<<0,
		KEEP_ALIGN_TO_MOTION	=0x1<<1,
		ALIGN_TO_MOTION			=0x1<<2,
		RANDOM_REF_POINT		=0x1<<3,
		ATTACHED_TO_EMITTER		=0x1<<4,
		ONE_PARTICLE			=0x1<<5

	};

		CVector3f 	//m_translate,
					m_from_position,
					m_ini_pos,
					m_vdir,
					m_refPoint;
		uint32_t relative_rotate_z_fp;

		CAppearance *m_appearance;

		FloatBuffer m_color_from_interpolator;
		float m_aspectX, m_aspectY;

		uint32_t      m_attribs;

		//CTransform3d t3d;

		//CAppearance *m_appearance;
		//CGeometry	*m_geometry;

		//CMatrix3f	m_transMatrix;

		float 	m_weight,
				m_sizeParticle,
				m_alpha,
				m_t_random, new_random_motion,
				m_previus_gravity, actual_gravity,
				m_spin,
				m_life_to_die,
				m_life_to_die_over_1_percent,
				m_step_life_to_die_over_1_percent,
				m_visibility,
				m_inc_spin,
				m_life,
				m_dtime,
				m_velocity,
				m_particle_angle_z,
				m_total_elapsed_time,
				m_elapsed_time;


		FloatBuffer absolute_scale_from_interpolator,
					 m_alpha_from_interpolator,
					 m_velocity_from_interpolator,
					 m_weight_from_interpolator,
					 m_random_motion_from_interpolator;

		CParticleEmitter *m_parent;


		CInterpolator1d *m_intAlpha,
						*m_intVelocity,
						*m_intWeight,
						*m_intSize,
						*m_intMotionRandom;
		CInterpolator3d *m_intColor;

		CParticle(
					float 			 *_translate_pointer,
					float 			 *_scale_pointer,
					float 			 *_rotate_pointer,
					unsigned char 			 *_color_pointer,

					uint32_t			_m_attribs,
					uint32_t			_angle,
					const CVector3f 	&	  _ref_Pos, // relative position from particle emitter ..
					CParticleEmitter * _m_emitter,
					CInterpolator1d * _m_intVelocity,
					CInterpolator1d * _m_intWeight,
					CInterpolator1d * _m_intSize,
					CInterpolator1d * _m_intMotionRandom,
					CInterpolator3d * _m_intColor,
					CInterpolator1d * _m_intAlpha

		);


		void startParticle(

				CVector3f &  _ini_pos,

			  	CVector3f &  _vect_dir,
				float new_weight,
				float new_size,
				float new_velocity,
				float new_random_motion,
				float new_inc_spin,		// Increase spin in every frame!!!!
				float new_visibility,
				float new_life,
				float new_dtime
			);// in millis (33FPS)



		virtual void update();

		virtual void draw();

		virtual ~CParticle();
};
