#include "../../nclgl/window.h"
#include "Renderer.h"

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
	w.ShowOSPointer(false);

	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		/*if (Window::GetKeyboard()->KeyDown(KEYBOARD_1)) {
			Vector2 v = w.GetMouse()->GetAbsolutePosition();
			GLint viewport[4]; //var to hold the viewport info
			GLdouble modelview[16]; //var to hold the modelview info
			GLdouble projection[16]; //var to hold the projection matrix info
			GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
			GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

			glGetDoublev(GL_MODELVIEW_MATRIX, modelview); //get the modelview info
			glGetDoublev(GL_PROJECTION_MATRIX, projection); //get the projection matrix info
			glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

			winX = (float)v.x;
			winY = (float)viewport[3] - (float)v.y;
			winZ = 0;

			//get the world coordinates from the screen coordinates
			gluUnProject(winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
			cout << worldX << "\n";
			cout << worldY << "\n";
			cout << worldZ << "\n";
			
		}*/
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());
		renderer.RenderScene();
	}
	return 0;
}

