class Texture{
public:
  Texture(){
	_faces = {"textures/right.tga", 
		"textures/left.tga", 
		"textures/top.tga", 
		"textures/bottom.tga", 
		"textures/back.tga", 
		"textures/front.tga"};
	/*Create 1 texture that are of type unsigned int,
	as indicated by the texture type*/
	glGenTextures(1, &_texture);
	/*Bind the texture so that any texture commands called after
	apply to this texture*/
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texture);
	for (unsigned int i = 0; i < _faces.size(); i++){
		_data = stbi_load(_faces[i].c_str(), 
			&_width, &_height, 
			&_colorChannels, 
			0);
		/*Adding by i because OpenGL's enums is linearly incremented.
		It will go through: 
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z and 
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z*/
		if (_data){
			/*Generate the texture by using the uploaded data.
			This needs to be done 6 times for each face*/
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,//Specifies the mipmap level
				GL_RGB,//How to store the texture: rgba
				_width, _height, 0,//Specifies border size
				GL_RGB,//The format of the data: rgb
				GL_UNSIGNED_BYTE,//The datatype of the data
				_data);
			stbi_image_free(_data);
		}else{
			printf("Cubemap texture failed to load.\n");
			stbi_image_free(_data);
			exit(1);
		}
	}
	/*Configure the texture with texture settings:
	Bi-linear filtering is used to clean up any minor aliasing
	when the camera rotates.*/
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/*Texture coordinates that are exactly between two faces
	might not hit an exact face (due to some hardware limitations)
	so by using GL_CLAMP_TO_EDGE, OpenGL always return their edge values
	whenever we sample between faces.*/
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//Specify how to wrap each texture coordinate
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	/*If you don't clamp to edge 
	then you might get a visible seam on the edges of your textures*/
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  Texture(std::string path){
	/*Generate a texture for all of the buildings
	Note that we can generate a texture for each building object
	but that will require more resources. 
	In fact when tried, it works but the program will be very slow*/
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	_data = stbi_load(path.c_str(), &_width, &_height, &_colorChannels, 0);
	if (_data){
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
		stbi_image_free(_data);
	}else{
		printf("Building texture failed to load.\n");
		stbi_image_free(_data);
		exit(1);
	}
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
  }

  virtual ~Texture(){}

  unsigned int getTexture(){
	return _texture;
  }

private:
  std::vector<std::string> _faces;//The inner faces of the skybox
  unsigned int _texture;//Skybox texture
  unsigned char* _data;//Texture data
  int _width;//Texture's width and height
  int _height;
  int _colorChannels;//Corresponds to a texture's rgba
};
