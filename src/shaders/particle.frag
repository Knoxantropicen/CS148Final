#version 410 core

in vec3 objectColor;

out vec4 color;

//uniform vec3 objectColor;

void main()
{
    color = vec4(objectColor, 1.0f);
    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
