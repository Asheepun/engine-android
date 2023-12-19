#version 330 core

in vec4 input_color;
in float input_textureAtlasOffset;

in vec3 input_localPosition;
in vec3 input_worldPosition;
in vec2 input_texturePosition;
in vec3 input_normal;
in vec2 input_shadowMapPosition;
in float input_shadowDepth;

out vec4 FragColor;

uniform sampler2D shadowMap; 
uniform sampler2D transparentShadowDepthMap;
uniform sampler2D transparentShadowColorMap;
uniform sampler2D colorTextureAtlas;
uniform sampler2D noiseTexture;

uniform float textureAtlasWidth;

uniform vec2 shadowMapPixelScale;

uniform float ambientLightFactor;
uniform float diffuseLightFactor;
uniform vec3 lightDirection;

uniform float colorChangeRadius;
uniform vec3 colorChangeDirection;
uniform vec4 colorChangeColor;

uniform vec4 skyColor;
uniform float shouldSkyBlend;
uniform vec3 cameraFocus;

uniform float time;

float borderWidth = 0.1;

vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);

void main(){

	FragColor = input_color;

	FragColor *= texture2D(colorTextureAtlas, vec2(input_texturePosition.x, input_textureAtlasOffset / textureAtlasWidth + (1.0 - input_texturePosition.y) / textureAtlasWidth));

	float diffuseLight = max(dot(-lightDirection, input_normal), 0.0);

	float shadowDepthTolerance = 0.0001;
	shadowDepthTolerance = 0.0;

	float shadowMapDepth = texture2D(shadowMap, input_shadowMapPosition).r;
	float transparentShadowMapDepth = texture2D(transparentShadowDepthMap, input_shadowMapPosition).r;
	vec4 transparentShadowMapColor = texture2D(transparentShadowColorMap, input_shadowMapPosition);

	float shadowFactor = float(shadowMapDepth + shadowDepthTolerance > input_shadowDepth);
	//float transparentShadowFactor = float(transparentShadowMapDepth + shadowDepthTolerance > input_shadowDepth);

	//lightColor.xyz = transparentShadowFactor * lightColor.xyz + (1.0 - transparentShadowFactor) * (lightColor.xyz * lightColor.a + transparentShadowMapColor.xyz * transparentShadowMapColor.a);
	//FragColor.xyz = transparentShadowFactor * FragColor.xyz + (1.0 - transparentShadowFactor) * (FragColor.xyz * (1.0 - transparentShadowMapColor.a) + transparentShadowMapColor.xyz * transparentShadowMapColor.a);

	if(transparentShadowMapDepth + shadowDepthTolerance < input_shadowDepth){

		lightColor.xyz = (lightColor.xyz * lightColor.a + transparentShadowMapColor.xyz * transparentShadowMapColor.a) / (lightColor.a + transparentShadowMapColor.a);
		FragColor.xyz = FragColor.xyz * (1.0 - transparentShadowMapColor.a) + transparentShadowMapColor.xyz * transparentShadowMapColor.a;

	}

	diffuseLight *= shadowFactor;

	float lightFactor = ambientLightFactor + diffuseLightFactor * diffuseLight;

	FragColor.xyz *= lightColor.xyz * lightFactor;

	//add height based sky fade gradient
	float t = min(1.0, max(0.0, -(input_worldPosition.y + 0.3) / (5.0 * 0.3 * 2.0)));
	float skyBlendFactor = (t * t + t) / 2.0;

	vec2 texturePosition = vec2(input_worldPosition.x / 10.0 + input_worldPosition.z / 10.0, input_worldPosition.y / 10.0);
	vec2 texturePosition2 = vec2(input_worldPosition.x / 10.0, + input_worldPosition.z / 10.0);
	float curl = texture2D(noiseTexture, texturePosition).r;
	skyBlendFactor += skyBlendFactor * texture2D(noiseTexture, texturePosition + vec2(curl) * 0.02 + vec2(time * 0.0003, 0.0)).r * 1.7;

	//add distance based sky fade gradient
	float distanceToCameraFocus = sqrt((input_worldPosition.x - cameraFocus.x) * (input_worldPosition.x - cameraFocus.x) + (input_worldPosition.z - cameraFocus.z) * (input_worldPosition.z - cameraFocus.z));
	float d = min(max(0.0, distanceToCameraFocus - 0.6 * 10.0) / 3.0, 1.0);

	skyBlendFactor += d * d;

	skyBlendFactor = min(skyBlendFactor, 1.0);

	skyBlendFactor *= shouldSkyBlend;

	FragColor.xyz = FragColor.xyz * (1.0 - skyBlendFactor) + skyColor.xyz * skyBlendFactor;

}  
