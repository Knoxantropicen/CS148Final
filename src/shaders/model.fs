#version 400 core

in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;

uniform vec3 viewPos;

out vec4 color;

void main()
{
    // Diffuse
    color = texture(texture_diffuse1, TexCoords);
}
