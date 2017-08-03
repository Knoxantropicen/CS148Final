#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in float time;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPos;

out vec3 objectColor;
out float lifeTime;

void main()
{
	gl_PointSize = 50.0f / length(position - cameraPos);
    gl_Position = projection * view * vec4(position, 1.0f);
    objectColor = color;
    lifeTime = time;
}
