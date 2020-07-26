#ifndef ELIMINATEANIMA_H
#define ELIMINATEANIMA_H

#include "EliminateModel.h"
#include "CreateWorld.h"

#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// 方块消除爆炸动画
class EliminateAnima
{
public:
	static EliminateAnima* getInstance();
	// EliminateModel 类初始化
	void init();
	// 爆炸动画的位置
	void push(glm::vec3 vec, CubeType cubeType);
	// 渲染所有的爆炸动画
	void draw();

private:
	EliminateAnima();
	EliminateAnima(const EliminateAnima&) = delete;
	EliminateAnima& operator = (const EliminateAnima&) = delete;

	// 方块碎末结构体
	struct OffscumStruct
	{
		// 碎末位置
		glm::vec3 pos;
		// 移动角度
		int angle;
		// 移动速度
		float speed;
		// 碎末开始高度
		float begin;

		// 向上移动
		bool isUp;
		// 是否绘制
		bool isDraw;
		// 爆炸的方块
		CubeType cubeType;

		OffscumStruct(glm::vec3 vec, int angle, float speed, CubeType cubeType):
			pos(std::move(vec)),
			angle(angle),
			speed(speed),
			isUp(true),
			isDraw(true),
			cubeType(cubeType)
		{
			begin = vec.y + 1;
		}

	};

	// 爆炸的方块位置集合
	using OffscumVec = vector<vector<OffscumStruct>>;
	list<OffscumVec> offscumVecList;

	EliminateModel eliminate1 = EliminateModel("土块.png");
	EliminateModel eliminate2 = EliminateModel("沙子.png");
	EliminateModel eliminate3 = EliminateModel("石头.png");
	EliminateModel eliminate4 = EliminateModel("红砖.png");
	EliminateModel eliminate5 = EliminateModel("木头.png");
	EliminateModel eliminate6 = EliminateModel("基岩.png");
	EliminateModel eliminate7 = EliminateModel("宝石.png");
	EliminateModel eliminate8 = EliminateModel("石砖.png");
	EliminateModel eliminate9 = EliminateModel("地砖.png");
	EliminateModel eliminate10 = EliminateModel("草地.png");
	EliminateModel eliminate11 = EliminateModel("树木.png");
	EliminateModel eliminate12 = EliminateModel("雪地.png");
	EliminateModel eliminate13 = EliminateModel("绿树叶.png");
	EliminateModel eliminate14 = EliminateModel("红树叶.png");
	EliminateModel eliminate15 = EliminateModel("黄树叶.png");
	EliminateModel eliminate16 = EliminateModel("白树叶.png");

	float offscumPosRand();
	void switchDraw(OffscumStruct &offstr);
};

#endif // !ELIMINATEANIMA_H