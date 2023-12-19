#version 330 core

in vec3 input_worldPosition;
in vec3 input_normal;
in vec2 input_shadowMapPosition;
in float input_shadowDepth;

out vec4 FragColor;

uniform sampler2D shadowMap; 
uniform sampler2D transparentShadowDepthMap;
uniform sampler2D transparentShadowColorMap;

uniform float ambientLightFactor;
uniform float diffuseLightFactor;
uniform vec3 lightDirection;

uniform vec4 skyColor;
uniform vec3 cameraFocus;

vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

void main(){

	FragColor = vec4(1.0, 1.0, 1.0, 1.0);

	float diffuseLight = max(dot(-lightDirection, input_normal), 0.0);

	float shadowDepthTolerance = 0.0001;
	shadowDepthTolerance = 0.0;

	float shadowMapDepth = texture2D(shadowMap, input_shadowMapPosition).r;
	float transparentShadowMapDepth = texture2D(transparentShadowDepthMap, input_shadowMapPosition).r;
	vec4 transparentShadowMapColor = texture2D(transparentShadowColorMap, input_shadowMapPosition);

	float shadowFactor = float(shadowMapDepth + shadowDepthTolerance > input_shadowDepth);
	float transparentShadowFactor = float(transparentShadowMapDepth + shadowDepthTolerance > input_shadowDepth);

	if(transparentShadowFactor == 0.0){
		lightColor.xyz = (lightColor.xyz * lightColor.a + transparentShadowMapColor.xyz * transparentShadowMapColor.a) / (lightColor.a + transparentShadowMapColor.a);
		FragColor.xyz = FragColor.xyz * (1.0 - transparentShadowMapColor.a) + transparentShadowMapColor.xyz * transparentShadowMapColor.a;
	}

	diffuseLight *= shadowFactor;

	float lightFactor = ambientLightFactor + diffuseLightFactor * diffuseLight;

	FragColor.xyz *= lightColor.xyz * lightFactor;

	//add height based sky fade gradient
	float t = min(1.0, max(0.0, -(input_worldPosition.y + 0.3) / (5.0 * 0.3 * 2.0)));
	float skyBlendFactor = (t * t + t) / 2.0;

	//add distance based sky fade gradient
	float distanceToCameraFocus = sqrt((input_worldPosition.x - cameraFocus.x) * (input_worldPosition.x - cameraFocus.x) + (input_worldPosition.z - cameraFocus.z) * (input_worldPosition.z - cameraFocus.z));
	float d = min(max(0.0, distanceToCameraFocus - 0.6 * 10.0) / 3.0, 1.0);

	skyBlendFactor += d * d;

	skyBlendFactor = min(skyBlendFactor, 1.0);

	FragColor.xyz = FragColor.xyz * (1.0 - skyBlendFactor) + skyColor.xyz * skyBlendFactor;

}  
