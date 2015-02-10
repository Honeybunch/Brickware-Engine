#include "Texture.h"

Texture::Texture(char* textureFileName)
{
	//TODO: check file name once we support multiple file namess
	loadBMP(textureFileName);

	bufferTexture();
}

void Texture::bufferTexture()
{
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

char* Texture::getPixels(){ return pixels; }

void Texture::loadBMP(char* textureFileName)
{
	char* bmpBytes = Utils::textFileRead(textureFileName);

	if (bmpBytes == NULL)
	{
		std::cout << "Error loading: " << textureFileName << std::endl;
		return;
	}

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

		int bytesPerPixel = colorDepth / 8;

		int rowSize = ((colorDepth * width + 31) / 32) * 4;
		int pixelArraySize = width * height * 3; //Size of the BMP pixel array in bytes

		//Calculate row padding; each row must have a size that is a multiple of 4 bytes
		//Not really necessary for 24 bbp
		//int rowPadding = (width * bytesPerPixel) % 4;

		pixels = new char[pixelArraySize];

		//Load the colors into the pixels array based on color depth
		if (colorDepth == 24)
		{
			textureType = TextureType::RGB;
			glTextureType = GL_RGB;

			for (int i = 0; i < pixelArraySize; i+= 3)
			{
				char r, g, b;

				b = *(char*)(bmpBytes + pixelArrayLocation + i) ;
				g = *(char*)(bmpBytes + pixelArrayLocation + i + 1);
				r = *(char*)(bmpBytes + pixelArrayLocation + i + 2);

				pixels[i] = r;
				pixels[i + 1] = g;
				pixels[i + 2] = b;
			}
		}
		else
		{
			std::cout << "Color Depth not 24bpp" << std::endl;
			std::cout << "BMPs with color depth of " << colorDepth << " bpp are not supported yet" << std::endl;
		}
	}

	delete bmpBytes;
}

void Texture::bufferGL()
{
	//don't buffer if it wasn't loaded properly
	if (width < 0 || height < 0)
		return;

	glGenTextures(1, &glTexture);
	glBindTexture(GL_TEXTURE_2D, glTexture);

	//Load texture data
	glTexImage2D(GL_TEXTURE_2D, 0, glTextureType, width, height, 0, glTextureType, GL_UNSIGNED_BYTE, pixels);

	//Setup mipmaps and parameters
	glGenerateMipmap(GL_TEXTURE_2D);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindGL()
{
	glBindTexture(GL_TEXTURE_2D, glTexture);
}

void Texture::freeGL()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	delete pixels;
}
