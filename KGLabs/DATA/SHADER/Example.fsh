#version 330 core

out vec4 FragColor;

uniform vec4 lDirection;
uniform vec4 lAmbient;
uniform vec4 lDiffuse;
uniform vec4 lSpecular;

uniform vec4 mAmbient;
uniform vec4 mDiffuse;
uniform vec4 mSpecular;
uniform float mShininess;

// Samplers
uniform sampler2D tex0;

// Eye pos
uniform vec3 eye_pos;

in vec3 normal;
in vec3 position_eyeCoord;
in vec3 position_global;
in vec2 fTexCoord;
// Fog density
uniform vec3 fog_color; 
uniform float fog_density;
uniform float fog_near;
uniform float fog_far;


vec3 apply_fog(vec3 col, float distance)
{
    float factor = (distance - fog_near)/(fog_far - fog_near);
    if (distance >= fog_far) factor = 1;
    if (distance <= fog_near) factor = 0;
    return mix(col, fog_color, factor);
}

void main (void)
{   
    vec3 n_Normal = normalize(normal);
    vec3 n_ToEye = normalize(-position_eyeCoord);
    vec3 n_ToLight = normalize(vec3(lDirection));
    vec3 n_Reflect = normalize(reflect(-n_ToLight, n_Normal));
    
    vec4 ambient = mAmbient * lAmbient;
    vec4 diffuse = mDiffuse * lDiffuse * max(dot(n_Normal, n_ToLight), 0.0);
    vec4 specular = mSpecular * lSpecular * pow(max(dot(n_ToEye, n_Reflect), 0.0), mShininess);
    vec3 final_color = vec3(texture(tex0, fTexCoord)) * vec3(ambient + diffuse + specular);
    
    vec3 eyeToPosGlobal =  eye_pos - position_eyeCoord;
    float distance = length(eyeToPosGlobal);
    
    
    FragColor = vec4(apply_fog(final_color, distance), mDiffuse.a); 
}

