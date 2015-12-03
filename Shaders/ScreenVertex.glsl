#version 130

out vec2 uv;

void main()
{
	uv.x = (gl_VertexID == 2) ? 2.0 : 0.0;
	uv.y = (gl_VertexID == 1) ? 2.0 : 0.0;

	gl_Position = vec4(uv * vec2(2.0, -2.0) + vec2(-1, 1), 1.0, 1.0);
}