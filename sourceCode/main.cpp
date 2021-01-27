#ifndef FOR_XES
#include "hackgame.h"
#endif

//主程序
int main()
{
    clearScreen();
    logo();
    init_preload_ptr_table();
    loadgame();
    localhost->run(false,"本地控制台");//启动电脑！
    while(true)
    {
        localhost->run(true,"本地控制台");//断开后再连接
    }
    
    return 0;
}
