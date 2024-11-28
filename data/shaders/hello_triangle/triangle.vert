#version 300 es
precision highp float;

out vec3 fragColor;
out vec2 TexCoord;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

uniform mat4 transform;

vec3 colors[3] = vec3[](
vec3(1.0, 1.0, 1.0),
vec3(1.0, 1.0, 1.0),
vec3(1.0, 1.0, 1.0)
);

vec2 triangle[3] = vec2[](
vec2(0.0, -0.5),
vec2(0.4, 0.5),
vec2(-0.4, 0.5)
);

vec2 square[6] = vec2[](
vec2(-0.3, -0.5), // bottom-left corner
vec2(0.3, 0.5),   // top-right corner
vec2(0.3, -0.5),  // bottom-right corner

vec2(-0.3, 0.5),   // top-left corner
vec2(-0.3, -0.5), // bottom-left corner
vec2(0.3, 0.5)   // top-right corner
);


vec2 texCoords[6] = vec2[](
vec2(1.0f, 1.0f), // up-right corner
vec2(0.0f, 0.0f), // down-left corner
vec2(0.0f, 1.0f), // down-right corner

vec2(1.0f, 0.0f), // up-left corner
vec2(1.0f, 1.0f), // up-right corner
vec2(0.0f, 0.0f) // down-left corner
);

void main()
{
    gl_Position = transform * vec4(square[gl_VertexID], 0.0, 1.0);
    fragColor = colors[gl_VertexID];
    TexCoord = texCoords[gl_VertexID];
    // TexCoord = vec2(texCoords.x, texCoords.y);
}