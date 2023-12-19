#version 330 core
layout (location = 0) in vec3 attribute_vertex;
layout (location = 1) in vec2 attribute_textureVertex;
layout (location = 2) in vec3 attribute_normalVertex;

out vec4 input_color;
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

//uniform mat4 modelMatrix;

uniform mat4 viewMatrix;
uniform mat4 lightViewMatrix;

//uniform vec4 inputColor;

void main(){

	input_localPosition = attribute_vertex.xyz;
	input_texturePosition = attribute_textureVertex;

	vec4 vertexPosition = vec4(attribute_vertex.xyz, 1.0);
	vec4 vertexNormal = vec4(attribute_normalVertex.xyz, 1.0);

	vertexPosition.xyz *= vec3(texelFetch(entityData, gl_InstanceID * 5 + 0).w, texelFetch(entityData, gl_InstanceID * 5 + 1).xy);

	vertexPosition.xyz += texelFetch(entityData, gl_InstanceID * 5 + 0).xyz;

	input_color = vec4(texelFetch(entityData, gl_InstanceID * 5 + 1).zw, texelFetch(entityData, gl_InstanceID * 5 + 2).xy);

	input_textureAtlasOffset = texelFetch(entityData, gl_InstanceID * 5 + 2).z;

	input_colorChangeRadius = texelFetch(entityData, gl_InstanceID * 5 + 2).w;

	input_colorChangeDirection = texelFetch(entityData, gl_InstanceID * 5 + 3).xyz;

	input_colorChangeColor = texelFetch(entityData, gl_InstanceID * 5 + 4);

	/*
	mat4 modelMatrix = mat4(
		texelFetch(entityData, gl_InstanceID * 5 + 0),
		texelFetch(entityData, gl_InstanceID * 5 + 1),
		texelFetch(entityData, gl_InstanceID * 5 + 2),
		texelFetch(entityData, gl_InstanceID * 5 + 3)
	);
	*/



	//input_color = texelFetch(entityData, gl_InstanceID * 5 + 4);

	//vertexPosition.xyz *= 0.1;
	//vertexPosition.z += 0.5 * gl_InstanceID;

	input_normal = vertexNormal.xyz;
	//input_normal *= mat3(modelMatrix[0].xyz, modelMatrix[1].xyz, modelMatrix[2].xyz);
	//input_normal = normalize(input_normal);

	//vertexPosition *= modelMatrix;

	input_worldPosition = vertexPosition.xyz;

	float normalShadowToleranceOffset = 0.02;

	vec4 lightRelativeVertexPosition = (vertexPosition + vec4(input_normal * normalShadowToleranceOffset, 0.0)) * lightViewMatrix;
	input_shadowMapPosition = (lightRelativeVertexPosition.xy + vec2(1.0, 1.0)) / 2.0;
	input_shadowDepth = lightRelativeVertexPosition.z * 0.5 + 0.5;

	gl_Position = vertexPosition * viewMatrix;

	if(input_color.a < 1.0){
		gl_Position.z -= 0.0001;
	}

}
