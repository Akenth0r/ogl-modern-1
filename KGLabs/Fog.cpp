#include "Fog.h"

void Fog::setHeight(float height)
{
	this->height = height;
}

void Fog::setDensity(float density)
{
	this->density = density;
}

void Fog::setColor(const glm::vec4& color)
{
	this->color = color;
}

float Fog::getHeight()
{
	return height;
}

float Fog::getDensity()
{
	return density;
}

glm::vec4 Fog::getColor()
{
	return color;
}
