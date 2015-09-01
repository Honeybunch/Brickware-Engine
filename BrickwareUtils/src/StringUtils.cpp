#define BRICKWARE_UTILITY_EXPORTS

#include "BrickwareUtils/StringUtils.hpp"

using namespace Brickware;
using namespace Utility;

/*
	Public Functions
*/

//return a vector of std::strings split by a delimeter
vector<string> StringUtils::stringSplit(const char* toSplit, const char* delimiter)
{
	//Modify the given string to add a delimeter to the end of the string so that the last value will be parsed
	char* toSplitMod = new char[strlen(toSplit) + 2];
	strcpy(toSplitMod, toSplit);
	strcat(toSplitMod, &delimiter[0]);
	toSplitMod[strlen(toSplitMod)] = '\0';

	vector<string> split;

	//Split string along spaces to get normals
	char* token = strtok_single(toSplitMod, delimiter);

	while(token)
	{
		if(strlen(token) > 0)
			split.push_back(std::string(token));

		token = strtok_single(nullptr, delimiter);
	}

	//Cleanup
	delete[] token;
	delete[] toSplitMod;

	return split;
}

char* StringUtils::textFileRead(const char *fileName){

	FILE *file;
	char *content = nullptr;

	int count = 0;

	if (fileName != nullptr)
	{
		file = fopen(fileName, "rt");

		if (file != nullptr)
		{
			fseek(file, 0, SEEK_END);
			count = ftell(file);
			rewind(file);

			if (count > 0)
			{
				content = (char*)malloc(sizeof(char) * (count + 1));

				count = fread(content, sizeof(char), count - 1, file);
				content[count] = '\0';
			}
			fclose(file);
		}
	}
	else
	{
		fprintf(stderr, "Invalid file name");
	}

	return content;
}

int StringUtils::textFileWrite(const char *fileName, char *stringToWrite)
{
	FILE *file;
	int status = 0;

	if (fileName != nullptr)
	{
		file = fopen(fileName, "w");

		if (file != nullptr)
		{
			if (fwrite(stringToWrite, sizeof(char), strlen(stringToWrite), file) == strlen(stringToWrite))
				status = 1;

			fclose(file);
		}
	}
	return(status);
}

std::string StringUtils::trimToLastChar(std::string string, char lastChar)
{
	int lastIndex = 0;

	//Get the index that we want to trim the string at
	for (unsigned int i = 0; i < string.length(); i++)
	{
		char indexedChar = string[i];
		if (indexedChar == lastChar)
			lastIndex = i;
	}

	//Return everything after that index
	return string.substr(0, lastIndex);
}

const char* StringUtils::trimAllWhitespace(const char* string)
{
	unsigned int spaceCount = 0;
	const unsigned int originalStringSize = strlen(string);

	//Determine how many spaces there are in the string so we can build a proper string
	for (unsigned int i = 0; i < originalStringSize; i++)
	{
		if (string[i] == ' ' || string[i] == '\n' || string[i] == '\t' || string[i] == '\r')
			spaceCount++;
	}

	const int trimmedStringSize = originalStringSize - spaceCount;

	char* trimmedString = new char[trimmedStringSize];

	//Time to fill the trimmed string with everything that isn't whitespace
	int offset = 0;
	for (unsigned int i = 0; i < originalStringSize; i++)
	{
		if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t' && string[i] != '\r')
			trimmedString[i - offset] = string[i];
		else
			offset++;
	}
	trimmedString[trimmedStringSize] = '\0';

	return trimmedString;
}

#ifdef GL_SUPPORT

void StringUtils::printShaderInfoLog(GLuint obj)
{
	GLint infoLogLength = 0;
	GLsizei charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 0)
	{
		infoLogLength++;
		infoLog = new char(infoLogLength);
		glGetShaderInfoLog(obj, infoLogLength, &charsWritten, infoLog);
		infoLog[infoLogLength] = '\0';
		cout << infoLog << endl;

		//delete[] infoLog;
	}
}

void StringUtils::printProgramInfoLog(GLuint obj)
{
	GLint infologLength = 0;
	GLsizei charsWritten = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = new char(infologLength + 1);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		infoLog[infologLength] = '\0';
		cout << infoLog << endl;

		//delete[] infoLog;
	}
}

#endif

#ifdef _WIN32

WCHAR* StringUtils::stringToWideString(char* string)
{
	int stringSize = strlen(string) + 1;

	WCHAR* wideString = new WCHAR[stringSize];

	swprintf(wideString, stringSize, L"%hs", string);

	return wideString;
}

char* StringUtils::wideStringToString(WCHAR* wideString)
{
	int stringLength = wcslen(wideString);

	char* string = new char[stringLength];

	wcstombs(string, wideString, stringLength);

	return string;
}

#endif

/*
	Private Functions
*/

//Used to more split a string while handling two consecutive delimeters which strok does not do by default
char* StringUtils::strtok_single(char* string, const char* delimeters)
{
	static char* src = nullptr;
	char* p = 0;
	char* ret = 0;

	if (string != nullptr)
	{
		src = string;
	}
	if(src == nullptr)
		return nullptr;

	if((p = strpbrk(src, delimeters)) != nullptr)
	{
		*p = 0;

		ret = new char[strlen(src) + 1];
		strcpy(ret, src);
		ret[strlen(src) + 1] = '\0';

		src = ++p;
	}

	return ret;
}
