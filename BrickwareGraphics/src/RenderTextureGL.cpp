#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/RenderTexture.hpp"

using namespace Brickware;
using namespace Graphics;

/*
	All Private
*/

#ifdef GL_SUPPORT

void RenderTexture::InitGL()
{
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//Setup depth texture if we wanted one
	if (depth > 0)
	{
		glGenTextures(1, &depthBufferTexture);
		glBindTexture(GL_TEXTURE_2D, depthBufferTexture);
	
		GLint glDepth = GL_DEPTH_COMPONENT16;
		if (depth == 24)
			glDepth = GL_DEPTH_COMPONENT24;
	
		glTexImage2D(GL_TEXTURE_2D, 0, glDepth, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
			depthBufferTexture, 0);
	}

	//Setup color texture
	glGenTextures(1, &colorBufferTexture);
	glBindTexture(GL_TEXTURE_2D, colorBufferTexture);

	GLint glFormat = GL_RGBA;

	switch (format)
	{
	case TextureFormat::RGBA:
		glFormat = GL_RGBA;
		break;
	case TextureFormat::BGRA:
		glFormat = GL_BGRA;
		break;
	case TextureFormat::RGB:
		glFormat = GL_RGB;
		break;
	case TextureFormat::BGR:
		glFormat = GL_BGR;
		break;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, glFormat, width, height, 0, glFormat, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
		colorBufferTexture, 0);

	//Draw to the first color attachment
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// Check that the buffer is OK
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "RenderTexture framebuffer encountered an error!"
			<< std::endl;

	// Unbind
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::BindGL()
{
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	glClearDepth(1.0f);
	glClearColor(1, 1, 1, 1);

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void RenderTexture::FreeGL()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderTexture::DestroyGL()
{
	glDeleteTextures(1, &depthBufferTexture);
	glDeleteTextures(1, &colorBufferTexture);

	glDeleteFramebuffers(1, &frameBuffer);
}

#endif