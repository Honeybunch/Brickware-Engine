#include "mesh.h"

//Constructor
//Take in the GPU program ID and the vertex data and buffer that data into the VBO and IBO
Mesh::Mesh(GLuint shaderProgram, Shape shape, char* textureFileName)
{
	//Store data from shape
	float* points = shape.getPoints();
	float* normals = shape.getNormals();
	GLushort* indicies = shape.getIndicies();
	float* texCoords = shape.getTexCoords();

	//Store sizes
	numberOfVerts = shape.getPointCount() / 4;
	pointSize = shape.getPointCount() * sizeof (float);
	normalSize = shape.getNormalCount() * sizeof(float);
	indexSize = numberOfVerts * sizeof(GLushort);
	texCoordSize = shape.getTexCoordSize() * sizeof(float);

	//Use the shader program
	glUseProgram(shaderProgram);

	//Setup the VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, pointSize + normalSize + texCoordSize, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pointSize, points);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize, normalSize, normals);
	glBufferSubData(GL_ARRAY_BUFFER, pointSize + normalSize, texCoordSize, texCoords);

    //Setup the IBO
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSize, indicies, GL_STATIC_DRAW);

	//Load texture with SOIL
	if(textureFileName != "" && textureFileName != NULL)
		texture = SOIL_load_OGL_texture(textureFileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	
	//Clear vertex data
	delete points;
	delete normals;
	delete indicies;

	//Unbind everything
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Destructor
Mesh::~Mesh()
{
	
}

//Draw everything in the VBOs
void Mesh::Render()
{
	Shader* shader = this->getGameObject()->getComponent<Shader>();
	GLuint shaderProgram = shader->getShaderProgram();

	if(texture != NULL)
		glBindTexture(GL_TEXTURE_2D, texture);

    GLuint vPosition = glGetAttribLocation(shaderProgram, "vPosition");
	GLuint vNormal = glGetAttribLocation(shaderProgram, "vNormal");
	GLuint vTexCoord = glGetAttribLocation(shaderProgram, "vTexCoord");

    glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vNormal);
	glEnableVertexAttribArray(vTexCoord);

	int normalOffset = pointSize;
	int texCoordOffset = pointSize + normalSize;

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(normalOffset));
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(texCoordOffset));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // draw your shape
	glDrawElements (GL_TRIANGLES, numberOfVerts,  GL_UNSIGNED_SHORT, (void *)0);

	//Unbind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}