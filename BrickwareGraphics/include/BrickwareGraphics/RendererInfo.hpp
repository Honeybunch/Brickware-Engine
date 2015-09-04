#ifndef RENDERER_INFO_H
#define RENDERER_INFO_H

#include <iostream>
#include <vector>

#include "BrickwareGraphicsDLL.hpp"

namespace Brickware
{
  namespace Graphics
  {
	class BRICKWARE_GRAPHICS_API RendererInfo{

    public:
		static std::string GetAPIVersion();
		static int GetAPIMajorVersion();
		static int GetAPIMinorVersion();

		static std::string GetVendor();

		static std::string GetRenderer();
		//static std::string GetExtensionList(); ??

		static std::string GetShadingLanguageVersion();
		static std::vector<std::string> GetAllSupportedShadingLanguageVersions();

    private:
		#ifdef GL_SUPPORT

		static std::string GetGLVersion();
		static int GetGLMajorVersion();
		static int GetGLMinorVersion();
		static std::string GetGLVendor();
		static std::string GetGLRenderer();
		static std::string GetGLSLVersion();
		static std::vector<std::string> GetAllSupportedGLSLVersions();

		#elif D3D_SUPPORT

		static std::string GetD3DVersion();
		static int GetD3DMajorVersion();
		static int GetD3DMinorVersion();
		static std::string GetD3DVendor();
		static std::string GetD3DRenderer();
		static std::string GetHLSLVersion();
		static std::vector<std::string> GetAllSupportedHLSLVersions();

		#endif
    };
  }
}

#endif
