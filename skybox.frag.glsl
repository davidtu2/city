# version 120
uniform sampler2D color_texture;//Used to sample my texels (Used to let the program know which shader to access)

void main (void){
  vec4 color = texture2D(color_texture, gl_TexCoord[0].st);
  gl_FragColor = color;
}
