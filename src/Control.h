#ifndef CONTROL_H
#define CONTROL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "CreateWorld.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <opencv2/opencv.hpp>

#include <iostream>

// 第一人称
#define EYE_HEIGHT 1

// 第三人称 (存在问题，不打算修复)
//#define EYE_HEIGHT 3

// 人物状态
enum class PeopleStatus
{
	Normal,		// 正常
	JumpUp,		// 跳起
	Eecline		// 下降
};

// OpenGL鼠标点击事件
void mousePress(GLFWwindow* window, int button, int action, int mods);
// OpenGL鼠标移动事件
void mouseMove(GLFWwindow* window, double xpos, double ypos);

// 玩家控制类
class Control
{
public:
	static Control* getInstance();
	// 键盘按下事件
	void keyPress();
	// 人物出现时的所在的海拔
	void eyeInit(int altitude);
	// 有关人物所在海拔的处理函数(跳跃,下降)
	void eyeHeight();
	// 放置
	void placeBlock();
	// 消除
	void destroyBlock();

	// 身体角度
	float bodyRadians = -270;
	// 视角角度
	float eyeRadians = 0.0f;
	// 放置动画
	bool placeAnimation = false;

	// 手持方块
	CubeType nowTakeCube = CubeType::MapLand;

	// 人物状态
	PeopleStatus ps = PeopleStatus::Normal;	
	// 起跳海拔
	float beginJumpAltitude = 0;

	// 视角海拔
	float eyeAltitude = 0;	
	glm::vec3 eyePos = glm::vec3(WORLD_WIDTH / 2, eyeAltitude, WORLD_WIDTH / 2);
	glm::vec3 cameraPos = glm::vec3(WORLD_WIDTH / 2, eyeAltitude, WORLD_WIDTH / 2 + 0.25f);

	// 相机前方向
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
	// 相机上方向
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	// 经过时间
	double elapseTime = 0.0f;
	// 上次时间
	double oldTime = 1.0f;

	int fps = 0;

	// 鼠标捕获
	bool isCursorDisabled = true;

private:
	Control();
	Control(const Control&) = delete;
	Control& operator = (const Control&) = delete;

	// 窗体事件
	void windowKey();
	// 窗体抓屏
	void windowGrab();
	// 人物移动
	void cameraMoveKey();
	// 人物跳跃
	void cameraJumpKey();
	// 切换手持方块
	void takeCubeKey();
	// 碰撞检测
	bool isCollision(float x, float y, float z);
};


#endif // !CONTROL_H

