#version 150 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 shadowMatrices[6];

out vec4 fragPos; //output per emitvertex

void main()
{
	for (int face = 0; face < 6; ++face)
	{
		gl_Layer = face; //gl_Layer is built in and specifies the face to render to
		for (int i = 0; i < 3; i++) //For each triangle's verts
		{
			fragPos = gl_in[i].gl_Position;
			gl_Position = shadowMatrices[face] * fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}
