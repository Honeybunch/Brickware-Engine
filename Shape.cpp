#include "Shape.h"

//Constructors

//Tesselate a primitive
Shape::Shape(PrimitiveType type, int division1, int division2, float radius)
{
	switch (type)
    {
        case CUBE: 
			makeCube (division1); 
			break;
        case CYLINDER: 
			makeCylinder (radius, division1, division2); 
			break;
        case CONE: 
			makeCone (radius, division1, division2); 
			break;
        case SPHERE: 
			makeSphere(radius, division1, division2);
			break;
    }

	pointElements = new float[triangles.size() * 9];
	normalElements = new float[triangles.size() * 9];
	indicies = new GLushort[triangles.size() * 9];
	texCoords = new float[triangles.size() * 6];
}

//Create a shape based on a file 
Shape::Shape(char* fileName)
{
	//Determine filetype
	if(strstr(fileName, ".obj"))	
		loadOBJ(fileName);
	else
		cout << "Can't load " << fileName << ". Invalid Type; must be .obj!" << endl;

	pointElements = new float[triangles.size() * 12];
	normalElements = new float[triangles.size() * 9];
	indicies = new GLushort[triangles.size() * 12];
	texCoords = new float[triangles.size() * 6];
}

//Destructor
Shape::~Shape()
{

}

/*
	Accessors
*/
float* Shape::getPoints()
{
	for(unsigned int i=0; i < triangles.size(); i++)
	{
		Vector3 point1 = triangles[i].points[0];
		Vector3 point2 = triangles[i].points[1];
		Vector3 point3 = triangles[i].points[2];

		int index = i * 9;

		pointElements[index] = point1.getX();
		pointElements[index+1] = point1.getY();
		pointElements[index+2] = point1.getZ();

		pointElements[index+3] = point2.getX();
		pointElements[index+4] = point2.getY();
		pointElements[index+5] = point2.getZ();

		pointElements[index+6] = point3.getX();
		pointElements[index+7] = point3.getY();
		pointElements[index+8] = point3.getZ();
	}

	return pointElements;
}

vector<Triangle> Shape::getTriangles()
{
	return triangles;
}

float* Shape::getNormals()
{
	for(unsigned int i=0; i < triangles.size(); i++)
	{
		Vector3 normal1 = triangles[i].normals[0];
		Vector3 normal2 = triangles[i].normals[1];
		Vector3 normal3 = triangles[i].normals[2];

		int index = i * 9;

		normalElements[index] = normal1.getX();
		normalElements[index+1] = normal1.getY();
		normalElements[index+2] = normal1.getZ();
									 
		normalElements[index+3] = normal2.getX();
		normalElements[index+4] = normal2.getY();
		normalElements[index+5] = normal2.getZ();

		normalElements[index+6] = normal3.getX();
		normalElements[index+7] = normal3.getY();
		normalElements[index+8] = normal3.getZ();
	}

	return normalElements;
}

GLushort* Shape::getIndicies()
{
	for(unsigned int i=0; i < triangles.size() * 9; i++)
	{
		indicies[i] = i;
	}

	return indicies;
}

float* Shape::getTexCoords()
{
	for(unsigned int i=0; i < triangles.size(); i++)
	{
		//Pass every uv element into the array, OpenGL should sort it out
		Vector2 uv1 = triangles[i].uvs[0];
		Vector2 uv2 = triangles[i].uvs[1];
		Vector2 uv3 = triangles[i].uvs[2];

		int index = i * 6;

		texCoords[index] = uv1.getX();
		texCoords[index+1] = uv1.getY();

		texCoords[index+2] = uv2.getX();
		texCoords[index+3] = uv2.getY();

		texCoords[index+4] = uv3.getX();
		texCoords[index+5] = uv3.getY();
	}

	return texCoords;
}


int Shape::getPointCount()
{
	return triangles.size() * 9;
}

int Shape::getNormalCount()
{
	return triangles.size() * 9;
}

int Shape::getTexCoordSize()
{
	//Every triangle has 6 uv elements
	return triangles.size() * 6;
}

/*
	Private Methods
*/

//Make divisions across a value
vector<float> Shape::makeLoopDivisions(float start, float length, int divisions)
{
	vector<float> v;

	float step = length/divisions;

	for(int i = 0; i < divisions + 1; i++)
	{
		float value = start + (i * step);

		v.push_back(value);
	}

	return v;
}

