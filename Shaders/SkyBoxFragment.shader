#version 450

out vec4 FragColor;

in vec3 texCoord;

layout(location = 0)uniform samplerCube skybox;

void main()
{
	FragColor = texture(skybox, texCoord);
}