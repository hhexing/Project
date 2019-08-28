#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <pthread.h>
#include "PlayerManager.hpp"
#include "RoomManager.hpp"

#define MATCH_LEVEL 101

using namespace std;

class Hall
{
    private:
        PlayerManager pm;
        RoomManager rm;
        vector<vector<uint32_t> >  match_pool;
        int match_num;
        pthread_mutex_t match_lock;
        pthread_cond_t match_cond;
    public:
    Hall()
        :match_pool(MATCH_LEVEL)
         ,match_num(0)
    {
    }
    int MatchNum()
    {
        return match_num;
    }
    void IncMatchNum()
    {
        match_num++;
    }
    void DecMatchNum()
    {
        match_num--;
    }
    void ResetMatchNum()
    {
        match_num = 0;
    }
    void GetAllMatchId(vector<uint32_t> &id_list)//把所有的id放到新的vector里
    {
        for(auto i = MATCH_LEVEL-1; i >= 0 ;i--)
        {
            auto &v = match_pool[i];
            if(v.empty())//是空，继续找下一个小vector
            {
                continue;
            }
            for(auto it = v.begin();it != v.end();it++)//不是空，把小vector里的数据一个一个的push_back到新vector里
            {
                id_list.push_back(*it);
            }
        }
    }

    void LockMatchPool()
    {
        pthread_mutex_lock(&match_lock);
    }
    void UnlockMatchPool()
    {
        pthread_mutex_unlock(&match_lock);
    }
    void ServiceWait()//让匹配线程等待
    {
        pthread_cond_wait(&match_cond,&match_lock);
    }
    void ServiceWakeUp()//唤醒匹配线程
    {
        pthread_cond_signal(&match_cond);
    }

    uint32_t Register(string &name, string &passwd)
    {
        return pm.InsertPlayer(name, passwd);
    }
    uint32_t Login(uint32_t &id,string &passwd)
    {
        return pm.SearchPlayer(id, passwd);
    }
    bool PushIdInMatchPool(uint32_t &id)//把用户id放在匹配池里
    {
        LOG(INFO,"Push player into match_pool...");
        pm.SetMatching(id);//用户状态变成MATCHING
        int rate = pm.GetRate(id);//获取当前id的胜率
        LockMatchPool();
        auto &v = match_pool[rate];//取到所有和当前胜率一样的那个存用户id的小vector
        auto it = v.begin();//遍历这个小vector看这个用户有没有在里面
        for(;it != v.end();it++)
        {
            if(*it == id)
            {
                return false;//有了就不进行插入，返回false
            }
        }
        v.push_back(id);//没有进行当前id的插入
        IncMatchNum();//匹配数量加1
        UnlockMatchPool();
        ServiceWakeUp();//唤醒匹配线程，让匹配池里的id进行匹配
        return true;
    }
    bool PopIdMatchPool(uint32_t &id)
    {
        LockMatchPool();
        int rate = pm.GetRate(id);
        auto &v = match_pool[rate];
        for(auto it = v.begin(); it != v.end(); it++)
        {
            if(*it == id)
            {
                v.erase(it);
                break;
            }
        }
        DecMatchNum();
        UnlockMatchPool();
        pm.SetOnline(id);
        return true;
    }
    void MatchPoolClear(uint32_t &id)
    {
        LOG(INFO,"match pool clear...");
        for(auto i = MATCH_LEVEL-1;i >=0;i--)
        {
            auto &v = match_pool[i];
            if(v.empty())//已经是空的小vector了，就继续找下一个小vector
            {
                continue;
            }
            vector<uint32_t>().swap(v);//创建匿名对象和v进行交换，出了作用域匿名对象会自动被销毁
        }
        ResetMatchNum();
        if(id >= 10000)//last
        {
            pm.SetOnline(id);
        }
    }
    int IsPlayerReady(uint32_t id)
    {
        return pm.Ready(id);
    }
    void GamePrepare(uint32_t &one,uint32_t &two)
    {
       pm.SetPlayerStatus(one, two);//设置PLAYING状态
       uint32_t room_id = rm.CreateRoom(one, two);//创建房间号
       pm.SetPlayerRoom(room_id, one, two);
    }
    static void *MatchService(void *arg)
    {
        pthread_detach(pthread_self());
        Hall *hp = (Hall *)arg;
        while(1)
        {
            uint32_t last = 0;
            hp->LockMatchPool();
            while(hp->MatchNum() < 2)//有可能被误唤醒
            {
                LOG(INFO,"service thread wait...");
                //等待
                hp->ServiceWait();
            }
            LOG(INFO,"service thread wakeup...");
            vector<uint32_t> id_list;//一个新的vector，存的是按顺序的用户id
            hp->GetAllMatchId(id_list);//把match_pool里的值都放到id_list里（按顺序）
            int num = id_list.size();//匹配的人数
            if(num & 1)//是奇数
            {
                last = id_list[id_list.size()-1];
                num &= (~1);
            }
            else
            {
                last = 0;
            }
            for(auto i = 0; i < num; i += 2)//两两匹配
            {
                uint32_t player_one = id_list[i];
                uint32_t player_two = id_list[i+1];
                hp->GamePrepare(player_one,player_two);//游戏准备工作
            }
            hp->MatchPoolClear(last);//清空匹配池
            hp->UnlockMatchPool();
        }
    }
    string GetPlayerBoard(uint32_t &room_id)
    {
        string board;
        //uint32_t room_id = pm.GetPlayerRoomId(id);
        rm.GetBoard(room_id,board);
        return board;
    }
    uint32_t GetPlayerRoomId(uint32_t &id)
    {
        return pm.GetPlayerRoomId(id);
    }
    char GetPlayerPiece(uint32_t &room_id, uint32_t &id)
    {
        return rm.GetPlayerPiece(room_id, id);
    }
    bool IsMyTurn(uint32_t &room_id, uint32_t &id)
    {
        return rm.IsMyTurn(room_id, id);
    }
    void Step(uint32_t &room_id, uint32_t &id, int x, int y)
    {
        return rm.Step(room_id, id, x, y);
    }
    char Judge(uint32_t &room_id, uint32_t &id)
    {
        return rm.Judge(room_id, id);
    }
    void InitHall()
    {
        pthread_mutex_init(&match_lock, NULL);
        pthread_cond_init(&match_cond,NULL);

        pthread_t tid;
        pthread_create(&tid, NULL, MatchService, this);
    }
    ~Hall()
    {
        pthread_mutex_destroy(&match_lock);
        pthread_cond_destroy(&match_cond);
    }
};





