//Adds a triangle to the current shape
void Shape::addTriangle (Vector3 point1, Vector3 point2, Vector3 point3,
						 Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3)
{
	Triangle triangle(point1, point2, point3,
					  texCoord1, texCoord2, texCoord3);

	triangles.push_back(triangle);
}

void Shape::addTriangle(Vector3 point1, Vector3 point2, Vector3 point3,
					 Vector2 texCoord1, Vector2 texCoord2, Vector2 texCoord3,
					 Vector3 normal1, Vector3 normal2, Vector3 normal3)
{
	Triangle triangle(point1, point2, point3,
					  texCoord1, texCoord2, texCoord3,
					  normal1, normal2, normal3);

	triangles.push_back(triangle);
}



//Add a bunch of triangles to make a cube
void Shape::makeCube(int subdivisions)
{
	float width = 1.0f;
	float halfWidth = width/2.0f;

	vector<float> xSet = makeLoopDivisions(-halfWidth, width, subdivisions);
	vector<float> ySet = makeLoopDivisions(-halfWidth, width, subdivisions);
	vector<float> zSet = makeLoopDivisions(-halfWidth, width, subdivisions);

	vector<float> uSet = makeLoopDivisions(0.0f, 1.0f, subdivisions);
	vector<float> vSet = makeLoopDivisions(0.0f, 1.0f, subdivisions);
	
	unsigned int maxXIndex = (unsigned int)(xSet.size() -1);
	unsigned int maxYIndex = (unsigned int)(ySet.size() -1);
	unsigned int maxZIndex = (unsigned int)(zSet.size() -1);

	for(unsigned int i = 0; i < xSet.size() - 1; i++)
	{
		float x1 = (float)xSet[i];
		float x2 = (float)xSet[i+1];
		float minX = (float)xSet[0];
		float maxX = (float)xSet[maxXIndex];

		//U for front back top bottom
		float u1fbtb = (float)uSet[i];
		float u2fbtb = (float)uSet[i+1];

		for(unsigned int j = 0; j < ySet.size() - 1; j++)
		{
			float y1 = (float)ySet[j];
			float y2 = (float)ySet[j + 1];
			float minY = (float)ySet[0];
			float maxY = (float)ySet[maxYIndex];

			//V for front back left right
			float v1fblr = (float)vSet[j];
			float v2fblr = (float)vSet[j+1];

			for(unsigned int k = 0; k < zSet.size() - 1; k++)
			{
				float z1 = (float)zSet[k];
				float z2 = (float)zSet[k+1];
				float minZ = (float)zSet[0];
				float maxZ = (float)zSet[maxZIndex];

				//U for left right
				float u1lr = (float)uSet[k];
				float u2lr = (float)uSet[k+1];

				//V for top bottom
				float v1tb = (float)vSet[k];
				float v2tb = (float)vSet[k+1];

				//Front face
				addTriangle(Vector3(x1, y1,	maxZ), 
							Vector3(x2,	y1,	maxZ),
     						Vector3(x2,	y2,	maxZ),
							Vector2(u1fbtb, v1fblr), 
							Vector2(u2fbtb, v1fblr),
							Vector2(u2fbtb, v2fblr));
				addTriangle(Vector3(x1,	y1,	maxZ), 
							Vector3(x2,	y2,	maxZ),
							Vector3(x1,	y2,	maxZ),
							Vector2(u1fbtb, v1fblr), 
							Vector2(u2fbtb, v2fblr),
							Vector2(u1fbtb, v2fblr));

				//Back Face
				addTriangle(Vector3(x1, y1,	minZ), 
							Vector3(x2,	y2,	minZ),
							Vector3(x2,	y1,	minZ),
							Vector2(u1fbtb, v1fblr), 
							Vector2(u2fbtb, v2fblr),
							Vector2(u2fbtb, v1fblr));
				addTriangle(Vector3(x1,	y1,	minZ), 
							Vector3(x1,	y2,	minZ),
							Vector3(x2,	y2,	minZ),
							Vector2(u1fbtb, v1fblr), 
							Vector2(u1fbtb, v2fblr),
							Vector2(u2fbtb, v2fblr));

				//Left Face
				addTriangle(Vector3(minX, y1, z2), 
							Vector3(minX, y2, z1),
							Vector3(minX, y1, z1),
							Vector2(u2lr, v1fblr), 
							Vector2(u1lr, v2fblr),
							Vector2(u1lr, v1fblr));
				addTriangle(Vector3(minX, y1, z2), 
							Vector3(minX, y2, z2),
							Vector3(minX, y2, z1),
							Vector2(u2lr, v1fblr), 
							Vector2(u2lr, v2fblr),
							Vector2(u1lr, v2fblr));

				//Right Face
				addTriangle(Vector3(maxX, y1, z2), 
							Vector3(maxX, y1, z1),
							Vector3(maxX, y2, z1),
							Vector2(u2lr, v1fblr), 
							Vector2(u1lr, v1fblr),
							Vector2(u1lr, v2fblr));
				addTriangle(Vector3(maxX, y1, z2), 
							Vector3(maxX, y2, z1),
							Vector3(maxX, y2, z2),
							Vector2(u2lr, v1fblr), 
							Vector2(u1lr, v2fblr),
							Vector2(u2lr, v2fblr));
			
				//Top Face
				addTriangle(Vector3(x1,	maxY, z2), 
							Vector3(x2,	maxY, z2),
							Vector3(x2,	maxY, z1),
							Vector2(u1fbtb, v2tb), 
							Vector2(u2fbtb, v2tb),
							Vector2(u2fbtb, v1tb));
				addTriangle(Vector3(x1,	maxY, z2), 
							Vector3(x2,	maxY, z1),
							Vector3(x1,	maxY, z1),
							Vector2(u1fbtb, v2tb), 
							Vector2(u2fbtb, v1tb),
							Vector2(u1fbtb, v1tb));
			
				//Bottom Face
				addTriangle(Vector3(x1,	minY, z2), 
							Vector3(x2,	minY, z1),
							Vector3(x2,	minY, z2),
							Vector2(u1fbtb, v2tb), 
							Vector2(u2fbtb, v1tb),
							Vector2(u2fbtb, v2tb));
				addTriangle(Vector3(x1,	minY, z2), 
							Vector3(x1,	minY, z1), 
							Vector3(x2, minY, z1),
							Vector2(u1fbtb, v2tb), 
							Vector2(u1fbtb, v1tb),
							Vector2(u2fbtb, v1tb));
			}
		}
	}
}

