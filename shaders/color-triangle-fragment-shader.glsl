#version 330 core

in vec2 textureCoord;

uniform float inputAngle;
uniform vec2 inputPos;

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

	vec2 pos = vec2((textureCoord.x - 0.5) * 2.0, textureCoord.y);

	//vec4 color = vec4(pos.x, pos.y, 0.0, 1.0);
	vec4 color = vec4(rgbFromAngle(inputAngle), 1.0);

	//create triangle
	if(abs(pos.x) + 1.0 - pos.y > 1.0){
		color.a = 0.0;
		gl_FragDepth = 1.0;
	}

	float blackFactor = (pos.y - pos.x) / 2.0;
	float whiteFactor = (pos.x + pos.y) / 2.0;

	color.xyz = vec3(0.0, 0.0, 0.0) * blackFactor + vec3(1.0, 1.0, 1.0) * whiteFactor + color.xyz * (1.0 - blackFactor - whiteFactor);

	/*
	vec2 origin = vec2(0.0, 1.0);
	vec2 v1 = vec2(-1.0, -1.0);
	vec2 v2 = vec2(1.0, -1.0);

	color.rgb *= 1.0 - length(pos - origin) / 2.0;

	float black = 1.0 - length(pos - v1) / 2.0;
	float white = 1.0 - length(pos - v2) / 2.0;

	vec3 mixRgb = (mix(color.rgb, vec3(0.0, 0.0, 0.0), black) +
				mix(color.rgb, vec3(1.0, 1.0, 1.0), white)) / 2.0;

	color.rgb = mixRgb;
	*/

	//color.rgb = mix(color.rgb, vec3(0.0, 0.0, 0.0), black);
	//color.rgb = mix(color.rgb, vec3(1.0, 1.0, 1.0), white);

	//color.rgb = (mix(color.rgb, vec3(0.0, 0.0, 0.0), black) +
				//mix(color.rgb, vec3(1.0, 1.0, 1.0), white)) / 2.0;

	//draw selection circle

	if(length((pos - inputPos) * vec2(0.5, 1.0)) < 0.09
	&& length((pos - inputPos) * vec2(0.5, 1.0)) > 0.05){
		color = vec4(0.3, 0.3, 0.3, 1.0);
	}

	gl_FragColor = color;

}


