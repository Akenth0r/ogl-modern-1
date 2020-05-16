#include "ResourceManager.h"

int ResourceManager::loadMesh(std::string filePath)
{
	auto iMeshId = mesh_ids.find(filePath);
	if (iMeshId != mesh_ids.end())
	{
		#ifdef _DEBUG
		printf("RESOURCE MANAGER: Using already loaded mesh...\n");
		#endif
		return iMeshId->second;
	}

	Mesh mesh;
	if (mesh.load(filePath))
	{
		meshes.push_back(mesh);
		int id = meshes.size() - 1;
		mesh_ids[filePath] = id;
		return id;
	}

	return -1;
}

int ResourceManager::loadTexture(std::string filepath)
{
	auto iTexId = tex_ids.find(filepath);
	if (iTexId != tex_ids.end())
	{
		#ifdef _DEBUG
		printf("RESOURCE MANAGER: Using already loaded texture...\n");
		#endif
		return iTexId->second;
	}

	Texture tex;
	if (tex.load(filepath))
	{
		textures.push_back(tex);
		int id = textures.size() - 1;
		tex_ids[filepath] = id;
		return id;
	}

	return -1;
}

int ResourceManager::createSharedMaterial(Material& mat)
{
	auto it = std::find(materials.begin(), materials.end(), mat);
	if (it != materials.end())
		return it - materials.begin();

	materials.push_back(mat);
	return materials.size() - 1;
}

Mesh* ResourceManager::getMesh(int index)
{
	if (index > meshes.size() - 1)
		return nullptr;
	return &meshes[index];
}

Texture* ResourceManager::getTexture(int index)
{
	if (index > textures.size() - 1)
		return nullptr;
	return &textures[index];
}

Material* ResourceManager::getMaterial(int index)
{
	if (index > materials.size() - 1)
		return nullptr;
	return &materials[index];
}