//Add a bunch of triangles to make a cylinder
void Shape::makeCylinder (float radius, int radialDivisions, int heightDivisions)
{
	float height = 1.0f;
	float halfHeight = height/2.0f;

	vector<float> xRadialSet = makeLoopDivisions(0, (float)(2*M_PI), radialDivisions);
	vector<float> zRadialSet = makeLoopDivisions(0, (float)(2*M_PI), radialDivisions);
	vector<float> yHeightSet = makeLoopDivisions(-halfHeight, height, heightDivisions);
	
	vector<float> uSet = makeLoopDivisions(0, 1, radialDivisions);
	vector<float> vSet = makeLoopDivisions(0, 1, heightDivisions);

	for(unsigned int i = 0; i < xRadialSet.size() - 1; i++)
	{
		float x1 = cos(xRadialSet[i]) * radius;
		float x2 = cos(xRadialSet[i + 1]) * radius;
		
		float z1 = sin(zRadialSet[i]) * radius;
		float z2 = sin(zRadialSet[i + 1]) * radius;

		//Us and Vs for top and bottom
		float u1tb = .5f;
		float u2tb = .5f + x1/2;
		float u3tb = .5f + x2/2;

		float v1tb = .5f;
		float v2tb = .5f + z1/2;
		float v3tb = .5f + z2/2;

		//Us for the sides
		float u1s = uSet[i];
		float u2s = uSet[i+1];

		//Create top and bottom circles
		addTriangle(Vector3(0, halfHeight, 0),
					Vector3(x2, halfHeight, z2),
					Vector3(x1, halfHeight, z1),
					Vector2(u1tb, v1tb),
					Vector2(u3tb, v3tb),
					Vector2(u2tb, v2tb));
		addTriangle(Vector3(0, -halfHeight, 0),
					Vector3(x1, -halfHeight, z1),
					Vector3(x2, -halfHeight, z2),
					Vector2(u1tb, v1tb),
					Vector2(u2tb, v2tb),
					Vector2(u3tb, v3tb));

		//Make the rest of the faces
		for(unsigned int j = 0; j < yHeightSet.size() - 1; j++)
		{
			float y1 = yHeightSet[j];
			float y2 = yHeightSet[j + 1];
			float minY = yHeightSet[0];
			float maxY = yHeightSet[yHeightSet.size() - 1];

			//Vs for the sides
			float v1s = vSet[j];
			float v2s = vSet[j+1];
			
			addTriangle(Vector3(x1, y1, z1), 
						Vector3(x2, y2, z2),
						Vector3(x2, y1, z2),
						Vector2(u1s, v1s),
						Vector2(u2s, v2s),
						Vector2(u2s, v1s));
			addTriangle(Vector3(x1, y1, z1),
						Vector3(x1, y2, z1),
						Vector3(x2, y2, z2),
						Vector2(u1s, v1s),
						Vector2(u1s, v2s),
						Vector2(u2s, v2s));
		}
		
	}

}

