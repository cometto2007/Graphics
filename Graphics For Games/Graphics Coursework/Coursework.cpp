#include "../../nclgl/window.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main() {
	Window w("Index Buffers!", 1600, 1200, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(false);
	w.ShowOSPointer(true);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_0)) {
			renderer.setCameraPosFromIndex(0);
			renderer.activateSlideshow(true);
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
			renderer.activateSlideshow(false);
			renderer.setCameraPosFromIndex(1);
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
			renderer.activateSlideshow(false);
			renderer.setCameraPosFromIndex(2);
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
			renderer.activateSlideshow(false);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_LEFT)) {
			renderer.moveLight(-100.0f, 0.0f, 0.0f);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_RIGHT)) {
			renderer.moveLight(100.0f, 0.0f, 0.0f);
		}
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
			renderer.moveLight(0.0f, 0.0f, 100.0f);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
			renderer.moveLight(0.0f, 0.0f, -100.0f);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_U)) {
			renderer.moveLight(0.0f, 100.0f, 0.0f);
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_J)) {
			renderer.moveLight(0.0f, -100.0f, 0.0f);
		}
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}
	return 0;
}

