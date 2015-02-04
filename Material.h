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

#ifdef D3D_SUPPORT
struct ConstantBufferLayout
{
	D3D11_SHADER_BUFFER_DESC description;
	std::vector<D3D11_SHADER_VARIABLE_DESC> variables;
	std::vector<D3D11_SHADER_TYPE_DESC> types;
};

#endif

#include "Component.h"
#include "Utils.h"

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"

#include <map>

class Material : public Component
{
public:
	Material(Shader* shader);

	void bindShader();
	void freeShader();

	void setVector4(char* valueName, Vector4 value);
	void setVector3(char* valueName, Vector3 value);
	void setVector2(char* valueName, Vector2 value);

	void setInt(char* valueName, int value);
	void setFloat(char* valueName, float value);
	void setDouble(char* valueName, double value);

	void setMatrix4(char* valueName, Matrix4 value);

	//Component Overrides
	virtual void Start();

	~Material();

private:
	Shader* shader;

#ifndef USE_D3D_ONLY
	//MUST use a string otherwise it will compare char*s as integer values and insert garbage data
	std::map<std::string, GLuint> uniformMap;	

	void startGL();

	void setVector4GL(char* valueName, Vector4 value);
	void setVector3GL(char* valueName, Vector3 value);
	void setVector2GL(char* valueName, Vector2 value);

	void setIntGL(char* valueName, int value);
	void setFloatGL(char* valueName, float value);
	void setDoubleGL(char* valueName, double value);

	void setMatrix4GL(char* valueName, Matrix4 value);
#endif

#ifdef D3D_SUPPORT
	std::vector<ConstantBufferLayout> constantBufferLayouts;
	

	D3D11_SHADER_VARIABLE_DESC* getVariableByName(char* valueName);

	void startD3D();

	void setVector4D3D(char* valueName, Vector4 value);
	void setVector3D3D(char* valueName, Vector3 value);
	void setVector2D3D(char* valueName, Vector2 value);

	void setIntD3D(char* valueName, int value);
	void setFloatD3D(char* valueName, float value);
	void setDoubleD3D(char* valueName, double value);

	void setMatrix4D3D(char* valueName, Matrix4 value);
#endif

};

#endif