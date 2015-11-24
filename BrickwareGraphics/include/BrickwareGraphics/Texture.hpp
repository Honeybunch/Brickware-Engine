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

		enum BRICKWARE_GRAPHICS_API TextureColorSpace
		{
			LINEAR,
			GAMMA
		};

		enum BRICKWARE_GRAPHICS_API MagFilterOption
		{
			MAG_NEAREST,
			MAG_LINEAR
		};

		enum BRICKWARE_GRAPHICS_API MinFilterOption
		{
			MIN_NEAREST,
			MIN_LINEAR,
			MIN_NEAREST_MIPMAP_NEAREST,
			MIN_LINEAR_MIPMAP_NEAREST,
			MIN_NEAREST_MIPMAP_LINEAR,
			MIN_LINEAR_MIPMAP_LINEAR
		};

		enum BRICKWARE_GRAPHICS_API TextureWrapOption
		{
			CLAMP_TO_EDGE,
			CLAMP_TO_BORDER,
			MIRRORED_REPEAT,
			REPEAT,
			MIRROR_CLAMP_TO_EDGE
		};

		class BRICKWARE_GRAPHICS_API Texture
		{
			friend class RenderTexture;

		public:
			Texture(const char* textureFileName, 
					TextureColorSpace textureColorSpace = TextureColorSpace::GAMMA,
					MagFilterOption textureMagOption = MagFilterOption::MAG_LINEAR,
					MinFilterOption textureMinOption = MinFilterOption::MIN_LINEAR,
					TextureWrapOption textureWrapS = TextureWrapOption::REPEAT, 
					TextureWrapOption textureWrapT = TextureWrapOption::REPEAT, 
					TextureWrapOption textureWrapR = TextureWrapOption::REPEAT);

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

			TextureColorSpace colorSpace;

#ifdef GL_SUPPORT
			GLuint glTexture;
			GLenum glInternalFormat;
			GLenum glPixelInputFormat;

			GLenum glMinFilterOption;
			GLenum glMagFilterOption;
			
			GLenum glWrap_s;
			GLenum glWrap_t;
			GLenum glWrap_r;

			void bufferGL();
			void bindGL(int location);
			void freeGL(int location);

			void setTextureOptionsGL(MagFilterOption textureMagOption,
									 MinFilterOption textureMinOption,
									 TextureWrapOption textureWrapS,
									 TextureWrapOption textureWrapT,
									 TextureWrapOption textureWrapR);
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
