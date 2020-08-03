#include "Control.h"
#include "GLWidget.h"
#include "CommTCP.h"
#include "EliminateAnima.h"

constexpr float moveSpeed = 6;
constexpr float placeSpeed = 0.5;

static CommTCP* commTCP = CommTCP::getInstantiate();
static Control* control = Control::getInstance();
static GLWidget* glWidget = GLWidget::getInstance();
static CreateWorld* createWorld = CreateWorld::getInstance();
static EliminateAnima* eliminateAnima = EliminateAnima::getInstance();

Control* Control::getInstance()
{
	static Control control;
	return &control;
}

Control::Control()
{
}

void Control::keyPress()
{
	windowKey();

	takeCubeKey();
	cameraMoveKey();
	cameraJumpKey();
}

void Control::eyeInit(int altitude)
{
	eyePos.y = altitude + EYE_HEIGHT;
	cameraPos.y = eyePos.y;
	eyeAltitude = eyePos.y;
}

void Control::eyeHeight()
{
	if (ps == PeopleStatus::JumpUp)
	{
		eyePos.y += (beginJumpAltitude - eyePos.y) * 0.025 + control->elapseTime;
		cameraPos.y = eyePos.y;
		eyeAltitude = eyePos.y;

		if (eyePos.y >= beginJumpAltitude)
		{
			eyeAltitude = beginJumpAltitude;
			eyePos.y = eyeAltitude;
			cameraPos.y = eyePos.y;
			ps = PeopleStatus::Eecline;
		}
	}
	else if (ps == PeopleStatus::Eecline)
	{
		eyePos.y -= (beginJumpAltitude - eyePos.y) * 0.025 + control->elapseTime;
		cameraPos.y = eyePos.y;
		eyeAltitude = eyePos.y;

		if (eyePos.y <= createWorld->getAltitude(ceil(eyePos.x - 0.5), ceil(eyePos.z - 0.5), ceil(eyePos.y - 1.5)) + EYE_HEIGHT)
		{
			eyeAltitude = createWorld->getAltitude(ceil(eyePos.x - 0.5), ceil(eyePos.z - 0.5), ceil(eyePos.y - 1.5)) + EYE_HEIGHT;
			eyePos.y = eyeAltitude;
			cameraPos.y = eyePos.y;
			ps = PeopleStatus::Normal;
		}
	}
	else if (isCollision(eyePos.x, eyePos.y - 1, eyePos.z))
	{
		ps = PeopleStatus::Eecline;
		beginJumpAltitude = eyePos.y;
	}
}

void mousePress(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		switch (button)
		{
		case GLFW_MOUSE_BUTTON_LEFT:
			if (!control->isCursorDisabled)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			control->destroyBlock();
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			control->placeBlock();
			break;
		default:
			return;
		}
	}
	return;
}

void Control::placeBlock()
{
	placeAnimation = true;

	glm::vec3 place;

	for (int i = 1; i < 15; i++)
	{
		place = cameraPos + placeSpeed * i * cameraFront;
		if (createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
		{
			place = cameraPos + placeSpeed * (i - 1) * cameraFront;
			if (createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] == CubeType::Null &&
				!(round(place.x) == ceil(eyePos.x - 0.5) && 
				  round(place.z) == ceil(eyePos.z - 0.5) &&
				  round(place.y) == ceil(eyePos.y - 1.5)))
			{
				createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] = nowTakeCube;

				commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
					control->nowTakeCube, PlaceStatus::Place,
					{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
					{ round(place.x), round(place.y), round(place.z)} });

				if (createWorld->plantData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
					createWorld->plantData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;
				if (createWorld->leafData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
					createWorld->leafData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;

				break;
			}
		}
	}
}

void Control::destroyBlock()
{
	placeAnimation = true;

	glm::vec3 place;

	for (int i = 1; i < 15; i++)
	{
		place = cameraPos + placeSpeed * i * cameraFront;
		if (createWorld->leafData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
		{
			CubeType cubeType = createWorld->leafData[round(place.x)][round(place.y)][round(place.z)];
			eliminateAnima->push({ round(place.x), round(place.y), round(place.z) }, cubeType);

			createWorld->leafData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;

			commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
				control->nowTakeCube, PlaceStatus::Eliminate,
				{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
				{ round(place.x), round(place.y), round(place.z)} });

			return;
		}

		if (createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
		{
			CubeType cubeType = createWorld->mapData[round(place.x)][round(place.y)][round(place.z)];
			eliminateAnima->push({ round(place.x), round(place.y), round(place.z) }, cubeType);

			createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;

			commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
				control->nowTakeCube, PlaceStatus::Eliminate,
				{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
				{ round(place.x), round(place.y), round(place.z)} });

			if (createWorld->plantData[round(place.x)][round(place.y) + 1][round(place.z)] != CubeType::Null)
			{
				createWorld->plantData[round(place.x)][round(place.y) + 1][round(place.z)] = CubeType::Null;

				commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
					control->nowTakeCube, PlaceStatus::Eliminate,
					{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
					{ round(place.x), round(place.y), round(place.z)} });
			}

			return;
		}
	}
}

