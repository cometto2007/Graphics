#version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform float height;

uniform sampler2D deptTex;

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
	float fragHeight;
} OUT;

void main(void) {
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	vec4 texel = texture(deptTex, position.xz / (16.0 * 513));
	OUT.fragHeight = texel.r * 1000;
	
	OUT.colour = colour;
	OUT.texCoord = (textureMatrix * vec4 (texCoord, 0.0, 1.0)).xy;
	
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.tangent = normalize(normalMatrix * normalize(tangent));
	OUT.binormal = normalize(normalMatrix * normalize (cross(normal, tangent)));
	
	OUT.worldPos = (modelMatrix * vec4(position ,1)).xyz;
	
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position.x, texel.r * height, position.z, 1.0);
}