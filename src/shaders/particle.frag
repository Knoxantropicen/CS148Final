#version 410 core

in vec3 objectColor;

out vec4 color;

uniform sampler2D tex;

void main()
{
    //color = texture(tex, gl_PointCoord) * vec4(objectColor, 1.0f);
    color = texture(tex, gl_PointCoord);
}
