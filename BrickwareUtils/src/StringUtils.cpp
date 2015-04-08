#define BRICKWARE_UTILITY_EXPORTS

#include "StringUtils.h"

using namespace Brickware;
using namespace Utility;

/*
	Public Functions
*/

//return a vector of c-strings split by a delimeter
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

		token = strtok_single(NULL, delimiter);
	}

	//Cleanup
	delete[] token;
	delete[] toSplitMod;

	return split;
}

char* StringUtils::textFileRead(char *fileName){

	FILE *file;
	char *content = NULL;

	int count = 0;

	if (fileName != NULL)
	{
		file = fopen(fileName, "rt");

		if (file != NULL)
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

int StringUtils::textFileWrite(char *fileName, char *stringToWrite)
{
	FILE *file;
	int status = 0;

	if (fileName != NULL)
	{
		file = fopen(fileName, "w");

		if (file != NULL)
		{
			if (fwrite(stringToWrite, sizeof(char), strlen(stringToWrite), file) == strlen(stringToWrite))
				status = 1;

			fclose(file);
		}
	}
	return(status);
}

char* StringUtils::trimToLastChar(char* string, char lastChar)
{
	unsigned int stringLength = strlen(string);
	int lastIndex = 0;

	//Get the index that we want to trim the string at
	for (unsigned int i = 0; i < stringLength; i++)
	{
		char indexedChar = string[i];
		if (indexedChar == lastChar)
			lastIndex = i;
	}

	//Return everything after that index
	unsigned int returnedStringLength = stringLength - lastIndex;
	char* toReturn = new char[returnedStringLength + 1];

	//Manual copy
	for (unsigned int i = 0; i < returnedStringLength; i++)
		toReturn[i] = string[i + lastIndex + 1];

	toReturn[returnedStringLength] = 0;
	
	return toReturn;
}

char* StringUtils::trimAllWhitespace(char* string)
{
	unsigned int spaceCount = 0;
	const unsigned int originalStringSize = strlen(string);

	//Determine how many spaces there are in the string so we can build a proper string
	for (unsigned int i = 0; i < originalStringSize; i++)
	{
		if (string[i] == ' ' || string[i] == '\n' || string[i] == '\t')
			spaceCount++;
	}

	const int trimmedStringSize = originalStringSize - spaceCount;

	char* trimmedString = (char*)(malloc(13 * sizeof(char)));

	//Time to fill the trimmed string with everything that isn't whitespace
	int offset = 0;
	for (unsigned int i = 0; i < originalStringSize; i++)
	{
		if (string[i] != ' ' && string[i] != '\n' && string[i] != '\t')
			trimmedString[i - offset] = string[i];
		else
			offset++;
	}
	trimmedString[trimmedStringSize] = '\0';

	return trimmedString;
}

void StringUtils::printShaderInfoLog(GLuint obj)
{
	GLint infologLength = 0;
	GLsizei charsWritten = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = new char(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		cout << infoLog << endl;
		
		delete infoLog;
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
		infoLog = new char(infologLength);

		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		cout << infoLog << endl;

		delete infoLog;
	}
}

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

/*
	Private Functions
*/

//Used to more split a string while handling two consecutive delimeters which strok does not do by default
char* StringUtils::strtok_single(char* string, char const* delimeters)
{
	static char* src = NULL;
	char* p = 0;
	char* ret = 0;

	if (string != NULL)
	{
		src = string;
	}
	if(src == NULL)
		return NULL;

	if((p = strpbrk(src, delimeters)) != NULL)
	{
		*p = 0;
		
		ret = new char[strlen(src) + 1];
		strcpy(ret, src);
		ret[strlen(src) + 1] = '\0';

		src = ++p;
	}

	return ret;
}