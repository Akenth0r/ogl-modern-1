#pragma once
#include <Windows.h>
#include <string>
#include "Camera.h"
#include "GraphicObject.h"
#include "Light.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stream.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "RenderManager.h"
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include "GLM/glm.hpp"
#include "Quadtree.h"

class Scene
{
public:
	Scene()=default;

	// Init scene (here we're loading models description)
	void init(std::string filepath);
	// Load scene from json (here we're loading scene.json)
	bool loadJSON(std::string filepath);
	// Init qtree
	void initQtree(Boundary boundary);

	// Scene simulation (if necessary)
	void simulate(float sec);
	// Draw entire scene (using rendermanager)
	void draw();

	// Set camera
	void setCamera(Camera* camera) { this->camera = camera; }
	// Set light
	void setLight(Light* light) { this->light = light; }

	// Get camera
	Camera* getCamera();

	// Scene info
	std::string getSceneDescription();
	int getObjectCount() { return graphicObjects.size(); }

private:
	// Create object by it's identifier
	GraphicObject createGraphicObject(std::string model_id);
	bool loadModels(std::string models_json);
	// Check if object is needed to draw
	// LOD algorithm is here
	bool isFar(GraphicObject& gObj);
	bool isGraphicObjectInFrustum(GraphicObject& gObj, glm::mat4& MVP);


private:
	// Camera
	Camera* camera;
	// Light source
	Light* light;
	// Scene graphic objects
	std::vector<GraphicObject> graphicObjects;
	// JSON
	rapidjson::Document modelsDescription;
	// Filename
	std::string sceneFilename;
	// Quadtree
	Quadtree qtree;
};