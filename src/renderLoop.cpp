//thi file only export a single function to window.cpp
#include <atomic>

#include "glHelper.hpp"
#include "renderer/renderer.hpp"
#include "dom.hpp"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

using namespace photon;

enum modeKeys:uint32_t{
	RIGHT_CTRL =  0b000001,
	LEFT_CTRL =   0b000010,
	CTRL =        0b000011,
	RIGHT_SHIFT = 0b000100,
	LEFT_SHIFT =  0b001000,
	SHIFT =       0b001100,
	RIGHT_ALT =   0b010000,
	LEFT_ALT =    0b100000,
	ALT =         0b110000

};

//raw pointers here are ok since everything that it contains only exist in the renderLoop 
struct userPointerData
{
	renderer::program* prog;
	uint32_t mods;
	float scale = 1;
	float aspectRatio = 640/480;
	glm::vec2 ofset = {0,0};
};

static void setProjection(userPointerData* upd)
{
	float scale = upd->scale;
	float ratio = upd->aspectRatio;
	glm::vec2 ofset = upd->ofset;
	glm::mat4 proj = glm::ortho(
		-scale,
		scale,
		ratio*scale,
		-scale*ratio,
		-1.0f,1.0f
	);
	glm::mat4 view = glm::translate(glm::mat4(1.f),glm::vec3(ofset,0.f));
	upd->prog->setUniform("mpv",proj* view);
}

static void onResize(GLFWwindow* window, int width, int height)
{
	GLC(glViewport(0, 0, width, height));

	userPointerData* upd = reinterpret_cast<userPointerData*>(glfwGetWindowUserPointer(window));
	upd->aspectRatio = static_cast<float>(height)/static_cast<float>(width);
	setProjection(upd);
}

static uint32_t changeModeKeys(int action,uint32_t key, uint32_t toChange)
{
	if(action == GLFW_PRESS)
		return toChange | key;
	else
		return toChange & ~key;
}

static void onKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	(void)scancode;
	(void)mods;
	userPointerData* upd = reinterpret_cast<userPointerData*>(glfwGetWindowUserPointer(window));
	if(action == GLFW_PRESS || action == GLFW_RELEASE)
		switch (key)
		{
		case GLFW_KEY_LEFT_SHIFT:
			upd->mods = changeModeKeys(action,LEFT_SHIFT,upd->mods);
			break;
		case GLFW_KEY_RIGHT_SHIFT:
			upd->mods = changeModeKeys(action,RIGHT_SHIFT,upd->mods);
			break;
		case GLFW_KEY_LEFT_ALT:
			upd->mods = changeModeKeys(action,LEFT_ALT,upd->mods);
			break;
		case GLFW_KEY_RIGHT_ALT:
			upd->mods = changeModeKeys(action,RIGHT_ALT,upd->mods);
			break;
		case GLFW_KEY_LEFT_CONTROL:
			upd->mods = changeModeKeys(action,LEFT_CTRL,upd->mods);
			break;
		case GLFW_KEY_RIGHT_CONTROL:
			upd->mods = changeModeKeys(action,RIGHT_CTRL,upd->mods);
			break;

		default:
			break;
		}
}

static void onScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	userPointerData* upd = reinterpret_cast<userPointerData*>(glfwGetWindowUserPointer(window));
	if(upd->mods & modeKeys::CTRL)
	{
		if(yoffset > 0)
			upd->scale *= 1.2f;
		else
			upd->scale /= 1.2f;
	}
	else if(upd->mods & modeKeys::SHIFT)
	{
		upd->ofset.x -= (yoffset*upd->scale)/10;
		upd->ofset.y -= (xoffset*upd->scale)/10;
	}
	else if(upd->mods == 0)
	{
		upd->ofset.x -= (xoffset*upd->scale)/10;
		upd->ofset.y -= (yoffset*upd->scale)/10;
	}
	setProjection(upd);
}

void renderLoop(std::atomic<bool>& runing, GLFWwindow* window)
{

	glfwMakeContextCurrent(window);

	//input callbacks
	glfwSetFramebufferSizeCallback(window,onResize);
	glfwSetKeyCallback(window,onKey);
	glfwSetScrollCallback(window,onScroll);

	glfwSwapInterval(1);
	if(glewInit() != GLEW_OK)
		throw std::system_error();

	userPointerData upd;
	glfwSetWindowUserPointer(window,&upd);

	photon::renderer::program prog("src/renderer/shaders/main.glslMake");
	upd.prog = &prog;
	_dom temp;
	auto mesh = photon::renderer::domToMesh(temp);

	glm::mat4 proj = glm::ortho(-1.0f,1.0f,-(320.0f/480.0f),320.0f/480.0f,-1.0f,1.0f);
	prog.setUniform("mpv",proj);

	GLC(glClearColor(1.0,1.0,1.0,1.0));
	GLC(glEnable(GL_BLEND));
	GLC(glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA));
	GLC(glPointSize(10.f));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window) && runing)
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		photon::renderer::draw(prog,mesh);
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
}