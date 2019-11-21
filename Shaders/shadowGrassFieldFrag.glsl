#version 150 core

in float discardFrag;

out vec4 fragColour;

void main(void) {
	//if (discardFrag > 0.5) {
	//	discard;
	//}
	fragColour = vec4(1.0);
}