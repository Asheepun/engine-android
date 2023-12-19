#version 330 core

in vec3 input_localPosition;
in vec3 input_worldPosition;
in vec2 input_texturePosition;
in vec3 input_normal;
in vec2 input_shadowMapPosition;
in float input_shadowDepth;

out vec4 FragColor;

uniform sampler2D noiseTexture;
uniform sampler2D shadowMap; 
uniform sampler2D transparentShadowDepthMap;
uniform sampler2D transparentShadowColorMap;

uniform float textureAtlasWidth;

uniform vec2 shadowMapPixelScale;

uniform vec4 inputColor;
uniform vec4 skyColor;

uniform float ambientLightFactor;
uniform float diffuseLightFactor;
uniform vec3 lightDirection;

uniform float gameTime;

vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

void main(){

	//float noiseFactor = texture2D(noiseTexture, input_texturePosition + vec2(float(int(gameTime) % 500) / 500.0)).r;

	FragColor = inputColor;

	//FragColor.xyz = FragColor.xyz * noiseFactor + vec3(1.0) * (1.0 - noiseFactor);

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

}  
