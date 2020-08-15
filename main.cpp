#include "hackgame.h"

//主程序
int main()
{
    loadgame();
    localhost->run(false,"本地控制台");//启动电脑！
    while(true)
    {
        localhost->run(true,"本地控制台");//断开后再连接
    }
    
    return 0;
}