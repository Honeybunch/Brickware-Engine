#ifndef TEXTURE_H
#define TEXTURE_H

#include "Settings.h"

#include "Utils.h"

class Texture
{
public:
	Texture(char* textureFileName);

	float* getPixels();

	~Texture();

private:
	void loadBMP(char* textureFileName);

	int width;
	int height;

	short colorDepth; //Bits per pixel
	int compressionType;

	float* pixels;
};

#endif