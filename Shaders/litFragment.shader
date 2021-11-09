#version 450

out vec4 FragColor;
in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Normal;
in vec3 Tangent;
in vec3 BiTangent;

layout(location = 0) uniform sampler2D diffuseTexture;
layout(location = 1)uniform sampler2D specularTexture;
layout(location = 2)uniform sampler2D normalTexture;
layout(location = 3)uniform sampler2D ambientTexture;
layout(location = 4)uniform sampler2D roughTexture;
layout(location = 5)uniform sampler2D displacementTexture;

uniform float diffuseStrength;
uniform float specularStrength;
uniform float normalStrength;
uniform float ambientStrength;
uniform float roughStrength;
uniform float displacementStrength;
uniform int pointLightTotal;

uniform vec3 viewPos;

const float PI = 3.14159265359;

struct DirectionalLight
{
	vec3 direction;

	vec3 ambient;
	vec3 albedo;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 albedo;
	vec3 specular;

	float strength;
	float constant;
	float linear;
	float quadratic;
};
#define MAX_LIGHTS 32
uniform PointLight pointLights[MAX_LIGHTS];
uniform DirectionalLight dirLight;

vec3 getNormalFromMap()
{
	vec3 tangentNormal = (texture(normalTexture, TexCoord).xyz) * 2.0 - 1.0;
	tangentNormal.z *= normalStrength;
	tangentNormal = normalize(tangentNormal);

	vec3 N = normalize(Normal);
	mat3 TBN = mat3(normalize(Tangent), normalize(BiTangent), N);

	return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 CalculatespotLight(PointLight light, vec3 V, vec3 N, vec3 F0, float roughness, float metallic, vec3 albedo)
{
	// calculate per-light radiance
	vec3 L = normalize(light.position - WorldPos);
	vec3 H = normalize(V + L);
	float distance = length(light.position - WorldPos);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = light.albedo * attenuation * light.strength;

	// Cook-Torrance BRDF
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
	vec3 specular = nominator / denominator;

	// kS is equal to Fresnel
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	// scale light by NdotL
	float NdotL = max(dot(N, L), 0.0);

	// add to outgoing radiance Lo
	return (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}

void main()
{
	vec3 albedo = pow(texture(diffuseTexture, TexCoord).rgb, vec3(2.2)) * diffuseStrength;
	float metallic = texture(specularTexture, TexCoord).r * specularStrength;
	float ao = texture(ambientTexture, TexCoord).r * ambientStrength;
	float roughness = texture(roughTexture, TexCoord).r * roughStrength;

	vec3 N = getNormalFromMap();
	vec3 V = normalize(viewPos - WorldPos);

	// calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
	// of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);

	// Directional Light (attempt)
	vec3 L = normalize(-dirLight.direction);
	vec3 H = normalize(V + L);

	vec3 radiance = dirLight.albedo;

	float NDF = DistributionGGX(N, H, roughness);
	float NoL = clamp(dot(N, L), 0.0, 1.0);
	float illuminance = 0.5 * NoL;

	vec3 Q = normalize(V + L);
	vec3 F = fresnelSchlick(max(dot(Q, V), 0.0), F0);

	float G = GeometrySmith(N, V, L, roughness);

	vec3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
	vec3 specular = nominator / denominator;

	// kS is equal to Fresnel
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	// scale light by NdotL
	float NdotL = max(dot(N, L), 0.0);

	// add to outgoing radiance Lo
	vec3 dirLightResult = (kD * albedo / PI + specular) * radiance * NdotL;

	Lo += dirLightResult;

	for (int i = 0; i < pointLightTotal; ++i)
	{
		Lo += CalculatespotLight(pointLights[i], V, N, F0, roughness, metallic, albedo);
	}

	vec3 ambient = vec3(0.1) * albedo * ao; //* F;

	vec3 color = ambient + Lo;

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(albedo, 1);
	//FragColor = texture(diffuseTexture, TexCoord);
}