#version 130

out float fragmentDepth;

void main()
{
	fragmentDepth = gl_FragCoord.z;
}