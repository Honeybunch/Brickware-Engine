#ifndef SIMPLE_SHAPE_H
#define SIMPLE_SHAPE_H

#define _USE_MATH_DEFINES 1
#define _CRT_SECURE_NO_WARNINGS 1

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <math.h>

using namespace std;

#include "Triangle.h"
#include "Utils.h"

enum PrimitiveType
{
	CUBE,
	CYLINDER,
	CONE,
	SPHERE,
};

class Shape
{
public :
	Shape(PrimitiveType type, int division1, int division2, float radius = 1.0f);
	Shape(char* fileName);
	~Shape();

	//Accessors
	float* getPoints();
	vector<Triangle> getTriangles();
	float* getNormals();
	GLushort* getIndicies();
	float* getTexCoords();

	int getPointCount();
	int getNormalCount();
	int getTexCoordSize();

private: 	
	vector<Triangle> triangles;
	float* pointElements;
	float* normalElements;
	GLushort* indicies;
	float* texCoords;


	vector<float> makeLoopDivisions(float start, float length, int divisions);

	//For creating basic shapes
	void makeCube(int subdivisions);
	void makeCylinder(float radius, int radialDivisions, int heightDivisions);
	void makeCone(float radius, int radialDivisions, int heightDivisions);
	void makeSphere(float radius, int slices, int stacks);

	//For loading file types
	void loadOBJ(char* fileName);

	void addTriangle(Vector3 point1, Vector3 point2, Vector3 point3,
					 Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3);

	void addTriangle(Vector3 point1, Vector3 point2, Vector3 point3,
					 Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3,
					 Vector3 normal1, Vector3 normal2, Vector3 normal3);
};

#endif