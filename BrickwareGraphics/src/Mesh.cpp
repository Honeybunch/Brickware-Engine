#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Mesh.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;
using namespace Math;

//No point data; can be set later
Mesh::Mesh()
{
	points = nullptr;
	normals = nullptr;
	texCoords = nullptr;
	indices = nullptr;

	//Init graphics specific systems, buffer mesh and set function pointers
	init();
}

Mesh::Mesh(std::vector<Math::Vector3> verts,
		   std::vector<Math::Vector3> norms,
		   std::vector<Math::Vector2> textureCoords,
		   std::vector<Math::Vector3> inds)
{
	points = nullptr;
	normals = nullptr;
	texCoords = nullptr;
	indices = nullptr;

	modelVerts = verts;
	modelNormals = norms;
	modelTexCoords = textureCoords;
	modelIndices = inds;

	//Init graphics specific systems, buffer mesh and set function pointers
	init();
}

//Load mesh data from file
Mesh::Mesh(const char* modelFileName)
{
	points = nullptr;
	normals = nullptr;
	texCoords = nullptr;
	indices = nullptr;

	loadOBJ(modelFileName);

	//Init graphics specific systems, buffer mesh and set function pointers
	init();
}

std::vector<Vector3> Mesh::getVerticies(){ return modelVerts; }
std::vector<Vector3> Mesh::getNormals(){ return modelNormals; }
std::vector<Vector2> Mesh::getTexCoords(){	return modelTexCoords; }
std::vector<Vector3> Mesh::getIndices(){ return modelIndices; }

size_t Mesh::getPointSize(){ return pointSize; }
size_t Mesh::getNormalSize(){ return normalSize; }
size_t Mesh::getTexCoordSize(){ return texCoordSize; }

size_t Mesh::getIndexSize(){ return indexSize; }
size_t Mesh::getNumberOfVerts(){ return numberOfVerts; }

Bounds Mesh::getBounds(){ return bounds; }
void Mesh::setBounds(Bounds newBounds){ bounds = newBounds; }

void Mesh::setBufferHint(BufferHint hint)
{
	(this->*setBufferHintPtr)(hint);
}

void Mesh::setVertices(std::vector<Vector3> newVerts){ modelVerts = newVerts; }
void Mesh::setNormals(std::vector<Vector3> newNormals){ modelNormals = newNormals; }
void Mesh::setTexCoords(std::vector<Vector2> newTexCoords){ modelTexCoords = newTexCoords; }
void Mesh::setIndices(std::vector<Vector3> newIndices){ modelIndices = newIndices; }

void Mesh::bufferChanges()
{
	//We need to take the data loaded from the model and order it the way that
	//OpenGL and DirectX want

	const size_t indexCount = modelIndices.size();
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
	unsigned int floatSize = (unsigned int)sizeof(float);

	pointSize = indexCount * 3 * floatSize;
	normalSize = pointSize;
	texCoordSize = indexCount * 2 * floatSize;

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
	(this->*bufferMeshPtr)();

	//We can clear this memory for now after it's buffered
	if (points)
	{
		delete[] points;
		points = nullptr;
	}
	if (normals)
	{
		delete[] normals;
		normals = nullptr;
	}
	if (texCoords)
	{
		delete[] texCoords;
		texCoords = nullptr;
	}
	if (indices)
	{
		delete[] indices;
		indices = nullptr;
	}
}

//Private functions

void Mesh::init()
{
	//Setup function pointers based on rendering API
	RenderingAPI renderer = GraphicsSettings::Renderer;
#ifdef GL_SUPPORT
	if (renderer = RenderingAPI::OpenGL)
	{
		if (RendererInfo::GetAPIMajorVersion() >= 3)
		{
			setBufferHintPtr = &Mesh::setBufferHintGL;
			bufferMeshPtr = &Mesh::bufferGL;
			cleanupMeshPtr = &Mesh::cleanupGL;
		}
		else
		{
			std::cout << "Error loading Shader: Your card does not support OpenGL 3+" << std::endl;
		}
	}
#endif

#ifdef D3D_SUPPORT
	if (renderer = RenderingAPI::DirectX)
	{
		if (RendererInfo::GetAPIMajorVersion() == 11)
		{
			setBufferHintPtr = &Mesh::setBufferHintD3D;
			bufferMeshPtr = &Mesh::bufferD3D;
			cleanupMeshPtr = &Mesh::cleanupD3D;
		}
	}
#endif

	setBufferHint(BufferHint::STATIC_DRAW);

	bufferChanges();
}

//Load a shape from an OBJ
void Mesh::loadOBJ(const char* fileName)
{
	std::ifstream objFile(fileName, std::ios::in);

	if (!objFile.is_open())
	{
		std::cout << "Error opening file: " << fileName << std::endl;
		return;
	}

	//Vectors of vectors to store data before we construct it
	std::vector<Vector3> modelVerticies;
	std::vector<Vector2> modelTextureCoords;
	std::vector<Vector3> modelNormals;
	std::vector<Vector3> modelIndices;

	Bounds modelBounds;
	Vector3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
	Vector3 max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

	std::string line;

	while (std::getline(objFile, line))
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
				std::vector<std::string> tokens = StringUtils::stringSplit(cLine + 2, " ");

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
				std::vector<std::string> tokens = StringUtils::stringSplit(cLine + 3, " ");

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
				std::vector<std::string> tokens = StringUtils::stringSplit(cLine + 3, " ");

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
			std::vector<std::string> rawFaces = StringUtils::stringSplit(cLine + 2, " ");

			for (unsigned int i = 0; i < rawFaces.size(); i++)
			{
				//Parse even further, now to get face info
				std::vector<std::string> tokens = StringUtils::stringSplit(rawFaces[i].c_str(), "/");

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

//Destructor
Mesh::~Mesh()
{
	(this->*cleanupMeshPtr)();
}
