#include "photon/window.hpp"

#include <GLFW/glfw3.h>

#include <thread>
#include <atomic>
#include <iostream>

struct photon::window::impl
{
	std::thread rendererThread;
	std::atomic<bool> threadRuning;
	static unsigned int windowCount;
};

static void renderLoop(std::atomic<bool>& runing);

unsigned int photon::window::impl::windowCount = 0;

photon::window::window()
{
	pimpl = new impl;
	if(pimpl->windowCount == 0)
		if(!glfwInit())
			throw "failed to initialize glfw";
	pimpl->windowCount++;
	pimpl->threadRuning = true;
	pimpl->rendererThread = std::thread(renderLoop, std::ref(pimpl->threadRuning));
}

photon::window::~window()
{
	pimpl->threadRuning = false;
	pimpl->rendererThread.join();
	pimpl->windowCount--;
	if(pimpl->windowCount == 0)
		glfwTerminate();
	delete pimpl;
}


static void renderLoop(std::atomic<bool>& runing){
	GLFWwindow* window;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		return;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window) && runing)
	{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
	}
	glfwDestroyWindow(window);
}