#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "soil/SOIL.h"
#include <string>
#include "glm/mat4x4.hpp"
#include "Observer.h"
class Texture
{
private:
	int textureID;
public:
	Texture() {}

	Texture(const char* picture) {
		createTexture(picture);
	}

	void createTexture(const char* picture) {
		//Bind the first texture to the first texture unit.
		glActiveTexture(GL_TEXTURE0);
		textureID = SOIL_load_OGL_texture(picture, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	}

	void bindTexture(unsigned int shader) {
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Set texture unit to fragment shader
		GLint uniformID = glGetUniformLocation(shader, "textureUnitID");
		glUniform1i(uniformID, 0);
	}
};

