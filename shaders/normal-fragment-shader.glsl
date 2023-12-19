#version 330 core

in vec3 input_normal;

out vec4 FragColor;

void main(){

	FragColor = vec4(input_normal / 2.0 + vec3(0.5), 1.0);

} 
