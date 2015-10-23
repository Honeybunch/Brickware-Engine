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

	if (pixelInputFormat == TextureFormat::RGBA)
		glPixelInputFormat = GL_RGBA;
	glInternalFormat = GL_RGBA;

	//Load texture data
	glTexImage2D(GL_TEXTURE_2D, 0, glInternalFormat, width, height, 0, glPixelInputFormat, GL_UNSIGNED_BYTE, pixels);

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
#endif