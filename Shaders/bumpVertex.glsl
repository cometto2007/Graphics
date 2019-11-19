#version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform float height;

uniform sampler2D deptTex;
uniform sampler2D grassMap;
uniform sampler2D sandMap;
uniform sampler2D sandWetMap;
uniform sampler2D cliffsMap;

in vec3 position;
in vec4 colour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal; 
	vec3 worldPos;
	float isGrass;
	float isSand;
	float isWetSand;
	float isCliffs;
	float fragHeight;
} OUT;

void main(void) {
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec4 texel = texture(deptTex, position.xz / (32.0 * 513));
	if (texture(grassMap, position.xz / (32.0 * 513)).r > 0.1){
		OUT.isGrass = 1.0f;
	} else {
		OUT.isGrass = 0.0f;
	}
	if (texture(sandMap, position.xz / (32.0 * 513)).r > 0.1){
		OUT.isSand = 1.0f;
	} else {
		OUT.isSand = 0.0f;
	}
	if (texture(sandWetMap, position.xz / (32.0 * 513)).r > 0.1){
		OUT.isWetSand = 1.0f;
	} else {
		OUT.isWetSand = 0.0f;
	}
	if (texture(cliffsMap, position.xz / (32.0 * 513)).r > 0.1){
		OUT.isCliffs = 1.0f;
	} else {
		OUT.isCliffs = 0.0f;
	}

	OUT.fragHeight = texel.r * 1000;
	
	OUT.colour = colour;
	OUT.texCoord = (textureMatrix * vec4 (texCoord * 2, 0.0, 1.0)).xy;
	
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.tangent = normalize(normalMatrix * normalize(tangent));
	OUT.binormal = normalize(normalMatrix * normalize (cross(normal, tangent)));
	
	OUT.worldPos = (modelMatrix * vec4(position ,1)).xyz;
	
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position.x, texel.r * height, position.z, 1.0);
}