#version 150 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec4 objColor;

out vec4 outColor;

void main()
{
	vec3 lightColor = vec3(1.0, 0.0, 0.0);
	vec3 lightPos = vec3(50.0, 50.0, 50.0);
	
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	vec3 resultantColor = (ambient + diffuse) * objColor.rgb;
	outColor = vec4(resultantColor, objColor.a);
}