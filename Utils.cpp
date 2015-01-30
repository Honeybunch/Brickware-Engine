#include "utils.h"

/*
	Public Functions
*/

//return a vector of c-strings split by a delimeter
vector<char*> Utils::stringSplit(char* toSplit, char* delimiter)
{
	//Modify the given string to add a delimeter to the end of the string so that the last value will be parsed
	char* toSplitMod = new char[strlen(toSplit) + 1];
	strcpy(toSplitMod, toSplit);
	strcat(toSplitMod, &delimiter[0]);

	vector<char*> split;

	//Split string along spaces to get normals
	char* token = strtok_single(toSplitMod, delimiter);
	
	while(token)
	{
		if(strlen(token) > 0)
			split.push_back(token);
		else
			split.push_back("0");

		token = strtok_single(NULL, delimiter);
	}

	delete token;

	return split;
}

char* Utils::textFileRead(char *fileName){

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
		fprintf(stderr, "Error reading %s\n", fileName);
	}

	return content;
}

int Utils::textFileWrite(char *fileName, char *stringToWrite)
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

void Utils::printShaderInfoLog(GLuint obj)
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

void Utils::printProgramInfoLog(GLuint obj)
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

/*
	Private Functions
*/

//Used to more split a string while handling two consecutive delimeters which strok does not do by default
char* Utils::strtok_single(char* string, char const* delimeters)
{
	static char* src = NULL;
	char* p = 0;
	char* ret = 0;

	if(string != NULL)
		src = string;
	if(src == NULL)
		return NULL;

	if((p = strpbrk(src, delimeters)) != NULL)
	{
		*p = 0;
		ret = src;
		src = ++p;
	}

	return ret;
}