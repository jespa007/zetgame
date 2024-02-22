

typedef struct{
	GLuint	index, 	// index object id
			vertex,	// vertex object id
			color, 	// vertex object id
			texture,// texture object id
			normal; // normal object id
}ZG_GeometryDataGL;

void  ZG_Geometry_GL_New(ZG_Geometry *geometry){

	ZG_GeometryDataGL * data= ZG_NEW(ZG_GeometryDataGL);
	*data=(ZG_GeometryDataGL){
		.index = GL_INVALID_VALUE, 	// index object id
		.vertex = GL_INVALID_VALUE,	// vertex object id
		.color = GL_INVALID_VALUE, 	// vertex object id
		.texture = GL_INVALID_VALUE,// texture object id
		.normal = GL_INVALID_VALUE // normal object id
	};

	// generate VBO for index and vertex...
	glGenBuffers(1, &data->index);
	glGenBuffers(1, &data->vertex);

	geometry->data=data;
}


void ZG_Geometry_GL_SetIndices(ZG_Geometry * _geometry, const uint32_t *indices,size_t indices_len) {

	if(_geometry==NULL) return;

	ZG_GeometryDataGL * data = (ZG_GeometryDataGL *)_geometry->data;


	if(data->index == GL_INVALID_VALUE) {
		ZG_LOG_ERRORF("Index VBO not build");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_len*sizeof(uint32_t), indices, GL_STATIC_DRAW);

}


void ZG_Geometry_GL_SetMeshVertex(ZG_Geometry *_geometry, const float *_vertexs,size_t _vertexs_len) {

	ZG_GeometryDataGL * data = NULL;

	if(_geometry==NULL ) return;
	/*if(_vertexs_len != (geometry->n_vertexs*ZG_VERTEX_COORDS_LEN)){
		ZG_LOG_ERRORF("Vertex count doesn't matches with indices len. In case of indices has been update set indices first");
		return;
	}*/

	data = (ZG_GeometryDataGL *)_geometry->data;

	if(data->vertex == GL_INVALID_VALUE) {
		ZG_LOG_ERRORF("Vertex VBO not build");
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, data->vertex);
	glBufferData(GL_ARRAY_BUFFER, _vertexs_len*sizeof(float), _vertexs, GL_DYNAMIC_DRAW);

}


void ZG_Geometry_GL_SetMeshTexture(ZG_Geometry * _geometry, const float *_vertexs,size_t _vertexs_len) {
	ZG_GeometryDataGL * data = NULL;

	if(_geometry==NULL) return;
	/*if(mesh_texture_vertexs_len != (geometry->n_vertexs*ZG_TEXTURE_COORDS_LEN)){
		ZG_LOG_ERRORF("Vertex count doesn't matches");
		return;
	}*/


	data = (ZG_GeometryDataGL *)_geometry->data;

	if(data->texture == GL_INVALID_VALUE) {
		//ZG_LOG_ERRORF("ZG_Texture VBO not build or not set in constructor (New_ZG_Geometry)");
		//return;
		// generate VBO for texture
		glGenBuffers(1, &data->texture);
	}

	// reserve GPU memory ...
	glBindBuffer(GL_ARRAY_BUFFER, data->texture);
	glBufferData(GL_ARRAY_BUFFER, _vertexs_len*sizeof(float), _vertexs, GL_DYNAMIC_DRAW);
}



void ZG_Geometry_GL_SetMeshColor(ZG_Geometry * _geometry,const float *_vertexs,size_t _vertexs_len) {

	ZG_GeometryDataGL * data = NULL;

	if(_geometry==NULL) return;
	/*if(mesh_color_vertexs_len != (geometry->n_vertexs*(ZG_COLOR_COORDS_LEN))){
		ZG_LOG_ERRORF("Vertex count doesn't matches");
		return;
	}*/

	data = (ZG_GeometryDataGL *)_geometry->data;

	if(data->color == GL_INVALID_VALUE) {
		//ZG_LOG_ERRORF("Color VBO not build or not set in constructor (New_ZG_Geometry)");
		//return;

		// generate VBO for color
		glGenBuffers(1, &data->color);
		// generate VBO for normal
	}

	// reserve GPU memory ...
	glBindBuffer(GL_ARRAY_BUFFER, data->color);
	glBufferData(GL_ARRAY_BUFFER, _vertexs_len*sizeof(float), _vertexs, GL_DYNAMIC_DRAW);
}


