#pragma once
#include "GLM/glm.hpp"

// Point
class Point
{
public:
	// Constructors
	Point() = default;
	Point(glm::vec3 pos, glm::vec3 dimension, int index) : pos(pos), dimension(dimension), index(index) {}

public:
	glm::vec3 pos;
	glm::vec3 dimension;
	int index;  // Graphic objects array index
};