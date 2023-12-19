#version 330 core

in vec2 input_texturePos;

out vec4 FragColor;

uniform sampler2D colorTexture;

uniform vec4 inputColor;

void main(){

	FragColor = texture2D(colorTexture, input_texturePos);

	FragColor *= inputColor;

}
