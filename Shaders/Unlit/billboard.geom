#version 330 core

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 geomViewProj;
uniform vec3 geomCameraUp;
uniform vec3 geomCameraRight;
uniform vec2 geomScale;

out vec2 TexCoord;

void main()
{
	vec3 pos = gl_in[0].gl_Position.xyz;
	vec3 up = geomCameraUp * geomScale.y;
	vec3 right = geomCameraRight * geomScale.x;

	// Bottom left vertex
	pos -= up / 2.0;
	pos -= right / 2.0;
	gl_Position = vec4(pos, 1.0) * geomViewProj;
	TexCoord = vec2(0.0, 1.0);
	EmitVertex();

	// Top left vertex
	pos += up;
	gl_Position =vec4(pos, 1.0) * geomViewProj;
	TexCoord = vec2(0.0, 0.0);
	EmitVertex();

	// Bottom right vertex
	pos -= up;
	pos += right;
	gl_Position =vec4(pos, 1.0) * geomViewProj;
	TexCoord = vec2(1.0, 1.0);
	EmitVertex();

	// Top right vertex
	pos += up;
	gl_Position =vec4(pos, 1.0) * geomViewProj;
	TexCoord = vec2(1.0, 0.0);
	EmitVertex();

	EndPrimitive();
}