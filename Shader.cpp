#define _CRT_SECURE_NO_WARNINGS

#include "Shader.h"
#include "Utils.h"
#include "GameObject.h"

#include "Game.h"

Shader::Shader(char* vertexShaderFileName, char* pixelShaderFileName)
{
	//TODO: Strip existing file extension if any
	char* trimmedVertexFileName = Utils::trimToLastChar(vertexShaderFileName, '/');
	char* trimmedPixelFileName = Utils::trimToLastChar(pixelShaderFileName, '/');

#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		loadHLSL(trimmedVertexFileName, trimmedPixelFileName);
	else
		loadGLSL(vertexShaderFileName, pixelShaderFileName);
#elif defined(USE_D3D_ONLY)
	loadHLSL(trimmedVertexFileName, trimmedPixelFileName);
#else
	loadGLSL(vertexShaderFileName, pixelShaderFileName);
#endif
}

void Shader::bindShader()
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		bindHLSL();
	else
		bindGLSL();
#elif defined(USE_D3D_ONLY)
	bindHLSL();
#else
	bindGLSL();
#endif
}

void Shader::freeShader()
{
#ifdef CAN_SWITCH_CONTEXT
	if (USE_DIRECTX)
		freeHLSL();
	else
		freeGLSL();
#elif defined(USE_D3D_ONLY)
	freeHLSL();
#else
	freeGLSL();
#endif
}

#ifndef USE_D3D_ONLY

