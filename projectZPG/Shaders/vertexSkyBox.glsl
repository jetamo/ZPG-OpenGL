#version 330 core
layout(location=0) in vec3 vp;
layout(location=1) in vec3 vertNormal;
layout(location=2) in vec2 in_uv;

out vec3 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 camPosition;

void main()
{
    TexCoords = vp;
     gl_Position = (projection * view * model) * vec4(vp, 1.0f);
}  