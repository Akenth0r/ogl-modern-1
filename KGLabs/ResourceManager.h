#pragma once
#include <string>
#include "Mesh.h"
#include <vector>
#include <map>
#include "Texture.h"
#include "Material.h"
#include <algorithm>

class ResourceManager
{
public:
	// Get instance
	static ResourceManager& Instance()
	{
		static ResourceManager resourceManager;
		return resourceManager;
	}

	// Public methods
	// Load one mesh
	int loadMesh(std::string filePath);
	// Load one texture
	int loadTexture(std::string filepath);
	// Create material
	int createSharedMaterial(Material& mat);

	// Get a mesh by it's index
	// returns 0 if there is no mesh with that index
	Mesh* getMesh(int index);
	// Get a texture by it's index
	Texture* getTexture(int index);
	// Get material index
	Material* getMaterial(int index);

private:
	// Constructors
	ResourceManager() = default;
	// Delete copy constructor and assignment operation as we don't want
	// to copy our resourceManager
	ResourceManager(const ResourceManager& rm) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;

	// Private variables
	// Vectors
	// Vector for mesh storing
	std::vector<Mesh> meshes;
	// Vector for texture storing
	std::vector<Texture> textures;
	// Materials
	std::vector<Material> materials;

	// Maps
	// Mesh id's for preventing multiple loadings of a one mesh
	std::map<std::string, unsigned int> mesh_ids;
	std::map<std::string, unsigned int> tex_ids;

};