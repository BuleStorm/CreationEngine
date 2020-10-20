#include <iostream>
#include <vector>
//#include <conio.h>
//#include <windows.h>
//#include <winsock.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CreateWorld.h"

using namespace std;
#pragma comment(lib, "ws2_32.lib")
// 地图字节大小
constexpr int RECV_SIZE = WORLD_WIDTH * WORLD_HEIGHT * WORLD_WIDTH * sizeof(CubeType);

// 客户端连接服务器的IP
//#define IP "192.168.1.100"
#define IP "127.0.0.1"

// 人物动作
enum class PlaceStatus
{
	Normal,			// 正常
	Place,			// 放置
	Eliminate,		// 消除
};

// 人物状态结构体
struct PlayerData
{
	float eyeRadians;
	float bodyRadians;
	bool placeAnimtaion;
	CubeType cubeType;
	PlaceStatus placeStatus;
	glm::vec3 playerPos;
	glm::vec3 placePos;
};

// TCP通信类
class CommTCP
{
public:
	static CommTCP* getInstance();
	// 开始连接或监听
	void start();
	bool get_isServer();
	bool get_isMapRecvTrue();
	// 发送自己的地图数据
	void sendMap();
	// 接收对方地图数据
	void recvMap();
	// 自己的状态数据发送
	void sendPlayer(PlayerData pd);
	// 对方玩家状态数据接收
	void recvPlayer();

	// 接收对方玩家的状态数据
	PlayerData playerData;
private:
	CommTCP();
	~CommTCP();
	CommTCP(const CommTCP&) = delete;
	CommTCP& operator = (const CommTCP&) = delete;

	// 自己是不是服务器
	bool isServer = false;
//    SOCKET sockServer;
//    SOCKET sockClient;
//    sockaddr_in addr = { 0 };

	// 接收数据缓冲区
	vector<char> recvVec;
	// 地图是否传输完毕
	bool isMapRecvTrue = false;

	// 发送地图数据
	void sendData(vector<vector<vector<CubeType>>> *vec);
	// 接收地图数据
	void recvData(vector<vector<vector<CubeType>>>* vec);
	// 客户端模式
	bool client();
	// 服务器模式
    void server();
};

