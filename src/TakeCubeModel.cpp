#include "TakeCubeModel.h"
#include "Control.h"
#include "GLWidget.h"

#include "stb_image.h"

unsigned int TakeCubeModel::firstPersonVAO = 0;
unsigned int TakeCubeModel::firstPersonVBO = 0;

unsigned int TakeCubeModel::thirdPersonVAO = 0;
unsigned int TakeCubeModel::thirdPersonVBO = 0;

static Control* control = Control::getInstance();

glm::mat4 TakeCubeModel::projection = glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 500.0f);
glm::mat4 TakeCubeModel::view = glm::mat4(1.0f);

Shader TakeCubeModel::shader = Shader("./Shader/take/take.vs", "./Shader/take/take.fs");

// 第一人称顶点数据
static const float firstPersonVertices[] =
{
		// 位置                  // 纹理
	   -0.20f, -0.125f,  0.5f,  1.0f,  1.0f,
		0.05f, -0.125f,  0.5f,  0.0f,  1.0f,
		0.05f, -0.375f,  0.5f,  0.0f,  0.0f,
		0.05f, -0.375f,  0.5f,  0.0f,  0.0f,
	   -0.20f, -0.375f,  0.5f,  1.0f,  0.0f,
	   -0.20f, -0.125f,  0.5f,  1.0f,  1.0f,

	   -0.20f, -0.375f,  0.5f,  0.0f,  0.0f,
	   -0.20f, -0.375f,  1.0f,  1.0f,  0.0f,
	   -0.20f, -0.125f,  1.0f,  1.0f,  1.0f,
	   -0.20f, -0.125f,  1.0f,  1.0f,  1.0f,
	   -0.20f, -0.125f,  0.5f,  0.0f,  1.0f,
	   -0.20f, -0.375f,  0.5f,  0.0f,  0.0f,

	   -0.20f, -0.125f,  1.0f,  1.0f,  1.0f,
		0.05f, -0.125f,  1.0f,  0.0f,  1.0f,
		0.05f, -0.375f,  1.0f,  0.0f,  0.0f,
		0.05f, -0.375f,  1.0f,  0.0f,  0.0f,
	   -0.20f, -0.375f,  1.0f,  1.0f,  0.0f,
	   -0.20f, -0.125f,  1.0f,  1.0f,  1.0f,

		0.05f, -0.375f,  0.5f,  0.0f,  0.0f,
		0.05f, -0.375f,  1.0f,  1.0f,  0.0f,
		0.05f, -0.125f,  1.0f,  1.0f,  1.0f,
		0.05f, -0.125f,  1.0f,  1.0f,  1.0f,
		0.05f, -0.125f,  0.5f,  0.0f,  1.0f,
		0.05f, -0.375f,  0.5f,  0.0f,  0.0f,

	   -0.20f, -0.125f,  1.0f,  1.0f,  0.0f,
		0.05f, -0.125f,  1.0f,  0.0f,  0.0f,
		0.05f, -0.125f,  0.5f,  0.0f,  1.0f,
		0.05f, -0.125f,  0.5f,  1.0f,  1.0f,
	   -0.20f, -0.125f,  0.5f,  0.0f,  1.0f,
	   -0.20f, -0.125f,  1.0f,  0.0f,  0.0f,

	   -0.20f, -0.375f,  1.0f,  0.0f,  1.0f,
		0.05f, -0.375f,  1.0f,  1.0f,  1.0f,
		0.05f, -0.375f,  0.5f,  1.0f,  0.0f,
		0.05f, -0.375f,  0.5f,  1.0f,  0.0f,
	   -0.20f, -0.375f,  0.5f,  0.0f,  0.0f,
	   -0.20f, -0.375f,  1.0f,  0.0f,  1.0f
};


