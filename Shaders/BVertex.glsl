#version 150
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

uniform sampler2D deptTex;
uniform float height;

in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
} OUT;

void main(void) {
	vec4 temp = modelMatrix * vec4(0,0,0,1);
	vec4 texel = texture(deptTex, temp.xz / (16.0 * 513));
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position.x, position.y + (height * texel.r) - 1000, position.z, 1.0);
	OUT.texCoord = texCoord;
}