void Shader::bindGLSL()
{ 
	glUseProgram(shaderProgram); 
}
void Shader::freeGLSL()
{ 
	glUseProgram(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Shader::loadGLSL(char* vertexShaderFileName, char* pixelShaderFileName)
{
	//Assume strings have no file extension

	//+5 for .glsl +1 for null terminator
	char* glslVertexFileName = new char[strlen(vertexShaderFileName) + 6];
	strcpy(glslVertexFileName, vertexShaderFileName);
	strcat(glslVertexFileName, ".glsl");

	char* glslPixelFileName = new char[strlen(pixelShaderFileName) + 6];
	strcpy(glslPixelFileName, pixelShaderFileName);
	strcat(glslPixelFileName, ".glsl");

	// Read in shader source
	char* vertexShaderSource;
	char* pixelShaderSource;

	// Create the shader 
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint pixelShader = glCreateShader(GL_FRAGMENT_SHADER);

	GLuint program;

	// Read in shader source
	vertexShaderSource = Utils::textFileRead(glslVertexFileName);
	if (!vertexShaderSource) {
		cerr << "Error reading vertex shader " << glslVertexFileName << endl;
		shaderProgram = 0;
		return false;
	}
	pixelShaderSource = Utils::textFileRead(glslPixelFileName);
	if (!pixelShaderSource) {
		cerr << "Error reading fragment shader " << glslPixelFileName << endl;
		shaderProgram = 0;
		return false;
	}

	//We need the source in a GLchar
	const GLchar* vertexShaderSourceGL = vertexShaderSource;
	const GLchar* fragmentShaderSourceGL = pixelShaderSource;

	glShaderSource(vertexShader, 1, &vertexShaderSourceGL, NULL);
	glShaderSource(pixelShader, 1, &fragmentShaderSourceGL, NULL);

	// Compile the shader
	glCompileShader(vertexShader);
	glCompileShader(pixelShader);
	Utils::printShaderInfoLog(vertexShader);
	Utils::printShaderInfoLog(pixelShader);

	// Create the program -- attaching your shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, pixelShader);
	Utils::printProgramInfoLog(program);

	// Link the program
	glLinkProgram(program);
	Utils::printProgramInfoLog(program);

	glUseProgram(program);

	//Get positions of Uniforms
	int totalUniforms;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &totalUniforms);

	//Build a map of all uniform locations
	for (int i = 0; i < totalUniforms; i++)
	{
		int nameLength, num;
		GLenum type = GL_ZERO;
		char* name = new char[100];

		glGetActiveUniform(program, (GLuint)i, 99,
			&nameLength, &num, &type, name);

		name[nameLength] = 0;

		uniformMap[std::string(name)] = i;
	}

	glUseProgram(0);

	//Delete unneeded strings
	delete vertexShaderSource;
	delete pixelShaderSource;
	delete glslVertexFileName;
	delete glslPixelFileName;

	//Save program
	shaderProgram = program;

	return true;
}

#endif
#ifdef D3D_SUPPORT
void Shader::bindHLSL()
{
	//Setup input layout
	Game::deviceContext->IASetInputLayout(inputLayout);
	Game::deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Setup shaders
	Game::deviceContext->VSSetShader(vertexShader, NULL, 0);
	Game::deviceContext->PSSetShader(pixelShader, NULL, 0);
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
	WCHAR* hlslVertexWideString = Utils::stringToWideString(hlslVertexFileName);
	WCHAR* hlslPixelWideString = Utils::stringToWideString(hlslPixelFileName);

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Load Vertex Shader --------------------------------------
	if (D3DReadFileToBlob(hlslVertexWideString, &vsBlob) != S_OK)
	{
		cerr << "Error reading vertex shader " << hlslVertexFileName << endl;
		return false;
	}

	// Create the shader on the device
	HR(Game::device->CreateVertexShader(
		vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(),
		NULL,
		&vertexShader));

	// Before cleaning up the data, create the input layout
	HR(Game::device->CreateInputLayout(
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
	HR(Game::device->CreatePixelShader(
		psBlob->GetBufferPointer(),
		psBlob->GetBufferSize(),
		NULL,
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
		std::map<std::string, D3D11_SHADER_VARIABLE_DESC> bufferVarMap;

		//Load the description and type of each variable 
		for (unsigned int j = 0; j < constantBufferDesc.Variables; j++)
		{
			//Load description
			ID3D11ShaderReflectionVariable* variable =
				constantBuffer->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC variableDescription;
			variable->GetDesc(&variableDescription);

			//variable sizes need to be factors of 16 so we may need 
			//to add on some extra space
			int bufferSizeAddition = 0;
			bufferSizeAddition = variableDescription.Size;
			bufferSizeAddition += (bufferSizeAddition % 16);

			bufferSize = bufferSizeAddition;

			//Add this variable to the buffer map
			bufferVarMap[std::string(variableDescription.Name)] = variableDescription;
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
		HR(Game::device->CreateBuffer(
			&cBufferDesc,
			NULL,
			&bufferSpace));

		constantBuffers.push_back(bufferSpace);
	}

	//Free unneeded data
	ReleaseMacro(vsBlob);
	ReleaseMacro(psBlob);
	ReleaseMacro(vertexShaderReflection);

	delete hlslVertexFileName;
	delete hlslPixelFileName;
	delete hlslVertexWideString;
	delete hlslPixelWideString;

	return true;
}

ConstVariableInfo Shader::getVariableInfoByName(char* valueName)
{
	ConstVariableInfo constVariableInfo;

	//Need to get the info about the shader variable that we asked for
	for (unsigned int i = 0; i < constantBufferMaps.size(); i++)
	{
		std::map<std::string, D3D11_SHADER_VARIABLE_DESC> bufferVarMap = constantBufferMaps[i];
		if (bufferVarMap.count(std::string(valueName)))
		{
			constVariableInfo.bufferIndex = i;
			constVariableInfo.variableInfo = bufferVarMap[std::string(valueName)];
		}
	}

	return constVariableInfo;
}

#endif

Shader::~Shader()
{
#ifdef D3D_SUPPORT
	ReleaseMacro(vsBlob);
	ReleaseMacro(psBlob);

	ReleaseMacro(vertexShader);
	ReleaseMacro(pixelShader);
	ReleaseMacro(inputLayout);
	ReleaseMacro(vsConstantBuffer);
#endif
}