//Add a bunch of triangles to make a cone
void Shape::makeCone (float radius, int radialDivisions, int heightDivisions)
{
	float height = 1.0f;
	float halfHeight = height/2.0f;

	vector<float> xRadialSet = makeLoopDivisions(0, (float)(2*M_PI), radialDivisions);
	vector<float> zRadialSet = makeLoopDivisions(0, (float)(2*M_PI), radialDivisions);
	vector<float> yHeightSet = makeLoopDivisions(-halfHeight, height, heightDivisions);
	
	vector<float> radialSet = makeLoopDivisions(radius, -radius, heightDivisions);

	vector<float> uSet = makeLoopDivisions(0, 1, radialDivisions);
	vector<float> vSet = makeLoopDivisions(0, 1, heightDivisions);

	for(unsigned int i = 0; i < xRadialSet.size() - 1; i++)
	{
		float bottomX1 = cos(xRadialSet[i]) * radius;
		float bottomX2 = cos(xRadialSet[i + 1]) * radius;
		float bottomZ1 = sin(zRadialSet[i]) * radius;
		float bottomZ2 = sin(zRadialSet[i + 1]) * radius;

		float minY = yHeightSet[0];
		float maxY = yHeightSet[yHeightSet.size() - 1];
		float notQuiteMaxY = yHeightSet[yHeightSet.size() - 2];

		//Us and Vs for bottom
		float u1b = .5f;
		float u2b = .5f + bottomX1/2;
		float u3b = .5f + bottomX2/2;

		float v1b = .5f;
		float v2b = .5f + bottomZ1/2;
		float v3b = .5f + bottomZ2/2;

		//Us for sides
		float u1s = uSet[i];
		float u2s = uSet[i+1];

		//Create bottom
		addTriangle(Vector3(0, -halfHeight, 0),
					Vector3(bottomX1, -halfHeight, bottomZ1),
					Vector3(bottomX2, -halfHeight, bottomZ2),
					Vector2(u1b, u1b),
					Vector2(u2b, v2b),
					Vector2(u3b, v3b));

		//Make the rest of the faces
		for(unsigned int j = 0; j < yHeightSet.size() - 1; j++)
		{
			float sideX1 = cos(xRadialSet[i]) * radialSet[j];
			float sideX2 = cos(xRadialSet[i + 1]) * radialSet[j];
			
			float sideZ1 = sin(zRadialSet[i]) * radialSet[j];
			float sideZ2 = sin(zRadialSet[i + 1]) * radialSet[j];

			//Size 2 is smaller
			float sideX1_size2 = cos(xRadialSet[i]) * radialSet[j+1];
			float sideX2_size2 = cos(xRadialSet[i + 1]) * radialSet[j+1];
			
			float sideZ1_size2 = sin(zRadialSet[i]) * radialSet[j+1];
			float sideZ2_size2 = sin(zRadialSet[i + 1]) * radialSet[j+1];

			float y1 = yHeightSet[j];
			float y2 = yHeightSet[j + 1];

			//Vs for sides
			float v1s = vSet[j];
			float v2s = vSet[j+1];

			addTriangle(Vector3(sideX1, y1, sideZ1), 
						Vector3(sideX2_size2, y2, sideZ2_size2),
						Vector3(sideX2, y1, sideZ2),
						Vector2(u1s, v1s),
						Vector2(u2s, v2s),
						Vector2(u2s, v1s));
			addTriangle(Vector3(sideX1, y1, sideZ1),
						Vector3(sideX1_size2, y2, sideZ1_size2),
						Vector3(sideX2_size2, y2, sideZ2_size2),
						Vector2(u1s, v1s),
						Vector2(u1s, v2s),
						Vector2(u2s, v2s));		
		}
	}
}

