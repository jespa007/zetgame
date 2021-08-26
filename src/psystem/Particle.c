#include "psystem_src.h"

float VELOCITY_FACTOR	  = 0.002f*2; // a bigger value you put faster will be.
float STEPS_TO_DIE		  = 1/VELOCITY_FACTOR;
float factor_360_to_fixed = 65536.0f/360.0f;

CParticle::CParticle(
		float 			 *_translate_pointer,
		float 			 *_scale_pointer,
		float 			 *_rotate_pointer,
		unsigned char 			 *_color_pointer,
		Uint32			 _m_attribs,
		Uint32			_particle_angle,
		const CVector3f 		&  _ref_Pos, // relative position from particle emitter ..
		CParticleEmitter *_m_emitter,
		CInterpolator1d *_m_intVelocity,
		CInterpolator1d *_m_intWeight,
		CInterpolator1d *_m_intSize,
		CInterpolator1d *_m_intMotionRandom,
		CInterpolator3d *_m_intColor,
		CInterpolator1d *_m_intAlpha


):
m_color_from_interpolator(3),
absolute_scale_from_interpolator(1),
m_alpha_from_interpolator(1),
 m_velocity_from_interpolator(1),
 m_weight_from_interpolator(1),
 m_random_motion_from_interpolator(1)
{

	m_translate=_translate_pointer;
	m_scale=_scale_pointer;
	m_rotate=_rotate_pointer;
	m_color=_color_pointer;


	m_geometry 	= _m_emitter->m_geomParticle;
	m_appearance= _m_emitter->m_appParticle;
	//relative_translate = t3d_relative.getTranslateRef();
	//relative_scale = t3d_relative.getScaleRef();
	//relative_rotate.set(0,0,0);



	m_parent = _m_emitter;
	m_intAlpha = _m_intAlpha;
	m_intVelocity = _m_intVelocity;
	m_intWeight = _m_intWeight;
	m_intSize = _m_intSize;
	m_intMotionRandom = _m_intMotionRandom;
	m_intColor = _m_intColor;
	m_attribs = _m_attribs;

	CTexture *text = m_appearance->getTexture();

	m_aspectX = 1.0f;
	m_aspectY = 1.0f;
	*m_scale = 1;

	if(text!=NULL) {

		m_aspectX = ZG_ACTIVE_RENDER->width_to_projection(text->getWidth());//((float)text->getWidth()/(float)m_render->getWidth());
		m_aspectY = ZG_ACTIVE_RENDER->width_to_projection(text->getHeight());//((float)text->getHeight()/(float)m_render->getWidth());
	}

	this->m_refPoint.set(_ref_Pos);
	this->m_particle_angle_z = _particle_angle;

}

