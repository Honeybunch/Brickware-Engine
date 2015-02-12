#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "BrickwareCoreDLL.h"

#include "BrickwareMath.h"

#include <vector>
using namespace std;

class BRICKWARE_CORE_API Triangle
{
public:
	Triangle();
	Triangle(Vector4 point1, Vector4 point2, Vector4 point3);
	Triangle(Vector3 point1, Vector3 point2, Vector3 point3);

	Triangle(Vector4 point1, Vector4 point2, Vector4 point3, 
			 Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3);
	Triangle(Vector3 point1, Vector3 point2, Vector3 point3,
			 Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3);

	Triangle(Vector4 point1, Vector4 point2, Vector4 point3, 
			 Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3,
			 Vector3 normal1, Vector3 normal2, Vector3 normal3);
	Triangle(Vector3 point1, Vector3 point2, Vector3 point3,
			 Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3,
			 Vector3 normal1, Vector3 normal2, Vector3 normal3);

	~Triangle(void);

	Vector3* points;
	Vector3* normals;
	Vector2* uvs;

private: 
	void calculateNormal();

};

#endif
