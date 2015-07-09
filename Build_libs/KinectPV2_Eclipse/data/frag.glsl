#ifdef GL_ES
precision mediump float;
precision mediump int;
#endif

varying vec4 vertColor;

void main() {
  //color image int BGRA Color format
  gl_FragColor = vec4(vertColor.b * 0.00390625f, vertColor.g * 0.00390625f , vertColor.r * 0.00390625f, 1.0f);
}