void ZG_Geometry_GL_SetMeshNormal(ZG_Geometry * _geometry,const float *_vertexs,size_t _vertexs_len) {

	ZG_GeometryDataGL * data = NULL;

	data = (ZG_GeometryDataGL *)_geometry->data;

	if(data->normal == GL_INVALID_VALUE) {
		//ZG_LOG_ERRORF("Normal VBO not build or not set in constructor (New_ZG_Geometry)");
		//return;
		glGenBuffers(1, &data->normal);
	}

	// reserve GPU memory ...
	glBindBuffer(GL_ARRAY_BUFFER, data->normal);
	glBufferData(GL_ARRAY_BUFFER, _vertexs_len*sizeof(float), _vertexs, GL_DYNAMIC_DRAW);
}


void ZG_Geometry_GL_Draw(ZG_Geometry * _geometry) {

	if(_geometry==NULL) return;

	ZG_GeometryDataGL * data=(ZG_GeometryDataGL *)_geometry->data;

	if(data==NULL){
		return;
	}

	if(_geometry->indices_length==0){
		return;
	}


	// configure vertex pointer
	if(data->vertex != GL_INVALID_VALUE) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, data->vertex);
		glVertexPointer(ZG_VERTEX_COORDS_LEN, GL_FLOAT, 0, 0L); // configured for XYZ
	}

	// configure texture pointer
	if(data->texture != GL_INVALID_VALUE) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, data->texture);
		glTexCoordPointer(ZG_TEXTURE_COORDS_LEN, GL_FLOAT, 0, 0L); // configured for UV
	}

	// configure color pointer
	if(data->color!= GL_INVALID_VALUE) {
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, data->color);
		glColorPointer(ZG_COLOR_COORDS_LEN, GL_FLOAT, 0, 0L); // configured for RGB
	}

	// configure normal pointer
	if(data->normal!= GL_INVALID_VALUE) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, data->normal);
		glNormalPointer(GL_FLOAT, 0, 0L); //  configured for XYZ
	}


	if(data->index != GL_INVALID_VALUE) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->index);
		GLenum mode=GL_TRIANGLES;
		switch(_geometry->geometry_type){
		default:
			break;
		case ZG_GEOMETRY_TYPE_POINTS:
			mode=GL_POINTS;
			break;
		case ZG_GEOMETRY_TYPE_TRIANGLE_STRIP:
			mode=GL_TRIANGLE_STRIP;
			break;
		case ZG_GEOMETRY_TYPE_LINE_LOOP:
			mode=GL_LINE_LOOP;
			break;

		}

		glDrawElements(mode,_geometry->indices_length,  GL_UNSIGNED_INT, (void *)(NULL));// indicesVertexBuffer->data_buffer);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if(data->normal != GL_INVALID_VALUE){
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if(data->color != GL_INVALID_VALUE){
		glDisableClientState(GL_COLOR_ARRAY);
	}

	if(data->texture != GL_INVALID_VALUE){
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if(data->vertex != GL_INVALID_VALUE){
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void ZG_Geometry_GL_DeInit(ZG_Geometry * geometry) {

	if(geometry==NULL) return;

	ZG_GeometryDataGL *data = (ZG_GeometryDataGL *)geometry->data;


	// generate VBO for index and vertex...
	if(data->index!=GL_INVALID_VALUE){
		glGenBuffers(1, &data->index);
	}

	if(data->vertex!=GL_INVALID_VALUE){
		glGenBuffers(1, &data->vertex);
	}

	// generate VBO for texture
	if(data->texture!=GL_INVALID_VALUE){
		glGenBuffers(1, &data->texture);
	}

	// generate VBO for color
	if(data->color!=GL_INVALID_VALUE){
		glGenBuffers(1, &data->color);
	}

	// generate VBO for normal
	if(data->normal!=GL_INVALID_VALUE){
		glGenBuffers(1, &data->normal);
	}

	// Disable the VBO index from the VAO attributes list
	glDisableVertexAttribArray(0);

	// Delete the vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	ZG_FREE(data);
}



