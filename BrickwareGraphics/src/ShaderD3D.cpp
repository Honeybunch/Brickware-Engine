#ifdef D3D_SUPPORT

#define BRICKWARE_GRAPHICS_EXPORTS

#include "BrickwareGraphics/Shader.hpp"
#include "BrickwareGraphics/RenderingManager.hpp"

using namespace Brickware;
using namespace Graphics;
using namespace Math;
using namespace Utility;

void Shader::bindHLSL()
{
	//Setup input layout
	RenderingManager::deviceContext->IASetInputLayout(inputLayout);
	RenderingManager::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Setup shaders
	RenderingManager::deviceContext->VSSetShader(vertexShader, nullptr, 0);
	RenderingManager::deviceContext->PSSetShader(pixelShader, nullptr, 0);
	RenderingManager::deviceContext->PSSetSamplers(0, 1, &samplerState);
}
void Shader::freeHLSL(){}

bool Shader::loadHLSL(char* vertexShaderFileName, char* pixelShaderFileName)
{
	//Assume strings have no file extension
	char* hlslVertexFileName = new char[strlen(vertexShaderFileName) + 5];
	strcpy(hlslVertexFileName, vertexShaderFileName);
	strcat(hlslVertexFileName, ".cso");

	char* hlslPixelFileName = new char[strlen(pixelShaderFileName) + 5];
	strcpy(hlslPixelFileName, pixelShaderFileName);
	strcat(hlslPixelFileName, ".cso");

	//Convert to wide strings because DirectX asked nicely
	WCHAR* hlslVertexWideString = StringUtils::stringToWideString(hlslVertexFileName);
	WCHAR* hlslPixelWideString = StringUtils::stringToWideString(hlslPixelFileName);

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 2, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Load Vertex Shader --------------------------------------
	if (D3DReadFileToBlob(hlslVertexWideString, &vsBlob) != S_OK)
	{
		cerr << "Error reading vertex shader " << hlslVertexFileName << endl;
		return false;
	}

	// Create the shader on the device
	HR(RenderingManager::device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		nullptr,
		&vertexShader));

	// Before cleaning up the data, create the input layout
	HR(RenderingManager::device->CreateInputLayout(
		vertexDesc,
		ARRAYSIZE(vertexDesc),
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		&inputLayout));

	// Load Pixel Shader ---------------------------------------
	D3DReadFileToBlob(hlslPixelWideString, &psBlob);
	if (false)
	{
		cerr << "Error reading pixel shader " << hlslPixelFileName << endl;
		return false;
	}

	// Create the shader on the device
	HR(RenderingManager::device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		nullptr,
		&pixelShader));

	//Get the shader description
	ID3D11ShaderReflection* vertexShaderReflection;

	//To avoid a linker error on IID_ID3D11ShaderReflection, make sure to link with dxguid.lib
	HR(D3DReflect(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection));

	D3D11_SHADER_DESC shaderDescription;
	vertexShaderReflection->GetDesc(&shaderDescription);

	//Find all constant buffers
	for (unsigned int i = 0; i < shaderDescription.ConstantBuffers; i++)
	{
		D3D11_SHADER_BUFFER_DESC constantBufferDesc;
		ID3D11ShaderReflectionConstantBuffer* constantBuffer =
			vertexShaderReflection->GetConstantBufferByIndex(i);

		constantBuffer->GetDesc(&constantBufferDesc);

		unsigned int bufferSize = 0;
		char* bufferData;

		//Gonna map variable names to more data
		std::map<std::string, D3D11_SHADER_VARIABLE_DESC*>* bufferVarMap =
			new std::map<std::string, D3D11_SHADER_VARIABLE_DESC*>();

		//Load the description and type of each variable 
		for (unsigned int j = 0; j < constantBufferDesc.Variables; j++)
		{
			//Load description
			ID3D11ShaderReflectionVariable* variable =
				constantBuffer->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC* variableDescription = new D3D11_SHADER_VARIABLE_DESC;
			variable->GetDesc(variableDescription);

			//variable sizes need to be factors of 16 so we may need 
			//to add on some extra space

			bufferSize += variableDescription->Size;

			//Add this variable to the buffer map
			(*bufferVarMap)[std::string(variableDescription->Name)] = variableDescription;
		}
		//Make sure buffer size is enough space for 16 byte alignment
		bufferSize += 16 - (bufferSize % 16);

		//Setup the bufferData
		bufferData = new char[bufferSize];

		//Add the map to the map vector and the bufferData to the data vector
		//These vectors are essentially mapped to one another
		constantBufferData.push_back(bufferData);
		constantBufferMaps.push_back(bufferVarMap);

		//Hack to allocate space for a new constant buffer
		ID3D11Buffer* bufferSpace = 0;

		//Actually create a constant buffer object
		D3D11_BUFFER_DESC cBufferDesc;
		cBufferDesc.ByteWidth = bufferSize;
		cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = 0;
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;
		HR(RenderingManager::device->CreateBuffer(
			&cBufferDesc,
			nullptr,
			&bufferSpace));

		constantBuffers.push_back(bufferSpace);
	}

	//Perform shader reflection on the Pixel Shader to get the texture names and any constant buffers in the pixel shader
	ID3D11ShaderReflection* pixelShaderReflection;
	HR(D3DReflect(psBlob->GetBufferPointer(), psBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)&pixelShaderReflection));

	D3D11_SHADER_DESC pixelShaderDescription;
	pixelShaderReflection->GetDesc(&pixelShaderDescription);

	//Find all constant buffers
	for (unsigned int i = 0; i < pixelShaderDescription.ConstantBuffers; i++)
	{
		D3D11_SHADER_BUFFER_DESC constantBufferDesc;
		ID3D11ShaderReflectionConstantBuffer* constantBuffer =
			pixelShaderReflection->GetConstantBufferByIndex(i);

		constantBuffer->GetDesc(&constantBufferDesc);

		unsigned int bufferSize = 0;
		char* bufferData;

		//Gonna map variable names to more data
		std::map<std::string, D3D11_SHADER_VARIABLE_DESC*>* bufferVarMap =
			new std::map<std::string, D3D11_SHADER_VARIABLE_DESC*>();

		//Load the description and type of each variable 
		for (unsigned int j = 0; j < constantBufferDesc.Variables; j++)
		{
			//Load description
			ID3D11ShaderReflectionVariable* variable =
				constantBuffer->GetVariableByIndex(j);

			ID3D11ShaderReflectionType* varType = variable->GetType();

			D3D11_SHADER_VARIABLE_DESC* variableDescription = new D3D11_SHADER_VARIABLE_DESC;
			variable->GetDesc(variableDescription);

			D3D11_SHADER_TYPE_DESC varTypeDescription;
			varType->GetDesc(&varTypeDescription);

			//If we have elements in this type description, it's an array
			if (varTypeDescription.Elements > 0)
			{
				for (unsigned int k = 0; k < varTypeDescription.Elements; k++)
				{					
					std::string elementName = variableDescription->Name;
					elementName.append("[");
					elementName.append(std::to_string(k));
					elementName.append("].");
					
					for (unsigned int l = 0; l < varTypeDescription.Members; l++)
					{
						D3D11_SHADER_VARIABLE_DESC* elementDesc = new D3D11_SHADER_VARIABLE_DESC;

						D3D11_SHADER_TYPE_DESC typeDesc;
						varType->GetMemberTypeByIndex(l)->GetDesc(&typeDesc);

						std::string memberName = elementName;
						memberName.append(varType->GetMemberTypeName(l));

						elementDesc->Name = memberName.c_str();
						elementDesc->StartOffset = bufferSize;
						elementDesc->Size = typeDesc.Rows * 16;

						bufferSize += elementDesc->Size;

						//Add this member to the buffer map
						(*bufferVarMap)[memberName] = elementDesc;
					}					
				}
			}
			else
			{
				bufferSize += variableDescription->Size;

				//Make sure buffer size is enough space for 16 byte alignment
				bufferSize += 16 - (bufferSize % 16);

				//Add this variable to the buffer map
				(*bufferVarMap)[std::string(variableDescription->Name)] = variableDescription;
			}
			
		}

		//Setup the bufferData
		bufferData = new char[bufferSize];

		//Add the map to the map vector and the bufferData to the data vector
		//These vectors are essentially mapped to one another
		constantBufferData.push_back(bufferData);
		constantBufferMaps.push_back(bufferVarMap);

		//Hack to allocate space for a new constant buffer
		ID3D11Buffer* bufferSpace = 0;

		//Actually create a constant buffer object
		D3D11_BUFFER_DESC cBufferDesc;
		cBufferDesc.ByteWidth = bufferSize;
		cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = 0;
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;
		HR(RenderingManager::device->CreateBuffer(
			&cBufferDesc,
			nullptr,
			&bufferSpace));

		constantBuffers.push_back(bufferSpace);
	}

	for (unsigned int i = 0; i < pixelShaderDescription.BoundResources; i++)
	{
		D3D11_SHADER_INPUT_BIND_DESC resourceDesc;
		pixelShaderReflection->GetResourceBindingDesc(i, &resourceDesc);

		if (resourceDesc.Type == D3D_SIT_TEXTURE)
			textureMap[std::string(resourceDesc.Name)] = nullptr;
	}

	//Setup Sampler state
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	samplerDesc.MaxAnisotropy = 0;

	HR(RenderingManager::device->CreateSamplerState(&samplerDesc, &samplerState));

	//Free unneeded data
	ReleaseMacro(vsBlob);
	ReleaseMacro(psBlob);
	ReleaseMacro(vertexShaderReflection);
	ReleaseMacro(pixelShaderReflection);

	delete hlslVertexFileName;
	delete hlslPixelFileName;
	delete hlslVertexWideString;
	delete hlslPixelWideString;

	return true;
}

