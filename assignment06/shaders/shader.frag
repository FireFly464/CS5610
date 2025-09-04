#version 330 core

uniform sampler2D tex;
uniform samplerCube envMap; 
layout(location = 0) out vec4 color;

in vec3 norm;
in vec3 reflectNorm;
in vec2 texCoord;
in vec3 position;
in vec3 camPos;



void main()
{
    vec3 reflectColor = normalize(vec3(1, 1, 1));
	float lightIntensity = 1.0f;
	float ambientIntensity = 0.2f;

	vec3 invCam = normalize((position - camPos));
	vec3 reflection = reflect(invCam, reflectNorm);

	vec3 camera = vec3(1,1,1);

    vec3 lightDir = normalize(vec3(1, 1, 1));

	vec3 n = normalize(norm);
    vec3 h = normalize(lightDir+camera);
	
	float cosPhi = max(dot(n, h), 0);

	float cosTheta = max(dot(n, lightDir), 0);
	vec3 tempTexture = vec3(texture(envMap, reflection));

    color = vec4(lightIntensity*((cosTheta*tempTexture) + (reflectColor*pow(cosPhi,10))) + ambientIntensity*tempTexture, 1);

}