#ifndef CREATEWORLD_H
#define CREATEWORLD_H

#include <glad/glad.h>
#include <vector>
#include <iostream>

// 地图长宽高 640 256 256
#define WORLD_WIDTH	640
#define WORLD_HEIGHT 256
// 雪地面积
#define SNOW_RADIUS 256

using namespace std;

// 方块类型
enum class CubeType
{
	Null,				// 空
	MapLand,			// 土块
	MapSand,			// 沙子
	MapStone,			// 石头
	MapRedBrick,		// 红砖
	MapWood,			// 木头
	MapAspenWood,		// 白木
	MapDesertBrick,		// 沙砖
	MapStoneBrick,		// 石砖
	MapSilverBrick,		// 银砖

	MapGrassland,		// 草地
	MapTree,			// 树木
	MapSnow,			// 雪地

	MapLeaveGreen,		// 绿树叶
	MapLeaveRed,		// 红树叶
	MapLeaveYellow,		// 黄树叶
	MapLeaveWhite,		// 白树叶

	PatMushroom,		// 蘑菇
	PatRedMushroom,		// 红蘑菇
	PatFlower,			// 小花
	PatGrass_1,			// 草1
	PatGrass_2,			// 草2
	PatGrass_3,			// 草3
};

using vec3 = vector<vector<vector<CubeType>>>;

// 随机创建地图类
class CreateWorld
{
public:
	static CreateWorld* getInstance();
	// 初始化地图容器
	void initMapVec();
	// 创建方块
	void CreateCubeMap();
	// 创建植物
	void CreatePlantMap();
	// 创建树
	void CreateTreeMap();
	// 所在地图位置的海拔
	int getAltitude(int x, int z, int y);
public:
	// 方块位置数据
	vec3 mapData;
	// 植物位置数据
	vec3 plantData;
	// 树叶位置数据
	vec3 leafData;
private:
	CreateWorld();
	CreateWorld(const CreateWorld&) = delete;
	CreateWorld& operator = (const CreateWorld&) = delete;

	// 前海拔 (创建随机地图时使用)
	vector<int> beforeAltitudeVec;
	// 地图海拔 (创建随机地图时使用)
	vector<vector<int>> altitudeMapVec;

	// 地图第一行的海拔随机初始化
	void randInitAltitude();
	// 地图中一行海拔数据的初始化
	void randLineAltitude();
	// 随机初始化地图的所有海拔
	void randAllAltitude();
	// 地图海拔平均化
	void avgAltitude();
	// 填充草地
	void fillingGrasslandCube();
	// 随机创建雪地
	void RandSnowCubeMap();
	// 周围是否存在树
	bool isAroundTree(int x, int z);
	// 填充树叶
	void fillingLeave(int x, int z, int height, CubeType ct);
};



#endif // !CREATEWORLD_H