#version 330 core

in vec4 input_color;

out vec4 FragColor;

void main(){

	FragColor = input_color;
	//FragColor.xyz *= float(input_color.a < 1.0);

} 

