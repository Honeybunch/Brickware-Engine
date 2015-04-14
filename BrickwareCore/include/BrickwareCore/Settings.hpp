#ifndef SETTINGS_H
#define SETTINGS_H

//If we're not on windows, we can't use DirectX
#ifndef _WIN32
#define GL_SUPPORT
#undef D3D_SUPPORT
#else

//If neither is defined, we fall back to GL
#if !defined(D3D_SUPPORT) && !defined(GL_SUPPORT)
#define GL_SUPPORT
#undef D3D_SUPPORT
#endif

//If we're on windows and asked for DirectX, lets no longer ask for GL
#if defined(D3D_SUPPORT) && defined(_WIN32)
#undef GL_SUPPORT
#endif

#endif

#endif