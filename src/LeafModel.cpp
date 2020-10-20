#include "LeafModel.h"
#include "GLWidget.h"
#include "Control.h"
#include "stb_image.h"

static Control* control = Control::getInstance();
static GLWidget* glWindow = GLWidget::getInstance();


glm::mat4 LeafModel::projection = glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 500.0f);
glm::mat4 LeafModel::view = glm::mat4(1.0f);

Shader LeafModel::shader = Shader("./resource/Shader/leaf/leaf.vs", "./resource/Shader/leaf/leaf.fs");


static const float vertices[] =
{
	// 前后
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,

	// 左右
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,

	// 上下
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
};

static const unsigned int indices[] =
{
	// 后前
	3,  0,  2,
	1,  2,  0,
	7,  5,  6,
	4,  7,  6,
	// 左右 
	11, 10,  8,
	11,  8,  9,
	13, 14, 15,
	14, 13, 12,
	// 上下
	18, 21, 19,
	19, 21, 23,
	22, 17, 16,
	20, 22, 16,
};

LeafModel::LeafModel(const char* textureStr)
{
	this->textureStr = textureStr;
}

LeafModel::~LeafModel()
{
}

void LeafModel::drawStart()
{
	shader.use();
	view = glm::lookAt(control->cameraPos, control->cameraPos + control->cameraFront, control->cameraUp);
	shader.setMat4("view", view);
}

void LeafModel::init()
{
	painterVec.resize(painterCount);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &painterVBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, painterVBO);
	glBufferData(GL_ARRAY_BUFFER, painterCount * sizeof(glm::mat4), &(painterVec[0]), GL_STATIC_DRAW);

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


	char texturePath[60] = "./resource/Textures/";
    strcat(texturePath, textureStr);
	texture = loadTexture(texturePath);


	shader.use();
	shader.setMat4("projection", projection);
}

void LeafModel::draw()
{
	if (painterCount == 0)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, painterVBO);
	void* buffer = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	memset(buffer, 0, painterCount * sizeof(glm::mat4));
	memcpy(buffer, &painterVec[0], painterCount * sizeof(glm::mat4));
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElementsInstanced(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0, painterCount);
}

void LeafModel::push(int x, int y, int z)
{
	painterCount++;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x, y, z));

	painterVec.push_back(model);
}

void LeafModel::clear()
{
	painterCount = 0;
	painterVec.clear();
}
