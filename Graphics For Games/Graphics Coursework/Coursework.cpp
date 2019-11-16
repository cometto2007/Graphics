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

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);

	//MousePicker* picker = new MousePicker(renderer.GetCamera(), renderer.GetProjectionMatrix(), renderer.GetTerrain());

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		//picker->update();
		//cout << picker->getRay();
		

		renderer.RenderScene();
	}
	return 0;
}

