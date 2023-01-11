
#ifndef FOR_XES
#include "hackgame.h"
#endif

//主程序
int main()
{
    try {
        clearScreen();
        logo();
        init_preload_ptr_table();
        loadgame();
        localhost->run(false,"本地控制台");//启动电脑！
    }catch(...){
        if (localhost){
            cout << "游戏抛出了未处理的异常，已经崩溃！幸而您可以导出当前的存档。" << endl;
            localhost->process_command("/bin/savegame.exe");
            cout << "请联系开发者（QQ：2492515121）获取帮助。我们将感谢您的反馈！" << endl;
        }else{
            cout << "游戏在加载时崩溃！请联系开发者（QQ：2492515121）获取帮助。我们将感谢您的反馈！" << endl;
        }
    }
    cout << "请保存游戏以便下次再来" << endl;
    localhost->process_command("/bin/savegame.exe");
    return 0;
}
