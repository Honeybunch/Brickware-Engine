#ifndef UTILS_H
#define UTILS_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//DLL Header
#include "BrickwareUtilsDLL.h"

#ifdef _WIN32
//Graphics Headers
#include <windows.h>
#endif

#include <GL/glew.h>

//System Level Headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <locale>

using namespace std;

namespace Brickware
{
	namespace Utility
	{
		class BRICKWARE_UTILITY_API StringUtils
		{
		public:
			static std::vector<std::string> stringSplit(const char* toSplit, const char* delimiter);
			static std::vector<std::string> splitOnce(const char* toSplit, const char* delimiter);

			static char* textFileRead(const char *fn);
			static int textFileWrite(const char *fn, char *s);

			static char* trimToLastChar(char* string, char lastChar);
			static const char* trimAllWhitespace(const char* string);

			static void printShaderInfoLog(GLuint obj);
			static void printProgramInfoLog(GLuint obj);
		
#ifdef _WIN32	
			static WCHAR* stringToWideString(char* string);
			static char* wideStringToString(WCHAR* wideString);
#endif

		private:
			static char* strtok_single(char* string, const char* delimeters);
		};
	}
}
#endif
