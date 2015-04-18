#ifndef UTILS_H
#define UTILS_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//DLL Header
#include "BrickwareUtils\BrickwareUtilsDLL.hpp"

#include <GL/glew.h>

#ifdef _WIN32
//Graphics Headers
#include <windows.h>
#endif

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
		/* A static class for performing some simple string operations.
		 *
		 * The std::string is used only when needed as this is a big project that.
		 * doesn't need the extra bloat. However char*'s are a bit of a beast to work with.
		 * so this class aims to make it a bit easier.
		 */
		class BRICKWARE_UTILITY_API StringUtils
		{
		public:
			/* Splits a string into a series of strings by a delimiter.
			 * @toSplit The string to split.
			 * @delimiter The string that acts as a delimiter to split the other string by.
			 * 
			 * @returns A std::vector of std::strings (not c-strings because they won't be able to be freed on windows).
			 */
			static std::vector<std::string> stringSplit(const char* toSplit, const char* delimiter);
			
			/* Splits a string into two strings by a delimiter.
			 * @toSplit The string to split.
			 * @delimiter The string that acts as a delimiter to split the other string by.
			 *
			 * @returns A std::vector of two std::strings split once along the given delimiter.
			 */
			static std::vector<std::string> splitOnce(const char* toSplit, const char* delimiter);

			/* Reads a file into a c-styled string.
			 * @filename The file name to read from Ex. "Data/mytext.txt".
			 *
			 * @returns The contents of the file if successful, 
			 *          NULL if there was an error reading
			 */
			static char* textFileRead(const char* fileName);

			/* Writes a string into a file.
			 * @filename The name of the file to create and or write into.
			 * @contents The data to fill the file with.
			 * 
			 * @returns 1 if sucessful, 
			 *          0 if there was an error.
			 */
			static int textFileWrite(const char* fileName, char* contents);

			/* Trims the string down to the last instance of a character.
			 * @string The string to trim down.
			 * @lastChar The character that you want to trim to.
			 * 
			 * Super useful if you need to trim off everything after the last '/'
			 * in a file path or a file extension from a file name.
			 *
			 * @returns A properly trimmed string if the character was found, 
			 *          the original string if the character was not found.
			 */
			static char* trimToLastChar(char* string, char lastChar);
			
			/* Trims all whitespace out of a string.
			 * @string The string that you want to remove whitespace from
			 * @returns The trimmed string
			 */
			static const char* trimAllWhitespace(const char* string);

			/* Prints the shader info log *GL_SUPPORT ONLY*
			 * @obj the int that points to the shader on the GPU
			 * 
			 * Used mostly for when GLSL Shaders are compiled
			 */
			static void printShaderInfoLog(GLuint obj);

			/* Prints the shader info log *GL_SUPPORT ONLY*
			* @obj the int that points to the shader program on the GPU
			*
			* Used mostly for when GLSL Shaders are linked
			*/
			static void printProgramInfoLog(GLuint obj);
		
#ifdef _WIN32	
			/* Converts a c-string to a windows wide-string *WINDOWS ONLY*
			 * @string The c-styled string to convert.
			 */
			static WCHAR* stringToWideString(char* string);

			/* Converts a windows wide-string to a c-string *WINDOWS ONLY*
			 * @wideString The windows wide-string to convert to a c-styled string
			 */
			static char* wideStringToString(WCHAR* wideString);
#endif

		private:
			static char* strtok_single(char* string, const char* delimeters);
		};
	}
}
#endif
