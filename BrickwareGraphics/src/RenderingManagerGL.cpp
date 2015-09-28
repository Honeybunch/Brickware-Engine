#ifdef GL_SUPPORT
#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/BrickwareGraphicsDLL.hpp"

#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;

void RenderingManager::RenderGL()
{
	PreRenderGL();

	ShadowPassGL();

	//Other user defined render passes here?

	ScenePassGL();

	//Cleanup
	lights.clear();
	renderables.clear();
}

void RenderingManager::PreRenderGL()
{
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderingManager::ShadowPassGL()
{
	glViewport(0, 0, 1024, 1024);
	for (unsigned int i = 0; i < lights.size(); i++)
		lights[i]->RenderShadowMap(ShadowShader);
}

void RenderingManager::ScenePassGL()
{
	glViewport(0, 0, 800, 600);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render every renderable object
	for (unsigned int i = 0; i < renderables.size(); i++)
	{
		Renderable renderable = renderables[i];
		Shader* shader = renderable.material->shader;

		if (shader != Shader::ActiveShader)
		{
			shader->bindShader();

			//Send light data to the shader
			for (unsigned int j = 0; j < lights.size(); j++)
			{
				lights[j]->RenderLight(shader);
				lights[j]->BindShadowMap(shader);
			}
		}

		Material* material = renderable.material;

		material->sendDataToGPU();

		//Render object
		RenderObjectGL(renderable);
	}

	Shader::ActiveShader->freeShader();
}

void RenderingManager::RenderObjectGL(Renderable renderable)
{
	Shader::ActiveShader->setGlobalMatrix4("modelMatrix",renderable.modelMatrix);

	Mesh* mesh = renderable.mesh;

	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

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

	//Draw Shape
	glDrawElements(GL_TRIANGLES, mesh->getNumberOfVerts(), GL_UNSIGNED_SHORT, (void *)0);
}

//Render every object in the scene; assume a bound shader
void RenderingManager::RenderPassGL()
{
	for(unsigned int i = 0; i < renderables.size(); i++)
		RenderObjectGL(renderables[i]);
}

#endif
