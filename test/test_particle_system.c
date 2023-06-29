

#include "../../../resources/image/dots.png.c"
#include "../../../resources/image/star.png.c"
#include "../../../resources/image/streaks.png.c"
#include "zetgame.h"


/*


		const string & name_emitter,

		uint32_t _attribs,
		uint32_t _ap_attribs,
		uint32_t _angle,

		CVector3f color,
		CTexture *p_image,
		//-------- MAIN PARAMS
		float _percentLife, // life inf millis (more life more tail)...
		float _percentNumParticles, // num_ particles...
		float _percentSize, // size...
		float _percentVelocity, // velocity
		float _percentWeight, // weight (from -1 to 1)
		float _percentSpin, // spin  (from -1 to 1)
		float _percentMotionRandom, // motion rand (from 0 to 1)
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
		const FLOAT_VECTOR  & _intColor
		const FLOAT_VECTOR  & _intAlpha



 */


CParticleEmitter 		*CParticleSystem::pe_stars;
CParticleEmitter 		*CParticleSystem::pe_dots;
CParticleEmitter 		*CParticleSystem::pe_streaks;
CParticleSystem			*CParticleSystem::ps_starBall2;
//class CParticleSystem;
//CParticleSystem *CParticleSystem::m_psStarBall2 = NULL;

