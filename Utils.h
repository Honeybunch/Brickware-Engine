#ifndef UTILS_H
#define UTILS_H

#define _CRT_SECURE_NO_WARNINGS 1

#ifdef __APPLE__
#include <OPENGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

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
