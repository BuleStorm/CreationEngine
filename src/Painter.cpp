#include "Painter.h"
#include "Control.h"

static CreateWorld* createWorld = CreateWorld::getInstance();
static Control* control = Control::getInstance();


Painter* Painter::getInstance()
{
	static Painter painter;
	return &painter;
}

void Painter::drawMap(glm::vec3 vec)
{
	clearAll();

	//  ”“∞
	int begin_x = vec.x - VISION_WIDTH;
	if (begin_x < 0)
		begin_x = 0;

	int end_x = vec.x + VISION_WIDTH;
	if (end_x > createWorld->mapData.size())
		end_x = createWorld->mapData.size();

	int begin_y = vec.y - VISION_HEIGHT;
	if (begin_y < 0)
		begin_y = 0;

	int end_y = vec.y + VISION_HEIGHT;
	if (end_y > createWorld->mapData[0].size())
		end_y = createWorld->mapData[0].size();

	int begin_z = vec.z - VISION_WIDTH;
	if (begin_z < 0)
		begin_z = 0;

	int end_z = vec.z + VISION_WIDTH;
	if (end_z > createWorld->mapData[0][0].size())
		end_z = createWorld->mapData[0][0].size();

	for (size_t x = size_t(begin_x) + 10; x < size_t(end_x) - 10; x++)
	{
		for (size_t y = size_t(begin_y) + 10; y < size_t(end_y) - 10; y++)
		{
			for (size_t z = size_t(begin_z) + 10; z < size_t(end_z) - 10; z++)
			{
				if (isDrawCube(x, y, z))
					push(createWorld->mapData[x][y][z], x, y, z);

				push(createWorld->leafData[x][y][z], x, y, z);
				push(createWorld->plantData[x][y][z], x, y, z);
			}
		}
	}

	drawAll();
}

void Painter::takeCubeInit()
{
	PersonModel::takeCubeInit();
}

void Painter::drawAll()
{
	// ø™∆Ù√ÊÃﬁ≥˝
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	CubeModel::drawStart();
	cube1.draw();
	cube2.draw();
	cube3.draw();
	cube4.draw();
	cube5.draw();
	cube6.draw();
	cube7.draw();
	cube8.draw();
	cube9.draw();
	cube10.draw();
	cube11.draw();
	cube12.draw();


	LeafModel::drawStart();
	leafRed.draw();
	leafYellow.draw();
	leafWhite.draw();
	leafGreen.draw();


	// »°œ˚√ÊÃﬁ≥˝
	glDisable(GL_CULL_FACE);

	PlantModel::drawStart();
	plant1.draw();
	plant2.draw();
	plant3.draw();
	plant4.draw();
	plant5.draw();
	plant6.draw();
}

void Painter::clearAll()
{
	cube1.clear();
	cube2.clear();
	cube3.clear();
	cube4.clear();
	cube5.clear();
	cube6.clear();
	cube7.clear();
	cube8.clear();
	cube9.clear();
	cube10.clear();
	cube11.clear();
	cube12.clear();

	leafRed.clear();
	leafYellow.clear();
	leafWhite.clear();
	leafGreen.clear();

	plant1.clear();
	plant2.clear();
	plant3.clear();
	plant4.clear();
	plant5.clear();
	plant6.clear();
}


void Painter::push(CubeType ct, int i, int j, int k)
{
	switch (ct)
	{
	case CubeType::Null:// ø’
		break;
	case CubeType::MapLand: // Õ¡øÈ
		this->cube1.push(i, j, k);
		break;
	case CubeType::MapSand: // …≥◊”
		this->cube2.push(i, j, k);
		break;
	case CubeType::MapStone: //  ØÕ∑
		this->cube3.push(i, j, k);
		break;
	case CubeType::MapRedBrick: // ∫Ï◊©
		this->cube4.push(i, j, k);
		break;
	case CubeType::MapWood: // ƒæÕ∑
		this->cube5.push(i, j, k);
		break;
	case CubeType::MapBedrock: // ª˘—“
		this->cube6.push(i, j, k);
		break;
	case CubeType::MapGem: // ±¶ Ø
		this->cube7.push(i, j, k);
		break;
	case CubeType::MapStoneBrick: //  Ø◊©
		this->cube8.push(i, j, k);
		break;
	case CubeType::MapGround: // µÿ◊©
		this->cube9.push(i, j, k);
		break;
	case CubeType::MapGrassland: // ≤›µÿ
		this->cube10.push(i, j, k);
		break;
	case CubeType::MapTree: //  ˜ƒæ
		this->cube11.push(i, j, k);
		break;
	case CubeType::MapSnow: // —©µÿ
		this->cube12.push(i, j, k);
		break;

	case CubeType::MapLeaveGreen: // ¬Ã ˜“∂
		this->leafGreen.push(i, j, k);
		break;
	case CubeType::MapLeaveRed: // ∫Ï ˜“∂
		this->leafRed.push(i, j, k);
		break;
	case CubeType::MapLeaveYellow: // ª∆ ˜“∂
		this->leafYellow.push(i, j, k);
		break;
	case CubeType::MapLeaveWhite: // ∞◊ ˜“∂
		this->leafWhite.push(i, j, k);
		break;

	case CubeType::PatMushroom: // ƒ¢πΩ
		this->plant1.push(i, j, k);
		break;
	case CubeType::PatRedMushroom: // ∫Ïƒ¢πΩ
		this->plant2.push(i, j, k);
		break;
	case CubeType::PatFlower: // –°ª®
		this->plant3.push(i, j, k);
		break;
	case CubeType::PatGrass_1: // ≤›1
		this->plant4.push(i, j, k);
		break;
	case CubeType::PatGrass_2: // ≤›2
		this->plant5.push(i, j, k);
		break;
	case CubeType::PatGrass_3: // ≤›3
		this->plant6.push(i, j, k);
		break;
	default:
		break;
	}
}

inline bool Painter::isDrawCube(int x, int y, int z)
{
	return  createWorld->mapData[x + 1][y][z] == CubeType::Null ||
		createWorld->mapData[x - 1][y][z] == CubeType::Null ||
		createWorld->mapData[x][y + 1][z] == CubeType::Null ||
		createWorld->mapData[x][y - 1][z] == CubeType::Null ||
		createWorld->mapData[x][y][z + 1] == CubeType::Null ||
		createWorld->mapData[x][y][z - 1] == CubeType::Null;
}
