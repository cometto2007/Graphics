#version 150 core
#pragma optionNV unroll all

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform sampler2D grassBumpMap;
uniform sampler2D grassTex;
uniform sampler2D sandTex;
uniform sampler2D sandWetTex;
uniform sampler2D cliffsTex;

uniform sampler2DShadow shadowTex;

uniform vec3 cameraPos;

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

uniform vec4 lightColour1;
uniform vec3 lightPos1;
uniform float lightRadius1;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	vec4 shadowProj;
	float isGrass;
	float isSand;
	float isWetSand;
	float isCliffs;
} IN;
	
out vec4 fragColour;

float calculateShadow() {
	float shadow = 1.0;

	if (IN.shadowProj.w > 0.0) {
		shadow = textureProj(shadowTex, IN.shadowProj);
	}
	return shadow;
}

vec4 applyTex(vec4 texel, float atten, float lambert, vec4 lightColour) {
	vec3 colour = texel.rgb * lightColour.rgb;
	vec4 ret = vec4(colour * atten * lambert, texel.a);
	ret.rgb += (texel.rgb * lightColour.rgb) * 0.1;
	return ret;
}

vec4 calculateLight(vec4 lightColour, vec3 lightPos, float lightRadius) {
	vec4 diffuse = texture(diffuseTex, IN.texCoord);
	vec4 grass = texture(grassTex, IN.texCoord);
	vec4 sand = texture(sandTex, IN.texCoord);
	vec4 wetSand = texture(sandWetTex, IN.texCoord);
	vec4 cliffs = texture(cliffsTex, IN.texCoord);

	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);

	vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));;
	if (IN.isCliffs > 0.5f) {
		normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));
	}
	else if (IN.isGrass > 0.5f) {
		normal = normalize(TBN * (texture(grassBumpMap, IN.texCoord).rgb * 2.0 - 1.0));
	}

	vec3 incident = normalize(lightPos - IN.worldPos);

	float lambert = max(0.0, dot(incident, normal));
	lambert *= calculateShadow();

	float atten = 1.0 - clamp(length(lightPos - IN.worldPos) / lightRadius, 0.0, 1.0);

	vec3 halfDir = normalize(incident + normalize(cameraPos - IN.worldPos));

	float sFactor = pow(max(0.0, dot(halfDir, normal)), 33.0);

	vec3 colour = (lightColour.rgb * sFactor) * 0.33;

	vec4 res;
	if (IN.isCliffs > 0.5f) {
		res = applyTex(cliffs, atten, lambert, lightColour);
	}
	else if (IN.isSand > 0.5f) {
		if (IN.isWetSand > 0.5f) {
			res = applyTex(wetSand, atten, lambert, lightColour);
			res += applyTex(sand, atten, lambert, lightColour);
		}
		res = applyTex(sand, atten, lambert, lightColour);
	}
	else if (IN.isGrass > 0.5f) {
		res = applyTex(grass, atten, lambert, lightColour);
	}
	else {
		res = applyTex(diffuse, atten, lambert, lightColour);
	}
	return res;
}


void main(void) {
	fragColour = calculateLight(lightColour, lightPos, lightRadius) + calculateLight(lightColour1, lightPos1, lightRadius1);
}