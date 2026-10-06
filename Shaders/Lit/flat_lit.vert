#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 tFragPos;
out vec3 tNormal;
out vec3 tObjScale;
out vec3 tObjNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix;
uniform vec3 scale;

void main()
{
	gl_Position = vec4(aPos, 1.0f) * model * view * projection;
	tFragPos = vec3(vec4(aPos, 1.0f) * model);

	tNormal = aNormal * mat3(normalMatrix);

	tObjScale = scale;

	tObjNormal = aNormal;
}