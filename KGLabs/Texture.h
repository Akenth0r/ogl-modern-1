#pragma once
#include "GL/glew.h"
#include <string>
#include "IL/il.h"
#include "IL/ilu.h"
#include "IL/ilut.h"

enum TexFilters { NEAREST, BILINEAR, BILINEAR_MIPMAP, THREELINEAR, ANISO2, ANISO4, ANISO8, ANISO16, ANISO32, TEXFILTERS_COUNT };

class Texture
{
public:
	Texture() : index(-1) {}
	// Load the texture
	bool load(std::string filepath);
	// Bind the texture ( to finish )
	void bind(GLenum texUnit);

	// Disable all units
	static void disableAll();

	static int FilteringMode;
private:
	// Texture index
	GLuint index;

	// Set texture filtering mode
	void applyTextureMode();
};