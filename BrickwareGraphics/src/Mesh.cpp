#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics\Mesh.hpp"
#include "BrickwareGraphics\RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;
using namespace Math;

Mesh::Mesh(char* modelFileName)
{
	points = NULL;
	normals = NULL;
	texCoords = NULL;
	indices = NULL;

	loadOBJ(modelFileName);

	setBufferHint(BufferHint::STATIC_DRAW);

	bufferChanges();
}

vector<Vector3> Mesh::getVerticies(){ return modelVerts; }
vector<Vector3> Mesh::getNormals(){ return modelNormals; }
vector<Vector2> Mesh::getTexCoords(){	return modelTexCoords; }
vector<Vector3> Mesh::getIndices(){ return modelIndices; }

int Mesh::getPointSize(){ return pointSize; }
int Mesh::getNormalSize(){ return normalSize; }
int Mesh::getTexCoordSize(){ return texCoordSize; }

int Mesh::getIndexSize(){ return indexSize; }
int Mesh::getNumberOfVerts(){ return numberOfVerts; }

Bounds Mesh::getBounds(){ return bounds; }
void Mesh::setBounds(Bounds newBounds){ bounds = newBounds; }

void Mesh::setBufferHint(BufferHint hint)
{
#ifdef GL_SUPPORT
	if (hint == BufferHint::STATIC_DRAW)
		glBufferHint = GL_STATIC_DRAW;
	else if (hint == BufferHint::DYNAMIC_DRAW)
		glBufferHint = GL_DYNAMIC_DRAW;
#endif
}

void Mesh::setVertices(vector<Vector3> newVerts){ modelVerts = newVerts; }
void Mesh::setNormals(vector<Vector3> newNormals){ modelNormals = newNormals; }
void Mesh::setTexCoords(vector<Vector2> newTexCoords){ modelTexCoords = newTexCoords; }
void Mesh::setIndices(vector<Vector3> newIndices){ modelIndices = newIndices; }

void Mesh::bufferChanges()
{
	//We need to take the data loaded from the model and order it the way that
	//OpenGL and DirectX want

	const unsigned int indexCount = modelIndices.size();
	indexSize = indexCount * sizeof(unsigned short);

	//Need to clear out data if it exits
	if (points)
		delete[] points;
	if (normals)
		delete[] normals;
	if (texCoords)
		delete[] texCoords;
	if (indices)
		delete[] indices;

	//Then we need to setup the arrays to hold the data we want to buffer
	points = new float[indexCount * 3];
	normals = new float[indexCount * 3];
	texCoords = new float[indexCount * 2];

	indices = new unsigned short[indexCount];

	//Setup sizes for data
	pointSize = indexCount * 3 * sizeof(float);
	normalSize = pointSize;
	texCoordSize = indexCount * 2 * sizeof(float);

	int pointCounter = 0;
	int normalCounter = 0;
	int texCoordCounter = 0;

	numberOfVerts = indexCount;

	for (unsigned int i = 0; i < indexCount; i++)
	{
		Vector3 indexVector = modelIndices[i];

		Vector3 vert = modelVerts[(int)indexVector[0] - 1];
		Vector3 normal = modelNormals[(int)indexVector[1] - 1];
		Vector2 texCoord = modelTexCoords[(int)indexVector[2] - 1];

		//Set vertex elements
		points[pointCounter] = vert[0];
		points[pointCounter + 1] = vert[1];
		points[pointCounter + 2] = vert[2];
		pointCounter += 3;

		//Set normal elements
		normals[normalCounter] = normal[0];
		normals[normalCounter + 1] = normal[1];
		normals[normalCounter + 2] = normal[2];
		normalCounter += 3;

		//Set texture coordinate elements
		texCoords[texCoordCounter] = texCoord[0];
		texCoords[texCoordCounter + 1] = texCoord[1];
		texCoordCounter += 2;

		indices[i] = i;
	}

	//Actually have the drawing API buffer data
#ifdef GL_SUPPORT
	bufferGL();
#endif

#ifdef D3D_SUPPORT
	bufferD3D();
#endif

	//We can clear this memory for now after it's buffered
	if (points)
	{
		delete[] points;
		points = NULL;
	}
	if (normals)
	{
		delete[] normals;
		normals = NULL;
	}
	if (texCoords)
	{
		delete[] texCoords;
		texCoords = NULL;
	}
	if (indices)
	{
		delete[] indices;
		indices = NULL;
	}
}
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
	vector<Vector3> modelIndices;

	Bounds modelBounds;
	Vector3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vector3 max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

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
				vector<string> tokens = StringUtils::stringSplit(cLine + 2, " ");

				float* rawVec = new float[3];

				for (unsigned int i = 0; i < tokens.size(); i++)
					rawVec[i] = (float)atof(tokens[i].c_str());

				float x = rawVec[0];
				float y = rawVec[1];
				float z = rawVec[2];

				Vector3 vert(x, y, z);
				modelVerticies.push_back(vert);

				float testX = vert[0];
				float testY = vert[1];
				float testZ = vert[2];

				if (testX < min.getX())
					min.setX(testX);
				if (testX > max.getX())
					max.setX(testX);

				if (testY < min.getY())
					min.setY(testY);
				if (testY > max.getY())
					max.setY(testY);

				if (testZ < min.getZ())
					min.setZ(testZ);
				if (testZ > max.getZ())
					max.setZ(testZ);

				delete[] rawVec;

				break;
			}
			case 't':
			{
				//Split string along spaces to get tex coord info
				vector<string> tokens = StringUtils::stringSplit(cLine + 3, " ");

				float* rawTexCoord = new float[2];

				for (unsigned int i = 0; i < tokens.size(); i++)
					rawTexCoord[i] = (float)atof(tokens[i].c_str());

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
				vector<string> tokens = StringUtils::stringSplit(cLine + 3, " ");

				float* rawNormal = new float[3];

				for (unsigned int i = 0; i < tokens.size(); i++)
					rawNormal[i] = (float)atof(tokens[i].c_str());

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
			vector<string> rawFaces = StringUtils::stringSplit(cLine + 2, " ");

			for (unsigned int i = 0; i < rawFaces.size(); i++)
			{
				//Parse even further, now to get face info
				vector<string> tokens = StringUtils::stringSplit(rawFaces[i].c_str(), "/");

				int* faceInfo = new int[3];

				for (unsigned int i = 0; i < tokens.size(); i++)
					faceInfo[i] = atoi(tokens[i].c_str());

				float vert = (float)faceInfo[0];
				float normal = (float)faceInfo[2];
				float texCoord = (float)faceInfo[1];

				Vector3 faceVert(vert, normal, texCoord);
				modelIndices.push_back(faceVert);

				delete[] faceInfo;
			}

			break;
		}
		break;
		}
	}

	setVertices(modelVerticies);
	setNormals(modelNormals);
	setTexCoords(modelTextureCoords);
	setIndices(modelIndices);

	//Finish creating bounds
	Vector3 size = max - min;
	modelBounds = Bounds(Vector3(), size[0], size[1], size[2]);
	setBounds(modelBounds);

	objFile.close();
}

#ifdef GL_SUPPORT
void Mesh::bufferGL()
{
	//Setup the VBO
	glDeleteBuffers(1, &vbo);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, pointSize + normalSize + texCoordSize, NULL, glBufferHint);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pointSize, points);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize, normalSize, normals);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize + normalSize, texCoordSize, texCoords);

	//Setup the IBO
	glDeleteBuffers(1, &ibo);
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

	for (unsigned int i = 0; i < numberOfVerts; i++)
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