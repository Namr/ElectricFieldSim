#version 150 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec4 objColor;
uniform int activeBlues;
uniform int activeReds;
uniform vec3 redPositions[10];
uniform vec3 bluePositions[10];

out vec4 outColor;

vec3 computeDiffuse(vec3 color, vec3 pos);

void main()
{
	vec3 redColor = vec3(1.0, 0.0, 0.0);
	vec3 blueColor = vec3(0.0, 0.0, 1.0);
	vec3 finalColor = vec3(0.0, 0.0, 0.0);
	
	for(int i = 0; i < activeReds; i++)
	{
		vec3 redDiffuse = computeDiffuse(redColor, redPositions[i]);
		finalColor += redDiffuse * objColor.rgb;
	}

	for(int i = 0; i < activeBlues; i++)
	{
		vec3 blueDiffuse = computeDiffuse(blueColor, bluePositions[i]);
		finalColor += blueDiffuse * objColor.rgb;
	}
	
	outColor = vec4(finalColor, objColor.a);
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