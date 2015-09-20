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
	
	//Setup 1024x1024 16 bit depth texture
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	
	glTexImage2D(GL_TEXTURE_2D,0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
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

void DirectionalLightInternal::RenderShadowMapGL(Shader* shadowShader)
{	
	//Bind shadow shader for rendering shadow maps
	shadowShader->bindShader();
	
	//Send info about the directional light to the shader for shadow mapping
	Vector3 inverseLightDir = direction * -1;
	
	//Compute MVP from light's direction
	Matrix4 depthProjection = Matrix4::getOrthographicProjection(-10, 10, -10, 10, -10, 20);
	Matrix4 depthView = Matrix4::getLookAtView(inverseLightDir, Vector3(), Vector3(0, 1, 0));
	Matrix4 depthModel = Matrix4::getIdentityMatrix();
	
	depthMVP = depthProjection * depthView * depthModel;
	
	Matrix4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
					   0.0f, 0.5f, 0.0f, 0.0f,
					   0.0f, 0.0f, 0.5f, 0.0f,
					   0.5f, 0.5f, 0.5f, 1.0f);
	
	//Apply bias to get texture coordinates
	depthBiasMVP = biasMatrix * depthMVP;
	
	shadowShader->setGlobalMatrix4("depthMVP", depthMVP);
	
	//Actually bind framebuffer and render shadow map for texture
	
	//Bind buffer for drawing
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);

	glClearDepth(1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);

	//render buffer
	RenderingManager::RenderPass();
	
	//Clean up
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	shadowShader->freeShader();
}

void DirectionalLightInternal::BindShadowMapGL(Shader* shader)
{
	shader->setGlobalMatrix4("depthBiasMVP", depthBiasMVP);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
}