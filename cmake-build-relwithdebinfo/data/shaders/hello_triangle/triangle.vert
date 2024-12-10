#version 300 es
precision highp float;

out vec3 fragColor;
out vec2 TexCoord;

layout (location = 1) in vec2 aTexCoord;

uniform mat4 transform;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//vec2 triangle[3] = vec2[](
//vec2(0.0, -0.5),
//vec2(0.4, 0.5),
//vec2(-0.4, 0.5)
//);

vec3 square[36] = vec3[](
vec3(-0.5f, -0.5f, -0.5f),
vec3(0.5f, -0.5f, -0.5f),
vec3(0.5f,  0.5f, -0.5f),
vec3(0.5f,  0.5f, -0.5f),
vec3(-0.5f,  0.5f, -0.5f),
vec3(-0.5f, -0.5f, -0.5f),

vec3(-0.5f, -0.5f,  0.5f),
vec3(0.5f, -0.5f,  0.5f),
vec3(0.5f,  0.5f,  0.5f),
vec3(0.5f,  0.5f,  0.5f),
vec3(-0.5f,  0.5f,  0.5f),
vec3(-0.5f, -0.5f,  0.5f),

vec3(-0.5f,  0.5f,  0.5f),
vec3(-0.5f,  0.5f, -0.5f),
vec3(-0.5f, -0.5f, -0.5f),
vec3(-0.5f, -0.5f, -0.5f),
vec3(-0.5f, -0.5f,  0.5f),
vec3(-0.5f,  0.5f,  0.5f),

vec3(0.5f,  0.5f,  0.5f),
vec3(0.5f,  0.5f, -0.5f),
vec3(0.5f, -0.5f, -0.5f),
vec3(0.5f, -0.5f, -0.5f),
vec3(0.5f, -0.5f,  0.5f),
vec3(0.5f,  0.5f,  0.5f),

vec3(-0.5f, -0.5f, -0.5f),
vec3(0.5f, -0.5f, -0.5f),
vec3(0.5f, -0.5f,  0.5f),
vec3(0.5f, -0.5f,  0.5f),
vec3(-0.5f, -0.5f,  0.5f),
vec3(-0.5f, -0.5f, -0.5f),

vec3(-0.5f,  0.5f, -0.5f),
vec3(0.5f,  0.5f, -0.5f),
vec3(0.5f,  0.5f,  0.5f),
vec3(0.5f,  0.5f,  0.5f),
vec3(-0.5f,  0.5f,  0.5f),
vec3(-0.5f,  0.5f, -0.5f)
);

vec3 colors[3] = vec3[](
vec3(1.0, 0.5, 0.7),
vec3(0.0, 1.0, 0.9),
vec3(0.0, 0.3, 0.1)
);

vec2 texCoords[36] = vec2[](
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0),
vec2(0.0, 0.0),
vec2(1.0, 0.0),
vec2(1.0, 1.0)
);

void main()
{
    gl_Position = projection * view * model * vec4(square[gl_VertexID], 1.0);
    fragColor = colors[gl_VertexID];
    TexCoord = texCoords[gl_VertexID];
}