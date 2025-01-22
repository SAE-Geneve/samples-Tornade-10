#version 300 es
precision mediump float;

out vec4 FragColor;   // Output fragment color

in vec2 TexCoords;    // Texture coordinates passed from vertex shader

uniform sampler2D screenTexture;  // 2D texture sampler

void main() {
    // Sample the texture at the given coordinates and output the color
    FragColor = texture(screenTexture, TexCoords);
}