void CParticle::startParticle(

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
)
{

	if( ((m_attribs & RANDOM_REF_POINT) == RANDOM_REF_POINT)
		|| ((m_attribs & KEEP_ALIGN_TO_MOTION) == KEEP_ALIGN_TO_MOTION)
	)
		m_particle_angle_z=(float)(unifRand()*360.0f);


#if 0
	if((bound_rect2f = ps.getRectangle2DAction())!=null)
	{


		// calculation bounding rectangle ...

		/*bound_rect2f = new CRectangle2f(_ini_pos.x + rect2f_ps.v0.x,
										_ini_pos.y + rect2f_ps.v0.y,
										_ini_pos.x + rect2f_ps.v1.x,
										_ini_pos.y + rect2f_ps.v1.y
										);

		// reposition ...*/
		_ini_pos.x = (float) (unifRand()*(bound_rect2f.v1.x-bound_rect2f.v0.x))*0.5f - (bound_rect2f.v1.x-bound_rect2f.v0.x)*0.25f;
		_ini_pos.y = (float) (unifRand()*(bound_rect2f.v1.y-bound_rect2f.v0.y))*0.5f;// - (bound_rect2f.v1.y-bound_rect2f.v0.y)*0.5f;

		//-bound_rect2f.v0.y*0.25f;//

	}

#endif

	int steps_to_die;

	new_velocity = new_velocity + new_random_motion;

	if(-0.1f <= new_velocity && new_velocity <= 0.1f) {

		if(new_velocity==0)
			steps_to_die = (int)((new_life*10)/VELOCITY_FACTOR);///(new_velocity*VELOCITY_FACTOR));
		else
			steps_to_die = (int)((new_life*10)/fabs(new_velocity*VELOCITY_FACTOR));

		new_life = fabs(new_life)/(0.25f);
		new_velocity = 0.0f;

		steps_to_die = MIN(100, steps_to_die);
	}
	else {

		new_life = fabs(new_life * new_velocity)/(0.25f*4);
		// dest point!!!
		steps_to_die = (int)(new_life/(fabs(new_velocity)*VELOCITY_FACTOR));
	}

	m_visibility = new_visibility;
	m_life_to_die = steps_to_die; //new_life;
	m_life_to_die_over_1_percent = 1.0f;
	m_step_life_to_die_over_1_percent = 0.0f;

	if(steps_to_die !=0)
		m_step_life_to_die_over_1_percent = 1.0f/steps_to_die; //new_dtime/new_life;///life_to_die_over_1_percent;

	m_ini_pos.set(_ini_pos);
	m_inc_spin = new_inc_spin*factor_360_to_fixed;
	m_velocity = (new_velocity)*VELOCITY_FACTOR*2; //new_velocity*0.2f;//*0.1f;///FACTOR;
	m_vdir.x = _vect_dir.x;
	m_vdir.y = _vect_dir.y;
	m_vdir.z = _vect_dir.z;
	m_sizeParticle = new_size;

	//setScale(m_sizeParticle);
	*m_scale=m_sizeParticle;
	*m_rotate = m_particle_angle_z;
	it_motion.x=0;
	it_motion.y=0;
	it_motion.z=0;

	//--------------- calcule new vdir
	// perform ref point trans...
	m_from_position.set(_ini_pos);

	m_translate[0]=_ini_pos.x;
	m_translate[1]=_ini_pos.y;
	m_translate[2]=_ini_pos.z;

	if((m_attribs & KEEP_ALIGN_TO_MOTION) == KEEP_ALIGN_TO_MOTION) {


		// After this, get transformed position...
		// Calcule inc vect...
		m_vdir.sub(m_refPoint);
		m_vdir.normalize();
	}

	m_total_elapsed_time = m_elapsed_time = 0;
	m_t_random = new_random_motion;// 0.03f; // by default..
	m_previus_gravity = actual_gravity = 0.0f;

	m_weight = new_weight;
	m_spin = 0.0f;

	m_life      	= m_life_to_die;//*new_life_mul;
	m_dtime 		= 0.33f;//new_dtime;
}

