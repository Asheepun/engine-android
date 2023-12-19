#version 330 core
layout (location = 0) in vec3 attribute_vertex;
layout (location = 1) in vec2 attribute_textureVertex;
layout (location = 2) in vec3 attribute_normalVertex;

out float input_textureAtlasOffset;
out float input_colorChangeRadius;
out vec3 input_colorChangeDirection;
out vec4 input_colorChangeColor;

out vec3 input_localPosition;
out vec3 input_worldPosition;
out vec2 input_texturePosition;
out vec3 input_normal;
out vec2 input_shadowMapPosition;
out float input_shadowDepth;

uniform samplerBuffer entityData;

uniform sampler2D noiseTexture;

uniform mat4 modelMatrix;

uniform mat4 viewMatrix;
uniform mat4 lightViewMatrix;

uniform float gameTime;

void main(){

	input_localPosition = attribute_vertex.xyz;
	input_texturePosition = attribute_textureVertex;

	vec4 vertexPosition = vec4(attribute_vertex.xyz, 1.0);
	vec4 vertexNormal = vec4(attribute_normalVertex.xyz, 1.0);

	//vertexPosition.xyz *= 1.05 + 0.03 * (1.0 + sin(vertexPosition.x + vertexPosition.y + gameTime / 30.0));
	//vertexPosition.xyz *= 1.1 + 0.07 * texture2D(noiseTexture, input_texturePosition + vec2(float(int(gameTime) % 500) / 500.0)).r;
	//vertexPosition.xyz *= 1.1;

	input_normal = vertexNormal.xyz;

	vertexPosition *= modelMatrix;

	input_worldPosition = vertexPosition.xyz;

	float normalShadowToleranceOffset = 0.02;

	vec4 lightRelativeVertexPosition = (vertexPosition + vec4(input_normal * normalShadowToleranceOffset, 0.0)) * lightViewMatrix;
	input_shadowMapPosition = (lightRelativeVertexPosition.xy + vec2(1.0, 1.0)) / 2.0;
	input_shadowDepth = lightRelativeVertexPosition.z * 0.5 + 0.5;

	gl_Position = vertexPosition * viewMatrix;

	gl_Position.z -= 0.0001;

}
