#ifndef LIGHT_H
#define LIGHT_H

#include "BrickwareCoreDLL.h"

#include "GameObject.h"

#include "Vector3.h"

#include <GL/glew.h>
#include <string>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

class BRICKWARE_CORE_API Light : public Component
{
public:
	Light();

	static int LightCount;

	void setAmbientColor(Vector3 ambientColor);
	void setDiffuseColor(Vector3 diffuseColor);
	void setSpecularColor(Vector3 specularColor);

	virtual void Start();
	virtual void Render();

#ifdef _DEBUG
	virtual void DebugDraw();
#endif

	~Light(void);

private:
	int lightCount;

	Vector3 ambientColor;
	Vector3 diffuseColor;
	Vector3 specularColor;
};

#endif