//Add a bunch of triangles to make a sphere
void Shape::makeSphere (float radius, int slices, int stacks)
{
	float height = 1.0f;
	float halfHeight = height/2.0f;

	vector<float> phiCoordinates = makeLoopDivisions((float)(-M_PI/2), (float)M_PI, stacks);
	vector<float> thetaCoordiantes = makeLoopDivisions(0 , (float)(2*M_PI), slices * 2);

	vector<float> uSet = makeLoopDivisions(0.0f, 1.0f, slices * 2);
	vector<float> vSet = makeLoopDivisions(0.0f, 1.0f, stacks);

	for(unsigned int i = 0; i < phiCoordinates.size() - 1; i++)
	{
		for(unsigned int j = 0; j < thetaCoordiantes.size() - 1; j++)
		{
			float x1 = radius * cos(phiCoordinates[i]) * sin(thetaCoordiantes[j]);
			float y1 = radius * sin(phiCoordinates[i]) * sin(thetaCoordiantes[j]);
			float z1 = radius * cos(thetaCoordiantes[j]);

			float x2 = radius * cos(phiCoordinates[i]) * sin(thetaCoordiantes[j + 1]);
			float y2 = radius * sin(phiCoordinates[i]) * sin(thetaCoordiantes[j + 1]);
			float z2 = radius * cos(thetaCoordiantes[j + 1]);

			float x3 = radius * cos(phiCoordinates[i + 1]) * sin(thetaCoordiantes[j + 1]);
			float y3 = radius * sin(phiCoordinates[i + 1]) * sin(thetaCoordiantes[j + 1]);
			float z3 = radius * cos(thetaCoordiantes[j + 1]);

			float x4 = radius * cos(phiCoordinates[i + 1]) * sin(thetaCoordiantes[j]);
			float y4 = radius * sin(phiCoordinates[i + 1]) * sin(thetaCoordiantes[j]);
			float z4 = radius * cos(thetaCoordiantes[j]);

			float u1 = uSet[j];
			float u2 = uSet[j+1];

			float v1 = vSet[i];
			float v2 = vSet[i+1];

			//Make triangles
			if(thetaCoordiantes[j] <= M_PI)
			{
				addTriangle(Vector3(x1, y1, z1), 
							Vector3(x2, y2, z2),
							Vector3(x3, y3, z3),
							Vector2(u1, v1),
							Vector2(u2, v1),
							Vector2(u2, v2));
				addTriangle(Vector3(x1, y1, z1), 
							Vector3(x3, y3, z3),
							Vector3(x4, y4, z4),
							Vector2(u1, v1),
							Vector2(u2, v2),
							Vector2(u2, v1));
			}
			
			if(thetaCoordiantes[j] >= M_PI)
			{
				addTriangle(Vector3(x1, y1, z1), 
							Vector3(x3, y3, z3),
							Vector3(x2, y2, z2),
							Vector2(u1, v1),
							Vector2(u2, v2),
							Vector2(u2, v1));
				addTriangle(Vector3(x1, y1, z1), 
							Vector3(x4, y4, z4),
							Vector3(x3, y3, z3),
							Vector2(u1, v1),
							Vector2(u1, v2),
							Vector2(u2, v2));
			}
		}
	}
}

