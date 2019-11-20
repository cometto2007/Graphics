#version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowMatrix;

uniform float height;

uniform sampler2D deptTex;

in vec3 position;

void main(void) {
	vec4 texel = texture(deptTex, position.xz / (32.0 * 513));
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position.x, position.y + (texel.r * height), position.z, 1.0);
}