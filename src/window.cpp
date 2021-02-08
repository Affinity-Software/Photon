#include "photon/window.hpp"

#include <GLFW/glfw3.h>

#include <thread>
#include <atomic>
#include <iostream>

using namespace photon;
struct window::impl
{
	std::thread rendererThread;
	std::atomic<bool> threadRuning;
	static unsigned int windowCount;
	std::shared_ptr<_dom> dom;
};

static void renderLoop(std::atomic<bool>& runing);

unsigned int window::impl::windowCount = 0;

window::window()
{
	pimpl = new impl;
	if(pimpl->windowCount == 0)
		if(!glfwInit())
			throw "failed to initialize glfw";
	pimpl->windowCount++;
	pimpl->threadRuning = true;
	pimpl->rendererThread = std::thread(renderLoop, std::ref(pimpl->threadRuning));
	pimpl->dom = std::make_shared<_dom>();
}

window::~window()
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
	glfwSwapInterval(1);

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

std::shared_ptr<_dom> window::getDom(){
	return pimpl->dom;
}