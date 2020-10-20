#include "PersonModel.h"
#include "GLWidget.h"
#include "Control.h"

#include "PersonVertexData.h"

static Control* control = Control::getInstance();
static GLWidget* glWindow = GLWidget::getInstance();

glm::mat4 PersonModel::projection = glm::perspective(glm::radians(45.0f), Width / Height, 0.1f, 500.0f);
glm::mat4 PersonModel::view = glm::mat4(1.0f);

Shader PersonModel::shader = Shader("./resource/Shader/person/person.vs", "./resource/Shader/person/person.fs");

unsigned int PersonModel::headVAO[COUNT] = { 0 };
unsigned int PersonModel::headVBO[COUNT] = { 0 };

unsigned int PersonModel::bodyVAO[COUNT] = { 0 };
unsigned int PersonModel::bodyVBO[COUNT] = { 0 };

unsigned int PersonModel::armVAO[COUNT] = { 0 };
unsigned int PersonModel::armVBO[COUNT] = { 0 };

unsigned int PersonModel::legVAO[COUNT] = { 0 };
unsigned int PersonModel::legVBO[COUNT] = { 0 };

TakeCubeModel PersonModel::takeCube1("default_dirt.png");
TakeCubeModel PersonModel::takeCube2("default_desert_sand.png");
TakeCubeModel PersonModel::takeCube3("default_stone.png");
TakeCubeModel PersonModel::takeCube4("default_brick.png");
TakeCubeModel PersonModel::takeCube5("default_wood.png");
TakeCubeModel PersonModel::takeCube6("default_aspen_wood.png");
TakeCubeModel PersonModel::takeCube7("default_desert_sandstone_brick.png");
TakeCubeModel PersonModel::takeCube8("default_stone_brick.png");
TakeCubeModel PersonModel::takeCube9("default_silver_sandstone_brick.png");


PersonModel::PersonModel(const char* textureStr)
{
	this->textureStr = textureStr;
}

PersonModel::~PersonModel()
{
}

void PersonModel::takeCubeInit()
{
	PersonModel::takeCube1.init();
	PersonModel::takeCube2.init();
	PersonModel::takeCube3.init();
	PersonModel::takeCube4.init();
	PersonModel::takeCube5.init();
	PersonModel::takeCube6.init();
	PersonModel::takeCube7.init();
	PersonModel::takeCube8.init();
	PersonModel::takeCube9.init();
}

void PersonModel::init()
{
	char texturePath[50] = "./resource/Person/";
    strcat(texturePath, textureStr);
	img = cv::imread(texturePath);

	bindTexture(headVAO, headVBO, head, headTexture, 0);
	bindTexture(bodyVAO, bodyVBO, body, bodyTexture, 1);
	bindTexture(armVAO, armVBO, arm, armTexture, 2);
	bindTexture(legVAO, legVBO, leg, legTexture, 3);

	shader.use();
	shader.setInt("texture1", 0);
	shader.setMat4("projection", projection);
}

void PersonModel::draw(const glm::vec3& pos, bool isMy)
{
	runAnimation(pos);

	shader.use();
	view = glm::lookAt(control->cameraPos, control->cameraPos + control->cameraFront, control->cameraUp);
	shader.setMat4("view", view);

	// 第一人称显示
	if (isMy)
	{
		firstPersonDrawTakeCube();
	}
	// 第三人称显示
	else
	{
		thirdPersonDrawPerson(pos);
		thirdPersonDrawTakeCube(pos);
	}
}

void PersonModel::placeAnimtaion(bool& placeAnimation)
{
	if (placeAnimation)
	{
		placeAnimation = false;
		placeValue = 0;
		this->placeAnimation = Animation::GO;
	}
}

void PersonModel::takeCubeAnimation(CubeType nowTakeCube)
{
	if (this->newTakeCube != nowTakeCube)
	{
		this->newTakeCube = nowTakeCube;
		this->takeAnimation = Animation::GO;
		takeValue = 0;
	}
}