// 第三人称顶点数据
static const float thirdPersonVertices[] =
{
	   -0.55f, -0.90f,  0.45f,  1.0f,  1.0f,
	   -0.20f, -0.90f,  0.45f,  0.0f,  1.0f,
	   -0.20f, -0.60f,  0.45f,  0.0f,  0.0f,
	   -0.20f, -0.60f,  0.45f,  0.0f,  0.0f,
	   -0.55f, -0.60f,  0.45f,  1.0f,  0.0f,
	   -0.55f, -0.90f,  0.45f,  1.0f,  1.0f,
	   
	   -0.55f, -0.60f,  0.45f,  0.0f,  0.0f,
	   -0.55f, -0.60f,  0.15f,  1.0f,  0.0f,
	   -0.55f, -0.90f,  0.15f,  1.0f,  1.0f,
	   -0.55f, -0.90f,  0.15f,  1.0f,  1.0f,
	   -0.55f, -0.90f,  0.45f,  0.0f,  1.0f,
	   -0.55f, -0.60f,  0.45f,  0.0f,  0.0f,
	   
	   -0.55f, -0.90f,  0.15f,  1.0f,  1.0f,
	   -0.20f, -0.90f,  0.15f,  0.0f,  1.0f,
	   -0.20f, -0.60f,  0.15f,  0.0f,  0.0f,
	   -0.20f, -0.60f,  0.15f,  1.0f,  0.0f,
	   -0.55f, -0.60f,  0.15f,  0.0f,  0.0f,
	   -0.55f, -0.90f,  0.15f,  0.0f,  1.0f,
	   
	   -0.20f, -0.60f,  0.45f,  0.0f,  0.0f,
	   -0.20f, -0.60f,  0.15f,  1.0f,  0.0f,
	   -0.20f, -0.90f,  0.15f,  1.0f,  1.0f,
	   -0.20f, -0.90f,  0.15f,  1.0f,  1.0f,
	   -0.20f, -0.90f,  0.45f,  0.0f,  1.0f,
	   -0.20f, -0.60f,  0.45f,  0.0f,  0.0f,
	   
	   -0.55f, -0.90f,  0.15f,  1.0f,  0.0f,
	   -0.20f, -0.90f,  0.15f,  0.0f,  0.0f,
	   -0.20f, -0.90f,  0.45f,  0.0f,  1.0f,
	   -0.20f, -0.90f,  0.45f,  1.0f,  1.0f,
	   -0.55f, -0.90f,  0.45f,  0.0f,  1.0f,
	   -0.55f, -0.90f,  0.15f,  0.0f,  0.0f,
	   
	   -0.55f, -0.60f,  0.15f,  0.0f,  1.0f,
	   -0.20f, -0.60f,  0.15f,  1.0f,  1.0f,
	   -0.20f, -0.60f,  0.45f,  1.0f,  0.0f,
	   -0.20f, -0.60f,  0.45f,  1.0f,  0.0f,
	   -0.55f, -0.60f,  0.45f,  0.0f,  0.0f,
	   -0.55f, -0.60f,  0.15f,  0.0f,  1.0f
};


TakeCubeModel::TakeCubeModel(const char* textureStr)
{
	this->textureStr = textureStr;
}

TakeCubeModel::~TakeCubeModel()
{
}

void TakeCubeModel::init()
{
	glGenVertexArrays(1, &firstPersonVAO);
	glGenBuffers(1, &firstPersonVBO);

	glBindVertexArray(firstPersonVAO);
	glBindBuffer(GL_ARRAY_BUFFER, firstPersonVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstPersonVertices), firstPersonVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &thirdPersonVAO);
	glGenBuffers(1, &thirdPersonVBO);

	glBindVertexArray(thirdPersonVAO);
	glBindBuffer(GL_ARRAY_BUFFER, thirdPersonVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(thirdPersonVertices), thirdPersonVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	char texturePath[60] = "./Textures/";
	strcat_s(texturePath, textureStr);
	texture = loadTexture(texturePath);

	shader.use();
	shader.setInt("texture1", 0);
	shader.setMat4("projection", projection);
}

void TakeCubeModel::firstPersonDraw(float eyeRadians, float bodyRadians, float limbRadians, float placeValue, float takeValue)
{
	shader.use();
	view = glm::lookAt(control->cameraPos, control->cameraPos + control->cameraFront, control->cameraUp);
	shader.setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, control->cameraPos);
	model = glm::rotate(model, glm::radians(bodyRadians - 20 + placeValue), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(eyeRadians - placeValue * 0.5f + takeValue), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(limbRadians * 0.025f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(firstPersonVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void TakeCubeModel::thirdPersonDraw(const glm::vec3& pos, float eyeRadians, float bodyRadians, float limbRadians, float placeValue, float takeValue)
{
	shader.use();
	view = glm::lookAt(control->cameraPos, control->cameraPos + control->cameraFront, control->cameraUp);
	shader.setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, { pos.x, pos.y + 0.975, pos.z });
	model = glm::rotate(model, glm::radians(bodyRadians + placeValue), glm::vec3(0.0f, 1.0f, 0.0f));
	float limbRadiansTemp = limbRadians + 10 - placeValue * 3 - takeValue;
	model = glm::rotate(model, glm::radians(limbRadiansTemp < -35 ? -35.0f : limbRadiansTemp), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(thirdPersonVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

unsigned int TakeCubeModel::loadTexture(const char* path)
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
