#ifndef RENDERER_INFO_H
#define RENDERER_INFO_H

#include <iostream>
#include <vector>

namespace Brickware
{
  namespace Graphics
  {
    class RendererInfo{

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
      #endif

      #ifdef D3D_SUPPORT
      static std::string GetDirectXVersion();
      static int GetDirectXMajor();
      static int GetDirectXMinor();
      static std::string GetDirectXVendor();
      static std::string GetDirectXRenderer();
      static std::string GetHLSLVersion();
      #endif
      static std::vector<std::string> GetAllSupportedHLSLVersions();
    };
  }
}

#endif