void CParticle::update() {

	CRender *m_render = ZG_ACTIVE_RENDER;
	float size_variation;
	float mod_weight,
		   mod_velocity,
		   mod_motion_rand,
		   aux_time;
	SDL_Color rgb_int;


		if(m_life_to_die_over_1_percent > 0.0f) {

			if(m_inc_spin != 0.0f){
				//addRotateZ(m_inc_spin);
				*m_rotate +=m_inc_spin;
			}else{
				//setRotateZ(m_particle_angle_z+m_parent->getRotate(CScreenObject::WORLD_COORDINATES).z);
				*m_rotate = m_particle_angle_z+m_parent->getRotate(CScreenObject::WORLD_COORDINATES).z;
			}



			relative_rotate_z_fp = CLuts::degrees2fixed(*m_rotate );

			aux_time = 1.0f-m_life_to_die_over_1_percent;

			m_color_from_interpolator[0]=0xff;
			m_color_from_interpolator[1]=0xff;
			m_color_from_interpolator[2]=0xff;

			m_intColor->getInterpolatedPoint(aux_time, m_color_from_interpolator);

			SDL_html2rgb(m_color_from_interpolator[0], rgb_int);

			m_intSize->getInterpolatedPoint(aux_time, absolute_scale_from_interpolator);


			size_variation = absolute_scale_from_interpolator[0];

			if(size_variation < 0.0f)
				size_variation = 0;

			float m_totalVisibility = CMaterial::ALPHA_VALUE_SOLID;

			m_totalVisibility = CMaterial::ALPHA_VALUE_SOLID;

			m_alpha = CMaterial::ALPHA_VALUE_SOLID;

			if(m_intAlpha->getInterpolatedPoint(aux_time, m_alpha_from_interpolator)) {
				m_alpha=(m_alpha)*(m_alpha_from_interpolator[0]);
			}

			// and multiply by the ps.
			m_alpha*=(m_alpha)*(m_totalVisibility);

			// updates color to array...
			m_color[0]=m_color_from_interpolator[0]*255;
			m_color[1]=m_color_from_interpolator[1]*255;
			m_color[2]=m_color_from_interpolator[2]*255;
			m_color[3]=m_alpha*255;

			// updates scale...
			 *m_scale = m_aspectX*size_variation*m_sizeParticle*m_parent->getScale(CScreenObject::WORLD_COORDINATES).x;

			//relative_scale->set(m_aspectX*(*m_scale), m_aspectY*(*m_scale),(*m_scale));

			mod_weight = m_weight;//*0.000098f;
			mod_motion_rand = m_t_random;

			m_intWeight->getInterpolatedPoint(aux_time, m_weight_from_interpolator);

			mod_weight = (m_weight*0.5f+(m_weight*m_weight_from_interpolator[0]*0.5f))*0.000098f;
			m_intMotionRandom->getInterpolatedPoint(aux_time, m_random_motion_from_interpolator);
			mod_motion_rand = m_t_random*m_random_motion_from_interpolator[0];
			mod_velocity = m_velocity;

			m_intVelocity->getInterpolatedPoint(aux_time, m_velocity_from_interpolator);
			mod_velocity=(((m_velocity)*m_velocity_from_interpolator[0]));// + mod_motion_rand)*0.5f;

			if((ONE_PARTICLE & m_attribs) != ONE_PARTICLE){ //&& wait_time.End()) {
				if((m_attribs & KEEP_ALIGN_TO_MOTION) != KEEP_ALIGN_TO_MOTION &&
				   (m_attribs & ATTACHED_TO_EMITTER) != ATTACHED_TO_EMITTER) {

					if( (m_elapsed_time > 3)) {

						m_elapsed_time = m_dtime;
						m_total_elapsed_time+=1;

						m_previus_gravity = actual_gravity;

						m_from_position.x=m_translate[0];
						m_from_position.y=m_translate[1];
						m_from_position.z=m_translate[2];

						it_motion.x=0;
						it_motion.y=0;
						it_motion.z=0;

						float rand_05 = mod_motion_rand*0.5f;
						// Change de vdir...
						m_vdir.x += (unifRand()*mod_motion_rand - rand_05);
						m_vdir.y += (unifRand()*mod_motion_rand - rand_05);
						m_vdir.z += (unifRand()*mod_motion_rand - rand_05);
						//m_vdir.z=0;
					}

					float parent_scale = 1;//m_parent->getScale(CScreenObject::WORLD_COORDINATES).x;
					float mul_elapsed_vel= m_elapsed_time*(mod_velocity*parent_scale);//+mod_motion_rand);
					actual_gravity = (mod_weight*m_total_elapsed_time*m_total_elapsed_time)*0.5f;
					m_translate[0] = m_from_position.x+it_motion.x;
					m_translate[1] = m_from_position.y+it_motion.y;
					m_translate[2] = m_from_position.z+it_motion.z;
					//relative_translate->z = m_ini_pos.z;// m_from_position.z;

					it_motion.x+=(float) ( m_vdir.x*mul_elapsed_vel);
					it_motion.y+=(float) ( m_vdir.y*mul_elapsed_vel-(actual_gravity-m_previus_gravity));
					it_motion.z+=(float)( m_vdir.z*mul_elapsed_vel);

				}
				else {

					// aligned to motion (simple transform its position on camera space)

					CVector3f pos;

					// perform the operation in its space...
					pos = this->m_refPoint;
					pos.scale((*m_scale)*1);
					CMatrix3f::GetRotateMatrix_Z(-OFFSET_LUT(relative_rotate_z_fp))->transform(pos);

					CCamera *camera=CCameraFactory::getDefaultResource();
					if(m_parent->getSystemParticle()->getScene()!=NULL){
						camera = m_parent->getSystemParticle()->getScene()->getCamera();
					}

					camera->getInverseTransformMatrix()->transform(pos);


					m_translate[0]=pos.x;
					m_translate[1]=pos.y;
					m_translate[2]=pos.z;


				}
			}

			if((ONE_PARTICLE & m_attribs) != ONE_PARTICLE /*&& wait_time.End()*/) {

				m_elapsed_time++;//=dtime;//= //(life_to_die-life);
				m_total_elapsed_time++;
				m_life--; //=dtime;
				m_life_to_die_over_1_percent -= m_step_life_to_die_over_1_percent;

				if(m_life <= 0.0f) {
				#if 0 // bounding rect
					if(bound_rect2f != null)
					{
					//wait_time.Start((int)(unitRand()*1000)+1000);
						if(relative_translation->x < bound_rect2f.v0.x ||
						   relative_translation->x > bound_rect2f.v1.x ||
						   relative_translation->y < bound_rect2f.v0.y ||
						   relative_translation->y > bound_rect2f.v1.y
						) // the particles dies ...
						{
							m_life = 0.0f;
							m_life_to_die_over_1_percent = 0.0f;
							//system.print_debug("die");


						}
						else // refills life ...
						{

							m_life = 1;
							m_life_to_die_over_1_percent = m_step_life_to_die_over_1_percent;
						}
					}
					else // the particle die ...
				#endif
					{
						m_life = 0.0f;
						m_life_to_die_over_1_percent = 0.0f;
					}
				}
			}
			else {
				m_translate[0]=m_parent->getTranslate(CScreenObject::WORLD_COORDINATES).x;
				m_translate[1]=m_parent->getTranslate(CScreenObject::WORLD_COORDINATES).y;
				m_translate[2]=m_parent->getTranslate(CScreenObject::WORLD_COORDINATES).z;

			}

			// only draws one particle (trail is rendered at the end of the loop
			if(m_alpha > 0 && ((ONE_PARTICLE & m_attribs) == ONE_PARTICLE)) {//this->m_appearance->getOpacity() > 0.0f)

				CCamera *camera=CCameraFactory::getDefaultResource();
				if(m_parent->getSystemParticle()->getScene()!=NULL){
					camera = m_parent->getSystemParticle()->getScene()->getCamera();
				}

				trans_aux.x = m_translate[0];
				trans_aux.y = m_translate[1];
				trans_aux.z = m_translate[2];
				m_render->putObjectToDraw(trans_aux,this,camera);
				//draw();
			}
			//------------------------ END UPDATE OBJECT-------------------------------------
		}
}

