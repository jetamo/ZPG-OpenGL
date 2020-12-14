#version 330
const int nrOfLights = 1;
struct light
{
	vec3 lightPos;	
	vec3 lightColor;
	vec3 direction;
	float radius;
	int type;
};

uniform light lights[nrOfLights];
uniform vec3 camDirection;
uniform sampler2D textureUnitID;
in vec3 ex_worldPosition;
in vec3 ex_worldNormal;
in vec3 ex_vector;
in vec3 ex_camPosition;
in vec2 uv;
out vec4 frag_colour;
void main () {
     frag_colour = vec4(0.0, 0.0, 0.0, 0.0);
	  for(int i = 0; i < nrOfLights; i++)
{
		if(lights[i].type == 1)
		{
			vec3 lightVector = lights[i].lightPos - ex_worldPosition;
			lightVector = normalize(lightVector);
			vec3 viewDirection = normalize(ex_camPosition - ex_worldPosition);
			vec3 reflectionVector = reflect(-lightVector, ex_worldNormal);
			vec4 specular = (pow(max(dot(normalize(viewDirection), normalize(reflectionVector)), 0.0), 32)) * vec4(0.385, 0.647, 0.812, 1.0);
			float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0);
			vec4 diffuse = dot_product * vec4(lights[i].lightColor, 1.0);
			frag_colour +=  diffuse + specular;
		}
		else if(lights[i].type == 2)
		{
			vec3 lightVector = ex_camPosition - ex_worldPosition;
			lightVector = normalize(lightVector);
		//					vec3 lightVector = lights[i].lightPos - ex_worldPosition;
		//					lightVector = normalize(lightVector);
		//					if(dot(lightVector, normalize(lights[i].lightPos - vec3(0, 0, 0))) > lights[i].radius)
					if(dot(lightVector, normalize(camDirection)) > 0.99f)
					{
						vec3 viewDirection = normalize(ex_camPosition - ex_worldPosition);
						vec3 reflectionVector = reflect(-lightVector, ex_worldNormal);
						vec4 specular = (pow(max(dot(normalize(viewDirection), normalize(reflectionVector)), 0.0), 32)) * vec4(0.385, 0.647, 0.812, 1.0);
						float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0);
						vec4 diffuse = dot_product * vec4(lights[i].lightColor, 1.0) * texture(textureUnitID, uv);
						frag_colour +=  diffuse + specular;
					}
					else
						frag_colour = vec4(0.0, 0.0, 0.0, 1.0);
				}
				else if(lights[i].type == 3)
				{
					vec3 lightVector = ex_camPosition - ex_worldPosition; 
					lightVector = normalize(lightVector);
					vec3 viewDirection = normalize(ex_camPosition - ex_worldPosition);
					vec3 reflectionVector = reflect(-lightVector, ex_worldNormal);
					vec4 specular = (pow(max(dot(normalize(viewDirection), normalize(reflectionVector)), 0.0), 32)) * vec4(0.385, 0.647, 0.812, 1.0);
					float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0);
					vec4 diffuse = dot_product * vec4(lights[i].lightColor, 1.0);
					frag_colour +=  diffuse + specular;
				}
		}

		vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0) * texture(textureUnitID, uv); 
		frag_colour += ambient;
		//frag_colour = texture(textureUnitID, uv);
		//     frag_colour += vec4(0.1, 0.0, 0.0, 1.0);
		};