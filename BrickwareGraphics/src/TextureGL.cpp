#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Texture.hpp"

using namespace Brickware;
using namespace Graphics;

#ifdef GL_SUPPORT
void Texture::bufferGL()
{
	//don't buffer if it wasn't loaded properly
	if (width < 0 || height < 0)
		return;

	glGenTextures(1, &glTexture);
	glBindTexture(GL_TEXTURE_2D, glTexture);

	if (colorSpace == TextureColorSpace::GAMMA)
	{
		switch (pixelInputFormat)
		{
		case TextureFormat::RGBA:
			glPixelInputFormat = GL_RGBA;
			glInternalFormat = GL_RGBA;
			break;
		case TextureFormat::BGRA:
			glPixelInputFormat = GL_BGRA;
			glInternalFormat = GL_RGBA;
			break;
		case TextureFormat::RGB:
			glPixelInputFormat = GL_RGB;
			glInternalFormat = GL_RGB;
			break;
		case TextureFormat::BGR:
			glPixelInputFormat = GL_BGR;
			glInternalFormat = GL_RGB;
			break;
		}
	}
	else if (colorSpace == TextureColorSpace::LINEAR)
	{
		switch (pixelInputFormat)
		{
		case TextureFormat::RGBA:
			glPixelInputFormat = GL_RGBA;
			glInternalFormat = GL_SRGB_ALPHA;
			break;
		case TextureFormat::BGRA:
			glPixelInputFormat = GL_BGRA;
			glInternalFormat = GL_SRGB_ALPHA;
			break;
		case TextureFormat::RGB:
			glPixelInputFormat = GL_RGB;
			glInternalFormat = GL_SRGB;
			break;
		case TextureFormat::BGR:
			glPixelInputFormat = GL_BGR;
			glInternalFormat = GL_SRGB;
			break;
		}
	}
	
	//Load texture data
	glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glPixelInputFormat, GL_UNSIGNED_BYTE, pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilterOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, glMinFilterOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glWrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glWrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, glWrap_r);

	//Setup mipmaps and parameters
	glGenerateMipmap(GL_TEXTURE_2D);

	//Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bindGL(int location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, glTexture);
}

void Texture::freeGL(int location)
{
	glActiveTexture(GL_TEXTURE0 + location);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setTextureOptionsGL(MagFilterOption textureMagOption,
								  MinFilterOption textureMinOption,
								  TextureWrapOption textureWrapS,
								  TextureWrapOption textureWrapT,
								  TextureWrapOption textureWrapR)
{
	switch (textureMagOption)
	{
	case MagFilterOption::MAG_LINEAR:
		glMagFilterOption = GL_LINEAR;
		break;
	case MagFilterOption::MAG_NEAREST:
		glMagFilterOption = GL_NEAREST;
		break;
	}
	switch (textureMinOption)
	{
	case MinFilterOption::MIN_LINEAR:
		glMinFilterOption = GL_LINEAR;
		break;
	case MinFilterOption::MIN_NEAREST:
		glMinFilterOption = GL_NEAREST;
		break;
	case MIN_NEAREST_MIPMAP_NEAREST:
		glMinFilterOption = GL_NEAREST_MIPMAP_NEAREST;
		break;
	case MIN_LINEAR_MIPMAP_NEAREST:
		glMinFilterOption = GL_LINEAR_MIPMAP_NEAREST;
		break;
	case MIN_NEAREST_MIPMAP_LINEAR:
		glMinFilterOption = GL_NEAREST_MIPMAP_LINEAR;
		break;
	case MIN_LINEAR_MIPMAP_LINEAR:
		glMinFilterOption = GL_LINEAR_MIPMAP_LINEAR;
		break;
	}
	switch (textureWrapS)
	{
	case CLAMP_TO_EDGE:
		glWrap_s = GL_CLAMP_TO_EDGE;
		break;
	case CLAMP_TO_BORDER:
		glWrap_s = GL_CLAMP_TO_BORDER;
		break;
	case MIRRORED_REPEAT:
		glWrap_s = GL_MIRRORED_REPEAT;
		break;
	case REPEAT:
		glWrap_s = GL_REPEAT;
		break;
	case MIRROR_CLAMP_TO_EDGE:
		glWrap_s = GL_MIRROR_CLAMP_TO_EDGE;
		break;
	}
	switch (textureWrapT)
	{
	case CLAMP_TO_EDGE:
		glWrap_t = GL_CLAMP_TO_EDGE;
		break;
	case CLAMP_TO_BORDER:
		glWrap_t = GL_CLAMP_TO_BORDER;
		break;
	case MIRRORED_REPEAT:
		glWrap_t = GL_MIRRORED_REPEAT;
		break;
	case REPEAT:
		glWrap_t = GL_REPEAT;
		break;
	case MIRROR_CLAMP_TO_EDGE:
		glWrap_t = GL_MIRROR_CLAMP_TO_EDGE;
		break;
	}
	switch (textureWrapR)
	{
	case CLAMP_TO_EDGE:
		glWrap_r = GL_CLAMP_TO_EDGE;
		break;
	case CLAMP_TO_BORDER:
		glWrap_r = GL_CLAMP_TO_BORDER;
		break;
	case MIRRORED_REPEAT:
		glWrap_r = GL_MIRRORED_REPEAT;
		break;
	case REPEAT:
		glWrap_r = GL_REPEAT;
		break;
	case MIRROR_CLAMP_TO_EDGE:
		glWrap_r = GL_MIRROR_CLAMP_TO_EDGE;
		break;
	}
}
#endif