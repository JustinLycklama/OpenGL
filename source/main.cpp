// standard C++ libraries
#include <iostream>
#include <stdexcept>
#include <cmath>

#include "Window.h"
#include "Viewer.h"

int main(int argc, char *argv[]) {
    try {

		Window* window = new Window();
		Viewer* viewer = new Viewer();
		Camera* camera = new Camera();

		window->setCamera(camera);
		window->setViewer(viewer);

		viewer->setCamera(camera);
		viewer->setWindow(window);

		viewer->initialize();

		// run while the window is open
		double lastTime = glfwGetTime();
		while(glfwGetWindowParam(GLFW_OPENED)){

			glfwPollEvents();

			double thisTime = glfwGetTime();
			float secondsElapsed = thisTime - lastTime;
			lastTime = thisTime;

			window->Update(secondsElapsed);
			viewer->Update(secondsElapsed);

			window->Render();
			viewer->Render();
		}

		// clean up and exit
		glfwTerminate();

    } catch (const std::exception& e){
        std::cerr << "\n\nERROR: " << e.what() << std::endl;
		getchar();
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}