/*
 *文件名:   common_exe.cpp
 *作者:     Nictheboy
 *内容:     实现游戏中的大部分exe
 *
 *最后一次修改: 2020/5/7
 */
#ifndef FOR_XES
#include "hackgame.h"
#endif

/*
本代码用途是实现游戏中的大部分exe
每个函数实现一个exe程序
命名规则是这样的：int exe_wget(int n,const char **t, Computer *c)实现wget.exe,其他函数以此类推
参数的解释：
n是命令行传入参数的个数，相当于argn；t是指向参数列表的指针，相当于argv
这两个与c语言里main（）参数的意义相同
c指向启动这个程序的那台计算机
*/

int exe_wget(int n,const char **t, Computer *c)//实现wget.exe
{
    using namespace FileSystem;//需要使用文件系统
    if (n<2){//参数个数小于2
        cout<<"缺少参数！使用方法：\n";
        cout<<"wget [IP](:端口)           显示远程主机上可以通过http下载的文件列表\n";
        cout<<"wget [IP](:端口)  [文件名] 从远处主机上下载文件\n";
        return 0;
    }
    string ip;//输入的ip
    int port;//输入的端口
    try{//使用class ipport_cutter将输入的ip和端口分开
        //如果没有输入端口，该类也会正常工作
        ipport_cutter cutter(t[1],21);
        ip=cutter.ip;
        port=cutter.port;
    }
    catch(string s)
    {
        cout<<"输入的参数不符合语法！"<<s<<endl;
        return 0;
    }
    connection_reply reply=c->netnode->connect(ip,"http");
    //这行是网络更新之后加入的
    //c->netnode是启动wget的计算机对应的网络节点
    //调用其connect方法，其实就是从c出发寻找计算机
    if (reply.computer == nullptr)//没找着。说明输入的ip不对
    {
        cout<<"定位目标主机失败。请检查IP地址是否正确。\n";
        return 0;
    }
    
    dir *http_dir=reply.computer->get_http_dir(port);
    if (!http_dir)
    {
        delay(2);
        cout<<"无法连接至目标主机"<<port<<"端口的http服务!请检查端口和ip\n";
        return 0;
    }
    //cout<<"ok\n";
    if (n==2)
    {
        cout<<"远程http文件列表:\n";
        for (int i=0; i<http_dir->content.size(); i++)
        {
            cout<<http_dir->content[i]->name<<endl;
        }
    }else{
        string name=t[2];
        file *from=http_dir->locate_file(name);
        if (from)
        {
            cout<<"正在将远程文件"<<name<<"复制到本地/data文件夹...\n";
            reply.computer->write_log("http",c->netnode->ip+" 远程拷贝了文件 "+from->name);
            delay(3);
            void *to=malloc(from->size);
            memcpy(to,from->data,from->size);
            c->locate_dir("/data")->add_file(new file(from->name,from->size,to));
        }else{
            cout<<"远程文件"<<name<<"不存在!\n";
        }
    }
    return 0;
}

