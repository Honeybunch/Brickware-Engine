#include "Texture.h"

Texture::Texture(char* textureFileName)
{
	//TODO: check file name once we support multiple file namess
	loadBMP(textureFileName);
}

float* Texture::getPixels(){ return pixels; }

void Texture::loadBMP(char* textureFileName)
{
	char* bmpBytes = Utils::textFileRead(textureFileName);

	//Check to see if this is a valid image
	//We check the first byte of the image; if it's not a B, C, I or P than this is not a valid BMP
	char headerField = *bmpBytes;

	if (headerField == 'B' || headerField == 'C' || headerField == 'I' || headerField == 'P')
	{
		//Parse the BMP header for the important data we need
		int pixelArrayLocation = *(int*)(bmpBytes + 10);

		width = *(int*)(bmpBytes + 18);
		height = *(int*)(bmpBytes + 22);

		colorDepth = *(short*)(bmpBytes + 28);
		compressionType = *(int*)(bmpBytes + 30);

		int rowSize = ((colorDepth * width + 31) / 32) * 4;
		int pixelArraySize = rowSize * height; //Size of the BMP pixel array in bytes

		//The size of the pixel array is going to be width * height (for the resolution)
		//multiplied by the color depth (which is in bits) / 8 (to make it in bytes)
		//so one size may be 512 * 512 * (24 / 8) = 512 * 512 * 3; because RGB
		//Another size may be 512 * 512 * (32 / 8) = 512 * 512 * 4; becasue RGBA
		pixels = new float[width * height * (colorDepth/8)];
	}

	delete bmpBytes;
}

Texture::~Texture()
{
	delete pixels;
}
