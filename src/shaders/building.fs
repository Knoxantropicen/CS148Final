#version 400 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D tex;

void main() {
	color = texture(tex, TexCoord);
}