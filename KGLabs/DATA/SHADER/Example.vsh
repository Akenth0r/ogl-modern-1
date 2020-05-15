#version 330 core

uniform mat4 model[5];
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

out vec3 normal;
out vec3 position_eyeCoord;
out vec3 position_global;
out vec2 fTexCoord;

void main ()
{
	position_eyeCoord = vec3(view * model[gl_InstanceID] * vec4(vPosition, 1.0));
    position_global = vec3(model[gl_InstanceID] * vec4(vPosition, 1.0));
    normal = vec3(view * model[gl_InstanceID] * vec4(vNormal, 0.0));
    fTexCoord = vTexCoord;
	gl_Position = projection * view * model[gl_InstanceID] * vec4(vPosition, 1.0);
}
