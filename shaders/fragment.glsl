#version 150 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec4 objColor;

out vec4 outColor;

vec3 computeDiffuse(vec3 color, vec3 pos);

void main()
{
	vec3 lightColor = vec3(1.0, 0.0, 0.0);
	vec3 lightPos = vec3(50.0, 50.0, 50.0);

	vec3 blueColor = vec3(0.0, 0.0, 1.0);
	vec3 bluePos = vec3(80.0, 80.0, 80.0);
	
	vec3 diffuse = computeDiffuse(lightColor, lightPos);
	vec3 blueDiff = computeDiffuse(blueColor, bluePos);
	
	vec3 resultantColor = diffuse * objColor.rgb;
	vec3 resultBlue = blueDiff * objColor.rgb;
	
	outColor = vec4(resultantColor + resultBlue, objColor.a);
}

vec3 computeDiffuse(vec3 color, vec3 pos)
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pos - FragPos);

	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * color;
	
	float diff = max(dot(norm, lightDir), 0.0);

	return (diff * color) + ambient;
}