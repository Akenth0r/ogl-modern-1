#version 330 core

out vec4 FragColor;

uniform vec4 fogColor; 
uniform float fogHeight;
uniform float fogDensity;


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

vec3 apply_fog(vec3 col, vec3 camToPoint, float distance)
{
    float factor = clamp(1.0, 0.0, (1 - exp(-distance * (fogHeight + camToPoint.y - 2.0) * fogDensity)));

    return mix(col, vec3(fogColor), factor);
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
    
    vec3 eyeToPosGlobal =  position_eyeCoord - position_global;
    float distance = length(eyeToPosGlobal);
    
    
    FragColor = vec4(final_color, 1.0);//vec4(apply_fog(final_color, eyeToPosGlobal, distance), mDiffuse.a); 
}

