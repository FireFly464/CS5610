#version 330 core

uniform sampler2D tex;
uniform vec3 tempColor;
layout(location = 0) out vec4 color;


in vec3 norm;
in vec2 texCoord;

void main()
{
    vec3 reflectColor = normalize(vec3(1, 1, 1));
	float lightIntensity = 1.0f;
	vec3 camera = vec3(1, 1, 1);
	float ambientIntensity = 0.2f;

    vec3 lightDir = normalize(vec3(1, 1, 1));

	vec3 tempTexture = vec3(texture(tex, texCoord).xyz);

	vec3 n = normalize(tempTexture);
    vec3 h = normalize(lightDir+camera);
		
	float cosPhi = max(dot(n, h), 0);

	float cosTheta = max(dot(n, lightDir), 0);
	
	
	
	vec3 ambientLight = ambientIntensity*tempColor;
	vec3 specularLight =  reflectColor*pow(cosPhi,10);
	vec3 difuseLight = (cosTheta*tempColor);

    color = vec4((lightIntensity*(difuseLight + specularLight)) + ambientLight, 1);

}