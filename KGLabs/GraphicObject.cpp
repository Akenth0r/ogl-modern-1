#include "GraphicObject.h"
#include <GLM\gtx\transform.hpp>

using namespace glm;

void GraphicObject::setMesh(int meshId)
{
	this->meshId = meshId;
}

void GraphicObject::setColor(const glm::vec4& color)
{
	this->color = color;
}

void GraphicObject::setPosition(const glm::vec3& position)
{
	this->position = position;
	updateModelMatrix();
}

void GraphicObject::setAngle(float degree)
{
	angle = degree;
	updateModelMatrix();
}

void GraphicObject::setMaterial(int matId)
{
	this->matId = matId;
}

int GraphicObject::getMeshId()
{
	return meshId;
}

glm::vec4& GraphicObject::getColor()
{
	return color;
}

glm::mat4& GraphicObject::getModelMatrix()
{
	return modelMatrix;
}

int GraphicObject::getMaterialId()
{
	return matId;
}

void GraphicObject::updateModelMatrix()
{
	modelMatrix = translate(position) * rotate(radians(angle), vec3(0.0f, 1.0f, 0.0f));
}