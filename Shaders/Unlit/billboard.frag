#version 330 core

uniform sampler2D billboardTexture;

in vec2 TexCoord;
out vec4 FragColor;

void main()
{
	FragColor = texture2D(billboardTexture, TexCoord);
}