#version 330 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 vertexUV;

out vec2 UV;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(pos,1);
	UV = vertexUV;
	Normal = mat3(transpose(inverse(model))) * normal;
	FragPos = vec3(model * vec4(pos,1));
}
