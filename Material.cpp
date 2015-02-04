#include "Material.h"

Material::Material(Shader* shader)
{
	this->shader = shader;
}

void Material::bindShader()
{
	shader->bindShader();
}

void Material::freeShader()
{
	shader->freeShader();
}

void Material::setVector4(char* valueName, Vector4 value)
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		setVector4D3D(valueName, value);
	else
		setVector4GL(valueName, value);
#elif defined(USE_D3D_ONLY)
	setVector4D3D(valueName, value);
#else
	setVector4GL(valueName, value);
#endif
}
void Material::setVector3(char* valueName, Vector3 value)
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		setVector3D3D(valueName, value);
	else
		setVector3GL(valueName, value);
#elif defined(USE_D3D_ONLY)
	setVector3D3D(valueName, value);
#else
	setVector3GL(valueName, value);
#endif
}
void Material::setVector2(char* valueName, Vector2 value)
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		setVector2D3D(valueName, value);
	else
		setVector2GL(valueName, value);
#elif defined(USE_D3D_ONLY)
	setVector2D3D(valueName, value);
#else
	setVector2GL(valueName, value);
#endif
}

void Material::setInt(char* valueName, int value)
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		setIntD3D(valueName, value);
	else
		setIntGL(valueName, value);
#elif defined(USE_D3D_ONLY)
	setIntD3D(valueName, value);
#else
	setIntGL(valueName, value);
#endif
}
void Material::setFloat(char* valueName, float value)
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		setFloatD3D(valueName, value);
	else
		setFloatGL(valueName, value);
#elif defined(USE_D3D_ONLY)
	setFloatD3D(valueName, value);
#else
	setFloatGL(valueName, value);
#endif
}
void Material::setDouble(char* valueName, double value)
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		setDoubleD3D(valueName, value);
	else
		setDoubleGL(valueName, value);
#elif defined(USE_D3D_ONLY)
	setDoubleD3D(valueName, value);
#else
	setDoubleGL(valueName, value);
#endif
}

void Material::setMatrix4(char* valueName, Matrix4 value)
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		setMatrix4D3D(valueName, value);
	else
		setMatrix4GL(valueName, value);
#elif defined(USE_D3D_ONLY)
	setMatrix4D3D(valueName, value);
#else
	setMatrix4GL(valueName, value);
#endif
}

//Component Overrides

void Material::Start()
{	
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		startD3D();
	else
		startGL();
#elif defined(USE_D3D_ONLY)
	startD3D();
#else
	startGL();
#endif
}