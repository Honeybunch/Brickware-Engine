#define BRICKWARE_CORE_EXPORTS

#include "Material.h"

Texture* Material::defaultTexture;

Material::Material(Shader* shader)
{
	this->shader = shader;

	//We copy this from the shader so that every material will have its own map of textures
	//With the the proper names from the shader's shader reflection
	textureMap = shader->textureMap;

	if (defaultTexture == NULL)
		defaultTexture = new Texture("Textures/defaultTexture.bmp");

	this->setTexture("diffuseTexture", Material::defaultTexture);
}


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

Component* Material::Clone(){ return new Material(*this); }

void Material::setVector4(char* valueName, Vector4 value)
{
#ifdef D3D_SUPPORT
	setVector4D3D(valueName, value);
#else
	setVector4GL(valueName, value);
#endif
}
void Material::setVector3(char* valueName, Vector3 value)
{
#ifdef D3D_SUPPORT
	setVector3D3D(valueName, value);
#else
	setVector3GL(valueName, value);
#endif
}
void Material::setVector2(char* valueName, Vector2 value)
{
#ifdef D3D_SUPPORT
	setVector2D3D(valueName, value);
#else
	setVector2GL(valueName, value);
#endif
}

void Material::setInt(char* valueName, int value)
{
#ifdef D3D_SUPPORT
	setIntD3D(valueName, value);
#else
	setIntGL(valueName, value);
#endif
}
void Material::setFloat(char* valueName, float value)
{
#ifdef D3D_SUPPORT
	setFloatD3D(valueName, value);
#else
	setFloatGL(valueName, value);
#endif
}
void Material::setDouble(char* valueName, double value)
{
#ifdef D3D_SUPPORT
	setDoubleD3D(valueName, value);
#else
	setDoubleGL(valueName, value);
#endif
}

void Material::setMatrix4(char* valueName, Matrix4 value)
{
#ifdef D3D_SUPPORT
	setMatrix4D3D(valueName, value);
#else
	setMatrix4GL(valueName, value);
#endif
}

void Material::setTexture(char* textureName, Texture* texture)
{
	std::string textureString (textureName);
	if (textureMap.size() > 0 && textureMap.find(textureString) != textureMap.end())
	{
		textureMap[textureString] = texture;
	}
}