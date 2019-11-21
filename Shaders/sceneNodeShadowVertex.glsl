#version 150 core
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowMatrix;

uniform float height;

uniform sampler2D deptTex;

in vec3 position;

float getYPosition() {
	vec4 temp = modelMatrix * vec4(0, 0, 0, 1);
	mat4 inv = inverse(modelMatrix);
	float scale = length(vec3(inv[0][0], inv[1][0], inv[2][0]));
	vec4 texel = texture(deptTex, temp.xz / (32.0f * 513.0f));
	return position.y + (height * texel.r * scale);
}

void main(void) {
	vec4 texel = texture(deptTex, position.xz / (32.0 * 513));
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position.x, getYPosition(), position.z, 1.0);
}