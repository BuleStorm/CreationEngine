#include "PlantModel.h"
#include "GLWidget.h"
#include "Control.h"
#include "stb_image.h"

static Control* control = Control::getInstance();
static GLWidget* glWindow = GLWidget::getInstance();

Shader PlantModel::shader = Shader("./Shader/plant/plant.vs", "./Shader/plant/plant.fs");
glm::mat4 PlantModel::projection = glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 500.0f);
glm::mat4 PlantModel::view = glm::mat4(1.0f);


static const float vertices[] =
{
	-0.5f, -0.5f, 0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, 0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f,  1.0f, 0.0f,
};

static const unsigned int indices[] =
{
	3,  0,  2,
	1,  2,  0,
};


PlantModel::PlantModel(const char* textureStr)
{
	this->textureStr = textureStr;
}

PlantModel::~PlantModel()
{
}

void PlantModel::drawStart()
{
	shader.use();
	view = glm::lookAt(control->cameraPos, control->cameraPos + control->cameraFront, control->cameraUp);
	shader.setMat4("view", view);
}

void PlantModel::init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &painterVBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, painterVBO);
	glBufferData(GL_ARRAY_BUFFER, painterCount * sizeof(glm::mat4), &painterVec[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)0);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (GLvoid*)(3 * sizeof(glm::vec4)));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);


	char texturePath[60] = "./Textures/";
	strcat_s(texturePath, textureStr);
	texture = loadTexture(texturePath);


	shader.use();
	shader.setMat4("projection", projection);
}

void PlantModel::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, painterVBO);
	void* buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	memset(buffer, 0, painterCount * sizeof(glm::mat4));
	memcpy(buffer, &painterVec[0], painterCount * sizeof(glm::mat4));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0, painterCount);
}

void PlantModel::push(int x, int y, int z)
{
	painterCount++;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, { x, y, z});
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	painterVec.push_back(model);

	painterCount++;
	model = glm::mat4(1.0f);
	model = glm::translate(model, { x, y, z});
	model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	painterVec.push_back(model);
}

void PlantModel::clear()
{
	painterCount = 0;
	painterVec.clear();
}

unsigned int PlantModel::loadTexture(const char* path)
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