#include "hackgame.h"

namespace namespace_exe_sniffer
{
    void package_arrive(data_package package, net_node *caller)
    {
        if (caller->binded_computer != NULL)
        {
            caller->binded_computer->write_log(
                "sniffer",
                string("\n") + "[" + package.from + "]->[" + package.to + "] : " + package.type + "\n" +
                "status:\t" + to_string(package.status) + "\n" +
                "data:\t" + package.data
            );
        }
    }
}

int exe_sniffer(int argc, const char *argv[], Computer *sender)
{
    if (argc<2)
    {
        cout<<"缺少参数！使用方法：\n";
        cout<<"sniffer start    启动嗅探器\n";
        cout<<"sniffer stop     停止嗅探器\n";
        return 0;
    }
    if (strcmp(argv[1],"start") == 0)
    {
        sender->netnode->on_package_arrive = &namespace_exe_sniffer::package_arrive;
        sender->netnode->on_package_pass = &namespace_exe_sniffer::package_arrive;
        return 0;
    }
    if (strcmp(argv[1],"stop") == 0)
    {
        sender->netnode->on_package_arrive = NULL;
        sender->netnode->on_package_pass = NULL;
        return 0;
    }
    cout<<"参数错误！使用方法：\n";
    cout<<"sniffer start    启动嗅探器\n";
    cout<<"sniffer stop     停止嗅探器\n";
    return 0;
}