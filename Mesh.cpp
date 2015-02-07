#include "Mesh.h"

#include "Game.h"

//Constructor
//Take in the GPU program ID and the vertex data and buffer that data into the VBO and IBO
Mesh::Mesh(Shape shape, char* textureFileName)
{
	//Store data from shape
	points = shape.getPoints();
	normals = shape.getNormals();
	indicies = shape.getIndicies();
	texCoords = shape.getTexCoords();

	//Store sizes
	numberOfVerts = shape.getPointCount() / 3;
	pointSize = shape.getPointCount() * sizeof (float);
	normalSize = shape.getNormalCount() * sizeof(float);
	indexSize = numberOfVerts * sizeof(GLushort);
	texCoordSize = shape.getTexCoordSize() * sizeof(float);

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		bufferD3D();
	else
		bufferGL(textureFileName);
#elif defined(USE_D3D_ONLY)
	bufferD3D();
#else
	bufferGL(textureFileName);
#endif
}

//Accessors
float* Mesh::getPoints(){ return points; }

int Mesh::getPointSize(){ return pointSize; }
int Mesh::getNormalSize(){ return normalSize; }
int Mesh::getIndexSize(){ return indexSize; }
int Mesh::getNumberOfVerts(){ return numberOfVerts; }
int Mesh::getTexCoordSize(){ return texCoordSize; }

#ifndef USE_D3D_ONLY
//Write a Texture2D class and get this outta here
GLuint Mesh::getTexture(){ return texture; }

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

void Mesh::bufferGL(char* textureFileName)
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indicies, GL_STATIC_DRAW);

	//Load texture with SOIL 
	//TODO: REMOVE THIS AND LOAD BMP OURSELVES
	if (textureFileName != "" && textureFileName != NULL)
		texture = SOIL_load_OGL_texture(textureFileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}
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
	HR(Game::device->CreateBuffer(&positionBufferDesc, &positionData, &positionBuffer));
	HR(Game::device->CreateBuffer(&normalBufferDesc, &normalData, &normalBuffer));
	HR(Game::device->CreateBuffer(&texCoordBufferDesc, &texCoordData, &texCoordBuffer));

	//Need to have ints rather than shorts
	unsigned int* d3dIndices = new unsigned int[numberOfVerts];

	for (int i = 0; i < numberOfVerts; i++)
		d3dIndices[i] = (unsigned int)indicies[i];

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
	HR(Game::device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer));
}

//Destructor
Mesh::~Mesh()
{
	
}

