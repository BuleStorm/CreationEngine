#include "OperationDB.h"

#include <iostream>
#include <string.h>

static CreateWorld* createWorld = CreateWorld::getInstance();


OperationDB* OperationDB::getInstance()
{
	static OperationDB operation_db;
	return &operation_db;
}

void OperationDB::init()
{
	int ret = 0;
	const char* path = "./ce.db";

	ret = sqlite3_open_v2(path, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, nullptr);

	if (ret == SQLITE_OK) 
	{
		std::cout << "打开数据库成功" << std::endl;
	}
	else 
	{
		std::cout << "创建数据库ing..." << std::endl;
		ret = sqlite3_open_v2(path, &sqlite, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, nullptr);

		if (ret == SQLITE_OK)
			std::cout << "创建数据库成功" << std::endl;
		else
			std::cout << "创建数据库失败" << std::endl;
	}
}

void OperationDB::saveBlockMap(vec3 *data, const char* table_name, int block_x, int block_z, int block_y)
{
	char cube_data[2] = { 0 };
	string block_data_str = "";

	for (int i = block_y * BLOCK_WIDTH; i < (block_y + 1) * BLOCK_WIDTH; i++)
		for (int j = block_z * BLOCK_WIDTH; j < (block_z + 1) * BLOCK_WIDTH; j++)
			for (int k = block_x * BLOCK_WIDTH; k < (block_x + 1) * BLOCK_WIDTH; k++)
			{
				if (int((*data)[k][i][j]) >= 10)
				{
					cube_data[0] = int((*data)[k][i][j]) / 10;
					cube_data[1] = int((*data)[k][i][j]) % 10;
					//sprintf(cube_data, "%d", (*data)[k][i][j]);
					//itoa(int((*data)[k][i][j]), cube_data, 10);
				}
				else
				{
					//sprintf(&cube_data[1], "%d", (*data)[k][i][j]);
					//itoa(int((*data)[k][i][j]), &cube_data[1], 10);
					cube_data[0] = '0';
					cube_data[1] = int((*data)[k][i][j]) % 10;
				}
				block_data_str.append(cube_data).append(" ");
			}


	string sql_str = "";
	sql_str.resize(13000);

	sprintf(&sql_str[0], "insert into %s values(%d, %d, %d, '%s')", table_name, block_x, block_z, block_y, block_data_str.data());

	sqlite3_stmt* stmt = nullptr;

	int ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (ret == SQLITE_OK)
	{
		sqlite3_step(stmt);
	}
	else
	{
		std::cout << "insert" << table_name << "失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}

	sqlite3_finalize(stmt);
}

void OperationDB::saveWorldMap(const char* map_name)
{
	if (isMapExist(map_name))
		deleteRecordMap(map_name);
	else
		createRecordMap(map_name);


	char* err_str;
	int ret = sqlite3_exec(sqlite, "begin", 0, 0, &err_str);

	if (ret != SQLITE_OK)
		cout << "开始事务失败: " << err_str << endl;

	sqlite3_free(err_str);

	string table_name_map = map_name;
	table_name_map.append("_map");

	string table_name_plant = map_name;
	table_name_plant.append("_plant");

	string table_name_leaf = map_name;
	table_name_leaf.append("_leaf");

	for (int block_y = 0; block_y < WORLD_HEIGHT / 16; block_y++)
		for (int block_z = 0; block_z < WORLD_WIDTH / 16; block_z++)
			for (int block_x = 0; block_x < WORLD_WIDTH / 16; block_x++)
			{
				saveBlockMap(&createWorld->mapData, table_name_map.data(), block_x, block_z, block_y);
				saveBlockMap(&createWorld->plantData, table_name_plant.data(), block_x, block_z, block_y);
				saveBlockMap(&createWorld->leafData, table_name_leaf.data(), block_x, block_z, block_y);
			}


	ret = sqlite3_exec(sqlite, "commit", 0, 0, &err_str);
	if (ret != SQLITE_OK)
		cout << "提交事务失败: " << err_str << endl;

	sqlite3_free(err_str);
}

void OperationDB::loadWorldMap(const char* map_name)
{
	int ret = 0;
	string sql_str = "";
	sqlite3_stmt* stmt = nullptr;


	sql_str = "select * from ";
	sql_str.append(map_name).append("_map");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK) 
	{
		while (sqlite3_step(stmt) == SQLITE_ROW) 
		{
			int block_x = sqlite3_column_int(stmt, 0);
			int block_z = sqlite3_column_int(stmt, 1);
			int block_y = sqlite3_column_int(stmt, 2);

			const unsigned char* block_data = sqlite3_column_text(stmt, 3);

			int read_index = 0;
			char cube_type[2] = { 0 };

			for (int i = block_y * BLOCK_WIDTH; i < (block_y + 1) * BLOCK_WIDTH; i++)
				for (int j = block_z * BLOCK_WIDTH; j < (block_z + 1) * BLOCK_WIDTH; j++)
					for (int k = block_x * BLOCK_WIDTH; k < (block_x + 1) * BLOCK_WIDTH; k++)
					{
						memcpy(cube_type, block_data + read_index, sizeof(cube_type));
						read_index += 3;

						createWorld->mapData[k][i][j] = CubeType(atoi(cube_type));
					}
		}
	}
	else 
	{
		std::cout << "select block_data失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);


	sql_str = "select * from ";
	sql_str.append(map_name).append("_plant");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			int block_x = sqlite3_column_int(stmt, 0);
			int block_z = sqlite3_column_int(stmt, 1);
			int block_y = sqlite3_column_int(stmt, 2);

			const unsigned char* block_data = sqlite3_column_text(stmt, 3);

			int read_index = 0;
			char cube_type[2] = { 0 };

			for (int i = block_y * BLOCK_WIDTH; i < (block_y + 1) * BLOCK_WIDTH; i++)
				for (int j = block_z * BLOCK_WIDTH; j < (block_z + 1) * BLOCK_WIDTH; j++)
					for (int k = block_x * BLOCK_WIDTH; k < (block_x + 1) * BLOCK_WIDTH; k++)
					{
						memcpy(cube_type, block_data + read_index, sizeof(cube_type));
						read_index += 3;

						createWorld->plantData[k][i][j] = CubeType(atoi(cube_type));
					}
		}
	}
	else
	{
		std::cout << "select block_data失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);



	sql_str = "select * from ";
	sql_str.append(map_name).append("_leaf");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
	{
		while (sqlite3_step(stmt) == SQLITE_ROW)
		{
			int block_x = sqlite3_column_int(stmt, 0);
			int block_z = sqlite3_column_int(stmt, 1);
			int block_y = sqlite3_column_int(stmt, 2);

			const unsigned char* block_data = sqlite3_column_text(stmt, 3);

			int read_index = 0;
			char cube_type[2] = { 0 };

			for (int i = block_y * BLOCK_WIDTH; i < (block_y + 1) * BLOCK_WIDTH; i++)
				for (int j = block_z * BLOCK_WIDTH; j < (block_z + 1) * BLOCK_WIDTH; j++)
					for (int k = block_x * BLOCK_WIDTH; k < (block_x + 1) * BLOCK_WIDTH; k++)
					{
						memcpy(cube_type, block_data + read_index, sizeof(cube_type));
						read_index += 3;

						createWorld->leafData[k][i][j] = CubeType(atoi(cube_type));
					}
		}
	}
	else
	{
		std::cout << "select block_data失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);
}

bool OperationDB::isMapExist(const char* map_name)
{
	bool ret = false;
	sqlite3_stmt* stmt = nullptr;
	string sql_str = "select count(*)  from sqlite_master where type = 'table' and name = '";
	sql_str.append(map_name).append("_map'");

	sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);

	if (sqlite3_step(stmt) == SQLITE_ROW && sqlite3_column_int(stmt, 0) > 0)
		ret = true;

	sqlite3_finalize(stmt);

	return ret;
}

