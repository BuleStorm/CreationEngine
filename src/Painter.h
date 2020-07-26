#ifndef PAINTER_H
#define PAINTER_H

#include "PersonModel.h"
#include "CubeModel.h"
#include "PlantModel.h"
#include "LeafModel.h"
#include "CreateWorld.h"

#define VISION_WIDTH	100		// 视野
#define VISION_HEIGHT	30		// 视野

class Painter
{
public:
	static Painter* getInstance();
	// 画地图
	void drawMap(glm::vec3 vec);
	// 手持方块初始化
	void takeCubeInit();

	CubeModel cube1 = CubeModel("土块.png", "土块.png");
	CubeModel cube2 = CubeModel("沙子.png", "沙子.png");
	CubeModel cube3 = CubeModel("石头.png", "石头.png");
	CubeModel cube4 = CubeModel("红砖.png", "红砖.png");
	CubeModel cube5 = CubeModel("木头.png", "木头.png");
	CubeModel cube6 = CubeModel("基岩.png", "基岩.png");
	CubeModel cube7 = CubeModel("宝石.png", "宝石.png");
	CubeModel cube8 = CubeModel("石砖.png", "石砖.png");
	CubeModel cube9 = CubeModel("地砖.png", "地砖.png");
	CubeModel cube10 = CubeModel("草地.png", "草阶.png");
	CubeModel cube11 = CubeModel("年轮.png", "树木.png");
	CubeModel cube12 = CubeModel("雪地.png", "雪阶.png");


	PlantModel plant1 = PlantModel("红菇.png");
	PlantModel plant2 = PlantModel("蘑菇.png");
	PlantModel plant3 = PlantModel("小花.png");
	PlantModel plant4 = PlantModel("草1.png");
	PlantModel plant5 = PlantModel("草2.png");
	PlantModel plant6 = PlantModel("草3.png");

	LeafModel leafRed = LeafModel("红树叶.png");
	LeafModel leafYellow = LeafModel("黄树叶.png");
	LeafModel leafWhite = LeafModel("白树叶.png");
	LeafModel leafGreen = LeafModel("绿树叶.png");

	PersonModel person1 = PersonModel("1.png");
	PersonModel person2 = PersonModel("2.png");
private:
	Painter() = default;

	Painter(const Painter&) = delete;
	Painter& operator = (const Painter&) = delete;

	// 画地图中所有元素
	void drawAll();
	// 清空绘制
	void clearAll();
	// 分类绘制
	void push(CubeType ct, int i, int j, int k);
	// 判断方块是否暴露在外并绘制
	inline bool isDrawCube(int x, int y, int z);
};

#endif // !PAINTER_H