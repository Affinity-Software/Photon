#include "photon/window.hpp"
#include "dom.hpp"
#include "renderer/renderer.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

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

static void renderLoop(std::atomic<bool>& runing, GLFWwindow* window);

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

static void onResize(GLFWwindow* window, int width, int height);

static void renderLoop(std::atomic<bool>& runing, GLFWwindow* window)
{
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window,onResize);
	glfwSwapInterval(1);
	if(glewInit() != GLEW_OK)
		throw std::system_error();

	photon::renderer::program prog("src/renderer/shaders/main.glslMake");
	glfwSetWindowUserPointer(window,&prog);

	_dom temp;
	auto mesh = photon::renderer::domToMesh(temp);

	glm::mat4 proj = glm::ortho(-1.0f,1.0f,-(320.0f/480.0f),320.0f/480.0f,-1.0f,1.0f);
	prog.setUniform("mpv",proj);

	GLC(glClearColor(1.0,1.0,1.0,1.0));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window) && runing)
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Swap front and back buffers */
		photon::renderer::draw(prog,mesh);
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
}

dom::node window::getRoot(){
	dom::node temp(pimpl->dom,0);
	return temp;
}

static void onResize(GLFWwindow* window, int width, int height)
{
	GLC(glViewport(0, 0, width, height));
	(void)window;

	float ratio = static_cast<float>(height)/static_cast<float>(width);
	glm::mat4 proj = glm::ortho(-1.0f,1.0f,-ratio,ratio,-1.0f,1.0f);

	renderer::program* prog = reinterpret_cast<renderer::program*>(glfwGetWindowUserPointer(window));
	prog->bind();
	prog->setUniform("mpv",proj);

}