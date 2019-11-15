#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform sampler2D deptTex;

in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
	float fragHeight;
} OUT;

void main(void) {
	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	vec4 texel = texture(deptTex, position.xz / (16.0 * 257));
	gl_Position = mvp * vec4(position.x, texel.r * 1000, position.z, 1.0);
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	OUT.fragHeight = texel.r * 1000;
}