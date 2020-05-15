#pragma once
#include "GLM/glm.hpp"

class Material
{
public:
	// Default constructor
	Material() : texId(-1), diffuse(glm::vec4(.85, .85, .85, 1.)), ambient(glm::vec4(.35, .35, .35, 1.)),
		specular(glm::vec4(.25, .25, .25, 1.)), shininess(64.0f) {}
	
	// Setters
	void setAmbient(const glm::vec4& ambient);
	void setDiffuse(const glm::vec4& diffuse);
	void setSpecular(const glm::vec4& specular);
	void setShininess(float shininess);
	void setTexture(int texId);

	// Getters
	const glm::vec4& getAmbient();
	const glm::vec4& getDiffuse();
	const glm::vec4& getSpecular();
	float getShininess();
	int getTexture();

	// Operators
	bool operator==(const Material& mat);
	bool operator<(const Material& mat);

private:
	// Resource manager texture id
	int texId;
	// Ambient
	glm::vec4 ambient;
	// Diffuse
	glm::vec4 diffuse;
	// Specular
	glm::vec4 specular;
	// Shininess
	float shininess;
};