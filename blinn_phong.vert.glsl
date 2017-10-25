# version 120
//These are passed in from the CPU program, camera_control_*.cpp
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

//These are variables that we wish to send to our fragment shader
//In later versions of GLSL, these are 'out' variables.
varying vec3 myNormal;
varying vec4 myVertex;

void main() {
  gl_Position = projectionMatrix * modelViewMatrix * gl_Vertex;
  myNormal = gl_Normal;
  myVertex = gl_Vertex;
}
