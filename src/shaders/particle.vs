#version 400 core

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
layout (location = 1) in vec3 color;

uniform mat4 view;
uniform mat4 projection;

out vec3 objectColor;

void main()
{
	gl_PointSize = 10.0;
    gl_Position = projection * view * vec4(position, 1.0f);
    objectColor = color;
}
