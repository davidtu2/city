# version 120
uniform samplerCube skybox;//built-in data for texture

varying vec3 TexCoords;

void main (void){
  gl_FragColor = textureCube(skybox, TexCoords);
}
