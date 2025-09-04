#version 330 core

uniform sampler2D renderBuffer;

layout(location = 0) out vec4 color;

in vec2 texCoordSquare;

void main()
{
    vec3 tempTexture = vec3(texture(renderBuffer, texCoordSquare).xyz);
    vec3 tempColor = vec3(0.5, 0.5, 1);
    vec3 colorTex = normalize(tempTexture + tempColor);


	color = vec4(texture(renderBuffer, texCoordSquare));
}