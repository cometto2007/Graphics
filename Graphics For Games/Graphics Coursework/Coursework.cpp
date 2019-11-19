#include "../../nclgl/window.h"
#include "Renderer.h"
#include "MousePicker.h"

#pragma comment(lib, "nclgl.lib")

int main() {
	Window w("Index Buffers!", 800, 600, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(false);
	w.ShowOSPointer(true);

	MousePicker* picker = new MousePicker(renderer.GetCamera(), renderer.GetProjectionMatrix(), renderer.GetTerrain());
	renderer.setMousePicker(picker);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
			renderer.setCameraConfsIndex(0);
			renderer.setCameraAuto(false);
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
			renderer.setCameraConfsIndex(1);
			renderer.setCameraAuto(false);
		}
		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_0)) {
			renderer.setCameraConfsIndex(0);
			renderer.setCameraAuto(true);
		}
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		//picker->update();
		//cout << picker->getRay();
		renderer.RenderScene();
	}
	return 0;
}

