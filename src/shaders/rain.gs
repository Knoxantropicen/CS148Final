#version 400 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = gl_in[0].gl_Position + projection * view * vec4(0.0, -0.1, 0.0, 0.0);
    EmitVertex();

    gl_Position = gl_in[0].gl_Position + projection * view * vec4(0.0, 0.1, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}