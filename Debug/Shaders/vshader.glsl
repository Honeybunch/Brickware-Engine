#version 330

#define pi 3.141592653589793238462643383279

//Attributes
in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;

//Uniforms
uniform mat4 modelMatrix; 
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec3 lightPosition;

uniform sampler2D tex;

uniform vec3 lookAt;
uniform vec3 eyePoint;
uniform vec3 up;

//To be passed to the fragment shader
out vec3 L;
out vec3 E;
out vec3 H;
out vec3 N;

out vec2 texCoord;

//Locals
mat4 translation;
mat4 rotation;
mat4 scales;


float zNear = 0.1f;
float zFar = 100.0f;
float height = 0.1f;
float width = 0.1f;
float FoV = 50;

/*
//Returns the model matrix
mat4 getModel()
{
	// Compute the sines and cosines of each rotation
    // about each axis
    vec3 angles = radians (theta);
    vec3 c = cos (angles);
    vec3 s = sin (angles);
    
    // rotation matricies
    mat4 rotationX = mat4 (1.0,  0.0,  0.0,  0.0, 
						   0.0,  c.x,  s.x,  0.0,
						   0.0, -s.x,  c.x,  0.0,
						   0.0,  0.0,  0.0,  1.0);
                    
    mat4 rotationY = mat4 (c.y,  0.0, -s.y,  0.0, 
						   0.0,  1.0,  0.0,  0.0,
						   s.y,  0.0,  c.y,  0.0,
						   0.0,  0.0,  0.0,  1.0);

    mat4 rotationZ = mat4 (c.z, -s.z,  0.0,  0.0, 
						   s.z,  c.z,  0.0,  0.0,
						   0.0,  0.0,  1.0,  0.0,
						   0.0,  0.0,  0.0,  1.0);

	translation = mat4 ( 1.0, 0.0, 0.0, 0.0,
						 0.0, 1.0, 0.0, 0.0,
						 0.0, 0.0, 1.0, 0.0,
						 trans.x, trans.y, trans.z, 1.0);

	rotation = rotationX * rotationY * rotationZ;
            
    scales = mat4 (scale.x, 0.0, 0.0, 0.0,
                  0.0, scale.y, 0.0, 0.0, 
                  0.0, 0.0, scale.z, 0.0,
                  0.0, 0.0, 0.0, 1.0);

	//return model matrix
	return translation * rotation * scales;
}
*/

//Returns the view matrix
mat4 getView()
{
	//Calculate axes
	vec3 zAxis = normalize(eyePoint - lookAt);
	vec3 xAxis = normalize(cross(up, zAxis));
	vec3 yAxis = cross(zAxis, xAxis);

	//Create orientation matrix
	mat4 view = mat4(xAxis.x, yAxis.x, zAxis.x, 0,
					 xAxis.y, yAxis.y, zAxis.y, 0,
					 xAxis.z, yAxis.z, zAxis.z, 0,
					 dot(-xAxis, eyePoint), dot(-yAxis, eyePoint), dot(-zAxis, eyePoint), 1);

	return view;
}

//Returns the projection matrix
mat4 getProjection()
{
	float aspectRatio = width / height;

	float top = zNear * tan((pi/180.0) * (FoV/2.0));
	float bottom = -top;
	float right = top * aspectRatio;
	float left = -right;

	/*
	float A = (2.0 * zNear)/(right - left);
	float B = (2.0 * zNear)/(top - bottom);
	float C = (right + left)/(right - left);
	float D = (top + bottom)/(top - bottom);
	float E = -(zFar + zNear)/(zFar - zNear);
	float F = -(2.0 * zFar * zNear)/(zFar - zNear);
	*/

	float A = zNear / right;
	float B = zNear / top;
	float C = -1.0 * ((zFar + zNear))/(zFar - zNear);
	float D = -2.0 * ((zFar * zNear)/(zFar - zNear));

	mat4 projection = mat4(A, 0, 0, 0,
						   0, B, 0, 0,
						   0, 0, C, -1, 
						   0, 0, D, 0);

	return projection;
}


void main()
{
	mat4 proj = getProjection();
    gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vPosition;

	//Shading
	vec4 worldCoord4v = modelMatrix * vPosition;
	vec3 worldCoord3v = vec3(worldCoord4v.x / worldCoord4v.w,
							 worldCoord4v.y / worldCoord4v.w,
							 worldCoord4v.z / worldCoord4v.w);

	mat4 rotation = mat4(modelMatrix[0][0], modelMatrix[1][0], modelMatrix[2][0], 0,
						 modelMatrix[0][1], modelMatrix[1][1], modelMatrix[2][1], 0,
						 modelMatrix[0][2], modelMatrix[1][2], modelMatrix[2][2], 0,
						 0, 0, 0, 1);
	vec4 worldNormal4v = rotation * vec4(vNormal, 1.0);
	vec3 worldNormal3v = vec3(worldNormal4v.x / worldNormal4v.w,
							  worldNormal4v.y / worldNormal4v.w,
							  worldNormal4v.z / worldNormal4v.w);

	L = normalize(lightPosition - worldCoord3v);
	E = normalize(eyePoint - worldCoord3v);
	H = normalize(L + E);
	N = normalize(worldNormal3v);

	texCoord = vTexCoord;
}

