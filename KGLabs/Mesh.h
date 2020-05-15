#pragma once
#include "GL/glew.h"
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <GLM/glm.hpp>
#include <vector>

// Describes a single vertice
struct Vertex
{
	float coords[3];
	float normal[3];
	float texCoords[2];
};

class Mesh
{
public:
	// Constructors
	Mesh();

	// Load mesh from file
	bool load(std::string filePath);
	// Draw mesh
	void draw();
	// Draw mesh with instancing
	void drawInstanced(int count);

private:
	// VAO
	GLuint vao;
	// VBO, EBO
	GLuint buffers[2];
	// Count of vertices
	int countIndices;
};