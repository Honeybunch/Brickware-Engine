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
			add = false;
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
#ifdef D3D_SUPPORT
	setVector4D3D(valueName, value);
#else
	setVector4GL(valueName, value);
#endif
}
void Material::setVector3(const char* valueName, Vector3 value)
{
#ifdef D3D_SUPPORT
	setVector3D3D(valueName, value);
#else
	setVector3GL(valueName, value);
#endif
}
void Material::setVector2(const char* valueName, Vector2 value)
{
#ifdef D3D_SUPPORT
	setVector2D3D(valueName, value);
#else
	setVector2GL(valueName, value);
#endif
}

void Material::setInt(const char* valueName, int value)
{
#ifdef D3D_SUPPORT
	setIntD3D(valueName, value);
#else
	setIntGL(valueName, value);
#endif
}
void Material::setFloat(const char* valueName, float value)
{
#ifdef D3D_SUPPORT
	setFloatD3D(valueName, value);
#else
	setFloatGL(valueName, value);
#endif
}
void Material::setDouble(const char* valueName, double value)
{
#ifdef D3D_SUPPORT
	setDoubleD3D(valueName, value);
#else
	setDoubleGL(valueName, value);
#endif
}

void Material::setMatrix4(const char* valueName, Matrix4 value)
{
#ifdef D3D_SUPPORT
	setMatrix4D3D(valueName, value);
#else
	setMatrix4GL(valueName, value);
#endif
}

void Material::setMatrix3(const char* valueName, Matrix3 value)
{
#ifdef D3D_SUPPORT
	setMatrix3D3D(valueName, value);
#else
	setMatrix3GL(valueName, value);
#endif
}

void Material::setTexture(const char* textureName, Texture* texture)
{
	std::string textureString (textureName);
	if (textureMap.size() > 0 && textureMap.find(textureString) != textureMap.end())
	{
		textureMap[textureString] = texture;
	}
}