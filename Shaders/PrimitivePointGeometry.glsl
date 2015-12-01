#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 6) out;

uniform float size;
uniform float aspectRatio;

void main()
{
	vec4 center = gl_in[0].gl_Position;

	float sizeX = size;
	float sizeY = size * aspectRatio;

	//Offsets for basic square
	vec4 offset[4] =
		vec4[](
		vec4(-sizeX, sizeY, 0.0, 0.0),
		vec4(sizeX, sizeY, 0.0, 0.0),
		vec4(sizeX, -sizeY, 0.0, 0.0),
		vec4(-sizeX, -sizeY, 0.0, 0.0));

	//1st triangle
	gl_Position = (center + offset[3]);
	EmitVertex();
	gl_Position = (center + offset[2]);
	EmitVertex();
	gl_Position = (center + offset[1]);
	EmitVertex();

	EndPrimitive();

	//2nd triangle
	gl_Position = (center + offset[3]);
	EmitVertex();
	gl_Position = (center + offset[1]);
	EmitVertex();
	gl_Position = (center + offset[0]);
	EmitVertex();

	EndPrimitive();
}