ConstVariableInfo Shader::getVariableInfoByName(const char* valueName)
{
	ConstVariableInfo constVariableInfo;

	//Need to get the info about the shader variable that we asked for
	for (unsigned int i = 0; i < constantBufferMaps.size(); i++)
	{
		std::map<std::string, D3D11_SHADER_VARIABLE_DESC*>* bufferVarMap = constantBufferMaps[i];

		if (bufferVarMap->find(std::string(valueName)) != bufferVarMap->end())
		{
			constVariableInfo.bufferIndex = i;
			constVariableInfo.variableInfo = *(*bufferVarMap)[std::string(valueName)];
		}
	}

	return constVariableInfo;
}

void Shader::setVector4D3D(const char* valueName, Vector4 value)
{
	ConstVariableInfo constVariableInfo = getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	float* data = value.getAsArray();

	char* bufferData = constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, data, variableSize);
}

void Shader::setVector3D3D(const char* valueName, Vector3 value)
{
	ConstVariableInfo constVariableInfo = getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	float* data = value.getAsArray();

	char* bufferData = constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, data, variableSize);
}

void Shader::setVector2D3D(const char* valueName, Vector2 value)
{
	ConstVariableInfo constVariableInfo = getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	float* data = value.getAsArray();

	char* bufferData = constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, data, variableSize);
}

