#version 330 core

out vec4 fin_col;
uniform vec3 col;
uniform sampler2D t;
in vec2 tex;


void main(){
	fin_col=mix(texture(t,tex),vec4(col,1.0),0.6);
	if(fin_col.a<=0.63) discard;
	//fin_col=vec4(col,1.0);
	//fin_col = vec4(0.0784,0.3176,0.8784,1.0); dark blue
    //fin_col= vec4(1.0,1.0,1.0,1.0); white
	//fin_col= vec4(0.2431,0.8705,0.9803,1.0); light blue
}