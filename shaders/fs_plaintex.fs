#version 330 core
in vec2 tex;
out vec4 frag;
uniform sampler2D t;

void main(){
    frag=texture(t,tex);
}