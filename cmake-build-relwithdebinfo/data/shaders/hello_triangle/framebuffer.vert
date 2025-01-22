#version 300 es
precision mediump float;

layout (location = 0) in vec2 inPos;         // Vertex position input
layout (location = 1) in vec2 inTexCoords;  // Texture coordinates input

//ALL CW
//vec2 vertices[6] = vec2[](
//vec2(-1.0f,  1.0f),
//vec2(-1.0f, -1.0f),
//vec2(1.0f, -1.0f),
//
//vec2(-1.0f,  1.0f),
//vec2(1.0f, -1.0f),
//vec2(1.0f,  1.0f)
//);
//
//vec2 position_of_the_texture[6] = vec2[](
//vec2(0.0f, 1.0f),
//vec2(0.0f, 0.0f),
//vec2(1.0f, 0.0f),
//
//vec2(0.0f, 1.0f),
//vec2(1.0f, 0.0f),
//vec2(1.0f, 1.0f)
//);

out vec2 TexCoords;  // Pass texture coordinates to fragment shader

void main() {
    gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0); // Use inPos
    TexCoords = inTexCoords;                       // Pass inTexCoords to fragment shader
}