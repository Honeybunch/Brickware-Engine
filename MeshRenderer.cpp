#include "MeshRenderer.h"

#include "Game.h"

MeshRenderer::MeshRenderer(Mesh* mesh)
{
	this->mesh = mesh;

	bounds = new Bounds(Vector3(), 0);
}

Bounds* MeshRenderer::getBounds(){ return bounds; }

//Recalculate the bounds of the mesh
void MeshRenderer::Update()
{
	calculateBounds();
}

//Draw everything in the VBOs
void MeshRenderer::Render()
{
	Material* material = this->getGameObject()->getComponent<Material>();
	
#ifdef D3D_SUPPORT
	renderD3D(material);
#else
	renderGL(material);
#endif
}

void MeshRenderer::calculateBounds()
{
	float minX = bounds->getMinBound().getX();
	float minY = bounds->getMinBound().getY();
	float minZ = bounds->getMinBound().getZ();

	float maxX = bounds->getMaxBound().getX();
	float maxY = bounds->getMaxBound().getY();
	float maxZ = bounds->getMaxBound().getZ();

	float* points = mesh->getPoints();
	int numOfVerts = mesh->getNumberOfVerts();

	for(int i = 0; i < numOfVerts; i+=3)
	{
		float testX = points[i];
		float testY = points[i+1];
		float testZ = points[i+2];

		if(testX < minX)
			minX = testX;
		else if(testX > maxX)
			maxX = testX;

		if(testY < minY)
			minY = testY;
		else if(testY > maxY)
			maxY = testY;

		if(testZ < minZ)
			minZ = testZ;
		else if(testZ > maxZ)
			maxZ = testZ;
	}

	Vector3 minPoint(minX, minY, minZ);
	Vector3 maxPoint(maxX, maxY, maxZ);

	float xWidth = maxPoint.getX() - minPoint.getX();
	float yWidth = maxPoint.getY() - minPoint.getY();
	float zWidth = maxPoint.getZ() - minPoint.getZ();

	Vector3 center;

	GameObject* gameObject = getGameObject();
	if (gameObject)
	{
		Transform* transform = gameObject->getTransform();
		
		center = *transform->getPosition();
	}
	bounds = new Bounds(center, xWidth, yWidth, zWidth);
}

//Private methods
#ifdef GL_SUPPORT
void MeshRenderer::renderGL(Material* material)
{
	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	if (mesh->getTexture() != NULL)
		glBindTexture(GL_TEXTURE_2D, mesh->getTexture());

	GLuint vPosition = glGetAttribLocation(shaderProgram, "vPosition");
	GLuint vNormal = glGetAttribLocation(shaderProgram, "vNormal");
	GLuint vTexCoord = glGetAttribLocation(shaderProgram, "vTexCoord");

	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);
	glEnableVertexAttribArray(vTexCoord);

	int normalOffset = mesh->getPointSize();
	int texCoordOffset = normalOffset + mesh->getNormalSize();

	glBindBuffer(GL_ARRAY_BUFFER, mesh->getVBO());
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(normalOffset));
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(texCoordOffset));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIBO());

	// draw your shape
	glDrawElements(GL_TRIANGLES, mesh->getNumberOfVerts(), GL_UNSIGNED_SHORT, (void *)0);
}
#endif

#ifdef D3D_SUPPORT
void MeshRenderer::renderD3D(Material* material)
{
	std::vector<ID3D11Buffer*> constantBuffers = material->getConstantBuffers();
	std::vector<char*> constantBufferData = material->getConstantBufferData();

	for (unsigned int i = 0; i < constantBuffers.size(); i++)
	{
		//Do this on variable setting
		Game::deviceContext->UpdateSubresource(
			constantBuffers[i],
			0,
			NULL,
			constantBufferData[i],
			0,
			0);			
		
		Game::deviceContext->VSSetConstantBuffers(i, 1, &(constantBuffers[i])); 
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

	Game::deviceContext->IASetVertexBuffers(0, 1, &positionBuffer, &posStride, &offset);
	Game::deviceContext->IASetVertexBuffers(1, 1, &normalBuffer, &normStride, &offset);
	Game::deviceContext->IASetVertexBuffers(2, 1, &texCoordBuffer, &texCoordStride, &offset);

	Game::deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//DRAW
	Game::deviceContext->DrawIndexed(mesh->getNumberOfVerts(), 0, 0);
}
#endif

MeshRenderer::~MeshRenderer()
{
}