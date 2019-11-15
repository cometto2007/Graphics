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
	virtual void UpdateScene(float msec);

	void SwitchToPerspective();
	void SwitchToOrthographic();

	inline void SetScale(float s) { scale = s; }
	inline void SetRotation(float r) { rotation = r; }
	inline void SetPosition(Vector3 p) { position = p; }
	inline void SetFov(float f) { fov = f; }

	bool GLLogCall(const char* function, const char* file, int line);
	void GLClearError();

protected:
	Mesh* triangle;

	float scale;
	float rotation;
	float fov;
	Camera* camera;
	Vector3 position;
};


