

typedef struct{
	GLuint	index, 	// index object id
			vertex,	// vertex object id
			color, 	// vertex object id
			texture,// texture object id
			normal; // normal object id
	GLuint  gl_type_geometry;
}GeometryDataGL;

void  Geometry_GL_New(Geometry *geometry, uint32_t properties){

	GeometryDataGL * geometry_data= NEW(GeometryDataGL);
	*geometry_data=(GeometryDataGL){
		.index = GL_INVALID_VALUE, 	// index object id
		.vertex = GL_INVALID_VALUE,	// vertex object id
		.color = GL_INVALID_VALUE, 	// vertex object id
		.texture = GL_INVALID_VALUE,// texture object id
		.normal = GL_INVALID_VALUE // normal object id
	};

	// generate VBO for index and vertex...
	glGenBuffers(1, &geometry_data->index);
	glGenBuffers(1, &geometry_data->vertex);

	// generate VBO for texture
	if(properties & GEOMETRY_TEXTURE){
		glGenBuffers(1, &geometry_data->texture);
	}

	// generate VBO for color
	if(properties & GEOMETRY_COLOR){
		glGenBuffers(1, &geometry_data->color);
	}

	// generate VBO for normal
	if(properties & GEOMETRY_NORMAL){
		glGenBuffers(1, &geometry_data->normal);
	}

	geometry_data->gl_type_geometry=GL_TRIANGLES;

	if(properties & GEOMETRY_PROPERTY_QUADS){
		geometry_data->gl_type_geometry=GL_QUADS;
	}

	geometry->geometry_data=geometry_data;
}


void Geometry_GL_SetIndices(Geometry * geometry, short *indices,size_t indices_len) {

	GeometryDataGL * geometry_data = NULL;

	if(geometry==NULL) return;
	if(indices_len != N_INDICES(geometry->n_vertexs)){
		Log_Error("Index count doesn't matches");
		return;
	}

	geometry_data = (GeometryDataGL *)geometry->geometry_data;

	if(geometry_data->index == GL_INVALID_VALUE) {
		Log_Error("Index VBO not build");
		return;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry_data->index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_len*sizeof(short), indices, GL_STATIC_DRAW);


}


void Geometry_GL_SetMeshVertex(Geometry *geometry, float *mesh_vertexs,size_t mesh_vertexs_len) {

	GeometryDataGL * geometry_data = NULL;

	if(geometry==NULL ) return;
	if(mesh_vertexs_len != (geometry->n_vertexs*VERTEX_COORDS_LEN)){
		Log_Error("Vertex count doesn't matches");
		return;
	}

	geometry_data = (GeometryDataGL *)geometry->geometry_data;

	if(geometry_data->vertex == GL_INVALID_VALUE) {
		Log_Error("Vertex VBO not build");
		return;
	}

	glBindBuffer(GL_ARRAY_BUFFER, geometry_data->vertex);
	glBufferData(GL_ARRAY_BUFFER, mesh_vertexs_len*sizeof(float), mesh_vertexs, GL_DYNAMIC_DRAW);

}


void Geometry_GL_SetMeshTexture(Geometry * geometry, float *mesh_texure_vertexs,size_t mesh_texture_vertexs_len) {
	GeometryDataGL * geometry_data = NULL;

	if(geometry==NULL) return;
	if(mesh_texture_vertexs_len != (geometry->n_vertexs*TEXTURE_COORDS_LEN)){
		Log_Error("Vertex count doesn't matches");
		return;
	}


	geometry_data = (GeometryDataGL *)geometry->geometry_data;

	if(geometry_data->texture == GL_INVALID_VALUE) {
		Log_Error("Texture VBO not build or not set in constructor (New_Geometry)");
		return;
	}

	// reserve GPU memory ...
	glBindBuffer(GL_ARRAY_BUFFER, geometry_data->texture);
	glBufferData(GL_ARRAY_BUFFER, mesh_texture_vertexs_len*sizeof(float), mesh_texure_vertexs, GL_DYNAMIC_DRAW);
}



