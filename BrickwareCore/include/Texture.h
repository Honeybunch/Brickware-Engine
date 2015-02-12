#ifndef TEXTURE_H
#define TEXTURE_H

#include "BrickwareCoreDLL.h"

#include "Settings.h"

#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#endif

#include "Utils.h"

enum BRICKWARE_CORE_API TextureType{
	RGB,
	RGBA
};

class BRICKWARE_CORE_API Texture
{
	friend class Material;

public:
	Texture(char* textureFileName);

	char* getPixels();

	~Texture();

private:
	void loadBMP(char* textureFileName);

	void bufferTexture();
	void bindTexture();
	void freeTexture();

	TextureType textureType;

#ifdef GL_SUPPORT
	GLuint glTexture;
	GLenum glTextureType;

	void bufferGL();
	void bindGL();
	void freeGL();
#endif

#ifdef D3D_SUPPORT
	void bufferD3D();
	void bindD3D();
	void freeD3D();
#endif

	int width;
	int height;

	short colorDepth; //Bits per pixel
	int compressionType;

	char* pixels;
};

#endif