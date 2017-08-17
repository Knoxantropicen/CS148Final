#version 400 core

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPos;

void main()
{
	gl_PointSize = 50.0f / max(length(position - cameraPos), 10.0f);
    gl_Position = projection * view * vec4(position, 1.0f);
}
