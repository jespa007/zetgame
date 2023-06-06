#include "zg_graphics.h"

void Material_GL_Apply(Material * material){
	// _GL_FRONT: affect only front facing triangles

	/*if(material->properties & MATERIAL_LIGHT){
		glMaterialfv(GL_FRONT, GL_EMISSION , (float *)&material->emissive);//emission.data_buffer);
		glMaterialfv(GL_FRONT, GL_DIFFUSE  , (float *)&material->diffuse);
		glMaterialfv(GL_FRONT, GL_AMBIENT  , (float *)&material->ambient);
		glMaterialfv(GL_FRONT, GL_SPECULAR , (float *)&material->specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, (float *)&material->shininess);
	}else{*/
	glColor4f(material->color.r,material->color.g,material->color.b,material->color.a);
	//}
}
