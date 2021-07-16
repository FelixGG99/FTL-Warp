#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texPos;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
uniform float s;
out vec2 tex;
void main(){
    gl_Position=s*projection*view*vec4(pos,1.0);
    tex=texPos;
}