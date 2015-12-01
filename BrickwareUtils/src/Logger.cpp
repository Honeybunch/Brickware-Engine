#define BRICKWARE_UTILITY_EXPORTS

#include "BrickwareUtils/Logger.hpp"
#include "BrickwareUtils/Filesystem.hpp"

using namespace Brickware;
using namespace Utility;

//Static Definitions
std::ofstream* Logger::logFileStream;

/*
	Public Methods
*/
bool Logger::Init()
{
	//If the Logs folder doesn't exist we should create it
	if (!Filesystem::DirExists("Logs/"))
		Filesystem::CreateDir("Logs/");

	logFileStream = new std::ofstream();

	logFileStream->open("Logs/Brickware.log", std::ios::out | std::ios::app);
	if (!logFileStream->is_open())
	{
		logFileStream->close();
		return false;
	}
	//Log when the file was opened / created
	char* currentTime = GetDateTime();

	(*logFileStream) << "--- Log File Opened At: " << currentTime << " --- " << std::endl << std::endl;

	//Flush stream but don't close
	logFileStream->flush();

	delete currentTime;

	return true;
}

void Logger::Close()
{
	logFileStream->close();
	delete logFileStream;
}

void Logger::Log(const char* output)
{
	//Timestamp all logs
	char* currentTime = GetDateTime();

	(*logFileStream) << output << " - " << currentTime << std::endl;

	logFileStream->flush(); //Flush so that we can read the file as the program runs

	delete currentTime;
}

void Logger::Log(std::string output)
{
	//Timestamp all logs
	char* currentTime = GetDateTime();

	(*logFileStream) << output << " - " << currentTime << std::endl;

	logFileStream->flush(); //Flush so that we can read the file as the program runs

	delete currentTime;
}

/*
	Private Methods
*/

char* Logger::GetDateTime()
{
	time_t rawTime;
	struct tm* timeInfo;

	char* buffer = new char[80];

	//Get the time
	time(&rawTime);
	timeInfo = localtime(&rawTime);

	//Convert to a readable format
	strftime(buffer, 80, "%d-%m-%Y %I:%M:%S", timeInfo);

	return buffer;
}