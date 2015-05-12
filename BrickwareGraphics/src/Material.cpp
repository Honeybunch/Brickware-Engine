#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Material.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;

Texture* Material::defaultTexture;

std::vector <Material*> Material::Materials;

Material::Material(Shader* shader)
{
	this->shader = shader;

	//We copy this from the shader so that every material will have its own map of textures
	//With the the proper names from the shader's shader reflection
	textureMap = shader->textureMap;

	if (defaultTexture == nullptr)
		defaultTexture = new Texture("Textures/defaultTexture.bmp");

	this->setTexture("diffuseTexture", Material::defaultTexture);

	Materials.push_back(this);
}

Material::Material(const Material& other)
{
	this->shader = other.shader;
	this->vector2Map = other.vector2Map;
	this->vector3Map = other.vector3Map;
	this->vector4Map = other.vector4Map;

	this->intMap = other.intMap;
	this->floatMap = other.floatMap;
	this->doubleMap = other.doubleMap;

	this->matrix3Map = other.matrix3Map;
	this->matrix4Map = other.matrix4Map;

	this->textureMap = other.textureMap;

	Materials.push_back(this);
}

Material::~Material(){}

void Material::bindShader()
{
	shader->bindShader();
}

void Material::freeShader()
{
	shader->freeShader();
}

void Material::setVector4(const char* valueName, Vector4 value)
{
	vector4Map[valueName] = value;
}
void Material::setVector3(const char* valueName, Vector3 value)
{
	vector3Map[valueName] = value;
}
void Material::setVector2(const char* valueName, Vector2 value)
{
	vector2Map[valueName] = value;
}

void Material::setInt(const char* valueName, int value)
{
	intMap[valueName] = value;
}
void Material::setFloat(const char* valueName, float value)
{
	floatMap[valueName] = value;
}
void Material::setDouble(const char* valueName, double value)
{
	doubleMap[valueName] = value;
}

void Material::setMatrix4(const char* valueName, Matrix4 value)
{
	matrix4Map[valueName] = value;
}

void Material::setMatrix3(const char* valueName, Matrix3 value)
{
	matrix3Map[valueName] = value;
}

void Material::setTexture(const char* textureName, Texture* texture)
{
	if (textureMap.size() > 0 && textureMap.find(textureName) != textureMap.end())
	{
		textureMap[textureName] = texture;
	}
}

void Material::sendDataToGPU()
{
	for (auto iter : vector2Map)
	{
#ifdef GL_SUPPORT
		setVector2GL(iter.first, iter.second);
#endif
#ifdef D3D_SUPPORT
		setVector2D3D(iter.first, iter.second);
#endif
	}

	for (auto iter : vector3Map)
	{
#ifdef GL_SUPPORT
		setVector3GL(iter.first, iter.second);
#endif
#ifdef D3D_SUPPORT
		setVector3D3D(iter.first, iter.second);
#endif
	}

	for (auto iter : vector4Map)
	{
#ifdef GL_SUPPORT
		setVector4GL(iter.first, iter.second);
#endif
#ifdef D3D_SUPPORT
		setVector4D3D(iter.first, iter.second);
#endif
	}

	for (auto iter : intMap)
	{
#ifdef GL_SUPPORT
		setIntGL(iter.first, iter.second);
#endif
#ifdef D3D_SUPPORT
		setIntD3D(iter.first, iter.second);
#endif
	}

	for (auto iter : floatMap)
	{
#ifdef GL_SUPPORT
		setFloatGL(iter.first, iter.second);
#endif
#ifdef D3D_SUPPORT
		setFloatD3D(iter.first, iter.second);
#endif
	}

	for (auto iter : doubleMap)
	{
#ifdef GL_SUPPORT
		setDoubleGL(iter.first, iter.second);
#endif
#ifdef D3D_SUPPORT
		setDoubleD3D(iter.first, iter.second);
#endif
	}

	for (auto iter : matrix3Map)
	{
#ifdef GL_SUPPORT
		setMatrix3GL(iter.first, iter.second);
#endif
#ifdef D3D_SUPPORT
		setMatrix3D3D(iter.first, iter.second);
#endif
	}

	for (auto iter : matrix4Map)
	{
#ifdef GL_SUPPORT
		setMatrix4GL(iter.first, iter.second);
#endif
#ifdef D3D_SUPPORT
		setMatrix4D3D(iter.first, iter.second);
#endif
	}

	//Bind textures
	for (std::pair<const char*, Texture*> texPair : textureMap)
	{
		if (texPair.second != nullptr)
			texPair.second->bindTexture();
	}
}