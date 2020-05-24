#include "Boundary.h"

int Boundary::countInside(Point point)
{
	glm::vec3 pPos = point.pos;
	glm::vec3 pDim = point.dimension / 2.0f;
	glm::vec3 vertices[] = {
		{pPos.x + pDim.x, pPos.y + pDim.y, pPos.z + pDim.z},
		{pPos.x + pDim.x, pPos.y + pDim.y, pPos.z - pDim.z},
		{pPos.x + pDim.x, pPos.y - pDim.y, pPos.z + pDim.z},
		{pPos.x + pDim.x, pPos.y - pDim.y, pPos.z - pDim.z},
		{pPos.x - pDim.x, pPos.y + pDim.y, pPos.z + pDim.z},
		{pPos.x - pDim.x, pPos.y + pDim.y, pPos.z - pDim.z},
		{pPos.x - pDim.x, pPos.y - pDim.y, pPos.z + pDim.z},
		{pPos.x - pDim.x, pPos.y - pDim.y, pPos.z - pDim.z}
	};
	int count = 0;
	for (int i = 0; i < 8; i++)
	{
		if (
			vertices[i].x >= pos.x - halfSize && vertices[i].x <= pos.x + halfSize &&
			vertices[i].y >= pos.y - halfSize && vertices[i].y <= pos.y + halfSize &&
			vertices[i].z >= pos.z - halfSize && vertices[i].z <= pos.z + halfSize
			)
			count++;
	}

	return count;
}

bool Boundary::inFrustum(glm::mat4 VP)
{
	glm::vec4 boundary_verts[] = {
		{pos.x + halfSize, pos.y + halfSize, pos.z + halfSize, 1.0},
		{pos.x + halfSize, pos.y + halfSize, pos.z - halfSize, 1.0},
		{pos.x + halfSize, pos.y - halfSize, pos.z + halfSize, 1.0},
		{pos.x + halfSize, pos.y - halfSize, pos.z - halfSize, 1.0},
		{pos.x - halfSize, pos.y + halfSize, pos.z + halfSize, 1.0},
		{pos.x - halfSize, pos.y + halfSize, pos.z - halfSize, 1.0},
		{pos.x - halfSize, pos.y - halfSize, pos.z + halfSize, 1.0},
		{pos.x - halfSize, pos.y - halfSize, pos.z - halfSize, 1.0}
	};
	/*for (int i = 0; i < 8; i++)
	{
		printf("source: (%f,%f,%f,%f)\n", boundary_verts[i].x, boundary_verts[i].y, boundary_verts[i].z, boundary_verts[i].w);
	}*/
	int left, up, down, right, front, back;
	left = up = down = right = front = back = 0;
	for (int i = 0; i < 8; i++)
	{
		glm::vec4 clipCoord = VP * boundary_verts[i];
		if (clipCoord.x < -clipCoord.w)
		{
			left++;
			continue;
		}
		if (clipCoord.x > clipCoord.w)
		{
			right++;
			continue;
		}
		if (clipCoord.y < -clipCoord.w)
		{
			down++;
			continue;
		}
		if (clipCoord.y > clipCoord.w)
		{
			up++;
			continue;
		}
		if (clipCoord.z < -clipCoord.w)
		{
			front++;
			continue;
		}
		if (clipCoord.z > clipCoord.w)
		{
			back++;
			continue;
		}
	}
	if (left == 8 || up == 8 || down == 8 || right == 8 || front == 8 || back == 8)
		return false;

	return true;
}