CParticleSystem * CParticleSystem::getStarBallPS()
{
	uint32_t base_ap_attributes =CAppearance::WITH_TRANSPARENCY;/*CAppearance.NO_DEPTH_BUFFER_TEST |
							CAppearance.NO_EMIT_REFLECTION_LIGHT |
							 CAppearance.WITH_TRANSPARENCY;*/

	 if(ps_starBall2 == NULL)
	 {
/*

 		const string & _name_ps ,
		uint32_t _life ,
		uint32_t _numberParticles,
		uint32_t _size,
		uint32_t _velocity,
		uint32_t _weight,
		uint32_t _spinVelocity,
		uint32_t _motionRandom,
		uint32_t _bounce,
		uint32_t _zoom,
		uint32_t _visibility,
		uint32_t _tintStrenght,
		uint32_t _emissionAngle,
		uint32_t _emissionRange


 */



			ps_starBall2 = NEW_PARTICLE_SYSTEM();

			ps_starBall2->setup(

					100, // xLIFE (x)
					180, // xNUM PARTICLES
					240, //  xSIZE (x)
					345, // xVELOCITY (x)
					0, // xWEIGHT
					100, //  xSPIN (x)
					100, // xMOTION
					100, // xBOUNCE
					1, // xZOOM (Scale)
					50, // xVISIBILITY (TRANSP)
					0, // xTINT STRENGHT
					0, // xEMISSION ANGLE
					360 // xEMISSION RANGE
			);

			ImageObject *p_image = NEW_IMAGE("star.png");

			if(p_image->load(star_png,star_png_len, 0)){
				NEW_TEXTURE("star.png")->setImage(p_image);
			}


			pe_stars = NEW_PARTICLE_EMITTER();
			pe_stars->load(
					ps_starBall2,
						CParticle::KEEP_ALIGN_TO_MOTION, //CParticle::ONE_PARTICLE  | CParticle::UNLIMITED_LIFE,
						base_ap_attributes | CAppearance::INTENSITY,
						0, // angle particle
						FLOAT_VECTOR{-0.40f,0,0}, // point ref
						"star.png",
						//---------- Basic
						0.18f, // life
						2.00f, // number
						0.20f, // Size
						0.10f, // Velocity
						0, // weight
						0, // spin
						0, // motion
						0.10f, // bounce
						1.00f, // visibility
						//---------- Variations
						0.20f, // life variation
						0.15f, // number variation
						0.25f, //size variation
						0.20f, // velocity variation
						0, // weight variation
						0, // spin variation
						0.35f, // motion variation
						0, // bounce variation
						// controllers...
						FLOAT_VECTOR{ // _intLife,
							1,0,
							1,1
						},
						FLOAT_VECTOR{// & _intVelocity,
                            0.20f,0.00f,
                            0.25f,0.10f,
                            0.80f,0.38f,
                            1.60f,0.55f,
                            1.90f,0.70f,
                            2.00f,0.80f,
                            2.00f,1.00f
						},
						FLOAT_VECTOR{//_intSize,
	                           2.0f	,0.00f,
	                           1.8f	,0.10f,
	                           1.4f	,0.30f,
	                           0.7f	,0.49f,
	                           0.5f	,0.55f,
	                           0.25f,0.70f,
	                           0.0f	,1.00f
						},
						FLOAT_VECTOR{//  & _intMotionRandom=
							1,0,
							1,1

						},
						FLOAT_VECTOR{//  & _intWeight,
							1,0,
							1,1
						},
						FLOAT_VECTOR//  & _intBounce
						{
							1,0,
							1,1
						},

						// appearance ...
						FLOAT_VECTOR // color
						{
							113.0f/256.0f,112.0f/256.0f,1.0f, 0,
							113.0f/256.0f,112.0f/256.0f,1.0f, 1
						},
						FLOAT_VECTOR{//  & _intAlpha=
							0.00f,0.00f,
							1.00f,0.15f,
							1.00f,0.20f,
							0.00f,0.50f,
							0.00f,1.00f

						}




					);

			// life / velocity ...
			//ep_big_glow.addPointSizeOverLife([2.0],0.0  );
			//ep_big_glow.addPointSizeOverLife([2.0],1.0 );


			p_image = NEW_IMAGE("dots.png");

			if(p_image->load(dots_png,dots_png_len, 0)){
					NEW_TEXTURE("dots.png")->setImage(p_image);
			}


			pe_dots = NEW_PARTICLE_EMITTER();

			pe_dots->load(
					ps_starBall2,
						CParticle::RANDOM_REF_POINT, //CParticle::ONE_PARTICLE  | CParticle::UNLIMITED_LIFE,
						base_ap_attributes | CAppearance::INTENSITY,
						0, // angle particle
						FLOAT_VECTOR{0,0,0}, // point ref
						"dots.png",
						//---------- Basic
						0.18f, // life
						0.50f, // number
						0.60f, // Size
						0.10f, // Velocity
						0, // weight
						0, // spin
						0.38f, // motion
						0.10f, // bounce
						1.0f, // visibility
						//---------- Variations
						0.20f, // life variation
						0.18f, // number variation
						0.22f, //size variation
						0.15f, // velocity variation
						0.40f, // weight variation
						0, // spin variation
						0, // motion variation
						0, // bounce variation
						// controllers...
						FLOAT_VECTOR{ // _intLife,
							1,0,
							1,1
						},
						FLOAT_VECTOR{// & _intVelocity,
                            0.20f,0.00f,
                            0.25f,0.10f,
                            0.80f,0.38f,
                            1.60f,0.55f,
                            1.90f,0.70f,
                            2.00f,0.80f,
                            2.00f,1.00f
						},
						FLOAT_VECTOR{//_intSize,
	                           2.0f	,0.0f,
	                           1.8f	,0.1f,
	                           1.4f	,0.3f,
	                           0.7f	,0.49f,
	                           0.5f	,0.55f,
	                           0.25f,0.7f,
	                           0.0f	,1.0f
						},
						FLOAT_VECTOR{//  & _intMotionRandom=
							1,0,
							1,1

						},
						FLOAT_VECTOR{//  & _intWeight,
							1,0,
							1,1
						},
						FLOAT_VECTOR//  & _intBounce
						{
							1,0,
							1,1
						},

						// appearance ...
						FLOAT_VECTOR // color
						{
							113.0f/256.0f,112.0f/256.0f,1.0f, 0,
							113.0f/256.0f,112.0f/256.0f,1.0f, 1
						},
						FLOAT_VECTOR{//  & _intAlpha=
							0.0f,0.00f,
							1.0f,0.20f,
							0.0f,0.50f,
							0.0f,1.00f

						}




					);



			p_image = NEW_IMAGE("streaks.png");

			if(p_image->load(streaks_png,streaks_png_len, 0)){
				NEW_TEXTURE("streaks.png")->setImage(p_image);
			}


			pe_streaks = NEW_PARTICLE_EMITTER();
			pe_streaks->load(
					ps_starBall2,
						CParticle::KEEP_ALIGN_TO_MOTION, //CParticle::ONE_PARTICLE  | CParticle::UNLIMITED_LIFE,
						base_ap_attributes | CAppearance::INTENSITY,
						0, // angle particle
						FLOAT_VECTOR{-0.40f,0,0}, // point ref
						"streaks.png",
						//---------- Basic
						0.28f, // life
						1.00f, // number
						0.70f, // Size
						0.08f, // Velocity
						0, // weight
						0, // spin
						0, // motion
						0.10f, // bounce
						1.0f, // visibility
						//---------- Variations
						0.20f, // life variation
						0.18f, // number variation
						0.20f, //size variation
						0.15f, // velocity variation
						0, // weight variation
						0, // spin variation
						0, // motion variation
						0, // bounce variation
						// controllers...
						FLOAT_VECTOR{ // _intLife,
							1,0,
							1,1
						},
						FLOAT_VECTOR{// & _intVelocity,
                            0.20f,0.00f,
                            0.25f,0.10f,
                            0.80f,0.38f,
                            1.60f,0.55f,
                            1.90f,0.70f,
                            2.00f,0.80f,
                            2.00f,1.00f
						},
						FLOAT_VECTOR{//_intSize,
	                           2.0f	,0.0f,
	                           1.8f	,0.1f,
	                           1.4f	,0.3f,
	                           0.7f	,0.49f,
	                           0.5f	,0.55f,
	                           0.25f,0.7f,
	                           0.0f	,1.0f
						},
						FLOAT_VECTOR{//  & _intMotionRandom=
							1,0,
							1,1

						},
						FLOAT_VECTOR{//  & _intWeight,
							1,0,
							1,1
						},
						FLOAT_VECTOR//  & _intBounce
						{
							1,0,
							1,1
						},

						// appearance ...
						FLOAT_VECTOR // color
						{
							113.0f/256.0f,112.0f/256.0f,1.0f, 0,
							113.0f/256.0f,112.0f/256.0f,1.0f, 1
						},
						FLOAT_VECTOR{//  & _intAlpha=
							0.00f,0.00f,
							1.00f,0.20f,
							1.00f,0.50f,
							0.00f,0.51f,
							0.00f,1.00f

						}




					);


		// add particle emitters ...

			//ps_starBall2->addChild(pe_copyGlowFlare);

			//ps_starBall2->addChild(pe_glowFlare);

			//ps_starBall2->addChild(pe_bigGlow);
			//ps_starBall2->addChild(pe_glowFlare);
			//ps_starBall2->addChild(pe_copyGlowFlare);
			//ps_starBall2->addChild(pe_streaks);
			ps_starBall2->addChild(pe_stars);
			ps_starBall2->addChild(pe_dots);
			ps_starBall2->addChild(pe_streaks);
			//ps_starBall2->addChild(pe_copyGlowFlare);


			//pe_bigGlow->setTranslate(0,0,0);

			//pe_glowFlare->setTranslate(0,0,0);


		//ps_starBall2->addChild(pe_glowFlare);

		//ps_starBall2->addChild(pe_tail);

		ZG_LOG_INFO("start ball created!");

	}


	 return ps_starBall2;

}



