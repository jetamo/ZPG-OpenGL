#pragma once
class Texture
{
private:
	int textureID;
public:
	void createTexture(unsigned int shader) {
		//Bind the first texture to the first texture unit.
		glActiveTexture(GL_TEXTURE0);

		textureID = SOIL_load_OGL_texture(picture, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

		glBindTexture(GL_TEXTURE_2D, textureID);

		//Set texture unit to fragment shader
		GLint uniformID = glGetUniformLocation(shader, "textureUnitID");
		glUniform1i(uniformID, 0);
	}

	void bindTexture(unsigned int shader) {
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Set texture unit to fragment shader
		GLint uniformID = glGetUniformLocation(shader, "textureUnitID");
		glUniform1i(uniformID, 0);
	}
};

