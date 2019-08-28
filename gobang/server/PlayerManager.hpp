#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include "Log.hpp"
#include "Player.hpp"

#define DEFAULT_ID 10000

using namespace std;

class PlayerManager
{
    private:
        unordered_map<uint32_t, Player> player_set;
        uint32_t assign_id;//起始id
    public:
        PlayerManager()
            :assign_id(DEFAULT_ID)
        {
            //pthread_mutex_init(&lock);
        }
        uint32_t InsertPlayer(string &name, string &passwd)//对应注册
        {
            //Lock();
            uint32_t id_ = assign_id++;
            Player p(name, passwd, id_);
            player_set.insert({id_, p});
            LOG(INFO, "Insert this player done...");
            //Unlock();
            return id_;
        }
        uint32_t SearchPlayer(uint32_t &id, string &passwd)//对应登录
        {
            //Lock();
            unordered_map<uint32_t,Player>::iterator it = player_set.find(id);
            if(it == player_set.end())
            {
                LOG(WARNING, "Search player error.This player is not register.");
                return 1;
            }
            //找到这个id的用户了
            Player &p = it->second;
            if(p.Passwd() != passwd)
            {
                LOG(WARNING, "Player passwd error.");
                return 2;
            }
            //密码相同可以登录
            p.Online();
            LOG(INFO, "Player online success!");
            //Unlock();
            return id;
        }
        void SetPlayerStatus(uint32_t &one,uint32_t &two)
        {
            //Lock();
            player_set[one].Playing();
            player_set[two].Playing();
            //Unlock();
        }
        void SetPlayerRoom(uint32_t &room_id, uint32_t &one, uint32_t &two)
        {
            //Lock();
            player_set[one].SetRoom(room_id);
            player_set[two].SetRoom(room_id);
            //Unlock();
        }
        uint32_t GetPlayerRoomId(uint32_t &id)
        {
            //Lock();
            uint32_t _room_id = player_set[id].Room();
            //Unlock();
            return _room_id;
        }
        void SetMatching(uint32_t &id)
        {
            //Lock();
            player_set[id].Matching();
            //Unlock();
        }
        void SetOnline(uint32_t &id)
        {
            //Lock();
            player_set[id].Online();
            //Unlock();
        }
        int GetRate(uint32_t &id)//获取胜率
        {
            //Lock();
            int rate = player_set[id].Rate();
            //Unlock();
            return rate;
        }
        int Ready(uint32_t &id)
        {
            //Lock();
            int st = player_set[id].Status();//返回用户的状态，看是不是游戏中
            //Unlock();
            return st;
        }
        ~PlayerManager()
        {
            //pthread_mutex_destroy(&lock);
        }
};

















