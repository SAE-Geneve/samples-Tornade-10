#version 310 es
precision highp float;

layout (location = 0) in vec2 aPos;

out vec3 color;

vec2 square[4] = vec2[](
    vec2(5.0f, 0.0f),
    vec2(0.0f, -5.0f),
    vec2(-5.0f, 0.0f),
    vec2(0.0f, -5.0f)
);

//void main()
//{
//    gl_Position = vec4(aPos.x, aPos.y, 0, 1.0);
//    color = vec3(abs(aPos.x+0.5), abs(aPos.y+0.5), 1.0);
//}

void main() {
    gl_Position = vec4(square[gl_VertexID], 0.0, 1.0);
}