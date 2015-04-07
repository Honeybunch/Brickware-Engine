#ifndef UTILS_H
#define UTILS_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//DLL Header
#include "BrickwareUtilsDLL.h"

//Graphics Headers
#include <windows.h>

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
			static std::vector<char*> stringSplit(char* toSplit, char* delimiter);

			static char* textFileRead(char *fn);
			static int textFileWrite(char *fn, char *s);

			static char* trimToLastChar(char* string, char lastChar);
			static char* trimAllWhitespace(char* string);

			static void printShaderInfoLog(GLuint obj);
			static void printProgramInfoLog(GLuint obj);

			static WCHAR* stringToWideString(char* string);
			static char* wideStringToString(WCHAR* wideString);

		private:
			static char* strtok_single(char* string, char const* delimeters);
		};
	}
}
#endif
