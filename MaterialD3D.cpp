#include "Material.h"

#ifdef D3D_SUPPORT
void Material::startD3D()
{
	//Get the shader description
	ID3D11ShaderReflection* vertexShaderReflection;
	ID3DBlob* vsBlob = shader->vsBlob;

	//To avoid a linker error on IID_ID3D11ShaderReflection, make sure to link with dxguid.lib
	HR(D3DReflect(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection));

	D3D11_SHADER_DESC shaderDescription;
	vertexShaderReflection->GetDesc(&shaderDescription);
	
	//Find all constant buffers
	for (unsigned int i = 0; i < shaderDescription.ConstantBuffers; i++)
	{
		unsigned int registerIndex;

		ID3D11ShaderReflectionConstantBuffer* buffer;
		buffer = vertexShaderReflection->GetConstantBufferByIndex(i);

		D3D11_SHADER_BUFFER_DESC bufferDesc;
		buffer->GetDesc(&bufferDesc);

		for (unsigned int j = 0; j < shaderDescription.BoundResources; j++)
		{
			D3D11_SHADER_INPUT_BIND_DESC inputBindDesc;
			vertexShaderReflection->GetResourceBindingDesc(j, &inputBindDesc);

			if (!strcmp(inputBindDesc.Name, bufferDesc.Name))
				registerIndex = inputBindDesc.BindPoint;
		}

		//Add the constant buffer to our list
		//This struct is self defined in Material.h
		ConstantShaderBuffer* constantBuffer = new ConstantShaderBuffer(registerIndex, bufferDesc.Name, buffer, &bufferDesc);
		constantBuffers.push_back(constantBuffer);

		std::vector<ConstantBufferVariable*> bufferVars;
		//Populate the constantBuffer with its variables
		for (unsigned int j = 0; j < constantBuffer->bufferDesc->Variables; j++)
		{
			ID3D11ShaderReflectionVariable* variable;
			variable = constantBuffer->buffer->GetVariableByIndex(j);

			D3D11_SHADER_VARIABLE_DESC variableDesc;
			variable->GetDesc(&variableDesc);

			ConstantBufferVariable* constantBufferVar = new ConstantBufferVariable();
			constantBufferVar->name = variableDesc.Name;
			constantBufferVar->length = variableDesc.Size;
			constantBufferVar->offset = variableDesc.StartOffset;

			bufferVars.push_back(constantBufferVar);
		}

		constantBufferVars.push_back(bufferVars);
	}


	/*
	//Read input layout from shader description
	for (unsigned int i = 0; i < shaderDescription.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		vertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		char* paramName = const_cast<char*>(paramDesc.SemanticName);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		inputLayout[paramName] = elementDesc;
	}
	*/
	ReleaseMacro(vertexShaderReflection);
}

void Material::setVector4D3D(char* valueName, Vector4 value)
{

}

void Material::setVector3D3D(char* valueName, Vector3 value)
{

}

void Material::setVector2D3D(char* valueName, Vector2 value)
{

}

void Material::setIntD3D(char* valueName, int value)
{

}

void Material::setFloatD3D(char* valueName, float value)
{

}

void Material::setDoubleD3D(char* valueName, double value)
{

}

void Material::setMatrix4D3D(char* valueName, Matrix4 value)
{

}

#endif