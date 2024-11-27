#version 300 es
precision highp float;

out vec3 fragColor;
out vec2 TexCoord;

const int circle_vertices = 96; // Number of vertices to approximate the circle
const float PI = 3.14159265359;

float circle_data[circle_vertices * 2];

vec3 colors[3] = vec3[](
vec3(1.0, 1.0, 1.0),
vec3(1.0, 1.0, 1.0),
vec3(1.0, 1.0, 1.0)
);

vec2 triangle[3] = vec2[](
vec2(0.0, -0.5),
vec2(0.5, 0.5),
vec2(-0.5, 0.5)
);

vec2 texCoords[3] = vec2[](
vec2(1.0f, 1.0f), // up-right corner
vec2(0.0f, 1.0f), // up-left corner
vec2(0.5f, 0.0f)   // top-center corner
);

void main()
{
    gl_Position = vec4(triangle[gl_VertexID], 0.0, 1.0);
    fragColor = colors[gl_VertexID];
    TexCoord = texCoords[gl_VertexID];
}