void CParticleSystem::destroyStarBallPS()
{
	// destroy emitters...
	if(ps_starBall2 != NULL)
	{
		delete ps_starBall2;
		ps_starBall2 = NULL;
	}

}



#include "../../../resources/image/big_glow.png.c"
#include "../../../resources/image/glow_flare.png.c"
#include "../../../resources/image/trail.png.c"
#include "factory/psystem_factory.h"


/*


		const string & name_emitter,

		uint32_t _attribs,
		uint32_t _ap_attribs,
		uint32_t _angle,

		CVector3f color,
		CTexture *p_image,
		//-------- MAIN PARAMS
		float _percentLife, // life inf millis (more life more tail)...
		float _percentNumParticles, // num_ particles...
		float _percentSize, // size...
		float _percentVelocity, // velocity
		float _percentWeight, // weight (from -1 to 1)
		float _percentSpin, // spin  (from -1 to 1)
		float _percentMotionRandom, // motion rand (from 0 to 1)
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
		const FLOAT_VECTOR  & _intColor
		const FLOAT_VECTOR  & _intAlpha



 */


CParticleEmitter 		*CParticleSystem::pe_glowFlare;
CParticleEmitter 		*CParticleSystem::pe_bigGlow;
CParticleEmitter 		*CParticleSystem::pe_copyGlowFlare;
CParticleEmitter 		*CParticleSystem::pe_tail;
CParticleSystem			*CParticleSystem::ps_starTrail;
//class CParticleEmitterFactory;
//CParticleSystem *CParticleSystem::m_psStarTrail = NULL;

