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

void Material::setShader(Shader* shader){ this->shader = shader; }
Shader* Material::getShader(){ return shader; }

Material::~Material(){}

void Material::setVector4(std::string valueName, Vector4 value)
{
	vector4Map[valueName] = value;
}
void Material::setVector3(std::string valueName, Vector3 value)
{
	vector3Map[valueName] = value;
}
void Material::setVector2(std::string valueName, Vector2 value)
{
	vector2Map[valueName] = value;
}

void Material::setInt(std::string valueName, int value)
{
	intMap[valueName] = value;
}
void Material::setFloat(std::string valueName, float value)
{
	floatMap[valueName] = value;
}
void Material::setDouble(std::string valueName, double value)
{
	doubleMap[valueName] = value;
}

void Material::setMatrix4(std::string valueName, Matrix4 value)
{
	matrix4Map[valueName] = value;
}

void Material::setMatrix3(std::string valueName, Matrix3 value)
{
	matrix3Map[valueName] = value;
}

void Material::setTexture(std::string textureName, Texture* texture)
{
	textureMap[textureName] = texture;
}

void Material::sendDataToGPU()
{
	for (auto iter : vector2Map)
	{
#ifdef GL_SUPPORT
		shader->setVector2GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		shader->setVector2D3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : vector3Map)
	{
#ifdef GL_SUPPORT
		shader->setVector3GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		shader->setVector3D3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : vector4Map)
	{
#ifdef GL_SUPPORT
		shader->setVector4GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		shader->setVector4D3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : intMap)
	{
#ifdef GL_SUPPORT
		shader->setIntGL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		shader->setIntD3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : floatMap)
	{
#ifdef GL_SUPPORT
		shader->setFloatGL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		shader->setFloatD3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : doubleMap)
	{
#ifdef GL_SUPPORT
		shader->setDoubleGL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		shader->setDoubleD3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : matrix3Map)
	{
#ifdef GL_SUPPORT
		shader->setMatrix3GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		shader->setMatrix3D3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : matrix4Map)
	{
#ifdef GL_SUPPORT
		shader->setMatrix4GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		shader->setMatrix4D3D(iter.first.c_str(), iter.second);
#endif
	}

	//Bind textures
	for (std::pair<std::string, Texture*> texPair : textureMap)
	{
		if (texPair.second != nullptr)
			shader->setGlobalTexture(texPair.first, texPair.second);
	}
}