void PersonModel::thirdPersonDrawPerson(const glm::vec3& pos)
{
	// 头
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, { pos.x, pos.y + 1.125, pos.z });
	model = glm::rotate(model, glm::radians(bodyRadians), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(eyeRadians), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	for (int i = 0; i < COUNT; i++)
		drawArrays(headVAO[i], headTexture[i]);

	// 身子
	model = glm::mat4(1.0f);
	model = glm::translate(model, { pos.x, pos.y + 0.75, pos.z });
	model = glm::rotate(model, glm::radians(bodyRadians), glm::vec3(0.0f, 1.0f, 0.0f));
	shader.setMat4("model", model);
	for (int i = 0; i < COUNT; i++)
		drawArrays(bodyVAO[i], bodyTexture[i]);

	// 手臂
	model = glm::mat4(1.0f);
	model = glm::translate(model, { pos.x, pos.y + 0.975, pos.z });
	model = glm::rotate(model, glm::radians(bodyRadians + placeValue), glm::vec3(0.0f, 1.0f, 0.0f));
	float limbRadiansTemp = limbRadians - placeValue * 3 - takeValue;
	model = glm::rotate(model, glm::radians(limbRadiansTemp < -45 ? -45.0f : limbRadiansTemp), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	shader.setMat4("model", model);
	for (int i = 0; i < COUNT; i++)
		drawArrays(armVAO[i], armTexture[i]);

	model = glm::mat4(1.0f);
	model = glm::translate(model, { pos.x, pos.y + 0.975, pos.z });
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(bodyRadians), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(limbRadians), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	shader.setMat4("model", model);
	for (int i = 0; i < COUNT; i++)
		drawArrays(armVAO[i], armTexture[i]);

	// 腿
	model = glm::mat4(1.0f);
	model = glm::translate(model, { pos.x, pos.y + 0.25, pos.z });
	model = glm::rotate(model, glm::radians(bodyRadians), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-limbRadians), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	for (int i = 0; i < COUNT; i++)
		drawArrays(legVAO[i], legTexture[i]);

	model = glm::mat4(1.0f);
	model = glm::translate(model, { pos.x, pos.y + 0.25, pos.z });
	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(bodyRadians), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(-limbRadians), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setMat4("model", model);
	for (int i = 0; i < COUNT; i++)
		drawArrays(legVAO[i], legTexture[i]);
}

void PersonModel::firstPersonDrawTakeCube()
{
	switch (nowTakeCube)
	{
	case CubeType::MapLand:
		takeCube1.firstPersonDraw(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	case CubeType::MapSand:
		takeCube2.firstPersonDraw(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	case CubeType::MapStone:
		takeCube3.firstPersonDraw(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	case CubeType::MapRedBrick:
		takeCube4.firstPersonDraw(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	case CubeType::MapWood:
		takeCube5.firstPersonDraw(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	case CubeType::MapAspenWood:
		takeCube6.firstPersonDraw(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	case CubeType::MapDesertBrick:
		takeCube7.firstPersonDraw(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	case CubeType::MapStoneBrick:
		takeCube8.firstPersonDraw(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	case CubeType::MapSilverBrick:
		takeCube9.firstPersonDraw(eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	default:
		break;
	}

	placeValueChange();
	takeValueChange();
}

void PersonModel::thirdPersonDrawTakeCube(const glm::vec3& pos)
{
	switch (nowTakeCube)
	{
	case CubeType::MapLand:
		takeCube1.thirdPersonDraw(pos, eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;						   
	case CubeType::MapSand:
		takeCube2.thirdPersonDraw(pos, eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;						   
	case CubeType::MapStone:
		takeCube3.thirdPersonDraw(pos, eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;						   
	case CubeType::MapRedBrick:
		takeCube4.thirdPersonDraw(pos, eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;						   
	case CubeType::MapWood:
		takeCube5.thirdPersonDraw(pos, eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;						   
	case CubeType::MapAspenWood:
		takeCube6.thirdPersonDraw(pos, eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;						   
	case CubeType::MapDesertBrick:
		takeCube7.thirdPersonDraw(pos, eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;						   
	case CubeType::MapStoneBrick:
		takeCube8.thirdPersonDraw(pos, eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;						   
	case CubeType::MapSilverBrick:
		takeCube9.thirdPersonDraw(pos, eyeRadians, bodyRadians, limbRadians, placeValue, takeValue);
		break;
	default:
		break;
	}

	placeValueChange();
	takeValueChange();
}

void PersonModel::placeValueChange()
{
	if (placeAnimation != Animation::End)
	{
		if (placeAnimation == Animation::GO)
		{
			placeValue += 2 * control->elapseTime * 64;
			if (placeValue >= 20)
				placeAnimation = Animation::Return;
		}
		else
		{
			placeValue -= 2 * control->elapseTime * 64;
			if (placeValue <= 0)
				placeAnimation = Animation::End;
		}
	}
}

void PersonModel::takeValueChange()
{
	if (takeAnimation != Animation::End)
	{
		if (takeAnimation == Animation::GO)
		{
			takeValue += control->elapseTime * 64;
			if (takeValue >= 10)
			{
				nowTakeCube = newTakeCube;
				takeAnimation = Animation::Return;
			}
		}
		else
		{
			takeValue -= control->elapseTime * 64;
			if (takeValue <= 0)
				takeAnimation = Animation::End;
		}
	}
}

void PersonModel::drawArrays(unsigned int VAO, unsigned int texture)
{
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void PersonModel::bindTexture(unsigned int *VAO, unsigned int *VBO, const float (*vertex)[48],
								unsigned int* texture, int rect)
{
	glGenVertexArrays(COUNT, VAO);
	glGenBuffers(COUNT, VBO);

	for (int i = 0; i < 6; i++)
	{
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex[i]), vertex[i], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		texture[i] = loadTexture(rect, i);
	}
}

unsigned int PersonModel::loadTexture(int rect, int i)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	cv::Mat g_img;
	cv::Mat m_img = img({ personRect[rect][i][0], personRect[rect][i][1], personRect[rect][i][2], personRect[rect][i][3] });

	cv::resize(m_img, g_img, cv::Size(m_img.cols * 10, m_img.rows * 10), 0, 0, cv::INTER_NEAREST);

	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_img.cols, g_img.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, g_img.data);

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return textureID;
}

void PersonModel::runAnimation(const glm::vec3& pos)
{
	if (limbRadians >= 45 && isLimbDir == true)
		isLimbDir = false;
	else if (limbRadians <= -45 && isLimbDir == false)
		isLimbDir = true;

	if (isLimbDir)
	{
		if (limbRadians > -30 && limbRadians < 30)
			limbRadians += 500 * control->elapseTime;
		else
			limbRadians += 200 * control->elapseTime;
	}
	else
	{
		if (limbRadians > -30 && limbRadians < 30)
			limbRadians -= 500 * control->elapseTime;
		else
			limbRadians -= 200 * control->elapseTime;
	}

	if (pos == oldPos)
		limbRadians = 0;

	oldPos = pos;
}
