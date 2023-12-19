#version 330 core
layout (location = 0) in vec3 attribute_vertex;
layout (location = 1) in vec2 attribute_textureVertex;
layout (location = 2) in vec3 attribute_normalVertex;

out vec3 input_worldPosition;
out vec3 input_normal;
out vec2 input_shadowMapPosition;
out float input_shadowDepth;

uniform samplerBuffer particleData;

uniform mat4 viewMatrix;
uniform mat4 lightViewMatrix;

void main(){

	vec4 vertexPosition = vec4(attribute_vertex.xyz, 1.0);
	vec4 vertexNormal = vec4(attribute_normalVertex.xyz, 1.0);

	input_normal = vertexNormal.xyz;

	vec4 data = texelFetch(particleData, gl_InstanceID);

	vertexPosition.xyz *= data.w;
	vertexPosition.xyz += data.xyz;

	input_worldPosition = vertexPosition.xyz;

	float normalShadowToleranceOffset = 0.02;

	vec4 lightRelativeVertexPosition = (vertexPosition + vec4(input_normal * normalShadowToleranceOffset, 0.0)) * lightViewMatrix;
	input_shadowMapPosition = (lightRelativeVertexPosition.xy + vec2(1.0, 1.0)) / 2.0;
	input_shadowDepth = lightRelativeVertexPosition.z * 0.5 + 0.5;

	gl_Position = vertexPosition * viewMatrix;

}
