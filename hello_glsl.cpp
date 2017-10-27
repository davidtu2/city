#include <tuple>
#include <cstdlib>
#include <cstdio>
#include <sys/time.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "GLFWApp.h"
#include "GLSLShader.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION//For textures
#include "stb_image.h"
//#include "bitmap.h"

void msglVersion(void){
	fprintf(stderr, "OpenGL Version Information:\n");
	fprintf(stderr, "\tVendor: %s\n", glGetString(GL_VENDOR));
	fprintf(stderr, "\tRenderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stderr, "\tOpenGL Version: %s\n", glGetString(GL_VERSION));
	fprintf(stderr, "\tGLSL Version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}

class SpinningLight{
	public:
		SpinningLight(){
		
		}

		SpinningLight(glm::vec3& color, glm::vec3& position, glm::vec3& center):_rotationDelta(0.01), _color(color), _position(position), _center(center), _savedColor(color), _isOn(true){
			glm::vec3 d = direction();//Get the vector from the light to the center of the world
			glm::vec3 random_vector = glm::sphericalRand(1.0);//Get a random vector of unit length = 1
			//You'll get a vector that's perp to both operands. This vector will be like worldUp to your forward vector, d
			_tangent = glm::normalize(glm::cross(d, random_vector));//Modified to also normalize. This may affect roll()
			updateLightVectors();
		}

		glm::vec4 color(){//getters
			return glm::vec4(_color, 1.0);
		}

		glm::vec4 position(){
			return glm::vec4(_position, 1.0);
		}

		//NOTE: All rotation methods used to determine ortho basis first
		void rotateUp(){//Create a rotation matrix that rotates about the right axis
			glm::mat3 rotationMatrix = glm::rotate(_rotationDelta, right);//Added negative sign
			_tangent = rotationMatrix * up;//Now rotate the "worldUp" about the right axis
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
			std::cerr << "position " << glm::to_string(_position) << "(" << glm::length(_position) << ")" << "\ncenter " << glm::to_string(_center) << "\ntangent " << glm::to_string(_tangent) << "(" << 
			glm::length(_tangent) << ")" << std::endl << std::endl;
		}
};

class Camera{
	public:
		Camera(	glm::vec3 eyePos = glm::vec3(0.0f, 0.0f, 0.0f))//Initialized eyePos 
				:_forward(glm::vec3(0.0f, 0.0f, -1.0f)),//Initialized forward
				_worldUp(glm::vec3(0.0f, 1.0f, 0.0f)),//Initialied up 
				_speed(1.0f),
				_fovy(45.0f),
				_rotationDelta(0.05f){
					_position = eyePos;
					updateCameraVectors();
		}

		glm::mat4 getViewMatrix(){
			return glm::lookAt(_position, _position + _forward, _up);//position + forward = what you are looking at because the ortho vectors NEVER change in value, just the position!!!
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

		void rotateCameraUp(){//Create a rotation matrix that rotates about the right axis
			glm::mat3 rotationMatrix = glm::rotate(_rotationDelta, _right);
			_up = rotationMatrix * _up;//Rotate the up and the forward
			_forward = rotationMatrix * _forward;
		}

		void rotateCameraDown(){
			glm::mat3 rotationMatrix = glm::rotate(-_rotationDelta, _right);//COUNTER CLOCKWISE???
			_up = rotationMatrix * _up;
			_forward = rotationMatrix * _forward;
		}

		void panCameraLeft(){
			glm::mat3 rotationMatrix = glm::rotate(_rotationDelta, _up);
			_forward = rotationMatrix * _forward;//Rotate the gaze, forward and the right
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

class Plane{
	public:
		Plane(int size):_size(size){

		}

		virtual ~Plane(){
			printf("Calling Plane destructor.\n");
		}

		void draw(){//Start by drawing the blocks (The regions where the buildings will sit on top of)
			float block = 10.0f;//Size of the block (a.k.a. the length of the "street")
			glColor4f(0.412, 0.412, 0.412, 1.0f);
			glBegin(GL_QUADS);//Start drawing a 17 x 17 quadrilateral
			for(int j = 0; j < _size; j += 12){//Go to one row
				for(int i = 0; i < _size; i += 12){//Draw all the "columns" of the row
					glVertex3f(0.0f + i, 0.0f, 0.0f - j);//Bottom Left
					glVertex3f(0.0f + block + i, 0.0f, 0.0f - j);//Bottom right
					glVertex3f(0.0f + block + i, 0.0f, 0.0f - block - j);//Top right
					glVertex3f(0.0f + i, 0.0f, 0.0f - block - j);//Top left
				}//>>Drawing quads will always be like this
			}
			glEnd();//Finish drawing

			glColor4f(1.0, 1.0, 1.0, 1.0);//Now draw the outer boundaries
			glBegin(GL_LINES);//Start drawing lines. Let's start with the left boundary
			glVertex3f(-2.0f, 0.0f, 2.0f);//Bottom left corner of the map
			glVertex3f(-2.0f, 0.0f, -_size - 8);//Top left corner of the map
			glVertex3f(-2.0f, 0.0f, -_size - 8);//Next, let's do the back boundary. Continuing from where we left off, this is the top left corner of the map
			glVertex3f(_size + 8, 0.0f, -_size - 8);//Top right corner of the map
			glVertex3f(_size + 8, 0.0f, -_size - 8);//Right boundary: top right corner of the map
			glVertex3f(_size + 8, 0.0f, 2.0f);//Bottom right corner of the map
			glVertex3f(_size + 8, 0.0f, 2.0f);//Front boundary: bottom right corner of the map
			glVertex3f(-2.0f, 0.0f, 2.0f);//Back to where we started: the bottom left corner of the map
			glEnd();//Finish drawing
		}

	private:
		int _size;
};

class Building{
	public:
		Building(float x, float y, float z, float size, float height):_x(x), _y(y), _z(z), _size(size), _height(height){

		}
        
		virtual ~Building(){
			printf("Calling Building destructor.\n");
		}
        
		void draw(){
			glBegin(GL_QUADS);//Start drawing quads
			glNormal3f(0.0, 0.0, 1.0);//Facing towards me -> Front facing
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);//Random coloring?????
			glVertex3f(-_size + _x, _height + _y,  _size + _z);//Top left
			glVertex3f(-_size + _x, _y,  _size + _z);//Bottom left
			glVertex3f(_size + _x, _y,  _size + _z);//Bottom right
			glVertex3f(_size + _x, _height + _y,  _size + _z);//Top right
			glNormal3f(1.0, 0.0, 0.0);//Right facing
			glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
			glVertex3f(_size + _x, _height + _y,  _size + _z);//Top left
			glVertex3f(_size + _x, _y,  _size + _z);//Bottom left
			glVertex3f(_size + _x, _y,  -_size + _z);//Bottom right
			glVertex3f(_size + _x, _height + _y,  -_size + _z);//Top right
			glNormal3f(-1.0, 0.0, 0.0);//Left facing
			glColor4f( 0.4f, 0.4f, 0.4f, 1.0f);
			glVertex3f(-_size + _x, _height + _y,  -_size + _z);//Top left
			glVertex3f(-_size + _x, _y,  -_size + _z);//Bottom left
			glVertex3f(-_size + _x, _y,  _size + _z);//Bottom right
			glVertex3f(-_size + _x, _height + _y,  _size + _z);//Top right
			glNormal3f(0.0, 0.0, -1.0);//Facing away from me -> Rear facing
			glColor4f( 0.3f, 0.3f, 0.3f, 1.0f);
			glVertex3f(-_size + _x, _y,  -_size + _z);//Bottom left
			glVertex3f(-_size + _x, _height + _y,  -_size + _z);//Top left
			glVertex3f(_size + _x, _height + _y,  -_size + _z);//Top right
			glVertex3f(_size + _x, _y,  -_size + _z);//Bottom right
			glNormal3f(0.0, 1.0, 0.0);//Facing straight up -> Top facing
			glColor4f( 0.6f, 0.6f, 0.6f, 1.0f);
			glVertex3f(-_size + _x, _height + _y,  -_size + _z);//Top left
			glVertex3f(-_size + _x, _height + _y,  _size + _z);//Bottom left
			glVertex3f(_size + _x, _height + _y,  _size + _z);//Bottom right
			glVertex3f(_size + _x, _height + _y,  -_size + _z);//Top right
			glEnd();//Not going to draw the bottom of the building
		}

	private:
		float _x;
		float _y;
		float _z;
		float _height;
		float _size;
};

class HelloGLSLApp : public GLFWApp{
	private:
		Camera camera;
		SpinningLight light0;//This can act as my "Moon" or "Sun"
		int planeSize = 196;//Size of the XZ plane
		Plane* XZ; //the XZ plane we will we working with
		Building* building;//Building model to be inserted into Buildings vector
		std::vector<Building*> buildings;//Vector of Buildings
		glm::mat4 modelViewMatrix;
		glm::mat4 projectionMatrix;
		glm::mat4 normalMatrix;
		GLSLProgram shaderProgram_A;
		unsigned int uModelViewMatrix_A;//Variables to set uniform params for lighting fragment shader
		unsigned int uProjectionMatrix_A;
		unsigned int uNormalMatrix_A;
		unsigned int uLight0_position_A;
		unsigned int uLight0_color_A;
		
		GLSLProgram shaderProgram_B;
		unsigned int uModelViewMatrix_B;
		unsigned int uProjectionMatrix_B;
		glm::mat4 modelViewMatrix_B;
		//glm::mat4 projectionMatrix_B;
		unsigned int skybox_texture;
		//unsigned int m_texture;
		unsigned int skyboxVAO;
		unsigned int skyboxVBO;

	public:
		HelloGLSLApp(int argc, char* argv[]):GLFWApp(argc, argv, std::string("City").c_str(), 600, 600){

		}

		void initCamera(){
			camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));//Let's set the camera in this position
		}
   
		void initLights(){
			glm::vec3 color0(0.0, 0.0, 1.0);
			glm::vec3 position0(0.0, 5.0, 10.0);
			glm::vec3 centerPosition(0.0, 0.0, 0.0);
			light0 = SpinningLight(color0, position0, centerPosition);
		}

		unsigned int loadCubemap(std::vector<std::string> faces){
    		unsigned int textureID;
    		glGenTextures(1, &textureID);//Create a texture
    		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);//Bind the texture

    		int width;
			int height;
			int nrChannels;//Corresponds to rgba
    		for (unsigned int i = 0; i < faces.size(); i++){
        		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        		if (data){//Adding by i because OpenGL's enums is linearly incremented. It will go through:
					//GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
					//GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z and GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
            		glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,//Fill the texture by uploading the image. This needs to be done 6 times for each face
									0, 
									GL_RGB,//Indicates that the data has 4 components: rgba
									width, 
									height, 
									0, 
									GL_RGB,////How components are represented in RAM
									GL_UNSIGNED_BYTE, 
									data);
            		stbi_image_free(data);
					printf("Texture successfully loaded.\n");
        		}else{
            		stbi_image_free(data);
					printf("Cubemap texture failed to load.\n");
        		}
    		}//Configure the texture with texture settings:
    		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Bi-linear filtering is used to clean up any minor aliasing when the camera rotates.
    		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			//Texture coordinates that are exactly between two faces might not hit an exact face (due to some hardware limitations)
    		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//so by using GL_CLAMP_TO_EDGE, OpenGL always return their edge values whenever we sample between faces.
    		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//Specify how to wrap each texture coordinate
    		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);//If you don't clamp to edge then you might get a visible seam on the edges of your textures
    		return textureID;
		}

		bool begin(){
			msglError();
			initCamera();
			initLights();

			const char* vertexShaderSource_A = "lighting.vert.glsl";//Load shader program A
			const char* fragmentShaderSource_A = "lighting.frag.glsl";
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

			const char* vertexShaderSource_B = "skybox2.vert.glsl";//Load shader program B
			const char* fragmentShaderSource_B = "skybox2.frag.glsl";
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

			uModelViewMatrix_A = glGetUniformLocation(shaderProgram_A.id(), "modelViewMatrix");//Set up uniform variables for shader program A
			uProjectionMatrix_A = glGetUniformLocation(shaderProgram_A.id(), "projectionMatrix");
			uNormalMatrix_A = glGetUniformLocation(shaderProgram_A.id(), "normalMatrix");
			uLight0_position_A = glGetUniformLocation(shaderProgram_A.id(), "light0_position");
			uLight0_color_A = glGetUniformLocation(shaderProgram_A.id(), "light0_color");

			uModelViewMatrix_B = glGetUniformLocation(shaderProgram_B.id(), "modelViewMatrix_B");
			uProjectionMatrix_B = glGetUniformLocation(shaderProgram_B.id(), "projectionMatrix_B");
			
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			/*CBitmap skybox("skybox.bmp");//Read image
    		glGenTextures(1, &skybox_texture);//Create a texture
    		glBindTexture(GL_TEXTURE_2D, skybox_texture);//Bind the texture
    		glTexImage2D(	GL_TEXTURE_2D,//Fill the texture by uploading the image
							0, 
							GL_RGBA,//Indicates that the data has 4 components: rgba
							skybox.GetWidth(),//Width 
							skybox.GetHeight(),//Height
							0,//should always be 0???
							GL_RGBA,//How the components are represented in RAM
							GL_UNSIGNED_BYTE, 
							skybox.GetBits());//The data itself
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Configure the texture with texture settings:
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//Bi-linear filtering is used to clean up any minor aliasing when the camera rotates.
			//Texture coordinates that are exactly between two faces might not hit an exact face (due to some hardware limitations)
			//so by using GL_CLAMP_TO_EDGE, OpenGL always return their edge values whenever we sample between faces.
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//Specify how to wrap each texture coordinate
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//If you don't clamp to edge then you might get a visible seam on the edges of your textures
    		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			//glUniform1i(skybox_texture, 0);//pass texture location to vertex shader//WHY DOES THIS MAKE THE SYSTEM BREAK????
    		glBindTexture(GL_TEXTURE_2D, 0);*/

			float skyboxVertices[] = {          
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
    		glGenVertexArrays(1, &skyboxVAO);//skybox VAO
			glBindVertexArray(skyboxVAO);
    		glGenBuffers(1, &skyboxVBO);
    		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
			glBufferData(	GL_ARRAY_BUFFER, 
							sizeof(skyboxVertices), 
							&skyboxVertices, 
							GL_STATIC_DRAW);
    		glEnableVertexAttribArray(0);//What does this mean?
    		glVertexAttribPointer(0,//What does this mean?
				3,//How much data to get
				GL_FLOAT,//data type
				GL_FALSE,//Is the data normalized?
				3 * sizeof(float),//How much data per row
				(void*)0);//How much data  I need to skip over
			std::vector<std::string> faces{
        		"right.jpg",
        		"left.jpg",
        		"top.jpg",
        		"bottom.jpg",
        		"back.jpg",
        		"front.jpg"
    		};
    		skybox_texture = loadCubemap(faces);

			drawXZPlane();
			drawBuildings();

			msglVersion();    
			return !msglError();
		}
  
		bool end(){
			buildings.clear();
			delete XZ;
			delete building;
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

		void drawXZPlane(){//X axis is 0 to positive values. Z axis is 0 to negative values. RGBA values are for the boundary lines
			XZ = new Plane(planeSize);
		}

		void drawBuildings(){//Procedural generation
			for(int j = -2; j > -planeSize - 6; j -= 6){
				for(int i = 0; i < planeSize + 6; i += 2){//101 iterations. This is the x value
					if(i % 12 != 10 && i % 12 != 0){//x value = {2, 4, 6, 8}
						float randomSize = rand() % 2 + 1;//randomSize = [1, 2] (We are adding 1 because we don't want 0 size/height)
						float randomHeight;
						if(rand() % 5 + 1 == 1){//If I get a 1...
							randomHeight = rand() % 12 + 1;//randomHeight = [1, 12] (Make a taller building)
						}else{
							randomHeight = rand() % 7 + 1;//randomHeight = [1, 7] (Make a shorter building)
						}
						building = new Building(i, 0.0f, j, randomSize, randomHeight);
						buildings.push_back(building);
					}
				}
			}
		}

		void drawSkybox() {
     		glBegin(GL_QUADS);//Back
     		glTexCoord2f(.5,1.0/3.0);
     		glVertex3f(500,-500,-500);
     		glTexCoord2f(.5,2.0/3.0);
     		glVertex3f(500,500,-500);
     		glTexCoord2f(.25,2.0/3.0);
     		glVertex3f(-500,500,-500);
     		glTexCoord2f(.25,1.0/3.0);
     		glVertex3f(-500,-500,-500);
     		glTexCoord2f(1,0.334);//Front
     		glVertex3f(-500,-500,500);
     		glTexCoord2f(1,0.665);
     		glVertex3f(-500,500,500);
     		glTexCoord2f(.75,0.665);
     		glVertex3f(500,500,500);
     		glTexCoord2f(.75,0.334);
     		glVertex3f(500,-500,500);
     		glTexCoord2f(0,0.334);//Left
     		glVertex3f(-500,-500,500);
     		glTexCoord2f(.25,0.334);
     		glVertex3f(-500,-500,-500);
     		glTexCoord2f(.25,0.665);
     		glVertex3f(-500,500,-500);
     		glTexCoord2f(0,0.665);
     		glVertex3f(-500,500,500);
     		glTexCoord2f(.5,0.334);//Right
     		glVertex3f(500,-500,-500);
     		glTexCoord2f(.75,0.334);
     		glVertex3f(500,-500,500);
     		glTexCoord2f(.75,0.665);
     		glVertex3f(500,500,500);
     		glTexCoord2f(.5,0.665);
     		glVertex3f(500,500,-500);
     		glTexCoord2f(.251,2.0/3.0);//Top
     		glVertex3f(-500,500,-500);
     		glTexCoord2f(.499,2.0/3.0);
     		glVertex3f(500,500,-500);
     		glTexCoord2f(.499,1.0);
     		glVertex3f(500,500,500);
     		glTexCoord2f(.251,1.0);
     		glVertex3f(-500,500,500);
     		glTexCoord2f(.251,1.0/3.0);//Bottom
     		glVertex3f(-500,-500,-500);
     		glTexCoord2f(.251,0.0/3.0);
     		glVertex3f(-500,-500,500);
     		glTexCoord2f(.499,0.0/3.0);
     		glVertex3f(500,-500,500);
     		glTexCoord2f(.499,1.0/3.0);
     		glVertex3f(500,-500,-500);
     		glEnd();
		}

		bool render(){
			glm::vec4 _light0;//This will be the new transformed light position
			GLfloat currentFrame = (GLfloat)glfwGetTime();
    
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			std::tuple<int, int> w = windowSize();
			double ratio = double(std::get<0>(w))/double(std::get<1>(w));
			projectionMatrix = glm::perspective(double(camera.getFovy()), ratio, 0.1, 1000.0);
	
			//Position the light. Just multiply the light position by the viewMatrix since it's modelMatrix is untransformed anyway (view * (model = 1) * lightPos)
			_light0 = camera.getViewMatrix() * light0.position();

			glm::mat4 model = glm::mat4();//Load the Identity matrix
			modelViewMatrix = camera.getViewMatrix() * model;
			normalMatrix = glm::inverseTranspose(modelViewMatrix);

			shaderProgram_A.activate();
			activateUniforms_A(_light0);

			XZ->draw();//Draw the plane
			for(std::vector<Building*>::iterator it = buildings.begin(); it != buildings.end(); ++it){//Draw Buildings
				(*it)->draw();
			}

			shaderProgram_B.activate();
			modelViewMatrix_B = glm::mat4(glm::mat3(camera.getViewMatrix()));//Remove translation from the view matrix so that the skybox won't translate
			//projectionMatrix_B = glm::perspective(double(camera.getFovy()), ratio, 0.1, 1000.0);
			glUniformMatrix4fv(uModelViewMatrix_B, 1, false, glm::value_ptr(modelViewMatrix_B));
			//glUniformMatrix4fv(uModelViewMatrix_B, 1, false, glm::value_ptr(modelViewMatrix));
			glUniformMatrix4fv(uProjectionMatrix_B, 1, false, glm::value_ptr(projectionMatrix));//Projection matricies are the same for the skybox and the city

        	glDepthFunc(GL_LEQUAL);//change depth function so depth test passes when values are equal to depth buffer's content
        	glBindVertexArray(skyboxVAO);//skybox cube
        	glActiveTexture(GL_TEXTURE0);
        	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox_texture);
        	glDrawArrays(GL_TRIANGLES, 0, 36);
        	glBindVertexArray(0);
        	glDepthFunc(GL_LESS);//set depth function back to default

			/*glEnable(GL_TEXTURE_2D);
    		glBindTexture(GL_TEXTURE_2D, skybox_texture);
    		drawSkybox();
    		//glUseProgram(0);
    		glDisable(GL_TEXTURE_2D);
    		glBindTexture(GL_TEXTURE_2D, 0);*/

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
	HelloGLSLApp app(argc, argv);
	return app();
}
