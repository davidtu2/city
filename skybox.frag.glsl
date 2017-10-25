# version 120
uniform sampler2D color_texture;

void main (void){
  vec4 color = texture2D(color_texture, gl_TexCoord[0].st);
  gl_FragColor = color;
}
