#pragma once
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include "Light.h"
#include "GraphicObject.h"
#include <vector>
#include "Shader.h"
#include "GLM/glm.hpp"
#include "ResourceManager.h"
#include "Fog.h"
#include "Scene.h"
#include <algorithm>

class RenderManager
{
public:
	static RenderManager& Instance()
	{
		static RenderManager renderManager;
		return renderManager;
	}

	// Init RenderManager (after opengl context setting up only)
	void init();
	// Prepare to render
	void prepare();
	// Set camera
	void setCamera(Camera* camera);
	// Set light source
	void setLight(Light* light);
	// Set fog
	void setFog(Fog* fog);
	// Add graphic object to the render queue
	void addToRenderQueue(GraphicObject* graphicObject);
	// Render
	void render();
	void clearFPSCount() { fps = 0; }

	// Get drawing objects count
	int getObjectCount() { return objCount; }
	// Get fps
	int getFPSCount() { return fps; }
	// get drawcall count
	int getDrawcallCount() { return drawcallCount; }
	// Material change count
	int getMaterialChangeCount() {return materialChangeCount;}

private:
	// Default constructor
	RenderManager() = default;
	// Copy constructor
	RenderManager(const RenderManager& root) = delete;
	// Assignment operator
	RenderManager& operator=(const RenderManager&) = delete;

	// Shaders
	std::vector<Shader> shaders;
	// Camera
	Camera* camera;
	// Light source
	Light* light;
	// Fog if exists
	Fog* fog;
	// Graphic objects to render
	std::vector<GraphicObject*> graphicObjects;
	// Object count
	int objCount;
	int drawcallCount;
	int materialChangeCount;
	int fps;
};