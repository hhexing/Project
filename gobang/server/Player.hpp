#pragma once

#include <iostream>
#include <string>
#include <pthread.h>

#define TIME_OUT 20
using namespace std;

typedef enum
{
    OFFLINE = 0,
    ONLINE,
    MATCHING,
    PLAYING,
}status_t;

class Player
{
    private:
        string name;
        string passwd;
        uint32_t id;

        int win;
        int lose;
        int tie;//ping ju
        
        status_t st;//player status
        uint32_t room_id;

        //pthread_mutex_t lock;
        //pthread_cond_t cond;

    public:
        Player()
        {

        }
        Player(string &name_, string &passwd_, uint32_t &id_)
            :name(name_)
             ,passwd(passwd_)
             ,id(id_)
        {
            win = 0;
            lose = 0;
            tie = 0;
            st = OFFLINE;
           // pthread_mutex_init(&lock,NULL);
           // pthread_cond_init(&cond,NULL);
        }
        const string& Passwd()
        {
            return passwd;
        }
        void Online()
        {
            st = ONLINE;
        }
        void Matching()
        {
            st = MATCHING;
        }
        void Playing()
        {
            st = PLAYING;
        }
        int Status()
        {
            return st;
        }
        int Rate()
        {
            int total = win + lose;
            if(total == 0)
            {
                return 0;
            }
            return win*100/total;
        }
        uint32_t Room()
        {
            return room_id;
        }
        void SetRoom(uint32_t room_id_)
        {
            room_id = room_id_;
        }
        ~Player()
        {
        }

};


















