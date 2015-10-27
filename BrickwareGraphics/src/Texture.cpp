#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Texture.hpp"
#include "BrickwareGraphics/GraphicsSettings.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;

Texture::Texture(const char* textureFileName)
{
	//TODO: check file name once we support multiple file namess
	loadBMP(textureFileName);

	//Setup function pointers
	RenderingAPI renderer = GraphicsSettings::Renderer;

	//Setup function pointers based on rendering API
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			bufferTexturePtr = &Texture::bufferGL;
			bindTexturePtr = &Texture::bindGL;
			freeTexturePtr = &Texture::freeGL;
		}
		else
		{
			std::cout << "Error loading Texture: Your card does not support OpenGL 3+" << std::endl;
		}
	}
	else if (renderer = RenderingAPI::DirectX)
	{
		if (RendererInfo::GetAPIMajorVersion() == 11)
		{
#ifdef D3D_SUPPORT
			bufferTexturePtr = &Texture::bufferD3D;
			bindTexturePtr = &Texture::bindD3D;
			freeTexturePtr = &Texture::freeD3D;
#endif
		}
	}

	bufferTexture();
}

void Texture::bufferTexture()
{
	(this->*bufferTexturePtr)();
}

void Texture::bindTexture(int location)
{
	(this->*bindTexturePtr)(location);
}

void Texture::freeTexture(int location)
{
	(this->*freeTexturePtr)(location);
}

unsigned char* Texture::getPixels(){ return pixels; }

void Texture::loadBMP(const char* textureFileName)
{
	char* bmpBytes = StringUtils::textFileRead(textureFileName);

	if (bmpBytes == nullptr)
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

		//Calculate row padding; each row must have a size that is a multiple of 4 bytes
		//Not really necessary for 24 bbp
		//int rowPadding = (width * bytesPerPixel) % 4;

		//Load the colors into the pixels array based on color depth
		switch (colorDepth)
		{
		case 1:
		{

		}
			break;
		case 2:
		{

		}
			break;
		case 4:
		{
		
		}
			break;
		case 8:
		{
		
		}
			break;
		case 16:
		{
			int pixelArraySize = width * height * 2; //Size of the BMP pixel array in bytes
			pixels = new unsigned char[pixelArraySize * 2];

			//BMPs are in BGR by default so use that
			pixelInputFormat = TextureFormat::BGRA;

			//4 elements every 2 bytes
			//Blue is the first 4 bits, Green is the next 4 etc.
			int loopCount = 0;
			for (int i = 0; i < pixelArraySize; i += 2)
			{
				char bg = *(char*)(bmpBytes + pixelArrayLocation + i);
				char ra = *(char*)(bmpBytes + pixelArrayLocation + i + 1);
			
				char b = bg & 0x0F;
				char g = bg >> 4;
			
				char r = ra & 0x0F;
				char a = ra >> 4;
			
				//Multiply by 16 to get into the 0-255 range
				pixels[loopCount] = b * 16;
				pixels[loopCount + 1] = g * 16;
				pixels[loopCount + 2] = r * 16;
				pixels[loopCount + 3] = a * 16;
			
				loopCount += 4;
			}
		}
			break;
		case 24:
		{
			int pixelArraySize = width * height * 3; //Size of the BMP pixel array in bytes
			pixels = new unsigned char[pixelArraySize];

			//BMPs are in BGR by default so use that
			pixelInputFormat = TextureFormat::BGR;

			//3 elements every 3 bytes
			for (int i = 0; i < pixelArraySize; i += 3)
			{
				//Set Blue
				pixels[i]     = *(char*)(bmpBytes + pixelArrayLocation + i);
				//Green
				pixels[i + 1] = *(char*)(bmpBytes + pixelArrayLocation + i + 1);
				//Red
				pixels[i + 2] = *(char*)(bmpBytes + pixelArrayLocation + i + 2);
			}
		}
			break;
		
		//Thanks to: https://en.wikipedia.org/wiki/BMP_file_format
		case 32:
		{
			int pixelArraySize = width * height * 4; //Size of the BMP pixel array in bytes
			pixels = new unsigned char[pixelArraySize];

			pixelInputFormat = TextureFormat::BGRA;

			for (int i = 0; i < pixelArraySize; i += 3)
			{
				//Set Blue
				pixels[i] = *(char*)(bmpBytes + pixelArrayLocation + i);
				//Green
				pixels[i + 1] = *(char*)(bmpBytes + pixelArrayLocation + i + 1);
				//Red
				pixels[i + 2] = *(char*)(bmpBytes + pixelArrayLocation + i + 2);
				//Alpha
				pixels[i + 3] = *(char*)(bmpBytes + pixelArrayLocation + i + 3);
			}

		}
			break;
		default:
			std::cout << "Color Depth not supported" << std::endl;
			break;
		}
	}

	delete[] bmpBytes;
}

Texture::~Texture()
{
	delete[] pixels;
}