void CParticle::drawOneParticle() {

    glPushMatrix();
    {


		//billboardBegin();
		{
			glTranslatef(m_translate[0],m_translate[1], m_translate[2]); // down a bit

			CCamera *camera=CCameraFactory::getDefaultResource();
			if(m_parent->getSystemParticle()->getScene()!=NULL){
				camera = m_parent->getSystemParticle()->getScene()->getCamera();
			}

			CMatrix3f m3;


			m3.set(*camera->getTransformMatrix3x3());
			m3.mul(*CMatrix3f::GetRotateMatrix_Z(OFFSET_LUT(relative_rotate_z_fp)));

			float *m_array=m3.getArray();

			float pp [] =
				{
						m_array[0],m_array[1], m_array[2],0,
						m_array[3],m_array[4], m_array[5],0,
						m_array[6],m_array[7], m_array[8],0,
						0.0f, 0.0f,	  0.0f,	1.0f


				};
			glMultTransposeMatrixf(pp);
			//glRotatef(*m_rotate,0,0,1);
			glScalef(*m_scale, *m_scale,1);

			CMaterial *m_material= m_appearance->getMaterial();

			if(m_material){
				m_material->setColor3i(m_color[0],m_color[1],m_color[2]);
				m_material->setOpacity(m_color[3]);
			}


			// set material or appearance ...
			m_appearance->apply();

			if(m_geometry != NULL)
				m_geometry->drawGeometry();


			// restore attribs app
			m_appearance->restore_attribs();

		}
		//billboardEnd();

	}
	glPopMatrix();

}

void CParticle::draw() {

	if(((CParticle::ONE_PARTICLE & m_attribs) == CParticle::ONE_PARTICLE)){
		drawOneParticle();
	}

}


CParticle::~CParticle(){

}

