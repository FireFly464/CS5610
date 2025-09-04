#version 410 core

layout(vertices = 4) out;

in vec2 textureCoord[];
in vec3 positionCoord[];

out vec2 TexCoord[];
// out vec3 posCoord[];

uniform float triangleCount;
void main()
{

    gl_TessLevelOuter[0] = triangleCount;
    gl_TessLevelOuter[1] = triangleCount;
    gl_TessLevelOuter[2] = triangleCount;
    gl_TessLevelOuter[3] = triangleCount;

    gl_TessLevelInner[0] = triangleCount;
    gl_TessLevelInner[1] = triangleCount;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    TexCoord[gl_InvocationID] = textureCoord[gl_InvocationID];

}