#ifdef D3D_SUPPORT
#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics\BrickwareGraphicsDLL.hpp"

#include "BrickwareGraphics\RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;

void RenderingManager::RenderD3D(Renderable renderable)
{
	Mesh* mesh = renderable.mesh;
	Material* material = renderable.material;

	material->bindShader();
	material->sendDataToGPU();

	std::vector<ID3D11Buffer*> constantBuffers = material->getConstantBuffers();
	std::vector<char*> constantBufferData = material->getConstantBufferData();

	for (unsigned int i = 0; i < constantBuffers.size(); i++)
	{
		//Do this on variable setting
		deviceContext->UpdateSubresource(
			constantBuffers[i],
			0,
			NULL,
			constantBufferData[i],
			0,
			0);

		deviceContext->VSSetConstantBuffers(i, 1, &(constantBuffers[i]));
		deviceContext->PSSetConstantBuffers(i, 1, &(constantBuffers[i]));
	}

	//I CAN SET MORE THAN ONE BUFFER
	//CONSIDER THIS LATER
	UINT posStride = sizeof(float) * 3;
	UINT normStride = sizeof(float) * 3;
	UINT texCoordStride = sizeof(float) * 2;

	UINT offset = 0;
	ID3D11Buffer* positionBuffer = mesh->getPositionBuffer();
	ID3D11Buffer* normalBuffer = mesh->getNormalBuffer();
	ID3D11Buffer* texCoordBuffer = mesh->getTexCoordBuffer();

	ID3D11Buffer* indexBuffer = mesh->getIndexBuffer();

	deviceContext->IASetVertexBuffers(0, 1, &positionBuffer, &posStride, &offset);
	deviceContext->IASetVertexBuffers(1, 1, &normalBuffer, &normStride, &offset);
	deviceContext->IASetVertexBuffers(2, 1, &texCoordBuffer, &texCoordStride, &offset);

	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//DRAW
	deviceContext->DrawIndexed(mesh->getNumberOfVerts(), 0, 0);

	material->freeShader();
}
#endif