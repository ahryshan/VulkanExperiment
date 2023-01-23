#version 450

vec2 vertices[3] = vec2[](
    vec2(0.0f, -0.5f),
    vec2(0.5f, 0.5f),
    vec2(-0.5f, 0.5f)
);

vec4 colors[3] = vec4[](
    vec4(0.8f, 0.2f, 0.2f, 1.0f),
    vec4(0.2f, 0.8f, 0.2f, 1.0f),
    vec4(0.2f, 0.2f, 0.8f, 1.0f)
);

layout (location = 0) out vec4 color;

void main() {
    gl_Position = vec4(vertices[gl_VertexIndex], 0, 1);
    color = colors[gl_VertexIndex];
}
