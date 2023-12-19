#version 330 core

in vec2 input_texturePosition;

out vec4 FragColor;

uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float far;
uniform float near;

uniform mat3 viewSpaceMatrix;

void main(){

	vec2 texturePosition = vec2(input_texturePosition.x, 1.0 - input_texturePosition.y);
	vec2 screenPosition = texturePosition * 2.0 - vec2(1.0);

	//vec3 normal = texture2D(normalMap, texturePosition).xyz;
	vec3 rawNormal = texture2D(normalMap, texturePosition).xyz;
	vec3 normal = normalize(rawNormal * 2.0 - vec3(1.0)) * viewSpaceMatrix;
	normal.z *= -1.0;

	//FragColor = vec4(normal, 1.0);

	//FragColor = vec4(screenPosition, 0.0, 1.0);
	//FragColor = vec4(gl_FragCoord.x / 1336.0, 0.0, 0.0, 1.0);
	
	float depth = texture2D(depthMap, vec2(texturePosition)).r;

	float z = ((depth * 2.0 - 1.0) * (far - near) + 2 * far * near) / (far + near);
	float x = screenPosition.x * z * (16.0 / 9.0);
	float y = screenPosition.y * z;

	float rayLength = 0.1;
	vec3 rayDirection = normal;

	vec3 checkPos = vec3(x, y, z) + rayDirection * rayLength;
	vec2 checkTexturePosition = (vec2(checkPos.x, checkPos.y) / checkPos.z + vec2(1.0, 1.0)) / 2.0;
	float checkDepth = texture2D(depthMap, checkTexturePosition).r;
	float checkZ = ((checkDepth * 2.0 - 1.0) * (far - near) + 2 * far * near) / (far + near);

	float occlusion = 0.7;

	if(checkZ > checkPos.z){
		occlusion = 0.3;
	}

	FragColor = vec4(vec3(float(length(rawNormal) == 0.0) + occlusion), 1.0);


	//FragColor = vec4(vec3(z / 10.0), 1.0);
	//FragColor = vec4(vec2(x, y) / 10.0, 0.0, 1.0);

} 

