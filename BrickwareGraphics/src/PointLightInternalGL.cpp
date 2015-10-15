#define BRICKWARE_GRAPHICS_EXPORTS

#define _USE_MATH_DEFINES

#include "BrickwareGraphics/PointLightInternal.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"
#include <cmath>

using namespace Brickware;
using namespace Graphics;
using namespace Math;

CameraDirection PointLightInternal::CameraDirections[] =
{
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_X, Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_X, Vector3(-1.0f, 0.0f, 0.0f), Vector3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Y, Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, -1.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, Vector3(0.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f) },
	{ GL_TEXTURE_CUBE_MAP_POSITIVE_Z, Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, -1.0f, 0.0f) },
	{ GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, Vector3(0.0f, 0.0f, -1.0f), Vector3(0.0f, -1.0f, 0.0f) }
};

void PointLightInternal::InitGL()
{
	//Gen framebuffer
	glGenFramebuffers(1, &shadowBuffer);

	//Create cube map
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthTexture);

	for (unsigned int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowMapRes, shadowMapRes, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Setup FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	//Disable the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//Check that the buffer is OK
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "PointLight framebuffer encountered an error!" << std::endl;

	//Unbind framebuffer
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointLightInternal::RenderShadowMapGL()
{
	RenderingManager::PointShadowShader->bindShader();

	Matrix4 depthProjection = Matrix4::getPerspectiveProjection((float)M_PI_2, 1.0f, 0.1f, farPlane);
	Matrix4 depthView;

	Matrix4 biasMatrix(1.0f, 0.0f, 0.0f, 0.0f,
					   0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glViewport(0, 0, shadowMapRes, shadowMapRes);

	//Send the 6 view * proj maps to the geometry shader
	std::vector<Matrix4> values;

	for (unsigned int i = 0; i < 6; i++)
	{
		//Setup look at for this direction
		CameraDirection cameraDirection = CameraDirections[i];
		depthView = Matrix4::getLookAtView(position, position + cameraDirection.Target, cameraDirection.Up);
		depthVP = depthView * depthProjection;

		values.push_back(depthVP);
	}

	RenderingManager::PointShadowShader->setGlobalVector3("lightPos", position);
	RenderingManager::PointShadowShader->setGlobalFloat("farPlane", farPlane);
	RenderingManager::PointShadowShader->setMultipleGlobalMatrix4("shadowMatrices[0]", values);

	//Render
	glClear(GL_DEPTH_BUFFER_BIT);
	RenderingManager::RenderSceneShadowsGL(RenderingManager::PointShadowShader);

	//Clean up
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderingManager::PointShadowShader->freeShader();
}

void PointLightInternal::BindShadowMapGL(Shader* shader)
{	
	//Set far plane for rendering
	std::string farPlaneString = "pointLights[" + std::to_string(lightIndex) + "].farPlane";
	shader->setGlobalFloat(farPlaneString.c_str(), farPlane);

	//Bind shadow map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthTexture);
}