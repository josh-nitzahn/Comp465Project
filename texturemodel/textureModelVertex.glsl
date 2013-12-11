/*
textureModelVertex.glsl

Vertex shader for light effects.
Adapted from OpenGL Programming Guide 8th edition sample code 
ch08_lightmodels.cpp function render_vs().

Mike Barnes
11/7/2013
*/

# version 330 core
        
in vec4 vPosition;
in vec4 vColor;
in vec2 vTexCoord;
in vec3 vNormal;

uniform int IsTexture;
uniform mat3 NormalMatrix;
uniform mat4 MVP;
  
out vec2 vs_texCoord;      
out vec3 vs_worldpos;
out vec3 vs_normal;
out vec4 vs_color;
        
void main(void) {
  vec4 position = MVP * vPosition;
  gl_Position = position;
  vs_worldpos = position.xyz;
  vs_normal = NormalMatrix * vNormal; 
  if (IsTexture == 0) { // color is used, not texture
      vs_texCoord =vec2(0, 0);
      vs_color = vColor; }
    else {  // texture is used, not color
      vs_texCoord = vTexCoord;
      vs_color = vec4(1,1,1,1); }
  }
 