void mouseMove(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX = Width / 2.0;
	static float lastY = Height / 2.0;

	static bool firstMouse = true;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	control->bodyRadians += xoffset;
	control->eyeRadians += yoffset;

	if (control->eyeRadians > 80.0f)
		control->eyeRadians = 80.0f;
	if (control->eyeRadians < -80.0f)
		control->eyeRadians = -80.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(control->bodyRadians)) * cos(glm::radians(control->eyeRadians));
	front.y = sin(glm::radians(control->eyeRadians));
	front.z = sin(glm::radians(control->bodyRadians)) * cos(glm::radians(control->eyeRadians));
	control->cameraFront = glm::normalize(front);

	if (EYE_HEIGHT == 1)
	{
		float radius = 0.25f;
		float camX = cos(glm::radians(control->bodyRadians)) * radius;
		float camZ = sin(glm::radians(control->bodyRadians)) * radius;
		control->cameraPos.x = control->eyePos.x + camX;
		control->cameraPos.z = control->eyePos.z + camZ;
	}
	else
	{
		float radius = 5.0f;
		float camX = cos(glm::radians(control->bodyRadians)) * radius;
		float camZ = sin(glm::radians(control->bodyRadians)) * radius;
		control->cameraPos.x = control->eyePos.x - camX;
		control->cameraPos.z = control->eyePos.z - camZ;
	}
}

void Control::windowKey()
{
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glWidget->getWindow(), true);

	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		isCursorDisabled = false;
		glfwSetInputMode(glWidget->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Control::cameraMoveKey()
{
	float cameraSpeed = moveSpeed * elapseTime;
	glm::vec3 oldPos = eyePos;

	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		eyePos += cameraSpeed * cameraFront;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		eyePos -= cameraSpeed * cameraFront;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		eyePos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		eyePos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (eyePos != oldPos)
	{
		glm::vec3 newPos = eyePos + cameraPos - oldPos;

		if (isCollision(newPos.x, newPos.y, newPos.z))
		{
			cameraPos = newPos;
			eyePos.y = eyeAltitude;
			cameraPos.y = eyePos.y;
		}
		else
		{
			eyePos = oldPos;
		}
	}
}

void Control::cameraJumpKey()
{
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (ps == PeopleStatus::Normal)
		{
			ps = PeopleStatus::JumpUp;
			beginJumpAltitude = eyePos.y + float(EYE_HEIGHT) * 1.1;
		}
	}

	eyeHeight();
}

void Control::takeCubeKey()
{
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_1) == GLFW_PRESS)
		nowTakeCube = CubeType::MapLand;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_2) == GLFW_PRESS)
		nowTakeCube = CubeType::MapSand;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_3) == GLFW_PRESS)
		nowTakeCube = CubeType::MapStone;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_4) == GLFW_PRESS)
		nowTakeCube = CubeType::MapRedBrick;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_5) == GLFW_PRESS)
		nowTakeCube = CubeType::MapWood;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_6) == GLFW_PRESS)
		nowTakeCube = CubeType::MapBedrock;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_7) == GLFW_PRESS)
		nowTakeCube = CubeType::MapGem;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_8) == GLFW_PRESS)
		nowTakeCube = CubeType::MapStoneBrick;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_9) == GLFW_PRESS)
		nowTakeCube = CubeType::MapGround;

	commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
		control->nowTakeCube, PlaceStatus::Normal,
		{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
		{ 0, 0, 0} });
}

bool Control::isCollision(float x, float y, float z)
{
	if (x <= 10 || z <= 10 || y <= 10 || x >= WORLD_WIDTH - 10 || z >= WORLD_WIDTH - 10 || y >= WORLD_HEIGHT - 10)
		return false;

	if (createWorld->mapData[ceil(x - 0.5)][ceil(y - 1.5)][ceil(z - 0.5)] == CubeType::Null &&
		createWorld->mapData[ceil(x - 0.5)][ceil(y - 0.5)][ceil(z - 0.5)] == CubeType::Null)
	{
		if (createWorld->mapData[ceil(x - 0.5 + 0.2)][ceil(y - 1.5)][ceil(z - 0.5)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5 - 0.2)][ceil(y - 1.5)][ceil(z - 0.5)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5)][ceil(y - 1.5)][ceil(z - 0.5 + 0.2)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5)][ceil(y - 1.5)][ceil(z - 0.5 - 0.2)] == CubeType::Null &&

			createWorld->mapData[ceil(x - 0.5 + 0.2)][ceil(y - 0.5)][ceil(z - 0.5)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5 - 0.2)][ceil(y - 0.5)][ceil(z - 0.5)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5)][ceil(y - 0.5)][ceil(z - 0.5 + 0.2)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5)][ceil(y - 0.5)][ceil(z - 0.5 - 0.2)] == CubeType::Null)
			return true;
	}

	return false;
}
