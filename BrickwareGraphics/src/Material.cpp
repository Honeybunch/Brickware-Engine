#define BRICKWARE_GRAPHICS_EXPORTS

#include "Material.h"

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

	if (defaultTexture == NULL)
		defaultTexture = new Texture("Textures/defaultTexture.bmp");

	this->setTexture("diffuseTexture", Material::defaultTexture);

	//Add this material to the list of materials if there isn't already a material stored with the same shader
	bool add = true;

	for (unsigned int i = 0; i < Materials.size(); i++)
	{
		if (this->shader == Materials[i]->shader)
			add = true;
	}

	if (add)
		Materials.push_back(this);
}

Material::~Material(){}

void Material::bindShader()
{
	shader->bindShader();

	for (std::pair<std::string, Texture*> texPair : textureMap)
	{
		if (texPair.second != NULL)
			texPair.second->bindTexture();
	}
}

void Material::freeShader()
{
	shader->freeShader();

	for (std::pair<std::string, Texture*> texPair : textureMap)
	{
		if (texPair.second != NULL)
			texPair.second->freeTexture();
	}
}

void Material::setVector4(const char* valueName, Vector4 value)
{
	vector4Map[std::string(valueName)] = value;
}
void Material::setVector3(const char* valueName, Vector3 value)
{
	vector3Map[std::string(valueName)] = value;
}
void Material::setVector2(const char* valueName, Vector2 value)
{
	vector2Map[std::string(valueName)] = value;
}

void Material::setInt(const char* valueName, int value)
{
	intMap[std::string(valueName)] = value;
}
void Material::setFloat(const char* valueName, float value)
{
	floatMap[std::string(valueName)] = value;
}
void Material::setDouble(const char* valueName, double value)
{
	doubleMap[std::string(valueName)] = value;
}

void Material::setMatrix4(const char* valueName, Matrix4 value)
{
	matrix4Map[std::string(valueName)] = value;
}

void Material::setMatrix3(const char* valueName, Matrix3 value)
{
	matrix3Map[std::string(valueName)] = value;
}

void Material::setTexture(const char* textureName, Texture* texture)
{
	std::string textureString (textureName);
	if (textureMap.size() > 0 && textureMap.find(textureString) != textureMap.end())
	{
		textureMap[textureString] = texture;
	}
}

void Material::sendDataToGPU()
{
	for (auto iter : vector2Map)
	{
#ifdef GL_SUPPORT
		setVector2GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		setVector2D3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : vector3Map)
	{
#ifdef GL_SUPPORT
		setVector3GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		setVector3D3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : vector4Map)
	{
#ifdef GL_SUPPORT
		setVector4GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		setVector4D3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : intMap)
	{
#ifdef GL_SUPPORT
		setIntGL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		setIntD3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : floatMap)
	{
#ifdef GL_SUPPORT
		setFloatGL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		setFloatD3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : doubleMap)
	{
#ifdef GL_SUPPORT
		setDoubleGL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		setDoubleD3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : matrix3Map)
	{
#ifdef GL_SUPPORT
		setMatrix3GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		setMatrix3D3D(iter.first.c_str(), iter.second);
#endif
	}

	for (auto iter : matrix4Map)
	{
#ifdef GL_SUPPORT
		setMatrix4GL(iter.first.c_str(), iter.second);
#endif
#ifdef D3D_SUPPORT
		setMatrix4D3D(iter.first.c_str(), iter.second);
#endif
	}
}