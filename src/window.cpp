#include "photon/window.hpp"
#include "dom.hpp"
#include "renderer/renderer.hpp"

#include <GLFW/glfw3.h>

#include <thread>
#include <atomic>
#include <iostream>
#include <exception>

using namespace photon;
struct window::impl
{
	std::thread rendererThread;
	std::atomic<bool> threadRuning;
	static unsigned int windowCount;
	std::shared_ptr<_dom> dom;
};

//this function comes from renderLoop.cpp
void renderLoop(std::atomic<bool>& runing, GLFWwindow* window);

unsigned int window::impl::windowCount = 0;

window::window()
{
	pimpl = new impl;
	if(pimpl->windowCount == 0)
		if(!glfwInit())
			throw std::system_error();
	pimpl->windowCount++;
	pimpl->threadRuning = true;
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
		throw std::system_error();
	pimpl->rendererThread = std::thread(renderLoop, std::ref(pimpl->threadRuning),window);
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

dom::node window::getRoot(){
	dom::node temp(pimpl->dom,0);
	return temp;
}
