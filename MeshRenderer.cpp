#include "MeshRenderer.h"


MeshRenderer::MeshRenderer(Mesh* mesh)
{
	this->mesh = mesh;
}


MeshRenderer::~MeshRenderer()
{
}

//Draw everything in the VBOs
void MeshRenderer::Render()
{
	Material* material = this->getGameObject()->getComponent<Material>();
	GLuint shaderProgram = material->getShaderProgram();

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
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(normalOffset));
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(texCoordOffset));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIBO());

	// draw your shape
	glDrawElements(GL_TRIANGLES, mesh->getNumberOfVerts(), GL_UNSIGNED_SHORT, (void *)0);

	//Unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}