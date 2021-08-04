#ifndef FOR_XES
#include "hackgame.h"
#endif
//所有全局变量和对象
int task_number=0;//关卡进度，一个全局变量
int sub_task_number=0;//子进度，另一个全局变量
net_node *Internet = NULL;
Computer *localhost;//一个指针，指向本地主机。用于方便访问

char base64_table_a[64];//从64个数映射到字符
char base64_table_b[255];//从字符映射到64个数。0表示无效

//两种样式
Style basic_style(  "请输入用户名:",
                    "请输入密码:",
                    "",
                    "密码错误，请重试!\n",
                    "登陆失败\n",
                    "[",
                    "@",
                    "]",
                    "#");//请根据这个东西和程序运行效果自行理解每个东西的意思
                    
Style telnet(   "login:",
                "Password:",
                "登录成功！\n",
                "",
                "",
                "",
                "@",
                ">",
                "#");

Style style_1( 	    "请输入用户名:",
                    "请输入密码:",
                    "",
                    "密码错误，请重试!\n",
                    "登陆失败\n",
                    string(RED)+"[",
                    string("@")+GREEN,
                    string(RED)+"]"+RESET,
                    "#");

string default_filename = "autobackup.tree";//保存时的默认文件名
bool autosave = true;//是否自动保存

list<pair<void*,string> > preload_ptr_table;
list<string> mail_list;
