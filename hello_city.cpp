/*
Name: David Tu
Email: david.tu2@csu.fullerton.edu

Class: CPSC 486-02
Assignment: Final Project
Desciption: Program which renders a city
*/

#include <tuple>
#include <cstdlib>
#include <cstdio>
#include <sys/time.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "GLFWApp.h"
#include "GLSLShader.h"
#include <vector>

//Our Image loading library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

#include "SpinningLight.h"
#include "Camera.h"
#include "Building.h"
#include "Plane.h"
#include "World.h"

void msglVersion(void){
  fprintf(stderr, "OpenGL Version Information:\n");
  fprintf(stderr, "\tVendor: %s\n", glGetString(GL_VENDOR));
  fprintf(stderr, "\tRenderer: %s\n", glGetString(GL_RENDERER));
  fprintf(stderr, "\tOpenGL Version: %s\n", glGetString(GL_VERSION));
  fprintf(stderr, "\tGLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

class CityApp : public GLFWApp{
private:
  Camera camera;
  SpinningLight light0;//TODO: Make this my "Moon"
  World* city;
  glm::mat4 modelViewMatrix;
  glm::mat4 projectionMatrix;
  glm::mat4 normalMatrix;
  GLSLProgram shaderProgram_A;

  //Variables to set uniform params for lighting fragment shader
  unsigned int uModelViewMatrix_A;
  unsigned int uProjectionMatrix_A;
  unsigned int uNormalMatrix_A;
  unsigned int uLight0_position_A;
  unsigned int uLight0_color_A;
  glm::mat4 modelViewMatrix_B;

  GLSLProgram shaderProgram_B;
  unsigned int uModelViewMatrix_B;
  unsigned int uProjectionMatrix_B;

public:
  CityApp(int argc, char* argv[]):GLFWApp(argc, argv, 
	std::string("CPSC 486-02 Final Project: City by David Tu").c_str(), 600, 600){}

  void initCamera(){
	//Set the camera in this position
	camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
  }
   
  void initLights(){
	glm::vec3 color0(1.0, 1.0, 1.0);
	glm::vec3 position0(0.0, 30.0, 50.0);
	glm::vec3 centerPosition(0.0, 0.0, 0.0);
	light0 = SpinningLight(color0, position0, centerPosition);
  }

  void initShaders(){
	//Load shader program A which has the light source and building texture
	const char* vertexShaderSource_A = "shaders/blinn_phong.vert.glsl";
	const char* fragmentShaderSource_A = "shaders/blinn_phong.frag.glsl";
	FragmentShader fragmentShader_A(fragmentShaderSource_A);
	VertexShader vertexShader_A(vertexShaderSource_A);
	shaderProgram_A.attach(vertexShader_A);
	shaderProgram_A.attach(fragmentShader_A);
	shaderProgram_A.link();
	shaderProgram_A.activate();
	printf("Shader program A built from %s and %s.\n", vertexShaderSource_A, fragmentShaderSource_A);
	if(shaderProgram_A.isActive()){
		printf("Shader program A is loaded and active with id %d.\n", shaderProgram_A.id());
	}else{
		printf("Shader program A did not load and activate correctly. Exiting.");
		exit(1);
	}
	//Load shader program B which has the skybox
	const char* vertexShaderSource_B = "shaders/skybox.vert.glsl";
	const char* fragmentShaderSource_B = "shaders/skybox.frag.glsl";
	FragmentShader fragmentShader_B(fragmentShaderSource_B);
	VertexShader vertexShader_B(vertexShaderSource_B);
	shaderProgram_B.attach(vertexShader_B);
	shaderProgram_B.attach(fragmentShader_B);
	shaderProgram_B.link();
	shaderProgram_B.activate();
	printf("Shader program B built from %s and %s.\n", vertexShaderSource_B, fragmentShaderSource_B);
	if(shaderProgram_B.isActive()){
		printf("Shader program B is loaded and active with id %d.\n", shaderProgram_B.id());
	}else{
		printf("Shader program B did not load and activate correctly. Exiting.");
		exit(1);
	}
	//Set up uniform variables for the shader programs
	uModelViewMatrix_A = glGetUniformLocation(shaderProgram_A.id(), "modelViewMatrix");
	uProjectionMatrix_A = glGetUniformLocation(shaderProgram_A.id(), "projectionMatrix");
	uNormalMatrix_A = glGetUniformLocation(shaderProgram_A.id(), "normalMatrix");
	uLight0_position_A = glGetUniformLocation(shaderProgram_A.id(), "light0_position");
	uLight0_color_A = glGetUniformLocation(shaderProgram_A.id(), "light0_color");
	uModelViewMatrix_B = glGetUniformLocation(shaderProgram_B.id(), "modelViewMatrix_B");
	uProjectionMatrix_B = glGetUniformLocation(shaderProgram_B.id(), "projectionMatrix_B");
  }

  void initWorld(){
	city = new World();
  }

  bool begin(){
	msglError();
	initCamera();
	initLights();
	initShaders();
	initWorld();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	msglVersion();    
	return !msglError();
  }
  
  bool end(){
	windowShouldClose();
	return true;
  }

  void activateUniforms_A(glm::vec4& _light0){
	glUniformMatrix4fv(uModelViewMatrix_A, 1, false, glm::value_ptr(modelViewMatrix));    
	glUniformMatrix4fv(uProjectionMatrix_A, 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uNormalMatrix_A, 1, false, glm::value_ptr(normalMatrix));
	glUniform4fv(uLight0_position_A, 1, glm::value_ptr(_light0));
	glUniform4fv(uLight0_color_A, 1, glm::value_ptr(light0.color()));
  }

  void activateUniforms_B(){
	glUniformMatrix4fv(uModelViewMatrix_B, 1, false, glm::value_ptr(modelViewMatrix_B));
	//Projection matricies are the same for the skybox and the city
	glUniformMatrix4fv(uProjectionMatrix_B, 1, false, glm::value_ptr(projectionMatrix));
  }

  bool render(){
	glm::vec4 _light0;//This will be the new transformed light position
	GLfloat currentFrame = (GLfloat)glfwGetTime();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::tuple<int, int> w = windowSize();
	double ratio = double(std::get<0>(w))/double(std::get<1>(w));
	projectionMatrix = glm::perspective(double(camera.getFovy()), ratio, 0.1, 1000.0);

	/*Position the light.
	Just multiply the light position by the viewMatrix since 
	it's modelMatrix is untransformed anyway (view * (model = 1) * lightPos)*/
	_light0 = camera.getViewMatrix() * light0.position();

	glm::mat4 model = glm::mat4();//Load the Identity matrix
	modelViewMatrix = camera.getViewMatrix() * model;
	normalMatrix = glm::inverseTranspose(modelViewMatrix);
	shaderProgram_A.activate();
	activateUniforms_A(_light0);
	city->drawLevel();

	//Remove translation from the view matrix so that the skybox won't translate
	modelViewMatrix_B = glm::mat4(glm::mat3(camera.getViewMatrix()));
	shaderProgram_B.activate();
	activateUniforms_B();
	city->drawSkybox();

	if(isKeyPressed('Q')){
		end();      
	}else if(isKeyPressed('R')){
		initLights();
		initCamera();
		printf("Lights and camera reinitialized.\n");
	}else if(isKeyPressed(GLFW_KEY_LEFT)){
		camera.panCameraLeft();
	}else if(isKeyPressed(GLFW_KEY_RIGHT)){
		camera.panCameraRight();
	}else if(isKeyPressed(GLFW_KEY_UP)){
		camera.moveForwards();
	}else if(isKeyPressed(GLFW_KEY_DOWN)){
		camera.moveBackwards();
	}else if(isKeyPressed('W')){
		camera.ascend();
	}else if(isKeyPressed('S')){
		camera.descend();
	}else if(isKeyPressed('A')){
		camera.sideStepLeft();
	}else if(isKeyPressed('D')){
		camera.sideStepRight();
	}else if(isKeyPressed('X')){
		camera.rotateCameraUp();
	}else if(isKeyPressed('Y')){
		camera.rotateCameraDown();
	}else if(isKeyPressed('H')){
		light0.rotateUp();
	}else if(isKeyPressed('G')){
		light0.rotateDown();
	}else if(isKeyPressed('J')){
		light0.rotateLeft();
	}else if(isKeyPressed('N')){
		light0.rotateRight();
	}
	return !msglError();
  }   
};

int main(int argc, char* argv[]){
  CityApp app(argc, argv);
  return app();
}
