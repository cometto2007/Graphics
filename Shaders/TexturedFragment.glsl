#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D waterTex;

in Vertex {
	vec2 texCoord;
	float fragHeight;
	vec4 	colour;
	vec2 	texCoord;
	vec3 	normal;
	vec3 	tangent;
	vec3 	worldPos;
} IN;

out vec4 fragColour;

void main(void) {
	if (IN.fragHeight < 300) {
		fragColour = texture(waterTex, IN.texCoord);
	} else {
		fragColour = texture(diffuseTex, IN.texCoord);
	}
}