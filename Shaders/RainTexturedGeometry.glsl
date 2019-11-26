#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 passColour[];
out vec3 finalColour;

const float size = 2.0;

void createVertex(vec3 offset) {
	mat4 newModelMatrix = modelMatrix;
	while (newModelMatrix[3][1] < 0) {
		newModelMatrix[3][1] = newModelMatrix[3][1] + 10000.0;
	}
	mat4 mvp = projMatrix * viewMatrix * newModelMatrix;
	vec4 actualOffset = vec4(offset * size, 0.0);
	vec4 oldPos = gl_in[0].gl_Position;

	vec4 worldPosition = oldPos + actualOffset;
	vec4 newPos = mvp * worldPosition;
	gl_Position = vec4(newPos.x, newPos.y, newPos.z, newPos.w);

	EmitVertex();
}

void main(void) {
	createVertex(vec3(-1.0, 1.0, 1.0));
	createVertex(vec3(-1.0, -1.0, 1.0));
	createVertex(vec3(1.0, 1.0, 1.0));
	createVertex(vec3(1.0, -1.0, 1.0));

	EndPrimitive();
}