int exe_help(int i,const char **t, Computer *c)
{
    cout<<"黑客游戏 帮助\n";
    cout<<"-------------\n";
//    cout<<BG_RED<<"重点：本程序未完成，为中间版本，请谅解！"<<RESET<<endl;
    cout<<"本程序基本的使用方法与cmd一样\n";
    cout<<"可用命令：\n";
    /*
    请无视一下代码
        void dc();
        void telnet(int i,const char **t);
        void dir(int i,const char **t);
        void cd(int i,const char **t);
        void help(int i,const char **t);
        void pwd(int i,const char **t);
        void mail(int i,const char **t);
        void type(int i,const char **t);
        void copy(int i,const char **t);
        void mkdir(int i,const char **t);
        void del(int i,const char **t);
        void scp(int i,const char **t);
        void install(int i,const char **t);
    */
    cout<<"\n";
    cout<<"cd      切换目录\n";
    cout<<"copy    复制一个文件\n";
    cout<<"dc      与远程计算机断开连接\n";
    cout<<"del     删除一个文件或文件夹\n";
    cout<<"dir     列出当前目录的文件\n";
    cout<<"help    输出帮助\n";
    cout<<"install 从本地安装一个新的程序\n";
    cout<<"ipconfig显示网络设置\n";
    cout<<"ping    测试是否能连接到某一主机\n";
    cout<<"pwd     显示当前目录\n";
    cout<<"savegame[*新*]保存游戏\n";
    cout<<"scp     将远程计算机上的文件复制到本地\n";
    cout<<"telnet  连接远程计算机\n";
    cout<<"tree    树形列出当前目录下所有文件\n";
    cout<<"type    显示一个文本文档的内容\n";
    cout<<"mail    查看邮件\n";
    cout<<"mkdir   新建一个文件夹\n";
    //cout<<"\n";
    //cout<<"没了，就这么几个，不过这个简单的文件系统我真做了老半天，命令行系统也是我自己做的。命令太少，请谅解。\n";
    //cout<<BG_RED<<"童鞋，一定要试试telnet 29.53.103.3这个指令啊！口令是123456"<<RESET<<endl;
    return 0;
}

int exe_passguesser(int i,const char **t, Computer *c)
{
    //117.51.143.120
    string input;
    string ip;
    //int type=-1;
    int port=-1;
    
    cout<<BG_BLUE<<"----密码破解器v3.1.0----"<<RESET<<endl;
    cout<<BG_BLUE<<"| 中国黑客联盟荣誉出品 |"<<RESET<<endl;
    cout<<BG_BLUE<<"------------------------"<<RESET<<endl;
    delay(0.3);
    cout<<"正在加载中...";
    delay(2);
    cout<<"加载完毕！\n";
    cout<<"请输入目标主机IP或是IP和端口(如241.141.31.34或192.168.0.0:23):";
    getline(cin, input);
    try
    {
        ipport_cutter cutter(input,-1);
        ip=cutter.ip;
        port=cutter.port;
    }
    catch(string){  return 0;  }
    cout<<"请输入协议类型(telnet/ssh/ftp):";
    getline(cin, input);
    connection_reply reply=c->netnode->connect(ip);
    //cout<<ip<<"\t"<<port<<endl;
    if (input=="telnet")
    {
        if (port==-1)
        {
            port=23;
        }
        cout<<"正在连接主机中";
        if (reply.computer==nullptr || reply.computer->get_port("telnet")!=port)
        {
            delay(1);
            cout<<".";
            delay(1);
            cout<<".";
            delay(1);
            cout<<".";
            cout<<"无法连接到该主机的telnet服务！\n";
            return 0;
        }
        delay(0.5);
        cout<<".";
        delay(0.5);
        cout<<".";
        delay(0.5);
        cout<<".";
        cout<<endl<<endl<<BG_BLUE<<"成功连接到主机"<<ip<<"!"<<RESET<<endl;
        cout<<"正在加载用户/密码字典";
        delay(1);
        cout<<".";
        delay(1);
        cout<<".";
        delay(1);
        cout<<"."<<endl;
        delay(1);
        cout<<"正在尝试密码中,请耐心等待..."<<endl;
        if (reply.computer->password=="123456"||
            reply.computer->password=="password123"||
            reply.computer->password=="root"||
            reply.computer->password=="password0abc"||
            reply.computer->password=="8848")
        {
            delay(10);
            cout<<endl<<BG_BLUE<<"破解成功！"<<RESET<<endl;
            cout<<"|-目标主机IP:"<<ip<<endl;
            cout<<"|-目标主机端口:"<<port<<endl;
            cout<<"|-目标服务类型:telnet"<<endl;
            cout<<"|"<<endl;
            cout<<"|-"<<BG_RED<<"用户:"<<reply.computer->username<<RESET<<endl;
            cout<<"|-"<<BG_RED<<"密码:"<<reply.computer->password<<RESET<<endl;
        }
        else
        {
            cout<<"破解失败！"<<endl;
            return 0;
        }
    }
    else if (input=="ssh")
    {
        if (port==-1)
        {
            port=22;
        }
        cout<<"该功能正在开发中！更多内容敬请\n";
    }
    else if (input=="ftp")
    {
        if (port==-1)
        {
            port=21;
        }
        cout<<"该功能正在开发中！更多内容敬请\n";
    }
    else
    {
        cout<<"暂不支持该协议!\n";
    }
    
    //cout<<"该程序正在开发中！更多内容敬请\n";
    return 0;
}

