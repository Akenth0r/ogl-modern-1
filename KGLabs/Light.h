#pragma once
#include "GLM/glm.hpp"
class Light
{
public:
	// Default sonctructor;
	Light() : direction(glm::vec4(.3, 1., .5, .0)), ambient(glm::vec4(1., 1., 1., 1.)),
			diffuse(glm::vec4(.6, .6, .6, 1.)), specular(glm::vec4(.4, .4, .4, 1.)){}

	// Setters
	void setDirection(const glm::vec4& direction);
	void setAmbient(const glm::vec4& ambient);
	void setDiffuse(const glm::vec4& diffuse);
	void setSpecular(const glm::vec4& specular);

	// Getters
	const glm::vec4& getDirection() const;
	const glm::vec4& getAmbient() const;
	const glm::vec4& getDiffuse() const;
	const glm::vec4& getSpecular() const;

private:
	// Light source direction
	glm::vec4 direction;
	// Ambient
	glm::vec4 ambient;
	// Diffuse
	glm::vec4 diffuse;
	// Specular
	glm::vec4 specular;
};