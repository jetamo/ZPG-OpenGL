#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "soil/SOIL.h"
#include <string>
#include "glm/mat4x4.hpp"
#include "Observer.h"

enum TextureType {
	TWOD = 0,
	SkyBox = 1
};

class Texture
{
private:
	int textureID;
	TextureType type;
public:
	Texture() {}

	Texture(TextureType _type, const char* picture = "") {
		type = _type;
		createTexture(picture);
	}

	void createTexture(const char* picture) {
		//Bind the first texture to the first texture unit.
		if (type == TextureType::TWOD) {
			glActiveTexture(GL_TEXTURE0);
			textureID = SOIL_load_OGL_texture(picture, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
		}
		else if (type == TextureType::SkyBox) {
			glActiveTexture(GL_TEXTURE0);
			textureID = SOIL_load_OGL_cubemap("posx.jpg", "negx.jpg", "posy.jpg", "negy.jpg", "posz.jpg", "negz.jpg", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, 0);
		}
	}

	void bindTexture(unsigned int shader) {
		if (type == TextureType::TWOD) {
			glBindTexture(GL_TEXTURE_2D, textureID);
		}
		else if (type == TextureType::SkyBox) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		}
		//Set texture unit to fragment shader
		GLint uniformID = glGetUniformLocation(shader, "textureUnitID");
		glUniform1i(uniformID, 0);
	}
};

