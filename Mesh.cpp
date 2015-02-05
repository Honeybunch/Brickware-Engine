#include "Mesh.h"

#include "Game.h"

//Constructor
//Take in the GPU program ID and the vertex data and buffer that data into the VBO and IBO
Mesh::Mesh(Shader* shader, Shape shape, char* textureFileName)
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

	//Use the shader program
	shader->bindShader();

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		bufferD3D(shader, textureFileName);
	else
		bufferGL(shader, textureFileName);
#elif defined(USE_D3D_ONLY)
	bufferD3D(shader, textureFileName);
#else
	bufferGL(shader, textureFileName);
#endif

	//Unbind everything
	shader->freeShader();
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
ID3D11Buffer* Mesh::getVertexBuffer(){ return vertexBuffer; }
ID3D11Buffer* Mesh::getIndexBuffer(){ return indexBuffer; }
#endif

//Private functions

void Mesh::bufferGL(Shader* shader, char* textureFileName)
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
	if (textureFileName != "" && textureFileName != NULL)
		texture = SOIL_load_OGL_texture(textureFileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
}
void Mesh::bufferD3D(Shader* shader, char* textureFileName)
{
	//Format data for D3D
	float* verticies = new float[pointSize + normalSize + texCoordSize];

	for (int i = 0; i < numberOfVerts ; i++)
	{
		int index = (i * 8);

		verticies[index] = points[i];
		verticies[index + 1] = points[i + 1];
		verticies[index + 2] = points[i + 2];
				  
		verticies[index + 3] = normals[i];
		verticies[index + 4] = normals[i + 1];
		verticies[index + 5] = normals[i + 2];
				  
		verticies[index + 6] = texCoords[i];
		verticies[index + 7] = texCoords[i + 1];
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.ByteWidth = numberOfVerts * sizeof(float) * 8;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = verticies;

	//Buffer
	HR(Game::device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer));

	//Create index buffer description
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.ByteWidth = sizeof(UINT) * indexSize; // Number of indices in the "model" you want to draw
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indicies;

	delete verticies;

	//Buffer
	HR(Game::device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer));
}

//Destructor
Mesh::~Mesh()
{
	
}