void OperationDB::deleteRecordMap(const char* map_name)
{
	int ret = 0;
	sqlite3_stmt* stmt = nullptr;
	string sql_str = "";


	sql_str = "delete from ";
	sql_str.append(map_name).append("_map");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
		sqlite3_step(stmt);
	else
		std::cout << "delete from失败: " << sqlite3_errmsg(sqlite) << std::endl;
	sqlite3_finalize(stmt);


	sql_str = "delete from ";
	sql_str.append(map_name).append("_plant");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
		sqlite3_step(stmt);
	else
		std::cout << "delete from失败: " << sqlite3_errmsg(sqlite) << std::endl;
	sqlite3_finalize(stmt);


	sql_str = "delete from ";
	sql_str.append(map_name).append("_leaf");
	ret = sqlite3_prepare_v2(sqlite, sql_str.data(), -1, &stmt, nullptr);
	if (ret == SQLITE_OK)
		sqlite3_step(stmt);
	else
		std::cout << "delete from失败: " << sqlite3_errmsg(sqlite) << std::endl;
	sqlite3_finalize(stmt);
}

OperationDB::OperationDB()
{

}

OperationDB::~OperationDB()
{
	if (sqlite) 
	{
		sqlite3_close_v2(sqlite);
		sqlite = nullptr;
	}
}

void OperationDB::createRecordMap(const char* map_name)
{
	int result = 0;
	char sql_str[100] = { 0 };
	sqlite3_stmt* stmt = nullptr;

	sprintf(sql_str, "create table %s_%s(block_x int, block_z int, block_y int, block_data text)", map_name, "map");
	result = sqlite3_prepare_v2(sqlite, sql_str, -1, &stmt, nullptr);
	if (result == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "创建表map成功" << std::endl;
	}
	else
	{
		std::cout << "创建表map失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);


	sprintf(sql_str, "create table %s_%s(block_x int, block_z int, block_y int, block_data text)", map_name, "plant");
	result = sqlite3_prepare_v2(sqlite, sql_str, -1, &stmt, nullptr);
	if (result == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "创建表plant成功" << std::endl;
	}
	else
	{
		std::cout << "创建表plant失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);


	sprintf(sql_str, "create table %s_%s(block_x int, block_z int, block_y int, block_data text)", map_name, "leaf");
	result = sqlite3_prepare_v2(sqlite, sql_str, -1, &stmt, nullptr);
	if (result == SQLITE_OK)
	{
		sqlite3_step(stmt);
		std::cout << "创建表leaf成功" << std::endl;
	}
	else
	{
		std::cout << "创建表leaf失败: " << sqlite3_errmsg(sqlite) << std::endl;
	}
	sqlite3_finalize(stmt);
}
