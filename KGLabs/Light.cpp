#include "Light.h"

void Light::setDirection(const glm::vec4& direction)
{
	this->direction = direction;
}

void Light::setAmbient(const glm::vec4& ambient)
{
	this->ambient = ambient;
}

void Light::setDiffuse(const glm::vec4& diffuse)
{
	this->diffuse = diffuse;
}

void Light::setSpecular(const glm::vec4& specular)
{
	this->specular = specular;
}

const glm::vec4& Light::getDirection() const
{
	return direction;
}

const glm::vec4& Light::getAmbient() const
{
	return ambient;
}

const glm::vec4& Light::getDiffuse() const
{
	return diffuse;
}

const glm::vec4& Light::getSpecular() const
{
	return specular;
}