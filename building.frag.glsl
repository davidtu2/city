# version 120
//These are passed in from the CPU program
uniform sampler2D building;     

void main (void){
  vec4 color = texture2D(building, gl_TexCoord[0].st);
  gl_FragColor = color;
}
