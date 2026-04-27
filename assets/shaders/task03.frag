#version 460

out vec4 outColor;

layout(location = 0) in vec3 in_Normal;
layout(location = 1) in vec3 in_ViewSpacePos;
layout(location = 2) in vec3 in_Color;


layout(location = 0) uniform mat4 u_ModelMat;
layout(location = 1) uniform mat4 u_ViewMat;
layout(location = 2) uniform mat4 u_ProjMat;
layout(location = 3) uniform vec3 u_LightPos;


void main()
{

    
    vec3 L = normalize(u_LightPos - in_ViewSpacePos);
    float diffuse = max(dot(normalize(in_Normal), L), 0.0);

    outColor = vec4(in_Color * diffuse, 1.0);
}
