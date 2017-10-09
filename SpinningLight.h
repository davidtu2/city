class SpinningLight{
public:
  SpinningLight(){}

  SpinningLight(glm::vec3& color, glm::vec3& position, glm::vec3& center):
	_rotationDelta(0.01), 
	_color(color), 
	_position(position), 
	_center(center), 
	_savedColor(color), 
	_isOn(true){
		//Get the vector from the light to the center of the world
		glm::vec3 d = direction();
		//Get a random vector of unit length = 1
		glm::vec3 random_vector = glm::sphericalRand(1.0);
		/*You'll get a vector that's perp to both operands.
		This vector will be like worldUp to your forward vector, d 
		Modified to also normalize, but this may affect roll()*/
		_tangent = glm::normalize(glm::cross(d, random_vector));
		updateLightVectors();
  }

  glm::vec4 color(){
	return glm::vec4(_color, 1.0);
  }

  glm::vec4 position(){
	return glm::vec4(_position, 1.0);
  }

  //NOTE: All rotation methods used to determine ortho basis first
  void rotateUp(){//Create a rotation matrix that rotates about the right axis
	glm::mat3 rotationMatrix = glm::rotate(_rotationDelta, right);
	//Now rotate the "worldUp" about the right axis
	_tangent = rotationMatrix * up;
	_position = rotationMatrix * _position;
  }

  void rotateDown(){
	glm::mat3 rotationMatrix = glm::rotate(-_rotationDelta, right);
	_tangent = rotationMatrix * up;
	_position = rotationMatrix * _position;
  }

  void rotateLeft(){//Create a rotation matrix that rotates about the up axis
	glm::mat3 rotationMatrix = glm::rotate(_rotationDelta, up);
	_position = rotationMatrix * _position;
  }

  void rotateRight(){
	glm::mat3 rotationMatrix = glm::rotate(-_rotationDelta, up);
	_position = rotationMatrix * _position;
  }

  void roll(){
	glm::mat3 m = glm::rotate(-_rotationDelta, direction());
	_tangent = m * _tangent;
  }
  
  void toggle(){
	_isOn = !_isOn;
	if(_isOn){
		_color = _savedColor;
	}else{
		_color = glm::vec3(0.0, 0.0, 0.0);
	}
  }

private:
  float _rotationDelta;
  glm::vec3 _color;
  glm::vec3 _position;
  glm::vec3 _center;
  glm::vec3 _tangent;//Similar to worldUp in my Camera class
  glm::vec3 _savedColor;
  bool _isOn;
  glm::vec3 forward;//Light Vectors
  glm::vec3 up;
  glm::vec3 right;
  
  glm::vec3 direction(){
	glm::vec3 d;
	d = glm::normalize(_center - _position);
	return d;
  }

  void updateLightVectors(){
	forward = glm::normalize(direction());
	right = glm::normalize(glm::cross(forward, _tangent));
	up = glm::normalize(glm::cross(right, forward));
  }

  void debug(){
	std::cerr << "position " << 
	glm::to_string(_position) << "(" << 
	glm::length(_position) << ")" << 
	"\ncenter " << 
	glm::to_string(_center) << 
	"\ntangent " << 
	glm::to_string(_tangent) << "(" << 
	glm::length(_tangent) << ")" << std::endl << std::endl;
  }
};
