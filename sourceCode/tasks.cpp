/*
 *文件名:   tasks.cpp
 *作者:     Nictheboy
 *内容:     实现游戏中的关卡
 *
 */
#ifndef FOR_XES
#include "hackgame.h"
#endif

//一下是新的剧情函数列表
bool task_new_1();
bool task_new_2();
bool task_new_3();
bool task_new_4();
bool task_new_5();
bool task_new_6();
bool task_new_7();
bool task_new_8();
bool task_new_9();
bool end_task();
typedef bool (*task_function)();
task_function task_added_function[10]={//新的剧情函数数组
    task_new_1,
    task_new_2,
    task_new_3,
    task_new_4,
    task_new_5,
    task_new_6,
    task_new_7,
    task_new_8,
    task_new_9,
    end_task
};

//下面是一下剧情函数

//用来结束游戏
void game_over(int i)//i=1:未删除日志
{
    switch(i)
    {
    case 1:
        cout<<BG_RED<<"您忘记删除日志了！！！警察发现了你的黑客行为，您被捕了"<<RESET<<endl;
        break;
    default:
        cout<<"Case Undifined!!!"<<endl;
    }
    delay(9999999);
    exit(0);
}

//这一堆函数控制剧情逻辑
void task_1_1()//这个任务还有让用户设置用户名密码的功能
{
#ifdef MAKE_SAVINGS  
    save_game_to_file("auto_created_saving_task1"); 
#endif
    clearScreen();
    cout<<"请您为自己起一个名字:";
    string tmp;
    getline(cin, tmp);
    localhost->username=tmp;
    cout<<"设置您的主机密码:";
    getline(cin, tmp);
    localhost->password=tmp;
    cout<<"设置您的主机名:";
    getline(cin, tmp);
    localhost->name=tmp; 
    clearScreen();
    //delay(2);
    cout<<BG_RED<<"您收到一封邮件！输入mail查看\n\n"<<RESET;
    
    sub_task_number=0;
    task_number=1;//设置关卡变量是因为mail程序根据关卡变量决定输出的邮件
    
    localhost->event_before_input=NULL;
    Internet->connect(COMPUTER1).computer->event_before_input=&task_1_2;//将任务绑定到主机2，用于检测玩家是否连接成功
}
void task_1_2()//玩家连接到主机2后，执行此程序
{
    sub_task_number=1;
    Internet->connect(COMPUTER1).computer->event_before_input=NULL;
    localhost->event_before_input=&task_2_1;//进入下一关
    //localhost->process_event_before_input();//先处理一下
}
void task_2_1()
{
#ifdef MAKE_SAVINGS  
    save_game_to_file("auto_created_saving_task2");
#endif
    cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
    task_number=2;
    localhost->event_before_input=&task_2_2;//下一子关卡
    localhost->process_event_before_input();//先处理一下
}
void task_2_2()
{
    using namespace FileSystem;
    //cout<<"okokokok\n";
    file *get=localhost->root->locate_file("data/wget.exe");
    //cout<<"okokokok\n";
    if (get)//是否有文件，检测是否拷贝了文件
    {
        task_3_1();
    }
}
void task_3_1()
{
#ifdef MAKE_SAVINGS  
    save_game_to_file("auto_created_saving_task3");
#endif
    task_number=3;
    cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
    localhost->event_before_input=&task_3_2;//同上
    localhost->process_event_before_input();
}
void task_3_2()
{
    if (localhost->root->locate_file("bin/wget.exe"))
    {
        localhost->event_before_input=&task_4_1;//同上
        localhost->process_event_before_input();
    }
}
void task_4_1()
{
#ifdef MAKE_SAVINGS  
    save_game_to_file("auto_created_saving_task4");
#endif
    task_number=4;
    cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
    localhost->event_before_input=task_4_2;//同上
    localhost->process_event_before_input();
}
void task_4_2()
{
    if (localhost->locate_file("/data/passguesser.exe"))//检测是否有文件
    {
        localhost->event_before_input=&task_5_1;
        localhost->process_event_before_input();
    }
}
void task_5_1()
{
#ifdef MAKE_SAVINGS  
    save_game_to_file("auto_created_saving_task5");
#endif
    task_number=5;
    cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
    localhost->event_before_input=task_5_2;//同上
    localhost->process_event_before_input();
}
void task_5_2()
{
    if (localhost->locate_file("/bin/passguesser.exe"))//检测是否有文件
    {
        localhost->event_before_input=&task_6_1;
        localhost->process_event_before_input();
    }
}
void task_6_1()
{
#ifdef MAKE_SAVINGS  
    save_game_to_file("auto_created_saving_task6");
#endif
    task_number=6;
    cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
    localhost->event_before_input=task_6_2;//同上
    localhost->process_event_before_input();
}
void task_6_2()
{
    //task_number=6;
    //cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
    //localhost->event_before_input=task_6_2;
    if (localhost->locate_file("/data/portscan.exe"))//检测文件
    {
        if (Internet->connect(COMPUTER3).computer->locate_file("/log/connect.log"))//是否删除了日志
        {
            game_over(1);//没删除文件则游戏结束
        }
        else
        {
            localhost->event_before_input=task_7_1;//下一关
            localhost->process_event_before_input();
        }
    }
}
void task_7_1()
{
#ifdef MAKE_SAVINGS  
    save_game_to_file("auto_created_saving_task7");
#endif
    task_number=7;
    cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
    localhost->event_before_input=task_7_2;
    localhost->process_event_before_input();
}
void task_7_2()
{
    Computer *temp=new Computer("180.21.10.220");
    Internet->add_node(temp->netnode);
    temp->style=telnet;
    temp->password="root";//设密码
    temp->open_port("telnet",256);//打开端口
    temp->locate_dir("/bin")->add_file(new FileSystem::file("tracer.exe",&exe_tracer));//放文件
    localhost->event_before_input=task_7_3;
    localhost->process_event_before_input();
}

