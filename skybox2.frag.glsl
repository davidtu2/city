# version 120
uniform samplerCube skybox;//Used to sample my texels (Used to let the program know which shader to access)

varying vec3 TexCoords;

void main (void){
  gl_FragColor = textureCube(skybox, TexCoords);
}
