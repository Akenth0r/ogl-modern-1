#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "ShaderManager.h"
#include <memory>
class Shader
{
public:
	Shader();

	// Load the shader from file
	bool load(std::string fileName, GLenum shaderType);

	// Choose the shader as a current
	void activate();

	// Disable the shader
	static void deactivate();

	// Set a uniform variable
	void setUniform(const std::string& name, const glm::vec4& value);
	void setUniform(const std::string& name, const glm::mat4& value);
	void setUniform(const std::string& name, const glm::mat4x2& value);
	void setUniform(const std::string& name, const glm::vec2& value);
	void setUniform(const std::string& name, std::vector<glm::mat4>& value);
	void setUniform(const std::string& name, float value);


	// Get shader paths
	std::map<GLuint, std::string>* getPaths();
	//  Get shader last times
	std::map<GLuint, INT64>* getLastTimes();
	// Get shader program
	GLuint getProgram();
private:
	// Shader program
	GLuint program;
	// Uniform index map
	std::map<std::string, GLuint> uniforms;
	// Shader paths
	std::map <GLuint, std::string> shader_paths;
	// Field for shader manager to store last times
	std::map <GLuint, INT64> last_times;
	// Shaders attached
	std::map <GLuint, GLuint> attachedShaders;

private:
	// Get uniform location
	GLuint getUniformLocation(const std::string& name);
};