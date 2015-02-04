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
		ConstantBufferLayout bufferLayout;
		ID3D11ShaderReflectionConstantBuffer* constantBuffer =
			vertexShaderReflection->GetConstantBufferByIndex(i);
		constantBuffer->GetDesc(&bufferLayout.description);

		//Load the description and type of each variable 
		for (unsigned int j = 0; j < bufferLayout.description.Variables; j++)
		{
			//Load description
			ID3D11ShaderReflectionVariable* variable =
				constantBuffer->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC variableDescription;
			variable->GetDesc(&variableDescription);

			bufferLayout.variables.push_back(variableDescription);

			//Load type
			ID3D11ShaderReflectionType* type = variable->GetType();
			D3D11_SHADER_TYPE_DESC typeDescription;
			type->GetDesc(&typeDescription);

			bufferLayout.types.push_back(typeDescription);
		}

		constantBufferLayouts.push_back(bufferLayout);
	}

	//LOAD REFLECTION DATA
	//SAVE SIZES
	//MAP VARIABLE NAMES TO OFFSETS IN A CHAR* BUFFER

	ReleaseMacro(vertexShaderReflection);
}

D3D11_SHADER_VARIABLE_DESC* Material::getVariableByName(char* valueName)
{
	/*
	//Loop through every constant buffer we have in reflection
	//Find the variable with a matching name
	for (unsigned int i = 0; i < constantBufferLayouts.size(); i++)
	{
		vector<ConstantBufferVariable*> bufferVars = constantBufferLayouts[i];


	}
	*/

	return 0;
}

void Material::setVector4D3D(char* valueName, Vector4 value)
{
	/*
	ConstantBufferVariable* bufferVarToSet = getVariableByName(valueName);
	if (!bufferVarToSet)
		return;
		*/
	


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