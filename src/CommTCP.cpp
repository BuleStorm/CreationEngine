#include "CommTCP.h"
#include "EliminateAnima.h"

static CreateWorld *createWorld = CreateWorld::getInstance();
static EliminateAnima* eliminateAnima = EliminateAnima::getInstance();

CommTCP* CommTCP::getInstance()
{
    static CommTCP commTCP;
    return &commTCP;
}

void CommTCP::start()
{
//    recvVec.resize(RECV_SIZE);

//    WSADATA wsa = { 0 };
//    WSAStartup(MAKEWORD(2, 2), &wsa);

//    sockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//    addr.sin_family = AF_INET;
//    addr.sin_port = htons(20203);

//    if (client())
//        server();
}

CommTCP::CommTCP()
{
}

CommTCP::~CommTCP()
{
//    closesocket(sockClient);
//    closesocket(sockServer);

//    WSACleanup();
}

void CommTCP::sendMap()
{
    sendData(&createWorld->mapData);
    sendData(&createWorld->plantData);
    sendData(&createWorld->leafData);

    isMapRecvTrue = true;
}

void CommTCP::recvMap()
{
    recvData(&createWorld->mapData);
    recvData(&createWorld->plantData);
    recvData(&createWorld->leafData);

    isMapRecvTrue = true;
}

void CommTCP::sendPlayer(PlayerData pd)
{
    // 防止传输地图时数据混乱, 懒得分端口, 但是还是会存在传输地图时的数据混乱, 不管了
//    if (isMapRecvTrue)
//        send(sockClient, reinterpret_cast<char*>(&pd), sizeof(PlayerData), 0);
}

void CommTCP::recvPlayer()
{
    int readSize = 0;
    int readAll = 0;

    recvVec.clear();

//    do
//    {
//        if (readSize != 0)
//            readSize = recv(sockClient, &recvVec[0] + readAll, sizeof(PlayerData) - readSize, 0);
//        else
//            readSize = recv(sockClient, &recvVec[0] + readAll, sizeof(PlayerData), 0);

//        readAll += readSize;

//        if (readAll == sizeof(PlayerData))
//        {
//            readAll = 0;
//            readSize = 0;
//            memcpy(&playerData, &recvVec[0], sizeof(PlayerData));

//            // 更新地图最新状态
//            if (playerData.placeStatus == PlaceStatus::Eliminate)
//            {
//                createWorld->mapData[playerData.placePos.x][playerData.placePos.y][playerData.placePos.z] = CubeType::Null;
//                eliminateAnima->push(playerData.placePos, playerData.cubeType);
//            }
//            else if (playerData.placeStatus == PlaceStatus::Place)
//            {
//                createWorld->mapData[playerData.placePos.x][playerData.placePos.y][playerData.placePos.z] = playerData.cubeType;
//            }
//        }

//    } while (true);
}

void CommTCP::sendData(vector<vector<vector<CubeType>>>* vec)
{
//    for (int i = 0; i < WORLD_WIDTH; i++)
//        for (int j = 0; j < WORLD_HEIGHT; j++)
//        {
//            const char* data = reinterpret_cast<char*>(&(*vec)[i][j][0]);
//            int sendSize = send(sockClient, data, WORLD_WIDTH * sizeof(CubeType), 0);
//            if (sendSize != WORLD_WIDTH * sizeof(CubeType))
//                return;
//        }
}

void CommTCP::recvData(vector<vector<vector<CubeType>>>* vec)
{
//    int readSize = 0;
//    int readAll = 0;

//    do
//    {
//        readSize = recv(sockClient, &recvVec[0] + readAll, RECV_SIZE, 0);
//        if (readSize > 0)
//            readAll += readSize;
//    } while (readAll < RECV_SIZE);

//    for (int i = 0; i < WORLD_WIDTH; i++)
//        for (int j = 0; j < WORLD_HEIGHT; j++)
//        {
//            memcpy(&(*vec)[i][j][0],
//                &recvVec[0] + i * (WORLD_HEIGHT)*WORLD_WIDTH * sizeof(CubeType) +
//                j * WORLD_WIDTH * sizeof(CubeType), WORLD_WIDTH * sizeof(CubeType));
//        }
}

bool CommTCP::client()
{
//    addr.sin_addr.S_un.S_addr = inet_addr(IP);
//    sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

//    int ret = connect(sockClient, (struct sockaddr*) & addr, sizeof(addr));
//    if (SOCKET_ERROR == ret)
//    {
//        closesocket(sockClient);
//        return true;
//    }
    
    return false;
}

void CommTCP::server()
{
    isServer = true;

//    addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
//    bind(sockServer, (struct sockaddr*) & addr, sizeof(addr));
//    listen(sockServer, SOMAXCONN);

//    sockaddr_in clientaddr = { 0 };

//    int len = sizeof(clientaddr);

//    sockClient = accept(sockServer, (struct sockaddr*) & clientaddr, &len);
}

bool CommTCP::get_isServer()
{
    return isServer;
}

bool CommTCP::get_isMapRecvTrue()
{
    return isMapRecvTrue;
}
