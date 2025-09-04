#version 330 core

layout(location = 0) in vec3 posSquare;
layout(location = 1) in vec2 txcSquare;


uniform mat4 mvpPlane;
out vec2 texCoordSquare;


void main()
{
    gl_Position = (mvpPlane*(vec4(posSquare,1)));

    texCoordSquare = txcSquare;

}