#pragma once
#include <GL/glew.h>
#include <map>
#include <string>
#include <thread>
#include <Windows.h>
#include <iostream>
#include <vector>
#include "Shader.h"
class Shader;	// Find in Shader.h

struct Reloadable
{
	Shader* program;
	std::string filepath;
	GLuint shaderType;
};

// Checks 
class ShaderManager
{
public:

	void AddShader(Shader* shader);
	void DeleteShader(Shader* shader);
	void Start();
	void Reload();
	static ShaderManager& getInstance();
private:
	// Map of shader programs

	std::map<Shader*, std::map<GLuint, std::string>*> programs;
	std::map<Shader*, std::map<GLuint, INT64>*> last_times;
	std::vector<Reloadable> programsToReload;
	std::thread* monThread;
private:
	ShaderManager();
	ShaderManager(const ShaderManager& root) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;

	void Monitor();
	INT64 GetShaderLastTime(std::string shader);
	void CheckLastTimes(Shader* shader);
};

