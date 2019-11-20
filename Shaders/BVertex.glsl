#version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowMatrix;

uniform sampler2D deptTex;
uniform float height;

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
	vec4 shadowProj;
} OUT;

float getYPosition() {
	vec4 temp = modelMatrix * vec4(0, 0, 0, 1);
	mat4 inv = inverse(modelMatrix);
	float scale = length(vec3(inv[0][0], inv[1][0], modelMatrix[2][0]));
	vec4 texel = texture(deptTex, temp.xz / (32.0f * 513.0f));
	return position.y + (height * texel.r * scale);
}

void main(void) {
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position.x, getYPosition(), position.z, 1.0);
	
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));
	OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.tangent = normalize(normalMatrix * normalize(tangent));
	OUT.binormal = normalize(normalMatrix * normalize(cross(normal, tangent)));
	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;
	OUT.shadowProj = (shadowMatrix * vec4(position + (normal * 1.5), 1));

	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	OUT.colour = colour;
}

