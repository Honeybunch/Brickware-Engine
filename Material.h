#ifndef MATERIAL_H
#define MATERIAL_H

#include "Shader.h"

#ifndef USE_D3D_ONLY
	#include <GL/glew.h>

	#ifdef _WIN32
		#define GLFW_DLL
	#endif

	#define GLFW_INCLUDE_GLU
	#include <glfw3.h>
#endif

#include "Component.h"

class Material : public Component
{
public:
	Material(Shader* shader);

	//TODO: replace this
	GLuint getModelMatrixPos();

	void bindShader();
	void freeShader();

	//Component Overrides
	virtual void Start();

	~Material();

private:
	Shader* shader;

	void startGL();
	void startD3D();

	GLuint modelMatrixPos;
};

#endif