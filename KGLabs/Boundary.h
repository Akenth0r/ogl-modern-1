#pragma once
#include "Point.h"
#include "GLM/glm.hpp"
// QuadTree node boundary
class Boundary
{
public:
	// Constructors
	Boundary() = default;
	Boundary(glm::vec3 pos, float halfSize) : pos(pos),  halfSize(halfSize){}

	// Methods
	// Does boundary contain this point
	int countInside(Point point);
	// Is in frustum
	bool inFrustum(glm::mat4 VP);

public:
	glm::vec3 pos;
	float halfSize;
};