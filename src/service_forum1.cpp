/*
 *文件名:   service_forum1.cpp
 *作者:     Nictheboy
 *内容:     实现service_forum1
 *
 *最后一次修改: 2023/1/11
 */
#ifndef FOR_XES
#include "hackgame.h"
#endif

int service_forum1(int n,const char **t, Computer *c)
{
    cout << "Welcome to hacker.ghg!" << endl;
    cout << "This is a web site for test." << endl;
    cout << "Info: ip=" << c->netnode->ip << ", path=" << t[0] << endl;
    c->process_command("tree /");
    for (int i=0; i<n; i++)
    {
        cout << "arg[" << i <<"] = " << t[i] <<endl;
    }
    return 0;
}

