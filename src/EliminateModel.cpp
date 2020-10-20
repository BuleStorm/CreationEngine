#include "EliminateModel.h"
#include "GLWidget.h"
#include "Control.h"

#include "stb_image.h"

static Control* control = Control::getInstance();

glm::mat4 EliminateModel::projection = glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 500.0f);
glm::mat4 EliminateModel::view = glm::mat4(1.0f);

unsigned int EliminateModel::painterVBO = 0;
unsigned int EliminateModel::VAO = 0;

Shader EliminateModel::shader = Shader("./resource/Shader/eliminate/eliminate.vs", "./resource/Shader/eliminate/eliminate.fs");


static const float vertices[] =
{
	// 后面				  // 纹理
	-0.05f, -0.05f, -0.05f,  1.0f, 1.0f,
	 0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
	 0.05f,  0.05f, -0.05f,  0.0f, 0.0f,
	 0.05f,  0.05f, -0.05f,  0.0f, 0.0f,
	-0.05f,  0.05f, -0.05f,  1.0f, 0.0f,
	-0.05f, -0.05f, -0.05f,  1.0f, 1.0f,

	-0.05f, -0.05f,  0.05f,  1.0f, 1.0f,
	 0.05f,  0.05f,  0.05f,  0.0f, 0.0f,
	 0.05f, -0.05f,  0.05f,  0.0f, 1.0f,
	 0.05f,  0.05f,  0.05f,  0.0f, 0.0f,
	-0.05f, -0.05f,  0.05f,  1.0f, 1.0f,
	-0.05f,  0.05f,  0.05f,  1.0f, 0.0f,

	-0.05f,  0.05f,  0.05f,  0.0f, 0.0f,
	-0.05f, -0.05f, -0.05f,  1.0f, 1.0f,
	-0.05f,  0.05f, -0.05f,  1.0f, 0.0f,
	-0.05f, -0.05f, -0.05f,  1.0f, 1.0f,
	-0.05f,  0.05f,  0.05f,  0.0f, 0.0f,
	-0.05f, -0.05f,  0.05f,  0.0f, 1.0f,

	 0.05f,  0.05f,  0.05f,  0.0f, 0.0f,
	 0.05f,  0.05f, -0.05f,  1.0f, 0.0f,
	 0.05f, -0.05f, -0.05f,  1.0f, 1.0f,
	 0.05f, -0.05f, -0.05f,  1.0f, 1.0f,
	 0.05f, -0.05f,  0.05f,  0.0f, 1.0f,
	 0.05f,  0.05f,  0.05f,  0.0f, 0.0f,

	-0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
	 0.05f, -0.05f,  0.05f,  1.0f, 0.0f,
	 0.05f, -0.05f, -0.05f,  1.0f, 1.0f,
	 0.05f, -0.05f,  0.05f,  1.0f, 0.0f,
	-0.05f, -0.05f, -0.05f,  0.0f, 1.0f,
	-0.05f, -0.05f,  0.05f,  0.0f, 0.0f,

	-0.05f,  0.05f, -0.05f,  0.0f, 1.0f,
	 0.05f,  0.05f, -0.05f,  1.0f, 1.0f,
	 0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
	 0.05f,  0.05f,  0.05f,  1.0f, 0.0f,
	-0.05f,  0.05f,  0.05f,  0.0f, 0.0f,
	-0.05f,  0.05f, -0.05f,  0.0f, 1.0f
};

EliminateModel::EliminateModel(const char* textureStr)
{
	this->textureStr = textureStr;
}

EliminateModel::~EliminateModel()
{
}

void EliminateModel::drawStart()
{
	shader.use();
	view = glm::lookAt(control->cameraPos, control->cameraPos + control->cameraFront, control->cameraUp);
	shader.setMat4("view", view);
}

void EliminateModel::init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &painterVBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, painterVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	char texturePath1[60] = "./resource/Textures/";
    strcat(texturePath1, textureStr);
	texture = loadTexture(texturePath1);


	shader.use();
	shader.setInt("texture1", 0);
	shader.setMat4("projection", projection);
}

void EliminateModel::draw(const glm::vec3& pos)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, pos);
	shader.setMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

unsigned int EliminateModel::loadTexture(const char* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = 3;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}

	stbi_image_free(data);
	return textureID;
}