void task_7_3()
{
    if (localhost->locate_file("/data/tracer.exe"))//检测文件
    {
        if (Internet->connect("180.21.10.220").computer->locate_file("/log/connect.log"))//是否删除了日志
        {
            game_over(1);//没删除文件则游戏结束
        }
        else
        {
            localhost->event_before_input=task_8_1;//下一关
            localhost->process_event_before_input();
        }
    }
}

void task_8_1()
{
#ifdef MAKE_SAVINGS  
    save_game_to_file("auto_created_saving_task8");
#endif
    task_number=8;
    cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
    Computer *temp;
    net_node *tempnode;
    net_node *tempnode2;

    tempnode2 = new net_node("24.120.41.2");
    Internet->add_node(tempnode2);
        //cout<<"debug6"<<endl;

    temp=new Computer("24.120.41.0");
    tempnode = temp->netnode;
    tempnode2->add_node(temp->netnode,true);//连接这个主机的节点
    temp->password="password0abc";//设密码
    temp->open_port("telnet",91);//打开端口
    temp->style = telnet;
    temp->event_before_input = &task_8_2;
       // cout<<"debug7"<<endl;
    
    temp=new Computer("24.120.41.10");
    tempnode->add_node(temp->netnode,true);//连接这个主机的节点
    temp->password="pass_lispt202";//设密码
    temp->open_port("telnet",23);//打开端口
    temp->locate_dir("data")->add_new_txt(
        "code.cpp",
        "int exe_ipconfig(int i,const char **t,Computer *c)\n{\n    cout<<\"本地网络配置器\"<<endl;\n    cout<<\"---------------------\"<<endl;\n    cout<<\"IPv4地址:\"<<c->netnode->ip<<endl;\n    cout<<\"默认网关:\"<<c->netnode->father->ip<<endl;\n    if (c->port_list.size()>0)"
    );
    temp->style = telnet;
       // cout<<"debug8"<<endl;

    temp=new Computer("24.120.41.20");
    tempnode->add_node(temp->netnode,true);//连接这个主机的节点
      //  cout<<"debug9"<<endl;

    //thread *looper = new thread(task_8_1_task);
    //looper->detach();

    connection_reply reply = Internet->connect(COMPUTER2);
    if (reply.computer == NULL)
    {
        cout<<"task_8_1():主机不存在"<<endl;
    }
    else
    {
        FileSystem::dir * http_dir = reply.computer->get_http_dir(21);
        if (http_dir == NULL)
        {
            cout<<"task_8_1():主机服务已关闭"<<endl;
        }
        else
        {
            http_dir->add_file(new FileSystem::file("sniffer.exe",&exe_sniffer));
        }
    }

    localhost->event_before_input = &task_8_2;
    localhost->process_event_before_input();
}