void Geometry_GL_SetMeshColor(Geometry * geometry,float *mesh_color_vertexs,size_t mesh_color_vertexs_len) {

	GeometryDataGL * geometry_data = NULL;

	if(geometry==NULL) return;
	if(mesh_color_vertexs_len != (geometry->n_vertexs*(COLOR_COORDS_LEN))){
		Log_Error("Vertex count doesn't matches");
		return;
	}

	geometry_data = (GeometryDataGL *)geometry->geometry_data;

	if(geometry_data->color == GL_INVALID_VALUE) {
		Log_Error("Color VBO not build or not set in constructor (New_Geometry)");
		return;
	}
	// reserve GPU memory ...
	glBindBuffer(GL_ARRAY_BUFFER, geometry_data->color);
	glBufferData(GL_ARRAY_BUFFER, mesh_color_vertexs_len*sizeof(float), mesh_color_vertexs, GL_DYNAMIC_DRAW);
}


void Geometry_GL_SetMeshNormal(Geometry * geometry,float *mesh_normal_vertexs,size_t mesh_normal_vertexs_len) {

	GeometryDataGL * geometry_data = NULL;

	if(geometry==NULL) return;
	if(mesh_normal_vertexs_len != (geometry->n_vertexs*NORMAL_COORDS_LEN)){
		Log_Error("Vertex count doesn't matches");
		return;
	}

	geometry_data = (GeometryDataGL *)geometry->geometry_data;

	if(geometry_data->normal == GL_INVALID_VALUE) {
		Log_Error("Normal VBO not build or not set in constructor (New_Geometry)");
		return;
	}

	// reserve GPU memory ...
	glBindBuffer(GL_ARRAY_BUFFER, geometry_data->normal);
	glBufferData(GL_ARRAY_BUFFER, mesh_normal_vertexs_len*sizeof(float), mesh_normal_vertexs, GL_DYNAMIC_DRAW);
}


void Geometry_GL_Draw(Geometry * geometry) {

	GeometryDataGL * geometry_data=NULL;
	if(geometry==NULL) return;

	geometry_data = (GeometryDataGL *)geometry->geometry_data;

	if(geometry_data==NULL){
		return;
	}


	// configure vertex pointer
	if(geometry_data->vertex != GL_INVALID_VALUE) {
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, geometry_data->vertex);
		glVertexPointer(VERTEX_COORDS_LEN, GL_FLOAT, 0, 0L); // configured for XYZ
	}

	// configure texture pointer
	if(geometry_data->texture != GL_INVALID_VALUE) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, geometry_data->texture);
		glTexCoordPointer(TEXTURE_COORDS_LEN, GL_FLOAT, 0, 0L); // configured for UV
	}

	// configure color pointer
	if(geometry_data->color!= GL_INVALID_VALUE) {
		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, geometry_data->color);
		glColorPointer(COLOR_COORDS_LEN, GL_FLOAT, 0, 0L); // configured for RGB
	}

	// configure normal pointer
	if(geometry_data->normal!= GL_INVALID_VALUE) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, geometry_data->normal);
		glNormalPointer(GL_FLOAT, 0, 0L); //  configured for XYZ
	}


	if(geometry_data->index != GL_INVALID_VALUE) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry_data->index);
		glDrawElements(geometry_data->gl_type_geometry,N_INDICES(geometry->n_vertexs),  GL_UNSIGNED_SHORT, (void *)(NULL));// indicesVertexBuffer->data_buffer);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	if(geometry_data->normal != GL_INVALID_VALUE){
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if(geometry_data->color != GL_INVALID_VALUE){
		glDisableClientState(GL_COLOR_ARRAY);
	}

	if(geometry_data->texture != GL_INVALID_VALUE){
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if(geometry_data->vertex != GL_INVALID_VALUE){
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

void Geometry_GL_DeInit(Geometry * geometry) {

	if(geometry==NULL) return;

	GeometryDataGL *geometry_data = (GeometryDataGL *)geometry->geometry_data;


	// generate VBO for index and vertex...
	if(geometry_data->index!=GL_INVALID_VALUE){
		glGenBuffers(1, &geometry_data->index);
	}

	if(geometry_data->vertex!=GL_INVALID_VALUE){
		glGenBuffers(1, &geometry_data->vertex);
	}

	// generate VBO for texture
	if(geometry_data->texture!=GL_INVALID_VALUE){
		glGenBuffers(1, &geometry_data->texture);
	}

	// generate VBO for color
	if(geometry_data->color!=GL_INVALID_VALUE){
		glGenBuffers(1, &geometry_data->color);
	}

	// generate VBO for normal
	if(geometry_data->normal!=GL_INVALID_VALUE){
		glGenBuffers(1, &geometry_data->normal);
	}

	// Disable the VBO index from the VAO attributes list
	glDisableVertexAttribArray(0);

	// Delete the vertex VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	FREE(geometry_data);
}



