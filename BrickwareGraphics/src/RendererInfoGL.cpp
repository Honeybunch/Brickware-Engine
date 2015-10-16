#ifdef GL_SUPPORT

#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/RendererInfo.hpp"

using namespace Brickware;
using namespace Graphics;

std::string RendererInfo::GetGLVersion()
{
	return std::string((char*)glGetString(GL_VERSION));
}
int RendererInfo::GetGLMajorVersion()
{
	int majorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	return majorVersion;
}
int RendererInfo::GetGLMinorVersion()
{
	int minorVersion;
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
	return minorVersion;
}

std::string RendererInfo::GetGLVendor()
{
	return std::string((char*)glGetString(GL_VENDOR));
}

std::string RendererInfo::GetGLRenderer()
{
	return std::string((char*)glGetString(GL_RENDERER));
}

std::string RendererInfo::GetGLSLVersion()
{
	return std::string((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}
std::vector<std::string> RendererInfo::GetAllSupportedGLSLVersions()
{
	std::vector<std::string> supportedVersions;

	//GL_NUM_SHADING_LANGUAGE_VERSIONS is not available below version 4.3
	if (GetGLMajorVersion() >= 4 && GetGLMinorVersion() >= 3)
	{
		int numShadingLangVersions;
		glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numShadingLangVersions);

		for (int i = 0; i < numShadingLangVersions - 1; i++)
		{
			const char* version = (char*)glGetStringi(GL_SHADING_LANGUAGE_VERSION, i);
			if (version)
				supportedVersions.push_back(std::string(version));
		}
	}
	else
	{
		supportedVersions.push_back("GL Version below 4.3; try using GetShadingLanguageVersion instead");
	}

	return supportedVersions;
}

#endif