/*
void task_8_1_task()
{
    while (true)
    {
       // cout<<"debug1"<<endl;
        if (rand()%10 == 0)
        {
            connection_reply reply = Internet->connect("24.120.41.20");
            if (reply.computer)
            {
                net_node *to = reply.computer->netnode;
                to->send_package("24.120.41.10", "试图建立连接", "telnet");
                to->send_package("24.120.41.10", "校验握手", "telnet");
                to->send_package("24.120.41.10", "建立连接数据通道", "telnet");
                to->send_package("24.120.41.10", "连接成功", "TCP");
                this_thread::sleep_for(std::chrono::milliseconds(3000));
                to->send_package("24.120.41.10", "登陆:root 密码:pass_lispt202", "TCP");
            }
        }
      //  cout<<"debug2"<<endl;
        this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
*/

void task_8_2()
{
    if (rand()%5 == 0)//生成数据包
    {
        connection_reply reply = Internet->connect("24.120.41.20");
        if (reply.computer)
        {
            net_node *to = reply.computer->netnode;
            to->send_package("24.120.41.10", "试图建立连接", "telnet");
            to->send_package("24.120.41.10", "校验握手", "telnet");
            to->send_package("24.120.41.10", "建立连接数据通道", "telnet");
            to->send_package("24.120.41.10", "连接成功", "TCP");
            to->send_package("24.120.41.10", "登陆:root 密码:pass_lispt202", "TCP");
        }
    }
    if (localhost->locate_file("/data/code.cpp"))//检测文件
    {
        if (
                Internet->connect("24.120.41.0").computer->locate_file("/log/connect.log")  ||
                Internet->connect("24.120.41.0").computer->locate_file("/log/sniffer.log")  ||
                Internet->connect("24.120.41.10").computer->locate_file("/log/connect.log")
            )//是否删除了日志
        {
            game_over(1);//没删除文件则游戏结束
        }
        else
        {
            localhost->event_before_input=task_9_1;//下一关
            localhost->process_event_before_input();
        }
    }
}

void task_9_1()
{
#ifdef MAKE_SAVINGS  
    save_game_to_file("auto_created_saving_end");
#endif
    task_number = 9;
    connection_reply reply = Internet->connect(COMPUTER2);
    if (reply.computer == NULL)
    {
        cout<<"task_9_1():主机不存在"<<endl;
    }
    else
    {
        FileSystem::dir * http_dir = reply.computer->get_http_dir(21);
        if (http_dir == NULL)
        {
            cout<<"task_9_1():主机服务已关闭"<<endl;
        }
        else
        {
            http_dir->add_file(new FileSystem::file("mail2.exe",&exe_mail2));
        }
    }

    cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
    localhost->event_before_input=task_9_2;
    localhost->process_event_before_input();
}

void task_9_2(){
    if (task_added_function[task_number - 9]()){//调用对应的新的剧情函数
        //if条件成立说明函数返回true
        task_number++;//关卡号加一。这样下一次if判断条件时，会调用下一个剧情函数
        //check_and_autosave();//自动保存
    }
}

bool end_task(){//永远不会通过的任务
    return false;
}

//task10
//task_number = 9
bool task_new_1(){
    if (localhost->locate_file("/bin/mail2.exe"))//检测文件
    {
        send_mail("网络教程","nic",
            /*
            计算机网络可以分成两种：公网和子网

            比如说主机29.53.103.3就是直接连接到公网上的。我们可以用如下图示表示：
            ｜--------｜
            ｜Internet｜
            ｜--------｜
                 ｜
            29.53.103.3

            但有时主机没有直接连接到公网，比如说您的计算机就是连接到路由器：
            ｜--------｜
            ｜Internet｜
            ｜--------｜
                 ｜
            路由器（IP为188.131.133.248）
            由子网发来的数据包全部发向公网
            由公网发来的数据包全部发向192.168.0.0
                 ｜
               ------------- 子网
               ｜           ｜
            您的计算机       你们家的另一台计算机
            IP=192.168.0.0 IP=192.168.0.1
            */
            string("计算机网络可以分成两种：公网和子网\n\n")+
            "比如说主机29.53.103.3就是直接连接到公网上的。我们可以用如下图示表示：\n"+
            "｜--------｜\n"+
            "｜Internet｜\n"+
            "｜--------｜\n"+
            "     ｜\n"+
            "29.53.103.3\n\n"+
            "但有时主机没有直接连接到公网，比如说您的计算机就是连接到路由器：\n"+
            "｜--------｜\n"+
            "｜Internet｜\n"+
            "｜--------｜\n"+
            "    ｜\n"+
            "路由器（IP为188.131.133.248）\n"+
            "由子网发来的数据包全部发向公网\n"+
            "由公网发来的数据包全部发向192.168.0.0\n"+
            "     ｜\n"+
            "   ------------- 子网\n"+
            "   ｜           ｜\n"+
            "您的计算机       你们家的另一台计算机\n"+
            "IP=192.168.0.0 IP=192.168.0.1\n"
        );
        return true;
    }
    return false;//循环
}

