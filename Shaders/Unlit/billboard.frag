#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D billboardTexture;
uniform vec3 tintColor;

void main()
{
	FragColor = vec4(tintColor, 1.0) * texture2D(billboardTexture, TexCoord);
}