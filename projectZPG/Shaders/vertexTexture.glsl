#version 330
layout(location=0) in vec3 vp;
layout(location=1) in vec3 vertNormal;
layout(location=2) in vec2 in_uv;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 camPosition;
out vec3 ex_worldPosition;
out vec3 ex_worldNormal;
out vec3 ex_fragPos;
out vec3 ex_camPosition;
out vec2 uv;
void main () {
     gl_Position = (projection * view * model) * vec4(vp, 1.0f);
     mat3 normalMatrix = transpose(inverse(mat3(model)));
	  ex_worldPosition = vec3(model * vec4(vp, 1.0f));
	  ex_worldNormal = normalMatrix * vertNormal;
	  ex_camPosition = camPosition;
	  uv = in_uv;
}