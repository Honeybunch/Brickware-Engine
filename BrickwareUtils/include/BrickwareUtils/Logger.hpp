#ifndef LOGGER_H
#define LOGGER_H

#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_SECURE_NO_WARNINGS  
#endif

//DLL Header
#include "BrickwareUtils/BrickwareUtilsDLL.hpp"

//System Level Includes
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

namespace Brickware
{
	namespace Utility
	{
		/*
		 * Used to log information out to a log file
		 */
		class BRICKWARE_UTILITY_API Logger
		{
		public:

			/* Initializes the logger opening the file for writing
			 * @return true if successful, false if there was a problem
			 */
			static bool Init();

			/* Closes the logger
			 */
			static void Close();			

			/* Logs out the c-string to the log file
			 * @output The c-string you wish to output to the log
			 */
			static void Log(const char* output);

			/* Logs out the string to the log file
			 * @output the std::string you wish to output to the log
			 */
			static void Log(std::string output);
		private:
			static std::ofstream* logFileStream;

			static char* GetDateTime();
		};
	}
}
#endif