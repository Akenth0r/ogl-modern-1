#pragma once
#include "GLM/glm.hpp"
#include "Material.h"


enum class GraphicObjectType {
	road, building, vehicle,
	big_nature, small_nature, big_prop, medium_prop, small_prop
};

class GraphicObject
{
public:
	// Default constructor
	GraphicObject() : position(glm::vec3(0.0f)), angle(0.0), matId(-1) {}

	// Set mesh for using by it's id
	void setMesh(int meshId);
	// Set object color
	void setColor(const glm::vec4& color);
	// Set object position
	void setPosition(const glm::vec3& position);
	// Angle relative to OY (CW)
	void setAngle(float degree);
	// Set material
	void setMaterial(int matId);
	// Set type
	void setType(GraphicObjectType type) { this->type = type; }
	// Set dimension
	void setDimension(const glm::vec3& dimensions) { this->dimensions = dimensions; }

	// Getters
	int getMeshId();
	glm::vec4& getColor();
	glm::mat4& getModelMatrix();
	int getMaterialId();
	GraphicObjectType getType() { return type; }
	glm::vec3 getDimension() { return dimensions; }
	glm::vec3 getPosition() { return position; }

private:
	int meshId;
	glm::vec4 color;
	glm::mat4 modelMatrix;
	glm::vec3 position;
	glm::vec3 dimensions;
	float angle;
	int matId;
	//Material material;
	GraphicObjectType type;

	void updateModelMatrix();
};