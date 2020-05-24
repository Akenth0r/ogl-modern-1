#include "Fog.h"

void Fog::setFar(float farDist)
{
	this->farDist = farDist;
}

void Fog::setNear(float nearDist)
{
	this->nearDist = nearDist;
}

void Fog::setDensity(float density)
{
	this->density = density;
}

void Fog::setColor(const glm::vec3& color)
{
	this->color = color;
}

float Fog::getFar()
{
	return farDist;
}

float Fog::getNear()
{
	return nearDist;
}

float Fog::getDensity()
{
	return density;
}

glm::vec3 Fog::getColor()
{
	return color;
}
