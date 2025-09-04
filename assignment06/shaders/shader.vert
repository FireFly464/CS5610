#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 txc;


out vec3 norm;
out vec3 reflectNorm;
out vec2 texCoord;
out vec3 lightDir;
out vec3 camPos; 
out vec3 position;
out vec3 invMV;


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
    
    position = vec3(model * vec4(pos, 1.0));
    camPos = vec3(mv* vec4(pos, 1.0));

    reflectNorm = mat3(transpose(inverse(model))) * normals;


}