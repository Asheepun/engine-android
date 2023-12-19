#version 330 core

in vec3 input_vertexPosition;

uniform vec3 lightDirection;

out vec4 FragColor;

void main(){

	vec3 skyDirection = normalize(vec3(0.0, 0.0, -1.0) + input_vertexPosition);

	vec4 color1 = vec4(0.8, 1.0, 1.0, 1.0);
	//vec4 color2 = vec4(0.6, 0.6, 1.0, 1.0);
	vec4 color2 = vec4(1.0, 1.0, 1.0, 1.0);
	//vec4 color2 = vec4(0.933, 0.415, 0.486, 1.0);

	float yFactor = (input_vertexPosition.y + 1.0) * 0.5;

	//float factor = max(-dot(skyDirection, lightDirection), 0.0);
	//FragColor = color1 * (yFactor) + color2 * (1.0 - yFactor);
	FragColor = color2;

	//FragColor.xyz *= 

} 

