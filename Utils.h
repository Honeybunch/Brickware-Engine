#ifndef UTILS_H
#define UTILS_H

#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>

using namespace std;

class Utils
{
public:
	static vector<char*> stringSplit(char* toSplit, char* delimiter);

	static char* textFileRead(char *fn);
	static int textFileWrite(char *fn, char *s);

	static void printShaderInfoLog(GLuint obj);
	static void printProgramInfoLog(GLuint obj);

private:
	static char* strtok_single(char* string, char const* delimeters);
};

#endif
