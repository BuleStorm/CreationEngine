#ifndef OPERATIONDB_H
#define OPERATIONDB_H

#include <sqlite3.h>

#include "CreateWorld.h"

// 区块边长
#define BLOCK_WIDTH 16

class OperationDB
{
public:
	static OperationDB* getInstance();
public:
	void init();
	void saveBlockMap(vec3 *data, const char* table_name, int block_x, int block_z, int block_y);
	void saveWorldMap(const char* map_name);
	void loadWorldMap(const char* map_name);
private:
	OperationDB();
	~OperationDB();
	OperationDB(const OperationDB&) = delete;
	OperationDB& operator = (const OperationDB&) = delete;

private:
	sqlite3* sqlite = nullptr;

	bool isMapExist(const char* map_name);
	void deleteRecordMap(const char* map_name);
	void createRecordMap(const char* map_name);
};

#endif // !OPERATIONDB_H