
//
// Best particle tutorial
//
// https://fivedots.coe.psu.ac.th/~ad/jg/ch12/chap12.pdf
//
// Particle illusion issues.
//
// Size is in function of width.
//
// example:
//
// Size base    = 100% --> 1.0f
// Size emitter = 50%  --> 0.5f (mid)
//
// Then if your render have a vial port of 640 x480 then the size is.
//
// 640*1*0.5 = 480.

#include "screen/zg_screen.h"

#include "ires/starTrail/starTrail.cpp"
#include "ires/starBall2/starBall2.cpp"

void CParticleSystem::destroyDefaultPS(){


	destroyStarTrailPS();
	destroyStarBallPS();

}



//----------------------------------------------------------------------------------------------------------------
//
// PRIVATE FUNCTIONS
//
// internal build ps.

CParticleSystem::CParticleSystem(){


	 BASE_LIFE=
	 INI_BASE_NUMBER_PARTICLES=
	 BASE_NUMBER_PARTICLES=
	 BASE_SIZE=
	 BASE_VELOCITY=
	 BASE_WEIGHT=		 	// % Weight of every particle
	 BASE_SPIN_VELOCITY=	// % Rotate from origin to died end.
	 BASE_MOTION_RAND=    	// Motion rand= affects random weight= velocity= direction in curse of particle...
	 BASE_BOUNCE=		 	//  ?!?!?
	 BASE_ZOOM=			 	// Zoom (Z increases or poligon increases ?)
	 BASE_VISIBILITY=	 	// Visibility of particles
	 BASE_TINT_STRENGHT=  	// Tint strnght: (1.0 --> gris= 0.0f --> original color.
	 BASE_EMISSION_ANGLE= 	// Angle emission in degres!
	 BASE_EMISSION_RANGE= 	// Emission range from 0= to 360�
	 WIDTH_START_RECTANGLE_ACTION=
	 HEIGHT_START_RECTANGLE_ACTION=0.0f;

}

void CParticleSystem::setup(
		Uint32 m_life ,
		Uint32 m_numberParticles,
		Uint32 m_size,
		Uint32 m_velocity,
		Uint32 m_weight,
		Uint32 m_spinVelocity,
		Uint32 m_motionRandom,
		Uint32 m_bounce,
		Uint32 m_zoom,
		Uint32 m_visibility,
		Uint32 m_tintStrenght,
		Uint32 m_emissionAngle,
		Uint32 m_emissionRange
)
{
	/*this->m_psData->m_life = _life;
	this->m_psData->m_numberParticles = _numberParticles;
	this->m_psData->m_size = _size;
	this->m_psData->m_velocity = _velocity;
	this->m_psData->m_weight = _weight;
	this->m_psData->m_spinVelocity = _spinVelocity;
	this->m_psData->m_motionRandom = _motionRandom;
	this->m_psData->m_bounce = _bounce;
	this->m_psData->m_zoom = _zoom;
	this->m_psData->m_visibility = _visibility;
	this->m_psData->m_tintStrenght = _tintStrenght;
	this->m_psData->m_emissionAngle = _emissionAngle;
	this->m_psData->m_emissionRange = _emissionRange;*/

	//setupInternalVars();

	//Luts = CLuts::getInstance();
	BASE_LIFE					=m_life/100.0f;
	INI_BASE_NUMBER_PARTICLES	=m_numberParticles;
    BASE_NUMBER_PARTICLES		=m_numberParticles;
    BASE_SIZE					=(m_size/100.0f);//*0.3f;
    BASE_VELOCITY				= m_velocity/100;
    BASE_WEIGHT					=m_weight/100.0f;
    BASE_SPIN_VELOCITY			=m_spinVelocity/4000.0f;//DIV_FACTOR_VELOCITY;
    BASE_MOTION_RAND			=m_motionRandom/100.0f;
    BASE_BOUNCE					=m_bounce;
    BASE_ZOOM					=m_zoom;
    BASE_VISIBILITY				=m_visibility /100.0f;//*0.50f;///50.0f;
    BASE_TINT_STRENGHT			=m_tintStrenght ;
    BASE_EMISSION_ANGLE			=m_emissionAngle;
    BASE_EMISSION_RANGE			=m_emissionRange;
   // WIDTH_START_RECTANGLE_ACTION  = (_WIDTH_START_RECTANGLE_ACTION/2);
   // HEIGHT_START_RECTANGLE_ACTION = (_HEIGHT_START_RECTANGLE_ACTION/2);
}


