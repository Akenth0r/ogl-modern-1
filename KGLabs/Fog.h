#pragma once
#include "GLM/glm.hpp"
class Fog
{
public:
	Fog() : height(2.0), density(0.2), color(glm::vec4(1.0)) {}

	// Setters
	void setHeight(float height);
	void setDensity(float density);
	void setColor(const glm::vec4& color);

	// Getetrs
	float getHeight();
	float getDensity();
	glm::vec4 getColor();
private:
	float height;
	float density;
	glm::vec4 color;
};