//Load a shape from an OBJ
void Shape::loadOBJ(char* fileName)
{
	ifstream objFile(fileName, ios::in);
	
	if(!objFile.is_open())
	{
		cout << "Error opening file: " << fileName << endl;
		return;
	}

	string line;

	//Vectors of vectors to store data before we construct it
	vector<Vector3> verticies;
	vector<Vector2> textureCoords;
	vector<Vector3> normals;
	vector<vector<Vector3>> faces;

	while(getline(objFile, line))
	{
		//copy to a cstring so we can tokenize
		char* cLine = new char[line.size() + 1];
		strcpy(cLine, line.c_str());

		//Check the first line of the file to determine how to read it
		switch(line[0])
		{
		case '#':
			//This is a comment in the obj, do nothing
			break;
		case 'o':
			//TODO: save the object name somewhere
			break;
		case 'v':

			//Check after the v for further instructions; normals, tex coords etc.
			switch(line[1])
			{
			case ' ':
				{
				//Split string along spaces to get vertex info
				vector<char*> tokens = Utils::stringSplit(cLine+2, " ");
				
				float* rawVec = new float[3];
				
				for(unsigned int i=0; i < tokens.size(); i++)
					rawVec[i] = (float)atof(tokens[i]);
				
				float x = rawVec[0];
				float y = rawVec[1];
				float z = rawVec[2];

				Vector3 vert(x,y,z);
				verticies.push_back(vert);

				delete[] rawVec;

				break;
				}
			case 't':
				{
				//Split string along spaces to get tex coord info
				vector<char*> tokens = Utils::stringSplit(cLine+3, " ");
				
				float* rawTexCoord = new float[2];
				
				for(unsigned int i=0; i < tokens.size(); i++)
					rawTexCoord[i] = (float)atof(tokens[i]);

				float x = rawTexCoord[0];
				float y = rawTexCoord[1];

				Vector2 texCoord(x,y);
				textureCoords.push_back(texCoord);

				delete[] rawTexCoord;

				break;
				}
			case 'n':
				{
				//Split string along spaces to get normals
				vector<char*> tokens = Utils::stringSplit(cLine+3, " ");
				
				float* rawNormal = new float[3];
				
				for(unsigned int i=0; i < tokens.size(); i++)
					rawNormal[i] = (float)atof(tokens[i]);

				float x = rawNormal[0];
				float y = rawNormal[1];
				float z = rawNormal[2];

				Vector3 normal(x,y,z);
				normals.push_back(normal);

				delete[] rawNormal;

				break;
				}
			}
			break;
		case 's':
			//This handles whether or not smooth shading is on; do nothing for now
			break;
		case 'f':
			{
				//Split string along spaces to get faces
				//use a string splitting method so we can nest with strtok
				vector<char*> rawFaces = Utils::stringSplit(cLine + 2, " ");
				
				vector<Vector3> face;

				for(unsigned int i = 0; i < rawFaces.size(); i++)
				{
					//Parse even further, now to get face info
					vector<char*> tokens = Utils::stringSplit(rawFaces[i], "/");
				
					float* faceInfo = new float[3];
				
					for(unsigned int i=0; i < tokens.size(); i++)
						faceInfo[i] = (float)atof(tokens[i]);

					float vert = faceInfo[0];
					float texCoord = faceInfo[1];
					float normal = faceInfo[2];

					Vector3 faceVert(vert, texCoord, normal);
					face.push_back(faceVert);

					delete[] faceInfo;
				}

				faces.push_back(face);

				break;
				}
			break;
		}
	}

	for(unsigned int i = 0;i < faces.size(); i++)
	{
		vector<Vector3> face = faces[i];

		vector<int> verts;
		vector<int> texCoords;
		vector<int> norms;

		for(unsigned int j = 0; j < face.size(); j++)
		{
			Vector3 faceInfo = face[j];

			//Get info about the face
			verts.push_back((int)faceInfo.getX() - 1);
			if(faceInfo.getY() > 0)
				texCoords.push_back((int)faceInfo.getY() - 1);
			else
				texCoords.push_back(0);
			norms.push_back((int)faceInfo.getZ() - 1);
		}

		//Create face
		int vertSize = verts.size();

		if(vertSize > 3)
		{
			cout << "Only supporting tris right now" << endl;
			break;
		}

		Vector3 vert1 = verticies[verts[0]];
		Vector3 vert2 = verticies[verts[1]];
		Vector3 vert3 = verticies[verts[2]];

		//We may not have texCoords
		Vector2 texCoord1;
		Vector2 texCoord2;
		Vector2 texCoord3;

		if(textureCoords.size() > 0)
		{
			texCoord1 = textureCoords[texCoords[0]];
			texCoord2 = textureCoords[texCoords[1]];
			texCoord3 = textureCoords[texCoords[2]];
		}

		Vector3 normal1 = normals[norms[0]];
		Vector3 normal2 = normals[norms[1]];
		Vector3 normal3 = normals[norms[2]];

		addTriangle(vert1, vert2, vert3,
					texCoord1, texCoord2, texCoord3,
					normal1, normal2, normal3);
	}

	objFile.close();
}