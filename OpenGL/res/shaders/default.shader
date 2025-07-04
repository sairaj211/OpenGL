#shader vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec3 a_Color;
layout(location = 3) in vec2 a_TexUV;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

void main()
{
    gl_Position = u_ViewProj * u_Model * vec4(a_Position, 1.0);
}


#shader fragment
#version 330 core

uniform vec4 u_Color;

out vec4 FragColor;

void main()
{
    FragColor = u_Color;
}