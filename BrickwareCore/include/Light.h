#ifndef LIGHT_H
#define LIGHT_H

#include "GameObject.h"

#include "Vector3.h"

#include <GL/glew.h>

#ifdef _WIN32
#define GLFW_DLL
#endif

#define GLFW_INCLUDE_GLU
#include <glfw3.h>

class Light : public Component
{
public:
	Light();

	virtual void Start();
	virtual void Render();

	~Light(void);
};

#endif

