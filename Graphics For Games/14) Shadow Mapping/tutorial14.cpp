#pragma comment(lib, "nclgl.lib")

#include "../NCLGL/window.h"
#include "Renderer.h"

int main() {
	Window w("Shadow Mapping! MD5 mesh courtesy of http://www.katsbits.com/", 800,600,false); //This is all boring win32 window creation stuff!
	if(!w.HasInitialised()) {
		return -1;
	}
	
	Renderer renderer(w); //This handles all the boring OGL 3.2 initialisation stuff, and sets up our tutorial!
	if(!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(false);
	w.ShowOSPointer(false);

	while(w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)){
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
			renderer.moveLight(-10.0f, 0.0f);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
			renderer.moveLight(10.0f, 0.0f);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
			renderer.moveLight(0.0f, 10.0f);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
			renderer.moveLight(0.0f, -10.0f);
		}
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}

	return 0;
}