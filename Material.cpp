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

#ifndef USE_D3D_ONLY
void Material::startGL()
{
	//Get currently bound shader	
	GLint shaderProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderProgram);

	//Get positions of Uniforms
	int totalUniforms = -1;
	glGetProgramiv(shaderProgram, GL_ACTIVE_UNIFORMS, &totalUniforms);
	
	//Build a map of all uniform locations in the bound shader
	for (int i = 0; i < totalUniforms; i++)
	{
		int nameLength = -1, num = -1;
		GLenum type = GL_ZERO;
		char* name = new char[100];

		glGetActiveUniform(shaderProgram, GLuint(i), sizeof(name) - 1,
			&nameLength, &num, &type, name);

		//Map the name of the uniform to its type
		uniformMap[name] = type;
	}
}


#endif

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

	ReleaseMacro(vertexShaderReflection);
}

#endif