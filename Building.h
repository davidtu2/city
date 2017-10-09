class Building{
public:
  Building(float x, float z, float size, float height, unsigned int texture):
	_x(x),
	_z(z), 
	_size(size),
	_height(height), 
	_texture(texture), 
	_noWindowsPerRow(1){}
        
  virtual ~Building(){
	printf("Calling Building destructor.\n");
  }
        
  void draw(){
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glBegin(GL_QUADS);

	//Facing towards me -> Front facing
	glNormal3f(0.0, 0.0, 1.0);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//Top left
	glTexCoord2f(0, 0);
	glVertex3f(-_size + _x, _height,  _size + _z);
	//Bottom left
	glTexCoord2f(0, _noWindowsPerRow);
	glVertex3f(-_size + _x, 0,  _size + _z);
	//Bottom right
	glTexCoord2f(_noWindowsPerRow, _noWindowsPerRow);
	glVertex3f(_size + _x, 0,  _size + _z);
	//Top right
	glTexCoord2f(_noWindowsPerRow, 0);
	glVertex3f(_size + _x, _height,  _size + _z);

	//Right facing
	glNormal3f(1.0, 0.0, 0.0);
	//Top left
	glTexCoord2f(0, 0);
	glVertex3f(_size + _x, _height,  _size + _z);
	//Bottom left
	glTexCoord2f(0, _noWindowsPerRow);
	glVertex3f(_size + _x, 0,  _size + _z);
	//Bottom right
	glTexCoord2f(_noWindowsPerRow, _noWindowsPerRow);
	glVertex3f(_size + _x, 0,  -_size + _z);
	//Top right
	glTexCoord2f(_noWindowsPerRow, 0);
	glVertex3f(_size + _x, _height,  -_size + _z);

	//Left facing
	glNormal3f(-1.0, 0.0, 0.0);
	//Top left
	glTexCoord2f(0, 0);
	glVertex3f(-_size + _x, _height,  -_size + _z);
	//Bottom left
	glTexCoord2f(0, _noWindowsPerRow);
	glVertex3f(-_size + _x, 0,  -_size + _z);
	//Bottom right
	glTexCoord2f(_noWindowsPerRow, _noWindowsPerRow);
	glVertex3f(-_size + _x, 0,  _size + _z);
	//Top right
	glTexCoord2f(_noWindowsPerRow, 0);
	glVertex3f(-_size + _x, _height,  _size + _z);

	//Facing away from me -> Rear facing
	glNormal3f(0.0, 0.0, -1.0);
	//Bottom left
	glTexCoord2f(0, 0);
	glVertex3f(-_size + _x, 0,  -_size + _z);
	//Top left
	glTexCoord2f(0, _noWindowsPerRow);
	glVertex3f(-_size + _x, _height,  -_size + _z);
	//Top right
	glTexCoord2f(_noWindowsPerRow, _noWindowsPerRow);
	glVertex3f(_size + _x, _height,  -_size + _z);
	//Bottom right
	glTexCoord2f(_noWindowsPerRow, 0);
	glVertex3f(_size + _x, 0,  -_size + _z);

	//Facing straight up -> Top facing
	glNormal3f(0.0, 1.0, 0.0);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//Top left
	glVertex3f(-_size + _x, _height,  -_size + _z);
	//Bottom left
	glVertex3f(-_size + _x, _height,  _size + _z);
	//Bottom right
	glVertex3f(_size + _x, _height,  _size + _z);
	//Top right
	glVertex3f(_size + _x, _height,  -_size + _z);

	glEnd();//Not going to draw the bottom of the building

	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
  }

private:
  float _x;
  float _z;
  float _height;//This willl be the y-coordinate
  float _size;//Normally determines the width or depth of a building
  int _noWindowsPerRow;
  unsigned int _texture;
};