void Shader::setIntD3D(const char* valueName, int value)
{
	ConstVariableInfo constVariableInfo = getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	char* bufferData = constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, &value, sizeof(int));
}

void Shader::setFloatD3D(const char* valueName, float value)
{
	ConstVariableInfo constVariableInfo = getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	char* bufferData = constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, &value, sizeof(float));
}

void Shader::setDoubleD3D(const char* valueName, double value)
{
	ConstVariableInfo constVariableInfo = getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	char* bufferData = constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, &value, sizeof(double));
}

void Shader::setMatrix4D3D(const char* valueName, Matrix4 value)
{
	ConstVariableInfo constVariableInfo = getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	float* data = value.getAsArray();

	char* bufferData = constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, data, variableSize);
}

void Shader::setMatrix3D3D(const char* valueName, Matrix3 value)
{
	ConstVariableInfo constVariableInfo = getVariableInfoByName(valueName);

	//We didn't find any variable with that name if the bufferIndex is still -1
	if (constVariableInfo.bufferIndex < 0)
		return;

	//Get info about variable
	int variableOffset = constVariableInfo.variableInfo.StartOffset;
	int variableSize = constVariableInfo.variableInfo.Size;

	float* data = value.getAsArray();

	char* bufferData = constantBufferData[constVariableInfo.bufferIndex];

	//Copy our data into the buffer's data
	memcpy(bufferData + variableOffset, data, variableSize);
}


#endif