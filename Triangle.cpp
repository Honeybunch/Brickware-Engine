#include "triangle.h"

Triangle::Triangle()
{
	points = new Vector3[3];
	normals = new Vector3[3];
	uvs = new Vector2[3];
}

Triangle::Triangle(Vector4 point1, Vector4 point2, Vector4 point3)
{
	points = new Vector3[3];
	normals = new Vector3[3];
	uvs = new Vector2[3];

	points[0] = (Vector3)point1;
	points[1] = (Vector3)point2;
	points[2] = (Vector3)point3;

	calculateNormal();
}

Triangle::Triangle(Vector3 point1, Vector3 point2, Vector3 point3)
{
	points = new Vector3[3];
	normals = new Vector3[3];
	uvs = new Vector2[3];

	points[0] = point1;
	points[1] = point2;
	points[2] = point3;

	calculateNormal();
}

Triangle::Triangle(Vector4 point1, Vector4 point2, Vector4 point3,
				   Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3)
{
	points = new Vector3[3];
	normals = new Vector3[3];
	uvs = new Vector2[3];

	points[0] = (Vector3)point1;
	points[1] = (Vector3)point2;
	points[2] = (Vector3)point3;

	uvs[0] = texCoord1;
	uvs[1] = texCoord2;
	uvs[2] = texCoord3;

	calculateNormal();
}

Triangle::Triangle(Vector3 point1, Vector3 point2, Vector3 point3,
				   Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3)
{
	points = new Vector3[3];
	normals = new Vector3[3];
	uvs = new Vector2[3];

	points[0] = point1;
	points[1] = point2;
	points[2] = point3;

	uvs[0] = texCoord1;
	uvs[1] = texCoord2;
	uvs[2] = texCoord3;

	calculateNormal();
}

Triangle::Triangle(Vector4 point1, Vector4 point2, Vector4 point3,
				   Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3,
				   Vector3 normal1, Vector3 normal2, Vector3 normal3)
{
	points = new Vector3[3];
	normals = new Vector3[3];
	uvs = new Vector2[3];

	points[0] = (Vector3)point1;
	points[1] = (Vector3)point2;
	points[2] = (Vector3)point3;

	uvs[0] = texCoord1;
	uvs[1] = texCoord2;
	uvs[2] = texCoord3;

	normals[0] = normal1;
	normals[1] = normal2;
	normals[2] = normal3;
}

Triangle::Triangle(Vector3 point1, Vector3 point2, Vector3 point3,
				   Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3,
				   Vector3 normal1, Vector3 normal2, Vector3 normal3)
{
	points = new Vector3[3];
	normals = new Vector3[3];
	uvs = new Vector2[3];

	points[0] = point1;
	points[1] = point2;
	points[2] = point3;

	uvs[0] = texCoord1;
	uvs[1] = texCoord2;
	uvs[2] = texCoord3;

	normals[0] = normal1;
	normals[1] = normal2;
	normals[2] = normal3;
}

void Triangle::calculateNormal()
{
	Vector3 point1 = (Vector3)points[0];
	Vector3 point2 = (Vector3)points[1];
	Vector3 point3 = (Vector3)points[2];

	//Ray from point 1 to point 2
	Vector3 ray1(point2.getX() - point1.getX(),
				 point2.getY() - point1.getY(),
				 point2.getZ() - point1.getZ());
	//Ray from point 1 to point 3
	Vector3 ray2(point3.getX() - point1.getX(),
				 point3.getY() - point1.getY(),
				 point3.getZ() - point1.getZ());

	//Get Normal (cross of ray1 and ray2
	Vector3 normal = Vector3::Cross(ray1, ray2);

	//Normalize normal
	normal = Vector3::Normalize(normal);

	//Add one normal for every point
	normals[0] = normal;
	normals[1] = normal;
	normals[2] = normal;
}

Triangle::~Triangle(void)
{
}