int exe_hvm(int i,const char *t[], Computer *c)
{
    /*
    cout<<"正在初始化虚拟主机.."<<endl;
    Computer vNet[1];
    vNet[0].this_computer=0;
    vNet[0].ip_address=LOCAL_HOST;
    vNet[0].name="HVM";
    
    cout<<"正在初始化虚拟网络..."<<endl;
    Computer *old=computer;
    computer=vNet;
    
    
    cout<<"正在启动虚拟主机..."<<endl;
    cout<<BG_RED<<"虚拟主机已启动！输入dc关闭虚拟主机"<<RESET<<endl<<endl;
    
    localhost->run(false,computer[c].ip_address);
    
    cout<<"虚拟主机已关闭"<<endl;
    
    computer=old;
    */
    return 0;
}

int exe_telnet(int i,const char **t,Computer *c)
{
    if (i<2)
    {
        cout<<"参数不足!语法:telnet [主机IP](:[端口号])\n";
        return 0;
    }
    
    int port;
    string ip;
    try
    {
        ipport_cutter cutter(t[1],23);
        port=cutter.port;
        ip=cutter.ip;
    }
    catch(string)
    {
        return 0;
    }
    connection_reply reply=c->netnode->connect(ip,"telnet");
    c->netnode->send_package(ip, "试图建立连接", "telnet");
    if (reply.computer&&reply.computer->get_port("telnet")==port)
    {
        cout<<"正在连接远程主机"<<ip<<"中...";
        delay(2);
        cout<<"连接成功\n";
        c->netnode->send_package(ip, "校验握手", "telnet");
        stringstream stm;
        stm<<"成功连接远程计算机"<<ip<<":"<<port;
        c->netnode->send_package(ip, "建立连接数据通道", "telnet");
        string log_to_write;
        stm>>log_to_write;
        c->write_log("telnet",log_to_write);
        reply.computer->run(true,c->netnode->ip,c->netnode);
        cout<<"[远程主机断开连接]\n";
        return 0;
    }
    cout<<"正在连接远程主机"<<ip<<"中...";
    delay(5);
    cout<<"\n响应超时。请检查IP地址是否正确!\n";
    return 0;
}

int exe_tree(int i,const char **t,Computer *c)
{
    if (i<2){
        c->dir_now->show();
    }else{
        if (!c->locate_dir(t[1])){
            cout<<"文件夹不存在"<<endl;
            return 0;
         }
        c->locate_dir(t[1])->show();
    }
    return 0;
}

int exe_ipconfig(int i,const char **t,Computer *c)
{
    cout<<"本地网络配置器"<<endl;
    cout<<"---------------------"<<endl;
    cout<<"IPv4地址:"<<c->netnode->ip<<endl;
    cout<<"默认网关:"<<c->netnode->father->ip<<endl;
    if (c->port_list.size()>0)
    {
        cout<<"开放端口:"<<endl;
        for (int i=0;i<c->port_list.size();i++)
        {
            cout<<c->port_list[i].port<<"/"<<c->port_list[i].name<<endl;
        }
    }
    cout<<"子网信息:"<<endl;
    c->netnode->show();
    if (true)
    {
        cout<<BG_RED<<"以下信息不会在正式版中显示！"<<RESET<<endl;
        cout<<"全局网络表:"<<endl;
        Internet->show();
    }
    return 0;
}

