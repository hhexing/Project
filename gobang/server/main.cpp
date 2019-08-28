#include <iostream>
#include <rpc_server.h>
#include "Hall.hpp"

using namespace std;
using namespace rest_rpc;
using namespace rpc_service;

Hall GameHall;

uint32_t RpcRegister(connection* conn, string name, string passwd)
{
    cout << "Get a register request. Name is: "<< name <<" Passwd is: "<< passwd<<endl;
    return GameHall.Register(name, passwd);
}
uint32_t RpcLogin(connection* conn, uint32_t id, string passwd)
{
    return GameHall.Login(id, passwd);
}
bool RpcPushIdInMatchPool(connection* conn, uint32_t id)
{
    return GameHall.PushIdInMatchPool(id);
}
int RpcPlayerReady(connection* conn, uint32_t id)
{
    return GameHall.IsPlayerReady(id);
}
string RpcBoard(connection* conn, uint32_t room_id)
{
    return GameHall.GetPlayerBoard(room_id);
}
uint32_t RpcPlayerRoomId(connection* conn, uint32_t id)
{
    return GameHall.GetPlayerRoomId(id);
}
char RpcPlayerPiece(connection* conn, uint32_t room_id, uint32_t id)
{
    return GameHall.GetPlayerPiece(room_id, id);
}
bool RpcIsMyTurn(connection* conn, uint32_t room_id, uint32_t id)
{
    return GameHall.IsMyTurn(room_id, id);
}
void RpcStep(connection* conn, uint32_t room_id, uint32_t id, int x, int y)
{
    return GameHall.Step(room_id, id, x, y);
}
char RpcJudge(connection* conn, uint32_t room_id, uint32_t id)
{
    return GameHall.Judge(room_id, id);
}
bool RpcPopMatchPool(connection* conn, uint32_t id)
{
    return GameHall.PopIdMatchPool(id);
}
int main() {
    rpc_server server(9000, 4);

    LOG(INFO, "Server init complete!");

    server.register_handler("RpcRegister", RpcRegister);
    server.register_handler("RpcLogin", RpcLogin);
    server.register_handler("RpcPushIdInMatchPool", RpcPushIdInMatchPool);
    server.register_handler("RpcPlayerReady", RpcPlayerReady);
    server.register_handler("RpcPlayerRoomId", RpcPlayerRoomId);
    server.register_handler("RpcPlayerPiece", RpcPlayerPiece);
    server.register_handler("RpcBoard", RpcBoard);
    server.register_handler("RpcIsMyTurn", RpcIsMyTurn);
    server.register_handler("RpcStep", RpcStep);
    server.register_handler("RpcJudge", RpcJudge);
    server.register_handler("RpcPopMatchPool", RpcPopMatchPool);

    LOG(INFO, "Register all function...done");
    LOG(INFO, "Server start...done");
    GameHall.InitHall();

    server.run();

	std::string str;
	std::cin >> str;
}
