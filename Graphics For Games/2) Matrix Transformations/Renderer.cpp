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
	camera = new Camera;

	currentShader = new Shader(SHADERDIR "MatrixVertex.glsl",
		SHADERDIR "colourFragment.glsl");

	if (!currentShader->LinkProgram()) {
		return;
	}
	init = true;

	SwitchToOrthographic();
}

Renderer::~Renderer(void) {
	delete triangle;
	delete camera;
}

void Renderer::RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());

	// Change random colors
	float n1 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float n2 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float n3 = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	Vector4 data[3];
	data[0] = Vector4(n1, n2, n3, 1.0f);
	data[1] = Vector4(n3, n2, n1, 1.0f);
	data[2] = Vector4(n2, n3, n1, 1.0f);
	memcpy(triangle->getColorBufferPointer(), data, sizeof(data));
	//

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"), 1, false, (float*)&projMatrix);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"), 1, false, (float*)&viewMatrix);

	for (int i = 0; i < 3; i++) {
		Vector3 tempPos = position;
		tempPos.z += (i * 500.0f);
		tempPos.x -= (i * 100.0f);
		tempPos.y -= (i * 100.0f);

		modelMatrix = Matrix4::Translation(tempPos) *
			Matrix4::Rotation(rotation, Vector3(0, 1, 0)) *
			Matrix4::Scale(Vector3(scale, scale, scale));

		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);
		triangle->Draw();
	}
	glUseProgram(0);
	SwapBuffers();
}

void Renderer::UpdateScene(float msec)
{
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	
}

void Renderer::SwitchToPerspective() {
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, fov);
}

void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 10000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}
