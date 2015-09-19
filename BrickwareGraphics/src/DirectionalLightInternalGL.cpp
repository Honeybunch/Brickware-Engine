#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/DirectionalLightInternal.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

void DirectionalLightInternal::InitGL()
{
	
	// //Gen framebuffer
	// glGenFramebuffers(1, &shadowBuffer);
	// glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	//
	// //Setup 1024x1024 16 bit depth texture
	// glGenTextures(1, &depthTexture);
	// glBindTexture(GL_TEXTURE_2D, depthTexture);
	//
	// glTexImage2D(GL_TEXTURE_2D,0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
	//
	// //Don't draw color buffer
	// glDrawBuffer(GL_FRONT);
	//
	// //Check that the buffer is OK
	// if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	// 	std::cout << "DirectionalLight framebuffer encountered an error!" << std::endl;
	//
	// //Unbind framebuffer
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DirectionalLightInternal::RenderGL(Shader* shader)
{
	// //Basic light render
	// BasicRender(shader);
	//
	// //Bind shadow shader for rendering shadow maps
	// ShadowShader->bindShader();
	//
	// //Send info about the directional light to the shader for shadow mapping
	// Vector3 inverseLightDir = direction * -1;
	//
	// //Compute MVP from light's direction
	// Matrix4 depthProjection = Matrix4::getOrthographicProjection(-10, 10, -10, 10, -10, 20);
	// Matrix4 depthView = Matrix4::getLookAtView(inverseLightDir, Vector3(), Vector3(0, 1, 0));
	// Matrix4 depthModel = Matrix4::getIdentityMatrix();
	//
	// Matrix4 depthMVP = depthProjection * depthView * depthModel;
	//
	// Matrix4 biasMatrix(0.5f, 0.0f, 0.0f, 0.0f,
	// 				   0.0f, 0.5f, 0.0f, 0.0f,
	// 				   0.0f, 0.0f, 0.5f, 0.0f,
	// 				   0.5f, 0.5f, 0.5f, 1.0f);
	//
	// //Apply bias to get texture coordinates
	// depthMVP = biasMatrix * depthMVP;
	//
	// ShadowShader->setGlobalMatrix4("worldMatrix", depthMVP);
	//
	// //Actually bind framebuffer and render shadow map for texture
	//
	// //Bind buffer for drawing
	// glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	//
	// //render buffer
	// glViewport(0, 0, 800, 600);
	//
	// //Unbind framebuffer
	// glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//
	// //Rebind other shader
	// shader->bindShader();
}