int exe_ping(int i,const char **t,Computer *c)
{
    int sent=0;
    int received=0;
    int lost=0;
    connection_reply reply(NULL,NULL,"software error");
    if (i<2)
    {
        cout<<"参数不足!语法:ping [主机IP]\n";
        return 0;
    }
    string ip = net_node::get_dns_reply(t[1]);
    cout<<"Ping "<<ip<<" with 32 bytes of data:"<<endl;
    for (int i=0;i<4;i++)
    {
        sent++;
        reply=c->netnode->connect(ip);
        if (reply.computer)
        {
            received++;
            cout<<"Reply from "<<ip<<": bytes=32 time="<<rand()%5+20<<" TTL=100"<<endl;
            delay(0.5);
        }
        else
        {
            lost++;
            delay(3);
            cout<<"Request time out. Error code: "<<reply.status<<endl;
        }
    }
    cout<<endl;
    cout<<"Ping statistics for "<<ip<<":"<<endl;
    cout<<"\tPackets: Sent = "<<sent
        <<", Received = "<<received
        <<", Lost = "<<lost
        <<" <"<<lost/sent*100<<"% loss>"<<endl;
    return 0;
}

int exe_savegame(int i,const char **t,Computer *c)
{
    cout<<"请输入存档文件名:";
    string input;
    getline(cin,input);
    save_game_to_file(input);
    return 0;
}

int exe_myexe(int n,const char **t, Computer *c)
{
    cout<<"Hello,HackGame!"<<endl;
    return 0;
}

int exe_portscan(int n,const char **t, Computer *c)
{
    if (n<2)
    {
        cout<<"参数不足!语法:portscan [主机IP]\n";
        return 0;
    }
    connection_reply reply(NULL,NULL,"software error");
    reply = c->netnode->connect(t[1]);
    Computer *rc = reply.computer;
    if (rc == NULL)
    {
        delay(5);
        cout<<"主机不可达或无开放端口"<<endl;
    }
    else
    {
        for (int i=0; i < rc->port_list.size(); i++)
        {
            delay(rand()%3+1);
            cout<<"发现开放端口"<<rc->port_list[i].port<<endl;
        }
    }
    return 0;
}

int exe_tracer(int n,const char **t, Computer *c)
{
    if (n<2)
    {
        cout<<"参数不足!语法:tracer [主机IP]\n";
        return 0;
    }
    cout<<string("正在追踪从 本地主机 到 ") + t[1] + " 的路由..."<<endl;
    bool is_successful = false;
    bool up_failed = false;
    bool end = false;
    net_node *now = c->netnode;
    cout<<"|发送者\t"<<now->ip<<endl;
    while (true)
    {
        if (now->ip == "Internet")
        {
            cout<<"|\t数据包离开公网"<<endl;
        }
        for (int i=0; i<now->subnet.size(); i++)
        {
            if (now->subnet[i]->ip == t[1])
            {
                cout<<"|接收者\t"<<t[1]<<endl;
                is_successful = true;
                end = true;
                break;
            }
        }
        if (end == true)
        {
            break;
        }
        if (now->net_map.count(t[1]) != 0)
        {
            up_failed = true;
            now = now->net_map[t[1]];
            cout << "|路由\t" << now->ip << endl;
        }
        else
        {
            if (up_failed)
            {
                cout<<RED<<BG_BLUE<<"数据包丢失，无法追踪 !"<<RESET<<endl;
                break;
            }
            else
            {
                if (now->father)
                {
                    now = now->father;
                    if (now->ip == "Internet")
                    {
                        cout<<"|\t数据包进入公网，无法追踪"<<endl;
                    }
                    else
                    {
                        cout << "|路由\t" << now->ip << endl;
                    }
                }
                else
                {
                    break;
                }
            }
        }
    }
    cout<<"-------------------------"<<endl;
    if (is_successful)
    {
        cout<<"追踪成功"<<endl;
    }
    else
    {
        cout<<"追踪失败"<<endl;
    }
    return 0;
}
