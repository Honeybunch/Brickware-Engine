#ifndef UTILS_H
#define UTILS_H

#include "BrickwareCoreDLL.h"

#define _CRT_SECURE_NO_WARNINGS

#include "Settings.h"

#ifdef GL_SUPPORT
#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>
#elif defined(D3D_SUPPORT)
#include <windows.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <locale>

using namespace std;

class Utils
{
public:
	static BRICKWARE_CORE_API vector<char*> stringSplit(char* toSplit, char* delimiter);

	static BRICKWARE_CORE_API char* textFileRead(char *fn);
	static BRICKWARE_CORE_API int textFileWrite(char *fn, char *s);

	static BRICKWARE_CORE_API char* trimToLastChar(char* string, char lastChar);

#ifdef GL_SUPPORT
	static BRICKWARE_CORE_API void printShaderInfoLog(GLuint obj);
	static BRICKWARE_CORE_API void printProgramInfoLog(GLuint obj);
#endif

#ifdef D3D_SUPPORT
	static BRICKWARE_CORE_API WCHAR* stringToWideString(char* string);
	static BRICKWARE_CORE_API char* wideStringToString(WCHAR* wideString);
#endif

private:
	static BRICKWARE_CORE_API char* strtok_single(char* string, char const* delimeters);
};

#endif
