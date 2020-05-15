#include "Mesh.h"

using namespace std;
using namespace glm;

Mesh::Mesh()
{
	countIndices = 0;
}

bool Mesh::load(std::string filePath)
{
	// Open the file and check if it's opened
	ifstream sFile;
	sFile.open(filePath);
	if (!sFile.is_open())
	{
		printf("Cannot open the file: %s\n", filePath.c_str());
		return false;
	}

	vector<vec3> coordinates;
	vector<vec2> texCoords;
	vector<vec3> normals;
	vector<vec3> faces;
	char sTemp[512];
	while (!sFile.eof())
	{
		char ch;
		string inpStr;
		sFile >> inpStr;

		if (inpStr == "v")
		{
			vec3 coords;
			sFile >> coords.x;
			sFile >> coords.y;
			sFile >> coords.z;
			coordinates.push_back(coords);
		}

		if (inpStr == "vt")
		{
			vec2 tex_coordinates;
			sFile >> tex_coordinates.s;
			sFile >> tex_coordinates.t;
			texCoords.push_back(tex_coordinates);
		}

		if (inpStr == "vn")
		{
			vec3 normal_coords;
			sFile >> normal_coords.x;
			sFile >> normal_coords.y;
			sFile >> normal_coords.z;

			normals.push_back(normal_coords);
		}

		if (inpStr == "f")
		{
			for (int i = 0; i < 3; i++)
			{
				vec3 face;
				sFile >> face.x;
				sFile >> ch;
				sFile >> face.y;
				sFile >> ch;
				sFile >> face.z;
				faces.push_back(face);
			}
		}

		sFile.getline(sTemp, 512);
	}
	// Close the file
	sFile.close();

	vector<Vertex> vertices;
	vector<GLuint> vIndices;
	map<string, int> m;

	int countVertices = 0;
	// Make a vertices
	for (auto it : faces)
	{
		vec3 vCoord = coordinates[it.x - 1];
		vec2 vTexCoord = texCoords[it.y - 1];
		vec3 vNormal = normals[it.z - 1];
		string str = to_string(it.x) + '/' + to_string(it.y) + '/' + to_string(it.z);

		auto v = m.find(str);
		if (v == m.end())
		{
			Vertex tmp{ {vCoord.x, vCoord.y, vCoord.z}, {vNormal.x, vNormal.y, vNormal.z}, {vTexCoord.s, vTexCoord.t}  };
			vertices.push_back(tmp);
			m.emplace(str, countVertices++);
		}
		vIndices.push_back(m[str]);
	}
	printf("Vertices loaded: %i\n", countVertices);
	countIndices = vIndices.size();

	// Generate buffers
	glGenBuffers(2, buffers);

	// VBO
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, countVertices * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	// VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::coords));	// 0 is vertex coordinate data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));	// 1 is vertex normal data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texCoords)); // 2 is vertex texture coordinate data
	glEnableVertexAttribArray(2);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndices.size() * sizeof(GLuint), vIndices.data(), GL_STATIC_DRAW);

	// Unbind all buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return true;
}

void Mesh::draw()
{
	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	// Bind VAO
	glBindVertexArray(vao);
	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

	// Start drawing
	glDrawElements(GL_TRIANGLES, countIndices, GL_UNSIGNED_INT, 0);

	// Unbind all
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::drawInstanced(int count)
{
	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	// Bind VAO
	glBindVertexArray(vao);
	// Bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

	// Start drawing
	glDrawElementsInstanced(GL_TRIANGLES, countIndices, GL_UNSIGNED_INT, 0, count);

	// Unbind all
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
