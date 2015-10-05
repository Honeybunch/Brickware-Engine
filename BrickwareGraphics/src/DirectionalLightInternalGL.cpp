#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/DirectionalLightInternal.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

void DirectionalLightInternal::InitGL()
{
	//Gen framebuffer
	glGenFramebuffers(1, &shadowBuffer);

	//Setup 16 bit depth texture
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadowMapRes, shadowMapRes, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	//Don't draw color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//Check that the buffer is OK
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "DirectionalLight framebuffer encountered an error!" << std::endl;

	//Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLightInternal::RenderShadowMapGL()
{
	RenderingManager::DirectionalShadowShader->bindShader();

	//Send info about the directional light to the shader for shadow mapping
	Vector3 focalPoint = direction * -30;

	//Compute MVP from light's direction
	Matrix4 depthProjection = Matrix4::getOrthographicProjection(-30, 30, -30, 30, -60, 60);
	Matrix4 depthView = Matrix4::getLookAtView(focalPoint, Vector3(0, 0, 0), Vector3(0, 1, 0));

	depthVP = depthView * depthProjection;

	Matrix4 biasMatrix(1.0f, 0.0f, 0.0f, 0.0f,
					   0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);

	//Apply bias to get texture coordinates
	depthBiasMVP = biasMatrix * depthVP;

	RenderingManager::DirectionalShadowShader->setGlobalMatrix4("depthVP", depthVP);

	//Setup for drawing to buffer
	glViewport(0, 0, shadowMapRes, shadowMapRes);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);

	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	//Render to texture
	RenderingManager::RenderSceneShadowsGL(RenderingManager::DirectionalShadowShader);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderingManager::DirectionalShadowShader->freeShader();
}

void DirectionalLightInternal::BindShadowMapGL(Shader* shader)
{
	shader->setGlobalMatrix4("depthBiasMVP", depthBiasMVP);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
}
