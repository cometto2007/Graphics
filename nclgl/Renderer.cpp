#include "Renderer.h"

void Renderer::GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool Renderer::GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		cout << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	triangle = Mesh::GenerateTriangle();
	
	currentShader = new Shader(SHADERDIR "basicVertex.glsl",
	SHADERDIR "colourFragment.glsl");
	
	if (!currentShader->LinkProgram()) {
		return;
	}
	init = true;
}

Renderer::~Renderer(void) {
	delete triangle;
}

void Renderer::RenderScene() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(currentShader->GetProgram());
	triangle->Draw();
	glUseProgram(0);
	
	SwapBuffers();
}
