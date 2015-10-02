#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/GraphicsSettings.hpp"

using namespace Brickware;
using namespace Graphics;

//Default Grapics Settings
RenderingAPI GraphicsSettings::Renderer = OpenGL;
bool GraphicsSettings::VSync = false;
bool GraphicsSettings::Shadows = true;
AntiAliasing GraphicsSettings::AA = AntiAliasing::MSAAx2;
TextureFiltering GraphicsSettings::TexFiltering = TextureFiltering::Anisotropicx2;
ShadowQuality GraphicsSettings::ShadowQual = ShadowQuality::Low;
