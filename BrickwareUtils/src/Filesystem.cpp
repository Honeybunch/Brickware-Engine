#define BRICKWARE_UTILITY_EXPORTS

#include "BrickwareUtils/Filesystem.hpp"

using namespace Brickware;
using namespace Utility;

bool Filesystem::CreateDir(const char* folderPath)
{
#ifdef _WIN32
	return (CreateDirectory(folderPath, NULL) != 0);
#endif
#ifdef __linux__
	return (mkdir(folderPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0);
#endif
}

bool Filesystem::DirExists(const char* folderPath)
{
	struct stat info;
	
	bool exists = false;
	bool isDir = false;

	//Check if the path is a directory
	if (stat(folderPath, &info) != 0)
		return false;

	//Check if the path is a directory
	if (!(info.st_mode & S_IFDIR))
		return false;
	
	return true;
}