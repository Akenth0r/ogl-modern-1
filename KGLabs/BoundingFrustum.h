#pragma once
#include "Boundary.h"
#include "GLM/glm.hpp"
class BoundingFrustum
{
public:
	BoundingFrustum(glm::vec3 pos, float zNear, float zFar, float fov, glm::vec3 forward) : pos(pos), zNear(zNear), zFar(zFar), forward(forward) {}
	/*bool intersects(Boundary boundary);*/

public:
	glm::vec3 pos;
	glm::vec3 forward;
	float zNear;
	float zFar;
	float fov;

};

