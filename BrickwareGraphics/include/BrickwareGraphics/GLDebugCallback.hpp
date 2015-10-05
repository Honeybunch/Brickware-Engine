// This is free and unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or distribute
// this software, either in source code form or as a compiled binary, for any
// purpose, commercial or non-commercial, and by any means.
// 
// In jurisdictions that recognize copyright laws, the author or authors of this
// software dedicate any and all copyright interest in the software to the
// public domain. We make this dedication for the benefit of the public at large
// and to the detriment of our heirs and successors. We intend this dedication
// to be an overt act of relinquishment in perpetuity of all present and future
// rights to this software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to <http://unlicense.org/>

// REQUIREMENTS: OpenGL version with the KHR_debug extension available.

#ifdef GL_SUPPORT

#ifndef GL_DEBUG_CALLBACK_H
#define GL_DEBUG_CALLBACK_H

#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

	// Callback function for printing debug statements
	BRICKWARE_GRAPHICS_API void APIENTRY GLPrintErrorCallback(GLenum source, GLenum type, GLuint id,
		GLenum severity, GLsizei length,
		const GLchar *msg, const void *data);

#ifdef __cplusplus
}
#endif

#endif 
#endif