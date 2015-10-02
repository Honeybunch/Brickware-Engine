#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/PointLightInternal.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

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

	//Setup 16 bit depth texture
	glGenBuffers(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadowMapRes, shadowMapRes, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Create cube map
	glGenTextures(1, &shadowCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMap);

	for (unsigned int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, shadowMapRes, shadowMapRes, 0, GL_RED, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//Setup FBO
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

	//Disable the color buffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	//Check that the buffer is OK
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "DirectionalLight framebuffer encountered an error!" << std::endl;

	//Unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointLightInternal::RenderShadowMapGL(Shader* shadowShader)
{
	Matrix4 depthProjection = Matrix4::getPerspectiveProjection(90.0f, (float)shadowMapRes, (float)shadowMapRes, 0.1f, 30.0f);
	Matrix4 depthView;

	Matrix4 biasMatrix(1.0f, 0.0f, 0.0f, 0.0f,
					   0.0f, 1.0f, 0.0f, 0.0f,
					   0.0f, 0.0f, 1.0f, 0.0f,
					   0.0f, 0.0f, 0.0f, 1.0f);

	glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowBuffer);
	glViewport(0, 0, shadowMapRes, shadowMapRes);

	//Render 6 maps, one in each direction
	for (unsigned int i = 0; i < 6; i++)
	{
		//Setup look at for this direction
		CameraDirection cameraDirection = CameraDirections[i];
		depthView = Matrix4::getLookAtView(position, cameraDirection.Target, cameraDirection.Up);

		//Set matrices
		depthVP = depthView * depthProjection;
		shadowShader->setGlobalMatrix4("depthVP", depthVP);

		//Setup for drawing to buffer
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cameraDirection.CubemapFace, shadowBuffer, 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		//Render to texture
		glClearDepth(1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);

		RenderingManager::RenderSceneShadowsGL();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PointLightInternal::BindShadowMapGL(Shader* shader)
{
	//Bind Shadow cube map
}