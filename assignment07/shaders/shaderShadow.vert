#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view; 
uniform mat4 mlp;
void main()
{
        mat4 mvp = projection * view * model;

    gl_Position = (mlp * (vec4(pos,1)));

}