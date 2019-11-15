#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

class Renderer :
	public OGLRenderer {
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();

	void ToggleScissor();
	void ToggleStencil();

	bool GLLogCall(const char* function, const char* file, int line);
	void GLClearError();

protected:
	Mesh* triangle;
	Mesh* quad;

	bool usingScissor;
	bool usingStencil;
};



