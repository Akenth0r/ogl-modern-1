#include "Texture.h"

using namespace std;

int Texture::FilteringMode;

bool Texture::load(std::string filepath)
{
	ILuint img = ilGenImage();
	ilBindImage(img);
	std::wstring fp(filepath.begin(), filepath.end());
	if (!ilLoadImage(fp.c_str()))
	{
		wprintf(L"IL: cannot  load texture image %s\n", fp.c_str());
		ilDeleteImage(img);
		return false;
	}

	// Initialize width and height values
	int width = ilGetInteger(IL_IMAGE_WIDTH);
	int height = ilGetInteger(IL_IMAGE_HEIGHT);
	wprintf(L"IL: loading texture.. path \"%s\" width: %i height %i\n", fp.c_str(), width, height);

	// Copy to temporary buffer
	ILubyte* buf = new ILubyte[width * height * 4];
	ilCopyPixels(0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, buf);

	// Copy from  temp buf to ogl texture object
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &index);
	glBindTexture(GL_TEXTURE_2D, index);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);
	glBindTexture(GL_TEXTURE_2D, 0);

	ilBindImage(0);

	delete[] buf;
	ilDeleteImage(img);

	return true;
}

void Texture::bind(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, index);
	applyTextureMode();
}

void Texture::disableAll()
{
	int textureUnits = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &textureUnits);
	for (int i = 0; i < textureUnits; i++)
		glDisable(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void Texture::applyTextureMode()
{
	const int params[3] = { GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_MAX_TEXTURE_MAX_ANISOTROPY };
	const int modes[9][3] =
	{
		{GL_NEAREST, GL_NEAREST, 1},
		{GL_LINEAR, GL_LINEAR, 1},
		{GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, 1},
		{GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, 1},
		{GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, 2},
		{GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, 4},
		{GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, 8},
		{GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, 16},
		{GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR, 32},
	};

	for (int i = 0; i < 3; i++)
		glTexParameteri(GL_TEXTURE_2D, params[i], modes[Texture::FilteringMode][i]);
}