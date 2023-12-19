#version 330 core
layout (location = 0) in vec3 attribute_vertex;
layout (location = 1) in vec2 attribute_textureVertex;
layout (location = 2) in vec3 attribute_normalVertex;

out vec2 input_texturePosition;

void main(){

	input_texturePosition = attribute_textureVertex;

	gl_Position = vec4(attribute_vertex, 1.0);

}

