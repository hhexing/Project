#pragma once

#include <iostream>
#include <rpc_client.hpp>
#include <chrono>
#include <stdlib.h>//atoi
#include <string>
#include <fstream>
#include "codec.h"

using namespace std;
using namespace std::chrono_literals;
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;

uint32_t Register(const string &ip, const int &port, string &name, string &passwd, uint32_t &id)
{
    cout << "请输入昵称 : ";
    cin >> name;
    cout << "请输入密码 : ";
    cin >> passwd;
    string passwd_;
    cout << "请确认密码 : ";
    cin >> passwd_;
    if(passwd != passwd_)
    {
        cout << "两次秘密输入不一致！" << endl;
        return 1;
    }
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 2;
        }
		id = client.call<uint32_t>("RpcRegister", name, passwd);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
uint32_t Login(const string &ip, const int &port)
{
    uint32_t id;
    string passwd;
    cout << "请输入你的登录id : ";
    cin >> id;
    cout << "请输入登录密码 : ";
    cin >> passwd;
    uint32_t result = 0;
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		result = client.call<uint32_t>("RpcLogin", id, passwd);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return result;
}
bool PushIdInMatchPool(string &ip, int &port, uint32_t &id)//把id放到匹配池里
{
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		return client.call<bool>("RpcPushIdInMatchPool", id);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
int CheckReady(string &ip, int &port, uint32_t id)
{
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		return client.call<int>("RpcPlayerReady", id);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
bool PopMatchPool(string &ip, int &port, uint32_t &id)
{
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		return client.call<bool>("RpcPopMatchPool", id);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return false;
}
bool Match(string &ip, int &port, uint32_t &id)//真正的匹配
{
    PushIdInMatchPool(ip,port,id);//把id先放到匹配池里
    //周期性的检测状态
    int count = 20;
    while(1)
    {
        int ready = CheckReady(ip, port, id);
        if(ready == 3)
        {
            return true;
        }
        else if(ready == 1)
        {
            cout << "匹配失败！" << endl;
            return false;
        }
        else//等于2，继续匹配
        {
            printf("匹配倒计时：%2d\r",count--);
            fflush(stdout);
            if(count < 0)
            {
                cout << endl;
                cout << "匹配超时！" << endl;
                PopMatchPool(ip, port, id);
                break;
            }
            sleep(1);
        }
    }
    return false;
}
int GetBoard(string &ip, int &port, uint32_t &room_id, string &board)
{
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		board = client.call<string>("RpcBoard", room_id);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
uint32_t GetMyRoomId(string &ip, int &port, uint32_t &id)
{
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		return client.call<uint32_t>("RpcPlayerRoomId", id);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
char GetMyPiece(string &ip, int &port, uint32_t &room_id, uint32_t &id)
{
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		return client.call<char>("RpcPlayerPiece", room_id, id);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
void ShowBoard(string &board)
{
    cout << "    ";
    for(auto i = 1; i <= 5; i++)
    {
        cout << i << "   ";
    }
    cout << endl;
    for(auto i = 0; i <= 5; i++)
    {
        cout << "----";
    }
    cout << endl;

    int size = board.size();
    int basic_size = 5;
    for(auto i = 0; i < size/basic_size; i++)
    {
        cout << i+1 << " |";
        for(auto j = 0; j < basic_size; j++)
        {
            cout << " " << board[i*basic_size+j] << " |";
        }
        cout << endl;
        for(auto i = 0; i <= 5; i++)
        {
            cout << "----";
        }
        cout << endl;
    }
}
bool IsMyTurn(string &ip, int &port, uint32_t &room_id, uint32_t &id)
{
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		return client.call<bool>("RpcIsMyTurn", room_id, id);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
bool PosIsRight(string &board, int x, int y)//判断位置是否已经被占用
{
    int pos = (x-1)*5 + (y-1);
    return board[pos] == ' ' ? true : false;
}
int Step(string &ip, int &port, uint32_t &room_id, uint32_t &id, int x, int y)
{
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		client.call<void>("RpcStep", room_id, id, x-1, y-1);
        return 0;
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
char Judge(string &ip, int &port, uint32_t &room_id, uint32_t &id)//判断有没有人赢
{
    try {
        rpc_client client(ip, port);
        bool r = client.connect();
        if (!r) {
            std::cout << "连接超时" << std::endl;
            return 3;
        }
		return client.call<char>("RpcJudge", room_id, id);
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
void PlayGame(string &ip, int &port, uint32_t &id)
{
    cout << "匹配成功，开始游戏！" << endl;
    int x,y;
    char result = 'N';
    string board;
    uint32_t room_id = GetMyRoomId(ip, port, id);
    if(room_id < 1024)
    {
        return;
    }
    cout << "房间号：" << room_id << endl;
    char piece = GetMyPiece(ip, port, room_id, id);
    cout << "您的棋子为：" << piece << endl;
    for( ; ; )
    {
        system("clear");
        GetBoard(ip, port, room_id, board);//获得棋盘
        ShowBoard(board);//显示棋盘
        if((result = Judge(ip,port,room_id,id)) != 'N')
        {
            break;
        }
        if(!IsMyTurn(ip, port, room_id, id))//判断是不是该我走
        {
            cout << "对手正在思考，请稍等" << endl;
            sleep(1);
            continue;
        }
        //该我走
        cout << "请输入你的落子位置：";
        cin >> x >> y;
        if(x >= 1 && x <= 5 && y >= 1 && y <= 5)//判断位置是否合法
        {
            if(!PosIsRight(board, x, y))//判断位置是否已经被占用
            {
                cout << "你输入的位置已经被占用，请重新输入！" << endl;
            }
            else
            {
                Step(ip, port, room_id, id, x, y);
                result = Judge(ip, port, room_id, id);//判断有没有人赢
                if(result != 'N')//有结果了
                {
                    break;
                }
            }
        }
        else
        {
            cout << "你输入的位置有误，请重新输入！" << endl;
        }
    }
    if(result == 'E')
    {
        cout << "平局" << endl;
    }
    else if(result == piece)
    {
        cout << "恭喜，你赢了" << endl;
    }
    else
    {
        cout << "你输了" << endl;
    }
}
void Game(string &ip, int &port, uint32_t &id)//登录成功会来到游戏界面
{
    int select = 0;
    volatile bool quit = false;
    while(!quit)
    {
        cout << "**************************************" << endl;
        cout << "***      1.匹配         2.退出     ***" << endl;
        cout << "**************************************" << endl;
        cout << "请选择操作 : ";
        cin >> select;
        switch(select)
        {
            case 1:
                {
                    if(Match(ip,port,id)){//匹配成功进入游戏
                        PlayGame(ip, port, id);
                    }
                    else{//匹配失败提示玩家匹配失败
                        cout << "匹配失败" << endl;
                    }
                }
                break;
            case 2:
                quit = true;
                break;
            default:
                cout << "输入有误！" << endl;
                break;
        }
    }
}












