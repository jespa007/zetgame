

typedef struct{
	GLuint	index, 	// index object id
			vertex,	// vertex object id
			color, 	// vertex object id
			texture,// texture object id
			normal; // normal object id
}GeometryDataGL;

void  Geometry_GL_New(Geometry *geometry, uint32_t properties){

	GeometryDataGL * data= NEW(GeometryDataGL);
	*data=(GeometryDataGL){
		.index = GL_INVALID_VALUE, 	// index object id
		.vertex = GL_INVALID_VALUE,	// vertex object id
		.color = GL_INVALID_VALUE, 	// vertex object id
		.texture = GL_INVALID_VALUE,// texture object id
		.normal = GL_INVALID_VALUE // normal object id
	};

	// generate VBO for index and vertex...
	glGenBuffers(1, &data->index);
	glGenBuffers(1, &data->vertex);

	// generate VBO for texture
	if(properties & GEOMETRY_PROPERTY_TEXTURE){
		glGenBuffers(1, &data->texture);
	}

	// generate VBO for color
	if(properties & GEOMETRY_PROPERTY_COLOR){
		glGenBuffers(1, &data->color);
	}

	// generate VBO for normal
	if(properties & GEOMETRY_PROPERTY_NORMAL){
		glGenBuffers(1, &data->normal);
	}

	geometry->data=data;
}


void Geometry_GL_SetIndices(Geometry * geometry, const short *indices,size_t indices_len) {

	if(geometry==NULL) return;

	GeometryDataGL * data = (GeometryDataGL *)geometry->data;


	if(indices_len != geometry->index_length){
		Log_Error("Index count doesn't matches (indices:%i vertexs:%i)",indices_len,geometry->index_length);
		return;
	}

	if(data->index == GL_INVALID_VALUE) {
		Log_Error("Index VBO not build");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data->index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_len*sizeof(short), indices, GL_STATIC_DRAW);


}


void Geometry_GL_SetMeshVertex(Geometry *geometry, const float *mesh_vertexs,size_t mesh_vertexs_len) {

	GeometryDataGL * data = NULL;

	if(geometry==NULL ) return;
	if(mesh_vertexs_len != (geometry->n_vertexs*VERTEX_COORDS_LEN)){
		Log_Error("Vertex count doesn't matches");
		return;
	}

	data = (GeometryDataGL *)geometry->data;

	if(data->vertex == GL_INVALID_VALUE) {
		Log_Error("Vertex VBO not build");
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, data->vertex);
	glBufferData(GL_ARRAY_BUFFER, mesh_vertexs_len*sizeof(float), mesh_vertexs, GL_DYNAMIC_DRAW);

}


void Geometry_GL_SetMeshTexture(Geometry * geometry, const float *mesh_texure_vertexs,size_t mesh_texture_vertexs_len) {
	GeometryDataGL * data = NULL;

	if(geometry==NULL) return;
	if(mesh_texture_vertexs_len != (geometry->n_vertexs*TEXTURE_COORDS_LEN)){
		Log_Error("Vertex count doesn't matches");
		return;
	}


	data = (GeometryDataGL *)geometry->data;

	if(data->texture == GL_INVALID_VALUE) {
		Log_Error("Texture VBO not build or not set in constructor (New_Geometry)");
		return;
	}

	// reserve GPU memory ...
	glBindBuffer(GL_ARRAY_BUFFER, data->texture);
	glBufferData(GL_ARRAY_BUFFER, mesh_texture_vertexs_len*sizeof(float), mesh_texure_vertexs, GL_DYNAMIC_DRAW);
}



void Geometry_GL_SetMeshColor(Geometry * geometry,const float *mesh_color_vertexs,size_t mesh_color_vertexs_len) {

	GeometryDataGL * data = NULL;

	if(geometry==NULL) return;
	if(mesh_color_vertexs_len != (geometry->n_vertexs*(COLOR_COORDS_LEN))){
		Log_Error("Vertex count doesn't matches");
		return;
	}

	data = (GeometryDataGL *)geometry->data;

	if(data->color == GL_INVALID_VALUE) {
		Log_Error("Color VBO not build or not set in constructor (New_Geometry)");
		return;
	}
	// reserve GPU memory ...
	glBindBuffer(GL_ARRAY_BUFFER, data->color);
	glBufferData(GL_ARRAY_BUFFER, mesh_color_vertexs_len*sizeof(float), mesh_color_vertexs, GL_DYNAMIC_DRAW);
}


void Geometry_GL_SetMeshNormal(Geometry * geometry,const float *mesh_normal_vertexs,size_t mesh_normal_vertexs_len) {

	GeometryDataGL * data = NULL;

	if(geometry==NULL) return;
	if(mesh_normal_vertexs_len != (geometry->n_vertexs*NORMAL_COORDS_LEN)){
		Log_Error("Vertex count doesn't matches");
		return;
	}

	data = (GeometryDataGL *)geometry->data;

	if(data->normal == GL_INVALID_VALUE) {
		Log_Error("Normal VBO not build or not set in constructor (New_Geometry)");
		return;
	}

	// reserve GPU memory ...
	glBindBuffer(GL_ARRAY_BUFFER, data->normal);
	glBufferData(GL_ARRAY_BUFFER, mesh_normal_vertexs_len*sizeof(float), mesh_normal_vertexs, GL_DYNAMIC_DRAW);
}


void Geometry_GL_Draw(Geometry * geometry) {
	if(geometry==NULL) return;
	GeometryDataGL * data=(GeometryDataGL *)geometry->data;

	if(data==NULL){
		return;
	}


	// configure vertex pointer
	if(data->vertex != GL_INVALID_VALUE) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, data->vertex);
		glVertexPointer(VERTEX_COORDS_LEN, GL_FLOAT, 0, 0L); // configured for XYZ
	}

	// configure texture pointer
	if(data->texture != GL_INVALID_VALUE) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, data->texture);
		glTexCoordPointer(TEXTURE_COORDS_LEN, GL_FLOAT, 0, 0L); // configured for UV
	}

	// configure color pointer
	if(data->color!= GL_INVALID_VALUE) {
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, data->color);
		glColorPointer(COLOR_COORDS_LEN, GL_FLOAT, 0, 0L); // configured for RGB
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
		switch(geometry->geometry_type){
		default:
			break;
		case GEOMETRY_TYPE_POINTS:
			mode=GL_POINTS;
			break;
		case GEOMETRY_TYPE_TRIANGLE_STRIP:
			mode=GL_TRIANGLE_STRIP;
			break;
		case GEOMETRY_TYPE_LINE_LOOP:
			mode=GL_LINE_LOOP;
			break;

		}

		glDrawElements(mode,geometry->index_length,  GL_UNSIGNED_SHORT, (void *)(NULL));// indicesVertexBuffer->data_buffer);

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

void Geometry_GL_DeInit(Geometry * geometry) {

	if(geometry==NULL) return;

	GeometryDataGL *data = (GeometryDataGL *)geometry->data;


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

	FREE(data);
}



