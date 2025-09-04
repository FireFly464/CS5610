#version 330 core

layout(location = 0) out vec4 color;

in vec3 norm;
void main()
{
    vec3 reflectColor = normalize(vec3(1, 1, 1));
	float lightIntensity = 1.0f;
	vec3 camera = vec3(1, 1, 1);
	float ambientIntensity = 0.2f;

    vec3 lightDir = normalize(vec3(1, 1, 1));

	vec3 n = normalize(norm);
    vec3 h = normalize(lightDir+camera);
		
	float cosPhi = max(dot(n, h), 0);

	float cosTheta = max(dot(n, lightDir), 0);
	vec3 tempTexture = normalize(vec3(0,0,1));


    color = vec4(lightIntensity*((cosTheta*tempTexture) + (reflectColor*pow(cosPhi,10))) + ambientIntensity*tempTexture, 1);

}