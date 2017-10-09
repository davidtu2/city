class Camera{
public:
  Camera(glm::vec3 eyePos = glm::vec3(0.0f, 0.0f, 0.0f))//Initialized eyePos 
	:_forward(glm::vec3(0.0f, 0.0f, -1.0f)),//Initialized forward
	_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),//Initialied up 
	_speed(1.0f),
	_fovy(45.0f),
	_rotationDelta(0.05f){
	_position = eyePos;
	updateCameraVectors();
  }

  glm::mat4 getViewMatrix(){
	/*position + forward (i.e. "translate back")
	Note that the ortho vectors don't change, just the position*/
	return glm::lookAt(_position, _position + _forward, _up);
  }

  GLfloat getFovy(){
	return _fovy;
  }

  glm::vec3 getPosition() {
	return _position;
  }

  void moveForwards(){
	_position += _forward * _speed;
  }

  void moveBackwards(){
	_position -= _forward * _speed;
  }

  void sideStepLeft(){
	_position -= _right * _speed;
  }

  void sideStepRight(){
	_position += _right * _speed;
  }

  void ascend(){
	_position += _up * _speed;
  }

  void descend(){
	_position -= _up * _speed;
  }

  void rotateCameraUp(){
	//Create a rotation matrix that rotates about the right axis
	glm::mat3 rotationMatrix = glm::rotate(_rotationDelta, _right);
	_up = rotationMatrix * _up;//Rotate the up and the forward
	_forward = rotationMatrix * _forward;
  }

  void rotateCameraDown(){
	glm::mat3 rotationMatrix = glm::rotate(-_rotationDelta, _right);
	_up = rotationMatrix * _up;
	_forward = rotationMatrix * _forward;
  }

  void panCameraLeft(){
	glm::mat3 rotationMatrix = glm::rotate(_rotationDelta, _up);
	//Rotate the gaze (forward) and the right
	_forward = rotationMatrix * _forward;
	_right = rotationMatrix * _right;
  }

  void panCameraRight(){
	glm::mat3 rotationMatrix = glm::rotate(-_rotationDelta, _up);
	_forward = rotationMatrix * _forward;
	_right = rotationMatrix * _right;
  }

private:
  glm::vec3 _position;//eyePosition
  glm::vec3 _forward;
  glm::vec3 _up;//Up vector
  glm::vec3 _right;
  glm::vec3 _worldUp;
  GLfloat _speed;//Speed of strafe, forward, backward, ascend and descend
  GLfloat _fovy;//a.k.a. zoom
  GLfloat _rotationDelta;

  void updateCameraVectors(){//Finds the orthonormal basis
	_forward = glm::normalize(_forward);
	_right = glm::normalize(glm::cross(_forward, _worldUp));
	_up = glm::normalize(glm::cross(_right, _forward));
  }
};