CParticleSystem * CParticleSystem::getStarTrailPS()
{
	uint32_t base_ap_attributes =CAppearance::TRANSPARENCY;/*CAppearance.NO_DEPTH_BUFFER_TEST |
							CAppearance.NO_EMIT_REFLECTION_LIGHT |
							 CAppearance.WITH_TRANSPARENCY;*/

	 if(ps_starTrail == NULL)
	 {
/*

 		const string & _name_ps ,
		uint32_t _life ,
		uint32_t _numberParticles,
		uint32_t _size,
		uint32_t _velocity,
		uint32_t _weight,
		uint32_t _spinVelocity,
		uint32_t _motionRandom,
		uint32_t _bounce,
		uint32_t _zoom,
		uint32_t _visibility,
		uint32_t _tintStrenght,
		uint32_t _emissionAngle,
		uint32_t _emissionRange


 */



			ps_starTrail = NEW_PARTICLE_SYSTEM();

			ps_starTrail->setup(
					100, // xLIFE (x)
					200, // xNUM PARTICLES
					60, //  xSIZE (x)
					150, // xVELOCITY (x)
					300, // xWEIGHT
					55, //  xSPIN (x)
					100, // xMOTION
					100, // xBOUNCE
					1, // xZOOM (Scale)
					100, // xVISIBILITY (TRANSP)
					0, // xTINT STRENGHT
					0, // xEMISSION ANGLE
					360 // xEMISSION RANGE
			);





			ImageObject *p_image = NEW_IMAGE("big_glow_isample.png");

			if(p_image->load(big_glow_png,big_glow_png_len,0)){
				NEW_TEXTURE("big_glow_isample.png")->setImage(p_image);
			}





			pe_bigGlow = NEW_PARTICLE_EMITTER();
			pe_bigGlow->load(
						ps_starTrail,
						CParticle::ONE_PARTICLE  | CParticle::UNLIMITED_LIFE,
						base_ap_attributes,
						0, // angle particle
						FLOAT_VECTOR{0,0,0}, // point ref
						"big_glow_isample.png",
						//---------- Basic
						0.1f, // life
						0.45f,// number
						0.70f, // Size
						0,// velocity
						0,// weight
						0,//spin
						0,// motion
						0.09f, // bounce
						1.0f, // visibility
						//---------- Variations
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						0,
						// controllers...
						FLOAT_VECTOR{ // _intLife,
							0,1,
							0,1
						},
						FLOAT_VECTOR{// & _intVelocity,
							0,1,
							1,1,
						},
						FLOAT_VECTOR{//_intSize,
							0,2,
							1,2
						},
						FLOAT_VECTOR{//  & _intMotionRandom=
							0,1,
							1,1

						},
						FLOAT_VECTOR{//  & _intWeight,
							0,1,
							1,1
						},
						FLOAT_VECTOR//  & _intBounce
						{
							0,1,
							1,1
						},

						// appearance ...
						FLOAT_VECTOR // color
						{
							0,256,77,112,
							1,256,77,112
						},
						FLOAT_VECTOR{//  & alpha=
							0,1,
							1,1

						}




					);

			// life / velocity ...
			//ep_big_glow.addPointSizeOverLife([2.0],0.0  );
			//ep_big_glow.addPointSizeOverLife([2.0],1.0 );

		p_image = NEW_IMAGE("glow_flare_isample.png");
		if(p_image->load(glow_flare_png,glow_flare_png_len,0)){
			NEW_TEXTURE("glow_flare_isample.png")->setImage(p_image);
		}

		pe_glowFlare = NEW_PARTICLE_EMITTER();

		pe_glowFlare->load(
			ps_starTrail,
			CParticle::ONE_PARTICLE  | CParticle::UNLIMITED_LIFE,
			base_ap_attributes,
			0, // angle particle
			FLOAT_VECTOR{0,0,0}, // point ref
			"glow_flare_isample.png",
			//---------- Basic
			0.1f,
			0.4f,
			0.6f,
			0.0f,
			0.0f,
			0.1f,
			0.0f,
			0.9f,
			1.0f,
			//---------- Variations

			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			// controllers...
			FLOAT_VECTOR{ // _intLife,
				0,1,
				1,1
			},
			FLOAT_VECTOR{// & _intVelocity,
				0,1,
				1,1
			},
			FLOAT_VECTOR{//_intSize,
				0,2,
				1,2
			},
			FLOAT_VECTOR{//  & _intMotionRandom=
				0,1,
				1,1

			},
			FLOAT_VECTOR{//  & _intWeight,
				0,1,
				1,1
			},
			FLOAT_VECTOR//  & _intBounce
			{
				0,1,
				1,1
			},

			// appearance ...
			FLOAT_VECTOR // color
			{
				0,256,77,112,
				1,256,77,112
			},
			FLOAT_VECTOR{//  /alpha
				0,1,
				1,1

			}
		);







			pe_copyGlowFlare = NEW_PARTICLE_EMITTER();

			pe_copyGlowFlare->load(
				ps_starTrail,
				CParticle::ONE_PARTICLE  | CParticle::UNLIMITED_LIFE,
				base_ap_attributes,
				0, // angle particle
				FLOAT_VECTOR{0,0,0}, // point ref
				"glow_flare_isample.png",//NEW_TEXTURE("glow_flare_isample.png",GET_IMAGE("glow_flare_isample.png")),
				//---------- Basic
				0.1f,	// life
				0.45f,  // number
				0.6f, // size
				0.0f,  // velocity
				0.0f,  // weight
				-0.1f, // spin
				0.0f,  // motion
				0.1f, // bounce
				1.0f, // visibility
				//---------- Variations
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				0,
				// controllers...
				FLOAT_VECTOR{ // _intLife,
					0,1,
					1,1
				},
				FLOAT_VECTOR{// & _intVelocity,
					0,1,
					1,1
				},
				FLOAT_VECTOR{//_intSize,
					0,2,
					1,2
				},
				FLOAT_VECTOR{//  & _intMotionRandom=
					0,1,
					1,1

				},
				FLOAT_VECTOR{//  & _intWeight,
					0,1,
					1,1
				},
				FLOAT_VECTOR//  & _intBounce
				{
					0,1,
					1,1
				},

				// appearance ...
				FLOAT_VECTOR // color
				{
					0,256,256,0,
					1,256,256,0
				},
				FLOAT_VECTOR{ // alpha
					0,1,
					1,1

				}
			);


			p_image = NEW_IMAGE("trail_isample.png");
			if(p_image->load(trail_png,trail_png_len,CImage::FORMAT::NO_FORMAT)){
				NEW_TEXTURE("trail_isample.png")->setImage(p_image);

			}


			pe_tail = NEW_PARTICLE_EMITTER();
			pe_tail->load(
					ps_starTrail,
					0,
					base_ap_attributes | CAppearance::INTENSITY,
					45, // angle particle
					FLOAT_VECTOR{0,0,0}, // point ref
					"trail_isample.png",
					//---------- Basic
					0.3f, // life inf millis (more life more tail)...
					0.5f, // num_ particles...
					0.1f*2, // size...
					0.0f, // velocity
					0.0f, // weight (from -1 to 1)
					0.0f, // spin  (from -1 to 1)
					0.0f, // motion rand (from 0 to 1)
					0.0f, // bounce ?!?!? ... (from 0 to 1)
					1.0f, // Visibility  (from 0 to 1)
					//---------- Variations  (all from 0 to 1)
					0.0f, // life
					0.22f, // number particles variation
					0.22f, // size variations
					0.4f, // velocity variations // contant over time.
					0.0f,  // weight var
					0.0f,  // spin var
					0.71f, // motion rand
					0.0f,  // bounce var
					// controllers...
					FLOAT_VECTOR{ // _intLife,
						0,1,
						1,1
					},
					FLOAT_VECTOR{// & _intVelocity,
						0,0.8f,
						1,2.0f
					},
					FLOAT_VECTOR{//_intSize,
						0	 ,1.6f,
						0.42f,1.4f,
						0.62f,1.2f,
						0.8f ,0.8f,
						1    ,0.0f
					},
					FLOAT_VECTOR{//  & _intMotionRandom=
						0,1,
						1,1

					},
					FLOAT_VECTOR{//  & _intWeight,
						0,1,
						1,1
					},
					FLOAT_VECTOR//  & _intBounce
					{
						0,1,
						1,1
					},

					// appearance ...
					FLOAT_VECTOR // color
					{
						0    ,256,128,151, // At middle of life must be green ...
						0.33f,256,140, 39, // At middle of life must be green ...
						0.66f,256,251,164, // At middle of life must be green ...
						0.90f,  0,  0,  0,
						1.0f ,  0,  0,  0

					},

					FLOAT_VECTOR // alpha
					{
							0	,0,
							0.33,1,
							0.66,1,
							0.90,0,
							1.0	,0
					}

			);



		// add particle emitters ...
	 	ps_starTrail->addChild(pe_bigGlow);
		ps_starTrail->addChild(pe_glowFlare);
		ps_starTrail->addChild(pe_copyGlowFlare);
		ps_starTrail->addChild(pe_tail);



	}


	 return ps_starTrail;

}



void CParticleSystem::destroyStarTrailPS()
{
	// destroy emitters...
	if(ps_starTrail != NULL)
	{
		delete ps_starTrail;
		ps_starTrail = NULL;
	}

}
