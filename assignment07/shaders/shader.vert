#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 txc;

// layout(location = 3) in vec3 posSquare;


out vec3 norm;
out vec2 texCoord;

out vec4 lightView_Position;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view; 


uniform mat4 matrixShadow;
void main()
{
    mat4 mvp = projection * view * model;
    mat4 mv = inverse(model * view);


    gl_Position = (mvp * (vec4(pos,1)));

    norm = normals;

    texCoord = txc;


    lightView_Position = matrixShadow * vec4(pos, 1);
}