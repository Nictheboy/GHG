/*
 *文件名:   tasks.cpp
 *作者:     Nictheboy
 *内容:     实现游戏中的关卡
 *
 */
#include "hackgame.h"

int init_num = 0;

//一下是新的剧情函数列表
void task_new_1();
void task_new_2();
void task_new_3();
void task_new_4();
void task_new_5();
void task_new_6();
void task_new_7();
void task_new_8();
void task_new_9();
void end_task();
typedef void (*task_function)();
// 用于剧情系统更新
bool next_task(task_function task, int task_num) {
  bool check = false;
  if (task == NULL) {
    ChangeColor(RED);
    cout << "内部错误: 严重: 下一关的关卡函数数据不存在\n";
    ChangeColor(RESET);
    return check;
  }
  if (task_num > task_number) {
    /*关卡数不符*/
    return check;
  }
  check = true;
  (*task)();
  return check;
}
//task_function task_added_function[10]={//新的剧情函数数组
//    task_new_1,
//    task_new_2,
//    task_new_3,
//    task_new_4,
//    task_new_5,
//    task_new_6,
//    task_new_7,
//    task_new_8,
//    task_new_9,
//    end_task
//};

//下面是一下剧情函数

//用来结束游戏
void game_over(int i)//i=1:未删除日志
{
    switch(i)
    {
    case 1:
        ChangeColor(BG_RED);
        cout<<"日志未删除，游戏结束."<<endl;
        cout<<"Game over."<<endl;
        ChangeColor(RESET);
        break;
    default:
        cout<<"Case Undifined!"<<endl;
    }
    delay(9999999);
    exit(0);
}

string get_name_data() {
  cout << "设置用户名:";
  ChangeColor(GREEN);
  string tmp;
  getline(cin, tmp);
  ChangeColor(RESET);
  localhost->username = tmp;
  return tmp;
}

string get_host_name() {
  cout << "设置主机名:";
  ChangeColor(GREEN);
  string tmp;
  getline(cin, tmp);
  ChangeColor(RESET);
  localhost->name = tmp;
  return tmp;
}

string get_pass_data() {
  cout << "设置主机密码(将不会显示到终端上):";
  ChangeColor(BLACK);
  string tmp;
  getline(cin, tmp);
  ChangeColor(RESET);
  localhost->password = tmp;
  return tmp;
}

void init_task() {
  while (true) {
    clearScreen();
    string tmp1 = get_name_data();
    if (tmp1 == "") {
      localhost->username = "root";
    }
    string tmp2 = get_host_name();
    if (tmp2 == "") {
      localhost->name = "LocalHost";
    }
    string tmp3 = get_pass_data();
    if (tmp3 == "") {
      ChangeColor(YELLOW);
      cout << "警告: 密码不可为空，请重新设置!";
      ChangeColor(RESET);
      delay(1.6);
      clearScreen();
      cout << "设置用户名:";
      ChangeColor(GREEN);
      cout << tmp1 << endl;
      ChangeColor(RESET);
      cout << "设置主机名:";
      ChangeColor(GREEN);
      cout << tmp2 << endl;
      ChangeColor(RESET);
      get_pass_data();
    } else {
      init_num+=1;
      return;
    }
  }
}

