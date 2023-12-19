#version 330 core
layout (location = 0) in vec3 attribute_vertex;
layout (location = 1) in vec2 attribute_textureVertex;
layout (location = 2) in vec3 attribute_normalVertex;

out vec4 input_color;
out float input_textureAtlasOffset;

out vec3 input_localPosition;
out vec3 input_worldPosition;
out vec2 input_texturePosition;
out vec3 input_normal;
out vec2 input_shadowMapPosition;
out float input_shadowDepth;

uniform samplerBuffer obstacleData;

uniform vec4 inputColor;

uniform float gameTime;

uniform mat4 viewMatrix;
uniform mat4 lightViewMatrix;

uniform float scaleT;
uniform float fadeDirection;

void main(){

	input_localPosition = attribute_vertex.xyz;
	input_texturePosition = attribute_textureVertex;

	vec4 vertexPosition = vec4(attribute_vertex.xyz, 1.0);
	vec4 vertexNormal = vec4(attribute_normalVertex.xyz, 1.0);

	vec4 instanceData = texelFetch(obstacleData, gl_InstanceID);
	vec3 pos = instanceData.xyz;
	float scale = pos.x - floor(pos.x);
	float textureAtlasOffset = floor(instanceData.w);
	float seed = instanceData.w - floor(instanceData.w);
	float hoverFactor = pos.y - floor(pos.y);

	//vertexPosition.xyz *= scale;
	float t = min(max(scaleT - seed * 0.5, 0.0), 1.0);
	t = t * t * t * (t * (6.0 * t - 15.0) + 10.0);//smoothstep
	vertexPosition.xyz = fadeDirection * (0.3 * vertexPosition.xyz * t) + (1.0 - fadeDirection) * (0.3 * vertexPosition.xyz * (1.0 - t));
	//vertexPosition.xyz *= 0.3 * seed;

	vertexPosition.xyz += floor(pos) * 0.3 * 2.0;

	input_textureAtlasOffset = textureAtlasOffset;

	input_color = inputColor;
	input_color.xyz *= 0.7 + 0.3 * seed;
	input_color.xyz *= (1.0 - hoverFactor);

	input_normal = vertexNormal.xyz;

	input_worldPosition = vertexPosition.xyz;

	float normalShadowToleranceOffset = 0.02;

	vec4 lightRelativeVertexPosition = (vertexPosition + vec4(input_normal * normalShadowToleranceOffset, 0.0)) * lightViewMatrix;
	input_shadowMapPosition = (lightRelativeVertexPosition.xy + vec2(1.0, 1.0)) / 2.0;
	input_shadowDepth = lightRelativeVertexPosition.z * 0.5 + 0.5;

	gl_Position = vertexPosition * viewMatrix;

}
