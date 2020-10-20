#ifndef PAINTER_H
#define PAINTER_H

#include "PersonModel.h"
#include "CubeModel.h"
#include "PlantModel.h"
#include "LeafModel.h"
#include "CreateWorld.h"

#define VISION_WIDTH	100		// 视野 100 50
#define VISION_HEIGHT	30		// 视野 30 50

class Painter
{
public:
	static Painter* getInstance();
	// 画地图
	void drawMap(glm::vec3 vec);
	// 手持方块初始化
	void takeCubeInit();

	CubeModel cube1 = CubeModel("default_dirt.png", "default_dirt.png");
	CubeModel cube2 = CubeModel("default_desert_sand.png", "default_desert_sand.png");
	CubeModel cube3 = CubeModel("default_stone.png", "default_stone.png");
	CubeModel cube4 = CubeModel("default_brick.png", "default_brick.png");
	CubeModel cube5 = CubeModel("default_wood.png", "default_wood.png");
	CubeModel cube6 = CubeModel("default_aspen_wood.png", "default_aspen_wood.png");
	CubeModel cube7 = CubeModel("default_desert_sandstone_brick.png", "default_desert_sandstone_brick.png");
	CubeModel cube8 = CubeModel("default_stone_brick.png", "default_stone_brick.png");
	CubeModel cube9 = CubeModel("default_silver_sandstone_brick.png", "default_silver_sandstone_brick.png");
	CubeModel cube10 = CubeModel("default_grass.png", "default_grass_side.png");
	CubeModel cube11 = CubeModel("default_tree_top.png", "default_tree.png");
	CubeModel cube12 = CubeModel("default_snow.png", "default_snow_side.png");


	PlantModel plant1 = PlantModel("honggu.png");
	PlantModel plant2 = PlantModel("mogu.png");
	PlantModel plant3 = PlantModel("xiaohua.png");
	PlantModel plant4 = PlantModel("default_grass_1.png");
	PlantModel plant5 = PlantModel("default_grass_2.png");
	PlantModel plant6 = PlantModel("default_grass_3.png");

	LeafModel leafRed = LeafModel("default_aspen_leaves.png");
	LeafModel leafYellow = LeafModel("default_jungleleaves.png");
	LeafModel leafWhite = LeafModel("default_white_leaves.png");
	LeafModel leafGreen = LeafModel("default_leaves.png");

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