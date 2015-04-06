#define BRICKWARE_GRAPHICS_EXPORTS

#include "Mesh.h"
#include "RenderingManager.h"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;
using namespace Math;

Mesh::Mesh(char* modelFileName)
{
	loadOBJ(modelFileName);

#ifdef D3D_SUPPORT
	bufferD3D();
#else
	bufferGL();
#endif
}

//Accessors
float* Mesh::getPoints(){ return points; }
Bounds Mesh::getBounds(){ return bounds; }

int Mesh::getPointSize(){ return pointSize; }
int Mesh::getNormalSize(){ return normalSize; }
int Mesh::getIndexSize(){ return indexSize; }
int Mesh::getNumberOfVerts(){ return numberOfVerts; }
int Mesh::getTexCoordSize(){ return texCoordSize; }

#ifdef GL_SUPPORT
GLuint Mesh::getVBO(){ return vbo; }
GLuint Mesh::getIBO(){ return ibo; }
#endif

#ifdef D3D_SUPPORT
ID3D11Buffer* Mesh::getPositionBuffer(){ return positionBuffer; }
ID3D11Buffer* Mesh::getNormalBuffer(){ return normalBuffer; }
ID3D11Buffer* Mesh::getTexCoordBuffer(){ return texCoordBuffer; }

ID3D11Buffer* Mesh::getIndexBuffer(){ return indexBuffer; }
#endif

//Private functions

//Load a shape from an OBJ
void Mesh::loadOBJ(char* fileName)
{
	ifstream objFile(fileName, ios::in);

	if (!objFile.is_open())
	{
		cout << "Error opening file: " << fileName << endl;
		return;
	}

	//Vectors of vectors to store data before we construct it
	vector<Vector3> modelVerticies;
	vector<Vector2> modelTextureCoords;
	vector<Vector3> modelNormals;
	vector<vector<Vector3>> faces;

	string line;

	while (getline(objFile, line))
	{
		//copy to a cstring so we can tokenize
		char* cLine = new char[line.size() + 1];
		strcpy(cLine, line.c_str());

		//Check the first line of the file to determine how to read it
		switch (line[0])
		{
		case '#':
			//This is a comment in the obj, do nothing
			break;
		case 'o':
			//TODO: save the object name somewhere
			break;
		case 'v':

			//Check after the v for further instructions; normals, tex coords etc.
			switch (line[1])
			{
			case ' ':
			{
				//Split string along spaces to get vertex info
				vector<char*> tokens = StringUtils::stringSplit(cLine + 2, " ");

				float* rawVec = new float[3];

				for (unsigned int i = 0; i < tokens.size(); i++)
					rawVec[i] = (float)atof(tokens[i]);

				float x = rawVec[0];
				float y = rawVec[1];
				float z = rawVec[2];

				Vector3 vert(x, y, z);
				modelVerticies.push_back(vert);

				delete[] rawVec;

				break;
			}
			case 't':
			{
				//Split string along spaces to get tex coord info
				vector<char*> tokens = StringUtils::stringSplit(cLine + 3, " ");

				float* rawTexCoord = new float[2];

				for (unsigned int i = 0; i < tokens.size(); i++)
					rawTexCoord[i] = (float)atof(tokens[i]);

				float x = rawTexCoord[0];
				float y = rawTexCoord[1];

				Vector2 texCoord(x, y);
				modelTextureCoords.push_back(texCoord);

				delete[] rawTexCoord;

				break;
			}
			case 'n':
			{
				//Split string along spaces to get normals
				vector<char*> tokens = StringUtils::stringSplit(cLine + 3, " ");

				float* rawNormal = new float[3];

				for (unsigned int i = 0; i < tokens.size(); i++)
					rawNormal[i] = (float)atof(tokens[i]);

				float x = rawNormal[0];
				float y = rawNormal[1];
				float z = rawNormal[2];

				Vector3 normal(x, y, z);
				modelNormals.push_back(normal);

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
			vector<char*> rawFaces = StringUtils::stringSplit(cLine + 2, " ");

			vector<Vector3> face;

			for (unsigned int i = 0; i < rawFaces.size(); i++)
			{
				//Parse even further, now to get face info
				vector<char*> tokens = StringUtils::stringSplit(rawFaces[i], "/");

				float* faceInfo = new float[3];

				for (unsigned int i = 0; i < tokens.size(); i++)
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

	//For every face we have 3 vertices and 3 components for each vertex
	unsigned int numOfFaces = faces.size();

	numberOfVerts = numOfFaces * 3;

	pointSize = numOfFaces * 9 * sizeof(float);
	normalSize = numOfFaces * 9 * sizeof(float);
	texCoordSize = numOfFaces * 6 * sizeof(float);

	indexSize = numberOfVerts * sizeof(unsigned short);

	points = new float[numOfFaces * 9];
	normals = new float[numOfFaces * 9];
	texCoords = new float[numOfFaces * 6];

	indices = new unsigned short[numOfFaces * 9];

	//We're going to use these to populate the bounds of the mesh
	float min = std::numeric_limits<float>::min();
	float max = std::numeric_limits<float>::max();

	Vector3 minPoint(max, max, max);
	Vector3 maxPoint(min, min, min);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//We need to determine which order the vertices, normals and texCoords will be added into the arrays
		vector<int> faceVertexIndices;
		vector<int> faceNormalIndices;
		vector<int> faceTextureCoordIndices;

		vector<Vector3> face = faces[i];

		for (unsigned int j = 0; j < face.size(); j++)
		{
			Vector3 faceInfo = face[j];

			//Get info about the face
			faceVertexIndices.push_back((int)faceInfo.getX() - 1);
			if (faceInfo.getY() > 0)
				faceTextureCoordIndices.push_back((int)faceInfo.getY() - 1);
			else
				faceTextureCoordIndices.push_back(0);
			faceNormalIndices.push_back((int)faceInfo.getZ() - 1);
		}

		//Add data to the arrays
		for (int j = 0; j < 3; j++)
		{
			Vector3 vertex;
			Vector3 normal;
			Vector2 texCoord;

			vertex = modelVerticies[faceVertexIndices[j]];

			//We may not have normals or texCoords
			if (faceNormalIndices.size() > 0)
				normal = modelNormals[faceNormalIndices[j]];
			if (faceTextureCoordIndices.size() > 0)
				texCoord = modelTextureCoords[faceTextureCoordIndices[j]];

			//put data into the arrays
			memcpy(points + (i * 9) + (j * 3), vertex.getAsArray(), 3 * sizeof(float));
			memcpy(normals + (i * 9) + (j * 3), normal.getAsArray(), 3 * sizeof(float));
			memcpy(texCoords + (i * 6) + (j * 2), texCoord.getAsArray(), 2 * sizeof(float));

			indices[(i * 9) + (j * 3)] = (i * 9) + (j * 3);
			indices[(i * 9) + (j * 3) + 1] = (i * 9) + (j * 3) + 1;
			indices[(i * 9) + (j * 3) + 2] = (i * 9) + (j * 3) + 2;

			//While we're at it also compare the components of these vertices to the current bounds of the mes
			float testX = vertex[0];
			float testY = vertex[1];
			float testZ = vertex[2];

			if (testX < minPoint.getX())
				minPoint.setX(testX);
			if (testX > maxPoint.getX())
				maxPoint.setX(testX);

			if (testY < minPoint.getY())
				minPoint.setY(testY);
			if (testY > maxPoint.getY())
				maxPoint.setY(testY);

			if (testZ < minPoint.getZ())
				minPoint.setZ(testZ);
			if (testZ > maxPoint.getZ())
				maxPoint.setZ(testZ);
		}
	}

	//Finish calculating bounds
	Vector3 size = maxPoint - minPoint;

	bounds = Bounds(Vector3(), size.getX(), size.getY(), size.getZ());

	objFile.close();
}

#ifdef GL_SUPPORT
void Mesh::bufferGL()
{
	//Setup the VBO
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, pointSize + normalSize + texCoordSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pointSize, points);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize, normalSize, normals);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize + normalSize, texCoordSize, texCoords);

	//Setup the IBO
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indices, GL_STATIC_DRAW);
}
#endif

