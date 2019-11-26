#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
out vec3 passColour;

void main(void) {
	gl_Position = vec4(position, 1.0);
	passColour = vec3(0.10, 0.32, 0.69);
}