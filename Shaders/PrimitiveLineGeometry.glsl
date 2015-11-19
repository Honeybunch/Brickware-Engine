#version 330

layout(lines) in;
layout(triangle_strip, max_vertices = 6) out;

uniform float size;

void main()
{
	vec4 start = gl_in[0].gl_Position;
	vec4 end = gl_in[1].gl_Position;

	float startX = start.x;
	float startY = start.y;

	float endX = end.x;
	float endY = end.y;

	//Offsets for basic square
	vec4 points[4] =
		vec4[](
		vec4(startX, startY - size, 0.0, 0.0),	//Bottom Left
		vec4(startX, startY + size, 0.0, 0.0),	//Top Left
		vec4(endX, endY - size, 0.0, 0.0),		//Bottom Right
		vec4(endX, endY + size, 0.0, 0.0));		//Top Right

	//1st triangle
	gl_Position = points[3];
	EmitVertex();
	gl_Position = points[2];
	EmitVertex();
	gl_Position = points[1];
	EmitVertex();

	EndPrimitive();

	//2nd triangle
	gl_Position = points[3];
	EmitVertex();
	gl_Position = points[1];
	EmitVertex();
	gl_Position = points[0];
	EmitVertex();

	EndPrimitive();
}