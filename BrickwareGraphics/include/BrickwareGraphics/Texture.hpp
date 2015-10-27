#ifndef TEXTURE_H
#define TEXTURE_H

//DLL Header
#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

//Graphics Headers
#ifdef GL_SUPPORT
#include <GL/glew.h>
#endif

#ifdef D3D_SUPPORT
#include <windows.h>
#include <d3d11.h>
#endif

//Other Project Headers
#include "BrickwareUtils/StringUtils.hpp"

//System Level Headers

//Project Headers

namespace Brickware
{
	namespace Graphics
	{
		enum BRICKWARE_GRAPHICS_API TextureFormat{
			RGBA,
			BGRA,
			RGB,
			BGR
		};

		class BRICKWARE_GRAPHICS_API Texture
		{
		public:
			Texture(const char* textureFileName);

			unsigned char* getPixels();

			void bindTexture(int location);
			void freeTexture(int location);

			~Texture();

		private:
			void loadBMP(const char* textureFileName);

			void bufferTexture();

			//Function pointers for branching between rendering APIs
			void(Texture::*bufferTexturePtr)();
			void(Texture::*bindTexturePtr)(int location);
			void(Texture::*freeTexturePtr)(int location);

			TextureFormat internalFormat;
			TextureFormat pixelInputFormat;

#ifdef GL_SUPPORT
			GLuint glTexture;
			GLenum glInternalFormat;
			GLenum glPixelInputFormat;

			void bufferGL();
			void bindGL(int location);
			void freeGL(int location);
#endif

#ifdef D3D_SUPPORT
			ID3D11Texture2D* d3dTexture;
			ID3D11ShaderResourceView* d3dTextureSRV;

			void bufferD3D();
			void bindD3D(int location);
			void freeD3D(int location);
#endif

			int width;
			int height;

			short colorDepth; //Bits per pixel
			int compressionType;

			unsigned char* pixels;
		};
	}
}
#endif
