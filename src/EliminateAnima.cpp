#include "EliminateAnima.h"
#include "Control.h"

static Control* control = Control::getInstance();
static CreateWorld* createWorld = CreateWorld::getInstance();


EliminateAnima* EliminateAnima::getInstance()
{
	static EliminateAnima eliminateAnima;
	return &eliminateAnima;
}

void EliminateAnima::init()
{
	eliminate1.init();
	eliminate2.init();
	eliminate3.init();
	eliminate4.init();
	eliminate5.init();
	eliminate6.init();
	eliminate7.init();
	eliminate8.init();
	eliminate9.init();
	eliminate10.init();
	eliminate10.init();
	eliminate11.init();
	eliminate12.init();
	eliminate13.init();
	eliminate14.init();
	eliminate15.init();
	eliminate16.init();
}

void EliminateAnima::push(glm::vec3 vec, CubeType cubeType)
{
	vector<vector<OffscumStruct>> offscumVec2;
	offscumVec2.reserve(10);
	for (int i = 0; i < 10; i++)
	{
		vector<OffscumStruct> offscumVec;
		offscumVec.reserve(10);
		for (int j = 0; j < 10; j++)
		{
			offscumVec.emplace_back(glm::vec3(vec.x + offscumPosRand(), vec.y + offscumPosRand(), vec.z + offscumPosRand()),
					rand() % 360, float(rand() % 3 + 1) / 100, cubeType);
		}

		offscumVec2.push_back(offscumVec);
	}

	offscumVecList.emplace_back(offscumVec2);

	if (offscumVecList.size() > 8)
		offscumVecList.pop_front();
}

void EliminateAnima::draw()
{
	for (auto item = offscumVecList.begin(); item != offscumVecList.end(); ++item)
	{
		eliminate1.drawStart();
		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				OffscumStruct *offscum = reinterpret_cast<OffscumStruct*>(&(*item)[i][j]);

				if (offscum->isDraw)
				{
					if (offscum->isUp)
					{
						offscum->pos.x += sin(offscum->angle) * offscum->speed;
						offscum->pos.z += cos(offscum->angle) * offscum->speed;
						offscum->pos.y += (offscum->begin - offscum->pos.y) * float(rand() % 25) / 1000 + control->elapseTime;

						if (offscum->pos.y > offscum->begin - 0.75)
							offscum->isUp = false;
					}
					else
					{
						if (offscum->pos.y >= createWorld->getAltitude(round(offscum->pos.x), round(offscum->pos.z), round(offscum->pos.y)) - 0.4)
						{
							offscum->pos.x += sin(offscum->angle) * offscum->speed;
							offscum->pos.z += cos(offscum->angle) * offscum->speed;
							offscum->pos.y -= (offscum->begin - offscum->pos.y) * float(rand() % 25) / 1000 + control->elapseTime;
						}
					}
				}

				if (offscum->isDraw)
				{
					if (rand() % 15 == 0)
						offscum->isDraw = false;
					else
						switchDraw(*offscum);
				}
			}
		}
	}
}

EliminateAnima::EliminateAnima()
{
}

float EliminateAnima::offscumPosRand()
{
	return float(rand() % 100 - 50) / 100;
}

void EliminateAnima::switchDraw(OffscumStruct &offstr)
{
	switch (offstr.cubeType)
	{
	case CubeType::MapLand:
		eliminate1.draw(offstr.pos);
		break;
	case CubeType::MapSand:
		eliminate2.draw(offstr.pos);
		break;
	case CubeType::MapStone:
		eliminate3.draw(offstr.pos);
		break;
	case CubeType::MapRedBrick:
		eliminate4.draw(offstr.pos);
		break;
	case CubeType::MapWood:
		eliminate5.draw(offstr.pos);
		break;
	case CubeType::MapAspenWood:
		eliminate6.draw(offstr.pos);
		break;
	case CubeType::MapDesertBrick:
		eliminate7.draw(offstr.pos);
		break;
	case CubeType::MapStoneBrick:
		eliminate8.draw(offstr.pos);
		break;
	case CubeType::MapSilverBrick:
		eliminate9.draw(offstr.pos);
		break;
	case CubeType::MapGrassland:
		eliminate10.draw(offstr.pos);
		break;
	case CubeType::MapTree:
		eliminate11.draw(offstr.pos);
		break;
	case CubeType::MapSnow:
		eliminate12.draw(offstr.pos);
		break;
	case CubeType::MapLeaveRed:
		eliminate13.draw(offstr.pos);
		break;
	case CubeType::MapLeaveYellow:
		eliminate14.draw(offstr.pos);
		break;
	case CubeType::MapLeaveWhite:
		eliminate15.draw(offstr.pos);
		break;
	case CubeType::MapLeaveGreen:
		eliminate16.draw(offstr.pos);
		break;
	default:
		break;
	}
}
