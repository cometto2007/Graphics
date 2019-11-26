#version 150 core

uniform sampler2D diffuseTex;

in vec3 finalColour;
out vec4 fragColour;

void main(void) {
	fragColour = vec4(finalColour, 1.0);
}