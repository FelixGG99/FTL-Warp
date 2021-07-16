#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tt;
uniform mat4 view;
uniform mat4 projection;
uniform float s;
uniform mat4 model;
out vec2 tex;
void main(){
    gl_Position = s*projection*view*model*vec4(pos,1.0);
	tex=tt;
}