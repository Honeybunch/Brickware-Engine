#include "Texture.h"

Texture::Texture(char* textureFileName)
{
	//TODO: check file name once we support multiple file namess
	loadBMP(textureFileName);

	//Lets buffer the texture onto the GPU
#ifdef D3D_SUPPORT
	bufferD3D();
#else
	bufferGL();
#endif
}

void Texture::bindTexture()
{
#ifdef D3D_SUPPORT
	bindD3D();
#else
	bindGL();
#endif
}

void Texture::freeTexture()
{
#ifdef D3D_SUPPORT
	freeD3D();
#else
	freeGL();
#endif
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
		//Also * 4 for each RGBA component
		//Not pixelArraySize because that's in bytes and floats are 4 bytes each
		pixels = new float[width * height * 4];
	
		//Load the colors into the pixels array based on color depth
		if (colorDepth == 24)
		{
			for (int i = 0; i < pixelArraySize; i+= 3)
			{
				char r, g, b;

				g = *(char*)bmpBytes + pixelArrayLocation + i ;
				r = *(char*)bmpBytes + pixelArrayLocation + i + 1;
				b = *(char*)bmpBytes + pixelArrayLocation + i + 2;

				int pixelIndex = i / 3;

				float red = r / 256.0f;
				float green = g / 256.0f;
				float blue = b / 256.0f;

				pixels[pixelIndex] = red;
				pixels[pixelIndex + 1] = green;
				pixels[pixelIndex + 2] = blue;
				pixels[pixelIndex + 3] = 0; //In 24bpp there is no alpha
			}
		}
		else
		{
			std::cout << "Color Depth not 24bpp" << std::endl;
		}
	}

	delete bmpBytes;
}

void Texture::bufferGL()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//Load texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, pixels);

	//Setup mipmap
	//TODO: check if texture is power of two first
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindGL()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::freeGL()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	delete pixels;
}
