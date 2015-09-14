#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/DirectionalLightInternal.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

void DirectionalLightInternal::InitGL()
{
	////Gen framebuffer
	//glGenFramebuffers(1, &shadowBuffer);
	//glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	//
	////Setup 1024x1024 16 bit depth texture
	//glGenTextures(1, &depthTexture);
	//glBindTexture(GL_TEXTURE_2D, depthTexture);
	//
	//glTexImage2D(GL_TEXTURE_2D,0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	//
	////Don't draw color buffer
	//glDrawBuffer(GL_NONE);
	//
	////Check that the buffer is OK
	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	//	std::cout << "DirectionalLight framebuffer did not initialize properly!" << std::endl;
}

void DirectionalLightInternal::RenderGL(Shader* shader)
{
	Vector3 inverseLightDir = direction * -1;

	//Compute MVP from light's direction
	Matrix4 depthProjection = Matrix4::getOrthographicProjection(-10, 10, -10, 10, -10, 20);
	Matrix4 depthView = Matrix4::getLookAtView(inverseLightDir, Vector3(), Vector3(0, 1, 0));
	Matrix4 depthModel = Matrix4::getIdentityMatrix();

	Matrix4 depthMVP = depthProjection * depthView * depthModel;

	shader->setGlobalMatrix4("depthMatrix", depthMVP);
}