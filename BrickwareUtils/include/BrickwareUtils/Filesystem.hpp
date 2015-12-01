#ifndef FILESYSTEM_H
#define FILESYSTEM_H

//Pre-Include Defines
#define _CRT_SECURE_NO_WARNINGS

//System level headers
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Brickware
{
	namespace Utility
	{
		class Filesystem
		{
		public:
			/*
			 * Creates a directory on the filesystem
			 * @dirPath a string representing the path to the directory
			 * @returns true if the operation succeeded, false if there was an error
			 */
			static bool CreateDir(const char* dirPath);

			/*
			 * Checks to see if a directory exists at the given path
			 * @dirPath a string representing the path to the directory
			 * @returns true on success and false on failure
			 */
			static bool DirExists(const char* dirPath);
		};
	}
}

#endif