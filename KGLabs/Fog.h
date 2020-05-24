#pragma once
#include "GLM/glm.hpp"
class Fog
{
public:
	Fog() : farDist(200.0), nearDist(10.0), density(0.2), color(glm::vec3(1.0)) {}
	Fog(float farDist, float nearDist, float density, glm::vec3 color) : farDist(farDist), nearDist(nearDist), density(density), color(color) {}

	// Setters
	void setFar(float farDist);
	void setNear(float nearDist);
	void setDensity(float density);
	void setColor(const glm::vec3& color);

	// Getetrs
	float getFar();
	float getNear();
	float getDensity();
	glm::vec3 getColor();
private:
	float farDist;
	float nearDist;
	float density;
	glm::vec3 color;
};