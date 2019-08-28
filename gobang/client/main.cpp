#include "game.hpp"

static void Usage(string proc)
{
    cout << "输入手册 : " << proc << " ~(ŎдŎ；)" << endl;
    cout << "请像这样输入 : ip地址 端口号" << endl;
}
static void LoginMenu()
{
    cout << "#############################################" << endl;
    cout << "####        1.登录          2.注册       ####" << endl;
    cout << "####        3.退出                       ####" << endl;
    cout << "#############################################" << endl;
}
int main(int argc,char *argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        return 1;
    }
    string ip = argv[1];
    int port = atoi(argv[2]);
    string name;
    string passwd;
    uint32_t id;
    int select = 0;
    while(1)
    {
        cout << "          欢迎来到ETalien的游戏！！！"<< endl;
        LoginMenu();
        cout << "请选择你的操作 : ";
        cin >> select;
        switch(select)
        {
            case 1:
                {
                    uint32_t result = Login(ip, port);
                    if(result >= 10000){
                        id = result;
                        cout << "登陆成功！快来开始一局游戏吧！" << endl;
                        Game(ip,port,id);
                    }
                    else{
                        cout << "登录失败！失败码为 : " << result << endl;
                        cout << "失败码解释：1.未注册 2.密码错误 3.连接超时" << endl;
                    }
                }
                break;
            case 2:
                {
                    Register(ip, port, name, passwd, id);
                    cout << "恭喜你(*^▽^*)" << endl;
                    cout << "你得到了一个登录id，登录id为 : [" << id << "]" << endl;
                    cout << "现在开始登录吧！" << endl;
                }
                break;
            case 3:
                cout << "退出成功！" << endl;
                cout << "拜拜 ヾ(✿ﾟ▽ﾟ)ノ" << endl;
                exit(2);
            default:
                cout << "哎呦!(>ω<*) 你的选择错误，请重试！" << endl;
                break;
        }
    }
    return 0;
}
