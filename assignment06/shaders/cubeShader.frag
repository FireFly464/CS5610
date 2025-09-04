#version 330 core

uniform samplerCube env;

layout(location = 0) out vec4 color;

in vec3 dir;
in vec3 texCord;



void main()
{
    gl_FragColor = texture(env, texCord);
}