# version 120
uniform mat4 modelViewMatrix_B;
uniform mat4 projectionMatrix_B;

void main() {
  gl_Position = projectionMatrix_B * modelViewMatrix_B * gl_Vertex;
  gl_TexCoord[0] = gl_MultiTexCoord0;
}