bool CParticleSystem::loadData(const string & m_data){

	 CParserJson<tJsonPropertyFile> *m_parser= new CParserJson<tJsonPropertyFile>();
	 bool ok = false;

	 if(m_parser->parseString(m_data)){

		 	 CParserVarPropertyGroup<tJsonPropertyFile> *grp = m_parser->getData()->at(0);
			 string file_to_load = this->getWorkingDirectory() + "/"+ grp->m_file.s_value;


		 	 if(load(file_to_load))//, CImage::FLIP_Y))
		 		 	 ok=true;
	 }

	 delete m_parser;

	return ok;

}

bool CParticleSystem::load(const string & m_filename){

	string file_to_load = this->m_working_dir+"/"+ m_filename;

	if(CObject::load(file_to_load)){
		CParserJson<tParticleSystemData> *m_parser = new CParserJson<tParticleSystemData>();






		if(m_parser->parseFile(file_to_load)){

			m_psData = m_parser->getData()->at(0);

			setup(
					this->m_psData->m_life, // xLIFE (x)
					this->m_psData->m_numberParticles, // xNUM PARTICLES
					this->m_psData->m_size, //  xSIZE (x)
					this->m_psData->m_velocity, // xVELOCITY (x)
					this->m_psData->m_weight, // xWEIGHT
					this->m_psData->m_spinVelocity, //  xSPIN (x)
					this->m_psData->m_motionRandom, // xMOTION
					this->m_psData->m_bounce, // xBOUNCE
					this->m_psData->m_zoom, // xZOOM (Scale)
					this->m_psData->m_visibility, // xVISIBILITY (TRANSP)
					this->m_psData->m_tintStrenght, // xTINT STRENGHT
					this->m_psData->m_emissionAngle, // xEMISSION ANGLE
					this->m_psData->m_emissionRange // xEMISSION RANGE
			);

			for(unsigned i = 0; i < m_psData->m_emitter.size(); i++) {
				if(!addEmitter(*m_psData->m_emitter[i]))
					return false;
			}
		}

		delete m_parser;

		return true;
	}

	return false;

}

/*bool CParticleSystem::addChild(CParticleEmitter * particle_emitter) {

	if(CScreenObject::addChild(particle_emitter)) {
		particle_emitter->setupEmitter(this);
		return true;
	}
	return false;
}*/

bool CParticleSystem::addEmitter(tEmitterInfo & m_peData) {

	CParticleEmitter *pe = CParticleEmitterFactory::getObject(m_peData.m_id,false);

	if(pe == NULL){
		pe=NEW_PARTICLE_EMITTER();
		//pe->setID(m_peData.m_id);
	}

	if(CScreenObject::addChild(pe)) {

		string path_image = CIO_Utils::getFolder(m_resourceFilename)+"/"+CIO_Utils::getFolder(m_peData.m_imageParticle.s_value);
		string image_name = CIO_Utils::getFileName(m_peData.m_imageParticle.s_value);
		// try to load image ...
		//id = CFile::getFilenameFromAbsolutePath(m_peData.m_imageParticle);

		if(CTextureFactory::getObject(m_peData.m_imageParticle,false)==NULL){
			CTexture *text = NEW_TEXTURE();
			CImageObject *img = NEW_IMAGE();
			CTextureFactory::setNewID(m_peData.m_imageParticle, text);
			CImageFactory::setNewID(m_peData.m_imageParticle, img);
			img->setWorkingDirectory(path_image);
			img->load(path_image +"/"+ image_name);
			text->setImage(img);
		}

		pe->loadFromEmitterInfo(this,&m_peData);
		return true;
	}
	return false;
}

//----------------------------------------------------------------------------------------------------------------
//
// PUBLIC FUNCTIONS
//
void CParticleSystem::update() {

	CRender *m_render = ZG_ACTIVE_RENDER;


	// update coord3d  scene graph...
	CScreenObject::updateSceneGraph();

	CCamera *camera=CCameraFactory::getDefaultResource();
	if(m_attached_scene!=NULL){
		camera = m_attached_scene->getCamera();
	}


	m_render->putObjectToDraw(*absolute_translation,this,camera);


	CScreenObject::post_update();

}


void CParticleSystem::draw() {

	string sError;





	for(unsigned i = 0; i < m_child.size(); i++){
		CParticleEmitter *pe;
		pe = dynamic_cast<CParticleEmitter *>(m_child[i]);
		if(pe != NULL){
			//print_info_cr("-");
			pe->updateAndDraw();
		}
	}
}



void CParticleSystem::unload(){
	// erases all
	CObject::unload();

}

CParticleSystem::~CParticleSystem(){
	unload();
}



