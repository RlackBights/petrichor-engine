#version 330 core

uniform sampler2D Texture;
uniform sampler2D SpecularMap;
uniform sampler2D NormalMap;
uniform vec4 baseCol;
uniform vec3 ambientLight;
uniform vec3 cameraPos;
uniform float specularStrength;
uniform int specularExponent;

struct PointLight // 8N
{
	vec3 pos; // 3 -> 4N
	vec3 col; // 3 -> 4N
};

struct DirectionalLight // 8N
{
	vec3 dir; // 3 -> 4N
	vec3 col; // 3 -> 4N
};

struct SpotLight // 12N
{
	vec3 pos; // 3 -> 4N
	vec3 dir; // 3 -> 4N
	vec3 col; // 3 -> 4N
};

layout (std140) uniform Lights // 1232 bytes
{
	int pointLightNum; // N
	PointLight pointLights[20]; // 160N
	
//	int directionalLightNum; // N
//	DirectionalLight directionalLights[3]; // 24N
//	
//	int spotLightNum; // N
//	SpotLight spotLights[10]; // 120N
};

const int maxLights = 20;
uniform int lightNum;
uniform vec3 lightPos[maxLights];
uniform vec3 lightCol[maxLights];

const int maxDirLights = 5;
uniform int dirLightNum;
uniform vec3 dirLightDir[maxDirLights];
uniform vec3 dirLightCol[maxDirLights];

const int maxSpotLights = 10;
uniform int spotLightNum;
uniform vec3 spotLightDir[maxSpotLights];
uniform vec3 spotLightPos[maxSpotLights];
uniform vec3 spotLightCol[maxSpotLights];
uniform float spotLightCutoff[maxSpotLights];
uniform float spotLightFocus[maxSpotLights];

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in mat4 ModelMatrix;
out vec4 FragColor;

void main()
{
	vec3 norm = normalize(Normal);

	vec3 diffuse = vec3(0.0f);
	vec3 specular = vec3(0.0f);
	vec3 viewDir = normalize(cameraPos - FragPos);

	for (int i = 0; i < lightNum; i++)
	{
		vec3 lightDir = normalize(lightPos[i] - FragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		diffuse += diff * lightCol[i] * (1 - smoothstep(0.0f, 20.0f, abs(length(FragPos - lightPos[i]))));
	
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), pow(2, specularExponent));
		specular += specularStrength * spec * lightCol[i];
	}

	for (int i = 0; i < dirLightNum; i++)
	{
		vec3 lightDir = normalize(-dirLightDir[i]);
		float diff = max(dot(norm, lightDir), 0.0);
		diffuse += diff * dirLightCol[i];

		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), pow(2, specularExponent));
		specular += specularStrength * spec * dirLightCol[i];
	}

	for (int i = 0; i < spotLightNum; i++)
	{
		vec3 lightDir = normalize(spotLightPos[i] - FragPos);
		float theta = dot(spotLightDir[i], -lightDir);
		float intensity = clamp((theta - (1 / (spotLightCutoff[i] + 1.001f))) * spotLightFocus[i], 0.0, 1.0);  
		if (theta > (1 / (spotLightCutoff[i] + 1.001f)))
		{
			float diff = max(dot(norm, lightDir), 0.0);
			diffuse += diff * spotLightCol[i] * intensity;
			
			vec3 reflectDir = reflect(-lightDir, norm);
			float spec = pow(max(dot(viewDir, reflectDir), 0.0), pow(2, specularExponent));
			specular += specularStrength * spec * spotLightCol[i] * intensity;
		}
	}
	
	if ((lightNum + dirLightNum + spotLightNum) > 0) {
		diffuse = diffuse / (lightNum + dirLightNum + spotLightNum);
		specular = specular / (lightNum + dirLightNum + spotLightNum);
	}

	FragColor = baseCol * vec4(((ambientLight + diffuse) * vec3(texture(Texture, TexCoord)) + specular * vec3(texture(SpecularMap, TexCoord))), texture(Texture, TexCoord).a);
}