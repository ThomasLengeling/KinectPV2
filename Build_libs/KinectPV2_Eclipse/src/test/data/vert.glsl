uniform mat4 transform;

attribute vec4 vertex;
attribute vec4 color;

varying vec4 vertColor;

void main() {
  gl_Position = transform * vertex;    
  vertColor = vertColor;//vec4(color.b * 0.00390625f, v.g * 0.00390625f , color.r * 0.00390625f, 1.0f);
}