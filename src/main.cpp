#include <iostream>
#include "GLWidget.h"
#include "WorldModel.h"
#include "Control.h"
#include "Painter.h"
#include "CreateWorld.h"
#include "Interface.h"
#include "EliminateAnima.h"
#include "CommTCP.h"
#include "OperationDB.h"
#include <ctime>
#include <thread>

#include <uv.h>

using namespace std;

#define COUT_FPS

static clock_t fpsStart, fpsEnd;

static GLWidget* glWindow = GLWidget::getInstance();
static WorldModel* worldModel = WorldModel::getInstance();
static Control* control = Control::getInstance();
static Painter* painter = Painter::getInstance();
static CreateWorld* createWorld = CreateWorld::getInstance();
static Interface* interFace = Interface::getInstance();
static EliminateAnima* eliminateAnima = EliminateAnima::getInstance();
static CommTCP* commTCP = CommTCP::getInstance();
static OperationDB* operationDB = OperationDB::getInstance();
double Worldtime = 0;
// 玩家指针
static PersonModel* player1 = nullptr;
static PersonModel* player2 = nullptr;


// 初始化所有的数据
void initAll();
// 创建世界地图
void createMap();
// 加载地图
void loadMap();
// 处理玩家数据
void drawPlayer();
// OpenGL渲染开始
void drawBegin();
// OpenGL渲染过程
void drawMain();
// OpenGL渲染结束
void drawEnd();
// 计算FPS
void countFps();
// 画开始画面文字
void drawBeginText(const char* text);


void initAll()
{
	operationDB->init();

	worldModel->initSkybox();

	interFace->init();
	eliminateAnima->init();

	painter->cube1.init();
	painter->cube2.init();
	painter->cube3.init();
	painter->cube4.init();
	painter->cube5.init();
	painter->cube6.init();
	painter->cube7.init();
	painter->cube8.init();
	painter->cube9.init();
	painter->cube10.init();
	painter->cube11.init();
	painter->cube12.init();

	painter->leafRed.init();
	painter->leafGreen.init();
	painter->leafWhite.init();
	painter->leafYellow.init();

	painter->person1.init();
	painter->person2.init();
	painter->takeCubeInit();

	painter->plant1.init();
	painter->plant2.init();
	painter->plant3.init();
	painter->plant4.init();
	painter->plant5.init();
	painter->plant6.init();

	createWorld->initMapVec();
}

void createMap()
{
	createWorld->CreateCubeMap();
	createWorld->CreatePlantMap();
	createWorld->CreateTreeMap();
}

void loadMap()
{
//	thread *recvThread = new thread([=]()
//	{
//		commTCP->start();

//		if (commTCP->get_isServer())
//			commTCP->sendMap();
//		else
//			commTCP->recvMap();

//		commTCP->recvPlayer();
//	});

	drawBeginText("Load Map ...");
	this_thread::sleep_for(3s);

//	if (commTCP->get_isServer())
//	{
		player1 = &painter->person1;
		player2 = &painter->person2;

		// 创建存档
		createMap();
		operationDB->saveWorldMap("user_001");
		
		// 读取存档(解开前，屏蔽创建存档代码)
		//operationDB->loadWorldMap("user_001");
		
//	}
//	else
//	{
//		player1 = &painter->person2;
//		player2 = &painter->person1;

//		while (!commTCP->get_isMapRecvTrue())
//			this_thread::sleep_for(0.2s);
//	}
}

void drawPlayer()
{
	// 传输自己的状态给对方玩家
	commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
		control->nowTakeCube, PlaceStatus::Normal, 
		{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z }, { 0, 0, 0} });

	// 玩家一状态
	player1->eyeRadians = -control->eyeRadians;
	player1->bodyRadians = -control->bodyRadians + 90;
	player1->placeAnimtaion(control->placeAnimation);
	player1->takeCubeAnimation(control->nowTakeCube);
	player1->draw({ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z }, true);


	// 玩家二状态
	player2->eyeRadians = commTCP->playerData.eyeRadians;
	player2->bodyRadians = commTCP->playerData.bodyRadians;
	player2->placeAnimtaion(commTCP->playerData.placeAnimtaion);
	player2->takeCubeAnimation(commTCP->playerData.cubeType);
	player2->draw(commTCP->playerData.playerPos, false);
}

void drawBeginText(const char* text)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	interFace->drawText(text);

	glfwSwapBuffers(glWindow->getWindow());
	glfwPollEvents();
}

inline void drawMain()
{
	painter->drawMap(control->eyePos);

	eliminateAnima->draw();

	drawPlayer();

	control->eyeHeight();

	interFace->draw();
}

int main()
{
	//uv_loop_t* loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	//uv_loop_init(loop);

	//printf("Now quitting.\n");
	//uv_run(loop, UV_RUN_DEFAULT);

	//uv_loop_close(loop);
	//free(loop);



	initAll();
	loadMap();

	control->eyeInit(createWorld->getAltitude(WORLD_WIDTH / 2, WORLD_WIDTH / 2, WORLD_HEIGHT - 1));

	while (!glfwWindowShouldClose(glWindow->getWindow()))
	{
		drawBegin();
		drawMain();
		drawEnd();
	}

	glfwTerminate();
	exit(0);
	return 0;
}

inline void drawBegin()
{
	fpsStart = clock();
	double nowTime = glfwGetTime();
	control->elapseTime = nowTime - control->oldTime;
	control->oldTime = nowTime;

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	control->keyPress();
}

inline void drawEnd()
{
	glDisable(GL_CULL_FACE);
	worldModel->drawSkybox();

#ifdef COUT_FPS
	countFps();
#endif // COUT_FPS

	glfwSwapBuffers(glWindow->getWindow());
	glfwPollEvents();
}

void countFps()
{
	static int fpsNum = 0;
	static double timeLong=0.0;

	Worldtime += control->elapseTime * 36.0;// (86400/24/60)
	if (Worldtime >= 86400)
	{
		Worldtime -= 86400;
	}

	timeLong += control->elapseTime;
	if (timeLong >= 1.0)
	{
		timeLong -= 1.0;
		control->fps = fpsNum;
		fpsNum = 0;
	}
	fpsNum++;

	interFace->drawDebug(control->fps,(int)Worldtime);
}
