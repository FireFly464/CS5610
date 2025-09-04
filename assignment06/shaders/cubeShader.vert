#version 330 core

layout(location = 0) in vec3 posCube;
layout(location = 1) in vec3 txc;



uniform mat4 model;
uniform mat4 projection;
uniform mat4 view; 

out vec3 dir;
out vec3 texCord;



void main()
{
    mat4 mvp = projection * view * model;
    mat4 mv = model * view;


    gl_Position = (mvp*(vec4(posCube,1)));

    dir = vec3(mv*vec4(posCube, 1));

    texCord = txc;
}