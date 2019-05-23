#version 150 core

in vec2 TexCoords;

uniform vec4 objColor;

out vec4 outColor;

void main()
{
	outColor = objColor;
}