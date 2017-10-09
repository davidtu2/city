class World{
public:
  World(): _size(196){
	_XZ = new Plane(_size);//First init the plane
	float skyboxVertices[] = {//Now init the skybox 
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	glGenVertexArrays(1, &_VAO);//Create 1 VAO
	glBindVertexArray(_VAO);//Then bind it
	glGenBuffers(1, &_VBO);//Create 1 VBO
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);//Then bind that
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), 
		&skyboxVertices, 
		GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3,//How much data to get
		GL_FLOAT,//data type
		GL_FALSE,//Is the data normalized?
		3 * sizeof(float),//How much data per row
		(void*)0);//How much data I need to skip over

	_skybox = new Texture();
  }
        
  virtual ~World(){
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
	delete _XZ;
  }

  void drawLevel(){
	_XZ->draw();
  }

  void drawSkybox(){
	/*Makes sure each uniform sampler associates with the correct texture unit
	glUniform1i(uSkybox_B, 0);*/

	/*Change depth function so depth test passes
	when values are equal to depth buffer's content*/
	glDepthFunc(GL_LEQUAL);
	glBindVertexArray(_VAO);//skybox cube
	/*Activate the texture unit first before binding.
	This allows us to use multiple textures. 
	If this is not called, the default will be: GL_TEXTURE0*/
	glActiveTexture(GL_TEXTURE0);
	/*Bind the texture before drawing to the texture unit specified earlier. 
	This also makes it available in the fragment shader as a sampler uniform*/
	glBindTexture(GL_TEXTURE_CUBE_MAP, _skybox->getTexture());
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);//set depth function back to default
  }
        
private:
  int _size;//Size of the plane
  Plane* _XZ;//the XZ plane
  unsigned int _VAO;//The following private variables are for the skybox
  unsigned int _VBO;
  Texture* _skybox;
};
