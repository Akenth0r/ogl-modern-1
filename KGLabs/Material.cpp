#include "Material.h"

void Material::setAmbient(const glm::vec4& ambient)
{
	this->ambient = ambient;
}

void Material::setDiffuse(const glm::vec4& diffuse)
{
	this->diffuse = diffuse;
}

void Material::setSpecular(const glm::vec4& specular)
{
	this->specular = specular;
}

void Material::setShininess(float shininess)
{
	this->shininess = shininess;
}

void Material::setTexture(int texId)
{
	this->texId = texId;
}

const glm::vec4& Material::getAmbient()
{
	return ambient;
}

const glm::vec4& Material::getDiffuse()
{
	return diffuse;
}

const glm::vec4& Material::getSpecular()
{
	return specular;
}

int Material::getTexture()
{
	return texId;
}

bool Material::operator==(const Material& mat)
{
	if (
		this->ambient == mat.ambient &&
		this->diffuse == mat.diffuse &&
		this->shininess == mat.shininess &&
		this->specular == mat.specular &&
		this->texId == mat.texId
		)
		return true;

	return false;
}

bool Material::operator<(const Material& mat)
{
	// ambient
	if (this->ambient.r < mat.ambient.r &&
		this->ambient.g < mat.ambient.g &&
		this->ambient.b < mat.ambient.b
		)
		return true;

	// diffuse
	if (this->diffuse.r < mat.diffuse.r &&
		this->diffuse.g < mat.diffuse.g &&
		this->diffuse.b < mat.diffuse.b
		)
		return true;

	// shininess
	if (this->shininess < mat.shininess)
		return true;

	// specular
	if (this->specular.r < mat.specular.r &&
		this->specular.g < mat.specular.g &&
		this->specular.b < mat.specular.b
		)
		return true;

	// texId
	if (this->texId < mat.texId)
		return true;

	return false;
}

float Material::getShininess()
{
	return shininess;
}