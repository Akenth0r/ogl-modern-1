// KGLabs.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <Windows.h>
#include "Shader.h"
#include <algorithm>
#include "Camera.h"
#include "Mesh.h"
#include "GraphicObject.h"
#include "ResourceManager.h"
#include <vector>
#include "RenderManager.h"
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"
#include "Texture.h"

#pragma comment(linker, "/STACK:36777216")

using namespace std;
using namespace glm;

GLFWwindow* window;
char sFPS[256];
double timeSummary = 0;
double speed = 0.4;
POINT oldCursPos;
POINT newCursPos;
const float fovy = 35;
const int w = 800;
const int h = 600;

static float oldTime;
static float deltaTime;

// Shader manager
ShaderManager& shaderManager = ShaderManager::getInstance();
// Resource manager
ResourceManager& resourceManager = ResourceManager::Instance();
// Render manager
RenderManager& renderManager = RenderManager::Instance();
// Light
Light light;
// Fog
Fog fog;
// Scene
Scene scene;

// Graphic objects
vector<GraphicObject> graphicObjects;

// Camera
Camera camera;

// Uniforms
vec4 color1(0.0, 0, 0.0, 1.0);
vec4 offset(0);

// Filtering mode

// functions
void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void key_handle();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void FPSCounter();
void simulation();
void display();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	// Initializing glfw
	if (!glfwInit())
		return -1;
	glfwSetErrorCallback(error_callback);

	// Creating the window
	glfwWindowHint(GLFW_SAMPLES, 1);
	window = glfwCreateWindow(w, h, "Lab1", nullptr, nullptr);
	if (!window)
		return -1;
	glfwMakeContextCurrent(window);
	
	// Set window callbacks
	glfwSetKeyCallback(window, key_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Initializing glew
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "glewInit failed: " << glewGetErrorString(err) << endl;
		return -1;
	}

	// IL
	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	iluInit();
	ilutInit();

	// Messing with camera
	camera.setPosition(vec3(5.0, 10.0, 5.0));
	camera.setProjectionMatrix(fovy, w/h, 1, 500.0);

	//glClearColor(0.7, .7, .7, 1); // Pretty clear color
	
	// Fog
	fog.setColor(glm::vec4(1.0));
	fog.setDensity(0.2);
	fog.setFar(400.0);
	fog.setNear(10.0);

	// Scene initialization
	scene.init("DATA/models.json");
	scene.loadJSON("DATA/demo_scene2.json");
	scene.setCamera(&camera);
	scene.setLight(&light);
	scene.initQtree(Boundary(glm::vec3(0, 0, 0), 600));

	// Render manager initialization
	//renderManager.setCamera(&camera);
	//renderManager.setLight(&light);
	renderManager.setFog(&fog);
	renderManager.init();
	
	// Start shader manager
	shaderManager.Start();
	
	//initGraphicObjects();




	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		oldTime = glfwGetTime();
		display();

		glfwPollEvents();
		simulation();
	}

	Texture::FilteringMode = TexFilters::ANISO16;

	glfwTerminate();
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void display()
{
	shaderManager.Reload();

	renderManager.prepare();

	scene.draw();
	
	/*for (int i = 0; i < graphicObjects.size(); i++)
	{
		renderManager.addToRenderQueue(&graphicObjects[i]);
	}*/

	renderManager.render();

	glfwSwapBuffers(window);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// useless function
	// doesn't work properly
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (!(width || height))	// If height or width is 0
		return;
	glViewport(0, 0, width, height);
	camera.setProjectionMatrix(fovy, width / height, 1.0f, 500.0f);
	display();
}

void simulation()
{
	deltaTime = glfwGetTime() - oldTime;
	FPSCounter();
	key_handle();
}

void FPSCounter()
{
	//fps++;
	timeSummary += deltaTime;
	RenderManager& rm = RenderManager::Instance();

	if (timeSummary >= 0.5)
	{
		sprintf_s(sFPS, "Lab5: %s | FPS: %i  [Object count: %i/%i] [MatChanges: %i] [Drawcalls: %i]", scene.getSceneDescription().c_str(), (int)((float)rm.getFPSCount()/timeSummary), rm.getObjectCount(), scene.getObjectCount(), rm.getMaterialChangeCount(), rm.getDrawcallCount());
		glfwSetWindowTitle(window, sFPS);
		timeSummary = 0;
		rm.clearFPSCount();
		//fps = 0;
	}
}

void key_handle()
{
	static float mouse_sensitivity = 60.0;
	bool bWindowFocused = glfwGetWindowAttrib(window, GLFW_FOCUSED);

	if (GetAsyncKeyState(VK_LBUTTON) && bWindowFocused)
	{
		GetCursorPos(&newCursPos);
		int deltax = newCursPos.x - oldCursPos.x;
		int deltay = newCursPos.y - oldCursPos.y;

		if (deltax || deltay)
		{
			camera.rotate(deltay * mouse_sensitivity * deltaTime, deltax * mouse_sensitivity * deltaTime);
		}
		oldCursPos = newCursPos;
	}
	else
	{
		oldCursPos = newCursPos;
		GetCursorPos(&newCursPos);
	}

	static float speed = 20.0;
	if (GetAsyncKeyState(0x57) && bWindowFocused) // w
	{
		camera.move(speed * deltaTime, 0);
	}

	if (GetAsyncKeyState(0x53) && bWindowFocused) // s
	{
		camera.move(-speed * deltaTime, 0);
	}

	if (GetAsyncKeyState(0x41) && bWindowFocused) // a
	{
		camera.move(0, speed * deltaTime);
	}

	if (GetAsyncKeyState(0x44) && bWindowFocused) // d
	{
		camera.move(0, -speed * deltaTime);
	}

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
		camera.zoomInOut(-0.5);
	else
		camera.zoomInOut(0.5);
}