#ifdef D3D_SUPPORT
void Mesh::bufferD3D()
{
	//Use 3 buffers instead of one interleaved buffer
	D3D11_BUFFER_DESC positionBufferDesc;
	positionBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	positionBufferDesc.ByteWidth = pointSize;
	positionBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	positionBufferDesc.CPUAccessFlags = 0;
	positionBufferDesc.MiscFlags = 0;
	positionBufferDesc.StructureByteStride = 0;

	D3D11_BUFFER_DESC normalBufferDesc;
	normalBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	normalBufferDesc.ByteWidth = normalSize;
	normalBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	normalBufferDesc.CPUAccessFlags = 0;
	normalBufferDesc.MiscFlags = 0;
	normalBufferDesc.StructureByteStride = 0;

	D3D11_BUFFER_DESC texCoordBufferDesc;
	texCoordBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texCoordBufferDesc.ByteWidth = texCoordSize;
	texCoordBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	texCoordBufferDesc.CPUAccessFlags = 0;
	texCoordBufferDesc.MiscFlags = 0;
	texCoordBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA positionData;
	positionData.pSysMem = points;

	D3D11_SUBRESOURCE_DATA normalData;
	normalData.pSysMem = normals;

	D3D11_SUBRESOURCE_DATA texCoordData;
	texCoordData.pSysMem = texCoords;

	//Buffer
	HR(RenderingManager::device->CreateBuffer(&positionBufferDesc, &positionData, &positionBuffer));
	HR(RenderingManager::device->CreateBuffer(&normalBufferDesc, &normalData, &normalBuffer));
	HR(RenderingManager::device->CreateBuffer(&texCoordBufferDesc, &texCoordData, &texCoordBuffer));

	//Need to have ints rather than shorts
	unsigned int* d3dIndices = new unsigned int[numberOfVerts];

	for (int i = 0; i < numberOfVerts; i++)
		d3dIndices[i] = (unsigned int)indices[i];

	//Create index buffer description
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(UINT) * numberOfVerts;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = d3dIndices;

	//Buffer
	HR(RenderingManager::device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer));
}
#endif

//Destructor
Mesh::~Mesh()
{

}