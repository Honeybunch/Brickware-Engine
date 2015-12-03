#version 130

uniform sampler2D frame;

in vec2 uv;
out vec4 fragColor;

void main()
{
	fragColor = texture(frame, uv);
}