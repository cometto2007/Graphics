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
uniform vec4 lightColourArr[2];
uniform vec3 lightPosArr[2];
uniform float lightRadiusArr[2];

uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

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

vec3 applyTex(vec4 texel, float[2] attens, float[2] lamberts) {
	vec3 colour = (texel.rgb * lightColourArr[0].rgb);
	for (int i = 1; i < 2; i++) {
		colour *= (texel.rgb * lightColourArr[i].rgb);
	}
	fragColour = vec4(colour * attens[0] * lamberts[0], texel.a);
	for (int i = 1; i < 2; i++) {
		fragColour *= vec4(colour * attens[i] * lamberts[i], texel.a);
	}
	fragColour.rgb += (texel.rgb * lightColourArr[0].rgb) * 0.1;
	for (int i = 1; i < 2; i++) {
		fragColour.rgb += (texel.rgb * lightColourArr[i].rgb) * 0.1;
	}
	return colour;
}

void main(void) {
	vec4 diffuse = texture(diffuseTex , IN.texCoord);
	vec4 grass = texture(grassTex , IN.texCoord);
	vec4 sand = texture(sandTex , IN.texCoord);
	vec4 wetSand = texture(sandWetTex , IN.texCoord);
	vec4 cliffs = texture(cliffsTex, IN.texCoord);
	
	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
	
	vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));;
	if (IN.isCliffs > 0.5f) {
		normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));
	} else if (IN.isGrass > 0.5f) {
		normal = normalize(TBN * (texture(grassBumpMap, IN.texCoord).rgb * 2.0 - 1.0));
	}

	//vec3 normal = normalize(TBN *(texture(bumpTex, IN.texCoord).rgb * 2.0 - 1.0));
	float lamberts[2];
	float attens[2];
	float sFactors[2];
	for (int i = 0; i < 2; i++) {
		vec3 incident = normalize(lightPosArr[i] - IN.worldPos);
		lamberts[i] = max(0.0, dot(incident, normal));
		lamberts[i] *= calculateShadow();
		float dist = length(lightPosArr[i] - IN.worldPos);
		attens[i] = 1.0 - clamp(dist / lightRadiusArr[i], 0.0, 1.0);

		vec3 viewDir = normalize(cameraPos - IN.worldPos);
		vec3 halfDir = normalize(incident + viewDir);

		float rFactor = max(0.0, dot(halfDir, normal));
		sFactors[i] = pow(rFactor, 33.0);
	}

	vec3 colour = (lightColourArr[0].rgb * sFactors[0]) * 0.33;
	for (int i = 1; i < 2; i++) {
		colour *= (lightColourArr[i].rgb * sFactors[i]) * 0.33;
	}

	if (IN.isCliffs > 0.5f) {
		colour += applyTex(cliffs, attens, lamberts);
	}
	else if (IN.isSand > 0.5f) {
		if (IN.isWetSand > 0.5f) {
			colour += applyTex(wetSand, attens, lamberts);
		}
		colour += applyTex(sand, attens, lamberts);
	}
	else if (IN.isGrass > 0.5f) {
		colour += applyTex(grass, attens, lamberts);
	}
	else {
		colour += applyTex(diffuse, attens, lamberts);
	}
	/*
	vec3 incident = normalize(lightPos - IN.worldPos);
	float lambert = max(0.0, dot(incident, normal));
	lambert *= calculateShadow();
	
	float dist = length(lightPos - IN.worldPos);
	float atten = 1.0 - clamp(dist/lightRadius, 0.0, 1.0);
	
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);
	
	float rFactor = max(0.0, dot(halfDir, normal));
	float sFactor = pow(rFactor, 33.0);
	
	vec3 colour = (lightColour.rgb * sFactor) * 0.33;
	if (IN.isCliffs > 0.5f) {
		colour += (cliffs.rgb * lightColour.rgb);
		fragColour = vec4(colour * atten * lambert, cliffs.a);
		fragColour.rgb += (cliffs.rgb * lightColour.rgb) * 0.1;
	} else if (IN.isSand > 0.5f) {
		if (IN.isWetSand > 0.5f) {
			colour += (wetSand.rgb * lightColour.rgb);
			fragColour = vec4(colour * atten * lambert, wetSand.a);
			fragColour.rgb += (wetSand.rgb * lightColour.rgb) * 0.1;
		}
		colour += (sand.rgb * lightColour.rgb);
		fragColour = vec4(colour * atten * lambert, sand.a);
		fragColour.rgb += (sand.rgb * lightColour.rgb) * 0.1;
	} else if (IN.isGrass > 0.5f) {
		colour += (grass.rgb * lightColour.rgb);
		fragColour = vec4(colour * atten * lambert, grass.a);
		fragColour.rgb += (grass.rgb * lightColour.rgb) * 0.1;
	} else {
		colour += (diffuse.rgb * lightColour.rgb);
		fragColour = vec4(colour * atten * lambert, diffuse.a);
		fragColour.rgb += (diffuse.rgb * lightColour.rgb) * 0.1;
	}*/
}