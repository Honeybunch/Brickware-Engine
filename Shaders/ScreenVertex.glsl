#version 130

out vec2 uv;

void main()
{
	uv.x = (gl_VertexID == 2) ? 2.0 : 0.0;
	uv.y = (gl_VertexID == 0) ? 2.0 : 0.0;

	gl_Position.x = (gl_VertexID == 2) ? 3.0 : -1.0;
	gl_Position.y = (gl_VertexID == 0) ? 3.0 : -1.0;
	gl_Position.zw = vec2(0,1);
}
