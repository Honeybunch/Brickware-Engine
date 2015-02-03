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

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

#include <unordered_map>

class Material : public Component
{
public:
	Material(Shader* shader);

	void bindShader();
	void freeShader();

	//Component Overrides
	virtual void Start();

	~Material();

private:
	Shader* shader;

#ifndef USE_D3D_ONLY
	std::unordered_map<char*, GLint> uniformMap;

	void startGL();
#endif

#ifdef D3D_SUPPORT
	std::unordered_map<D3D11_INPUT_ELEMENT_DESC, char*> inputLayout;

	void startD3D();
#endif

};

#endif