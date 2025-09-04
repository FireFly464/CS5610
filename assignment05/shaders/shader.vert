#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 txc;

out vec3 norm;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view; 
void main()
{
    mat4 mvp = projection * view * model;
    mat4 mv = inverse(model * view);


    gl_Position = (mvp * (vec4(pos,1)));
    norm = normals;

    texCoord = txc;
}