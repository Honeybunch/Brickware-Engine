#ifdef D3D_SUPPORT

#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Mesh.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Utility;
using namespace Math;


ID3D11Buffer* Mesh::getPositionBuffer(){ return positionBuffer; }
ID3D11Buffer* Mesh::getNormalBuffer(){ return normalBuffer; }
ID3D11Buffer* Mesh::getTexCoordBuffer(){ return texCoordBuffer; }

ID3D11Buffer* Mesh::getIndexBuffer(){ return indexBuffer; }

//Private functions

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

void Mesh::cleanupD3D()
{
	//TODO: Free D3D buffer
}

#endif