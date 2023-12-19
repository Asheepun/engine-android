#version 330 core

in vec2 textureCoord;

uniform float inputAngle;

float PI = 3.14159;
float SIXTH = PI * 2.0 / 6.0;

vec3 rgbFromAngle(float angle){

	vec3 rgb = vec3(0.0, 0.0, 0.0);

	rgb.r = float(angle < SIXTH) * angle / SIXTH +
			float(angle >= SIXTH && angle < SIXTH * 3) +
			float(angle >= SIXTH * 3 && angle < SIXTH * 4) * (SIXTH * 4 - angle) / SIXTH;

	rgb.g = float(angle >= SIXTH * 2 && angle < SIXTH * 3) * (angle - SIXTH * 2) / SIXTH +
			float(angle >= SIXTH * 3 && angle < SIXTH * 5) +
			float(angle >= SIXTH * 5 && angle < SIXTH * 6) * (SIXTH * 6 - angle) / SIXTH;

	rgb.b = float(angle >= SIXTH * 4 && angle < SIXTH * 5) * (angle - SIXTH * 4) / SIXTH +
			float(angle >= SIXTH * 5 && angle < SIXTH * 6 || angle >= 0 && angle < SIXTH) +
			float(angle >= SIXTH && angle < SIXTH * 2) * (SIXTH * 2 - angle) / SIXTH;

	return rgb;

}

void main(){

	gl_FragDepth = 0.0;

	vec2 pos = vec2(textureCoord.x - 0.5, 0.5 - textureCoord.y);

	float angle = acos(dot(normalize(pos), vec2(1.0, 0.0)));
	if(pos.y < 0.0){
		angle = PI * 2.0 - angle;
	}

	vec4 color = vec4(rgbFromAngle(angle), 1.0);

	if(length(pos) > 0.4){
		color.a = 0.0;
	}

	if(length(pos) < 0.3){
		color.rgb = vec3(0.5, 0.5, 0.5);
	}

	if(abs(angle - inputAngle) < 0.05
	&& length(pos) > 0.35
	&& length(pos) < 0.45){
		color.rgb = vec3(0.3, 0.3, 0.3);
		color.a = 1.0;
		gl_FragDepth = 1.0;
	}

	gl_FragColor = color;

}