//这一堆函数控制剧情逻辑
void task_1_1()//这个任务还有让用户设置用户名密码的功能
{
    if (init_num == 0) {
      init_task();
      clearScreen();
    } 
      
      //delay(2);
      ChangeColor(BG_RED);
      cout << "您收到一封邮件！输入mail查看\n\n";
      ChangeColor(RESET);

      task_number += 1;//设置关卡变量是因为mail程序根据关卡变量决定输出的邮件

      localhost->event_before_input = NULL;
      Internet->connect(COMPUTER1).computer->event_before_input = &task_1_2;//将任务绑定到主机2，用于检测玩家是否连接成功
}
void task_1_2()//玩家连接到主机2后，执行此程序
{
    //sub_task_number=1;
    Internet->connect(COMPUTER1).computer->event_before_input=NULL;
    task_number+=1;//进入下一关
    next_task(&task_2_1, 2);
    //localhost->process_event_before_input();//先处理一下
}
void task_2_1()
{
    ChangeColor(BG_RED);
    cout<<"您收到一封邮件！输入mail查看\n\n"<<endl;
    ChangeColor(RESET);
    task_number += 1;
    next_task(&task_2_2, 2);
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
    task_number=3;
    ChangeColor(BG_RED);
    cout<<"您收到一封邮件！输入mail查看\n\n"<<endl;
    ChangeColor(RESET);
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
    task_number=4;
    ChangeColor(BG_RED);
    cout<<"您收到一封邮件！输入mail查看\n\n"<<endl;
    ChangeColor(RESET);
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
    task_number=5;
    ChangeColor(BG_RED);
    cout<<"您收到一封邮件！输入mail查看\n\n"<<endl;
    ChangeColor(RESET);
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
    task_number=6;
    ChangeColor(BG_RED);
    cout<<"您收到一封邮件！输入mail查看\n\n"<<endl;
    ChangeColor(RESET);
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
    task_number=7;
    ChangeColor(BG_RED);
    cout<<"您收到一封邮件！输入mail查看\n\n"<<endl;
    ChangeColor(RESET);
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
    task_number=8;
    ChangeColor(BG_RED);
    cout<<"您收到一封邮件！输入mail查看\n\n"<<endl;
    ChangeColor(RESET);
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
        FileSystem::dir * www_dir = reply.computer->get_www_dir(21);
        if (www_dir == NULL)
        {
            cout<<"task_8_1():主机服务已关闭"<<endl;
        }
        else
        {
            www_dir->add_file(new FileSystem::file("sniffer.exe",&exe_sniffer));
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
  task_number = 9;
    //cout<<BG_RED<<"您收到一封邮件！输入mail查看"<<RESET<<endl;
  next_task(&task_9_2, 9);
}

void task_9_2(){
    //if (task_added_function[task_number - 9]()){//调用对应的新的剧情函数
        //if条件成立说明函数返回true
        task_number++;
        check_and_autosave();//自动保存
        next_task(&task_new_1, 10);
    /*}*/
}

void end_task(){//永远不会通过的任务
  
}

void task_new_1(){
    //29.102.4.31    
    ChangeColor(BG_RED);
    cout<<"您收到一封邮件！输入mail查看\n\n"<<endl;
    ChangeColor(RESET);
    Computer *temp=new Computer("29.102.4.31");
    Internet->add_node(temp->netnode);
    temp->style=telnet;
    temp->password="root";//设密码
    temp->open_port("telnet",81);//打开端口
    temp->locate_dir("/log")->add_file(new FileSystem::file("connect.log","nwfofeweofowfiowjofwojfwojfowjojwoejonvru"));//放文件
    temp->locate_dir("/log")->add_file(new FileSystem::file("firewall.log","nwfofeweofowfiowjofwojfwojfowjojwoejonvru"));//放文件
    temp->locate_dir("/log")->add_file(new FileSystem::file("telnet.log","nwfofeweofowfiowjofwojfwojfowjojwoejonvru"));//放文件
    temp->locate_dir("/log")->add_file(new FileSystem::file("about.log","nwfofeweofowfiowjofwojfwojfowjojwoejonvru"));//放文件
    next_task(&task_new_2, 10);
}

void task_new_2(){
    connection_reply reply = Internet->connect("29.102.4.31");
    if (reply.computer){
        Computer * c = reply.computer;
        if (!(c->locate_file("/log/connect.log") || 
            c->locate_file("/log/firewall.log") || 
            c->locate_file("/log/telnet.log") || 
            c->locate_file("/log/about.log"))){
            task_number+=1;
            next_task(&end_task, 11);
        }
    }
}

//void task_new_3(){
//    ChangeColor(BG_RED);
//    cout<<"您收到一封邮件！输入mail查看\n\n"<<endl;
//    ChangeColor(RESET);
//    Computer* temp = new Computer("187.56.222.10");
//    Internet->add_node(temp->netnode);
//    temp->style = telnet;
//    temp->password = "new_author_ink";
//    temp->open_port("telnet", 23);
//    // 放文件
//    temp->locate_dir("/bin")->add_file(new FileSystem::file("FTPoverflow.exe", &exe_FTPoverflow));
//    next_task(&task_new_4, 11);
//}
//
//void task_new_4(){
//   connection_reply reply = Internet->connect("29.102.4.31");
//   if (reply.computer) {
//     Computer* c = reply.computer;
//     if (c->locate_file("/log/connect.log")) {
//       game_over(1);
//     }
//     else {
//       task_number++;
//       next_task(&end_task, 12);
//     }
//   }
//}

void task_new_5(){
    return;
}

void task_new_6(){
    return;
}

void task_new_7(){
    return;
}

void task_new_8(){
    return;
}

void task_new_9(){
    return;
}

