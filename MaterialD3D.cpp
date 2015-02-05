#include "Material.h"

#include "Game.h"

#ifdef D3D_SUPPORT
std::vector<ID3D11Buffer*> Material::getConstantBuffers(){ return shader->constantBuffers; }
std::vector<char*> Material::getConstantBufferData(){ return shader->constantBufferData; }

void Material::setVector4D3D(char* valueName, Vector4 value)
{
	ConstVariableInfo constVariableInfo = shader->getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	float* data = value.getAsArray();

	char* bufferData = shader->constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, data, variableSize);
}

void Material::setVector3D3D(char* valueName, Vector3 value)
{
	ConstVariableInfo constVariableInfo = shader->getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	float* data = value.getAsArray();

	char* bufferData = shader->constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, data, variableSize);
}

void Material::setVector2D3D(char* valueName, Vector2 value)
{
	ConstVariableInfo constVariableInfo = shader->getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	float* data = value.getAsArray();

	char* bufferData = shader->constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, data, variableSize);
}

void Material::setIntD3D(char* valueName, int value)
{
	ConstVariableInfo constVariableInfo = shader->getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	char* bufferData = shader->constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, &value, sizeof(int));
}

void Material::setFloatD3D(char* valueName, float value)
{
	ConstVariableInfo constVariableInfo = shader->getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	char* bufferData = shader->constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, &value, sizeof(float));
}

void Material::setDoubleD3D(char* valueName, double value)
{
	ConstVariableInfo constVariableInfo = shader->getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	char* bufferData = shader->constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, &value, sizeof(double));
}

void Material::setMatrix4D3D(char* valueName, Matrix4 value)
{
	ConstVariableInfo constVariableInfo = shader->getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	float* data = value.getAsArray();

	char* bufferData = shader->constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, data, variableSize);
}

#endif