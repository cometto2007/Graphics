#include "../../nclgl/window.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main() {
	Window w("Index Buffers!", 1200, 800, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(false);
	w.ShowOSPointer(false);

	renderer.setCameraPosFromIndex(0);
	renderer.activateSlideshow(true);
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
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_9)) {
			renderer.activateSlideshow(false);
		}

		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}
	Loader::getInstance().Destroy();
	return 0;
}