bool task_new_2(){
    send_mail("删除博客","Fightingme",
        string("帮帮我吧！有个该死的**发了个博客骂我是**，您能不能帮我删了啊！！！\n")+
        "那个博客网站的ip是52.79.3.105，那个用户叫jiaoyy"
    );

    //临时指针
    Computer *temp;
    net_node *tempnode;
    FileSystem::dir *tempdir;

    //新建一个节点
    tempnode=new net_node("52.79.3.104");
    Internet->add_node(tempnode);
    //新建一个主机
    temp=new Computer("52.79.3.105");
    tempnode->add_node(temp->netnode,true);//连接这个主机的节点
    temp->style=telnet;
    temp->password="123456";//设密码
    temp->open_port("telnet",23);//打开端口
    temp->username="mainserver";
    temp->locate_dir("/data")->add_new_dir("web");
    tempdir = temp->locate_dir("/data/web");
    tempdir->add_new_txt("网络说明.txt","数据库服务器(用户root，密码no_one_know_this)：\n192.168.0.1\n192.168.0.2");
    tempdir->add_new_dir("doc");
    tempdir = temp->locate_dir("/data/web/doc");
    tempdir->add_new_txt("index.php","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("login.php","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("blog.php","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("user_profile.php","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    

    temp=new Computer("192.168.0.1");
    tempnode->add_node(temp->netnode,false);//连接这个主机的节点
    temp->style=telnet;
    temp->password="no_one_know_this";//设密码
    temp->open_port("telnet",23);//打开端口
    temp->name="db1";
    tempdir=temp->locate_dir("/data");
    tempdir->add_new_txt("user_profile_1.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("user_profile_2.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("user_profile_3.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("user_profile_4.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("user_profile_为节约空间省略1867个文件.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("user_profile_1872.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");


    temp=new Computer("192.168.0.2");
    tempnode->add_node(temp->netnode,false);//连接这个主机的节点
    temp->style=telnet;
    temp->password="no_one_know_this";//设密码
    temp->open_port("telnet",23);//打开端口
    temp->name="db2";
    temp->locate_dir("/data")->add_new_dir("blogs");
    tempdir = temp->locate_dir("/data/blogs");
    tempdir->add_new_txt("jiaoyy_blog1.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("jiaoyy_blog2.txt","Fightingme这个**，我******（不文明的语言此处遮挡）");
    tempdir->add_new_txt("jiaoyy_blog3.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("sfehuhu_blog1.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");
    tempdir->add_new_txt("为节约空间省略92784个文件.txt","这个文件与游戏无关\n[TODO]谁愿意为GHG作贡献的可以帮助把这里补上内容:)，我是懒得写了");

    return true;
}

bool task_new_3(){
    Computer * temp = Internet->connect("52.79.3.104").node->connect("192.168.0.2").computer;
    if(!(temp->locate_file("/data/blogs/jiaoyy_blog2.txt"))){
        if(temp->locate_file("/log/connect.log")||
           Internet->connect("52.79.3.104").node->connect("52.79.3.105").computer->locate_file("/log/connect.log")||
           Internet->connect("52.79.3.104").node->connect("192.168.0.1").computer->locate_file("/log/connect.log")){
            game_over(1);
            return false;
        }else{
            send_mail("[通关]删除博客","Fightingme","谢谢！");
            return true;//下一个剧情函数
        }
    }else{
        return false;
    }
}

bool task_new_4(){
    return false;
}

bool task_new_5(){
    return false;
}

bool task_new_6(){
    return false;
}

bool task_new_7(){
    return false;
}

bool task_new_8(){
    return false;
}

bool task_new_9(){
    return false;//永远不会通关
}

void jump_through_task(int index){
    if(index>=9){
        switch(index){
        case 9:
            localhost->locate_dir("/bin")->add_file(new FileSystem::file("mail2.exe",&exe_mail2));
            task_number = 9;
            localhost->event_before_input=task_9_2;
            //localhost->process_event_before_input();
            break;
        default:
            cout<<"ERROR:jump_through_task(int index):关卡不存在"<<endl;
            break;
        }
    }else{
        cout<<"ERROR:jump_through_task(int index):index<9"<<endl;
    }
}
