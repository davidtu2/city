# version 120
uniform mat4 modelViewMatrix_B;
uniform mat4 projectionMatrix_B;

varying vec3 TexCoords;

void main() {
  vec4 position = projectionMatrix_B * modelViewMatrix_B * gl_Vertex;
  gl_Position = position.xyww;//To trick the depth testing. Will always pass whenever there's no object in front of my skybox
  TexCoords = gl_Vertex.xyz;
}
