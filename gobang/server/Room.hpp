#pragma once

#include <iostream>

using namespace std;

#define SIZE 5
#define BLACK 'X'
#define WHITE 'O'

class Room
{
    private:
        uint32_t one;
        uint32_t two;
        uint32_t current;
        char piece[2];
        char board[SIZE][SIZE];
        char result;//X:第一个玩家赢  O:第二个玩家赢  E:平局  N:继续游戏

        pthread_mutex_t lock;
    public:
        Room()
        {
        }
        Room(uint32_t &id1,uint32_t &id2)
            :one(id1)
             ,two(id2)
        {
            piece[0] = 'X';
            piece[1] = 'O';
            memset(board,' ',sizeof(board));
            result = 'N';
            current = one;
            pthread_mutex_init(&lock,NULL);
        }
        void Board(string &_board)
        {
            for(auto i = 0; i < SIZE; i++)
            {
                for(auto j = 0; j < SIZE; j++)
                {
                    _board.push_back(board[i][j]);
                }
            }
        }
        char Piece(uint32_t &id)
        {
            int pos = (id == one ? 0 : 1);
            return piece[pos];
        }
        bool IsMyTurn(uint32_t &id)
        {
            return id == current ? true : false;
        }
        void Step(uint32_t &id, int &x, int &y)
        {
            if(current == id)
            {
                int pos = (id == one ? 0 : 1);
                board[x][y] = piece[pos];
                current = (id == one ? two : one);
                result = Judge();
            }
        }
        char GameResult(uint32_t &id)
        {
            return result;
        }
		
		int SumL(int x, int y)
		{
			int sum = 0;
			int i = x;
			while(i >= 0)
			{
				if(board[x][y] == board[--i][y])
					sum++;
				else
					break;
			}
			return sum;
		}
		
		int SumL(int x, int y)
		{
			int sum = 0;
			int i = x;
			while(i < SIZE)
			{
				if(board[x][y] == board[++i][y])
					sum++;
				else
					break;
			}
			return sum;
		}
		
		int SumA(int x, int y)
		{
			int sum = 0;
			int i = y;
			while(y >= 0)
			{
				if(board[x][y] == board[x][--y])
					sum++;
				else
					break;
			}
			return sum;
		}
		int SumD(int x, int y)
		{
			int sum = 0;
			int i = y;
			while(y < SIZE)
			{
				if(board[x][y] == board[x][++y])
					sum++;
				else
					break;
			}
			return sum;
		}
		
		int Sum1(int x, int y)
		{
			int sum = 0;
			int i = x;
			int j = y;
			while(x >= 0 && y >= 0)
			{
				if(board[x][y] == board[--x][--y])
					sum++;
				else
					break;
			}
			return sum;
		}
		
		int Sum2(int x, int y)
		{
			int sum = 0;
			int i = x;
			int j = y;
			while(x < SIZE && y < SIZE)
			{
				if(board[x][y] == board[++x][++y])
					sum++;
				else
					break;
			}
			return sum;
		}
		
		int Sum3(int x, int y)
		{
			int sum = 0;
			int i = x;
			int j = y;
			while(x < SIZE && y >= 0)
			{
				if(board[x][y] == board[++x][--y])
					sum++;
				else
					break;
			}
			return sum;
		}
		
		int Sum4(int x, int y)
		{
			int sum = 0;
			int i = x;
			int j = y;
			while(x >= 0 && y < SIZE)
			{
				if(board[x][y] == board[--x][++y])
					sum++;
				else
					break;
			}
			return sum;
		}
		
		char Judge(int x, int y)
		{
			int across = SumL(x, y) + SumR(x, y) +1;//横向
			int upright = SumA(x, y) +SumD(x, y) +1;//竖向
			int italicD = Sum1(x, y) + Sum2(x, y) +1;//斜下方
			int italicA = Sum3(x, y) + Sum4(x, y) +1;//斜上方
			int 
			if(across >= 5 || upright >= 5 || italicD >= 5 || italicA >= 5)
				return board[x][y];
			for(int i = 0; i <SIZE; i++)
			{
				for(int j  0; j < SIZE; j++)
				{
					if(board[i][j] ==' ')
						return 'N'
				}	
			}
			return 'E'	
		}

		
        // char Judge()
        // {
            // int row = SIZE;
            // int col = SIZE;
            // //1.有结果的情况
            // //判断行
            // for(auto i = 0; i < row; i++)
            // {
                // if(board[i][0] !=' ' && \
                   // board[i][0] == board[i][1] && \
                   // board[i][1] == board[i][2] && \
                   // board[i][2] == board[i][3] && \
                   // board[i][3] == board[i][4])
                // {
                    // return board[i][0];
                // }
            // }
            // //判断列
            // for(auto i = 0; i < col; i++)
            // {
                // if(board[0][i] !=' ' && \
                   // board[0][i] == board[1][i] && \
                   // board[1][i] == board[2][i] && \
                   // board[2][i] == board[3][i] && \
                   // board[3][i] == board[4][i])
                // {
                    // return board[0][i];
                // }
            // }
            // //判断左对角线
            // if(board[0][0] !=' ' && \
               // board[0][0] == board[1][1] && \
               // board[1][1] == board[2][2] && \
               // board[2][2] == board[3][3] && \
               // board[3][3] == board[4][4])
            // {
                // return board[0][0];
            // }
            // //判断右对角线
            // if(board[0][4] !=' ' && \
               // board[0][4] == board[1][3] && \
               // board[1][3] == board[2][2] && \
               // board[2][2] == board[3][1] && \
               // board[3][1] == board[4][0])
            // {
                // return board[0][4];
            // }
            // //2.继续的情况
            // for(auto i = 0; i < row ; i++)
            // {
                // for(auto j = 0; j < col; j++)
                // {
                    // if(board[i][j]==' ')
                        // return 'N';
                // }
            // }
            // //3.平局的情况
            // return 'E';
        // }
        ~Room()
        {
            pthread_mutex_destroy(&lock);
        }
};










