# version 120
//These are passed in from the CPU program
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main() {
  gl_Position = projectionMatrix * modelViewMatrix * gl_Vertex;
  gl_TexCoord[0] = gl_MultiTexCoord0;
}
