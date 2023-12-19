#version 330 core

in vec2 input_texturePos;

out vec4 FragColor;

uniform sampler2D colorTexture;

uniform vec4 inputColor;

void main(){

	vec4 textureColor = texture2D(colorTexture, input_texturePos);

	float alpha = textureColor.w * textureColor.r;

	if(alpha < 0.01){
		discard;
	}

	FragColor = inputColor;
	FragColor.w = alpha;

}
