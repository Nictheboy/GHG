#ifndef hackgame
#define hackgame
/*
-----#####------#--------#--------#####------
----#-----#-----#--------#-------#-----------
---#------------#--------#------#------------
---#------------##########------#------------
---#--######----#--------#------#--######----
---#-----#------#--------#------#-----#------
----#----#------#--------#-------#----#------
-----#####------#--------#--------#####------
*/

//这是黑客游戏GHG
//GHG是GHG：HackGame的缩写
//递归缩写（不懂啥事递归缩写的请百度）

/*
更新日志：
V0.2.2  解决了文本文件结尾没有/0的bug
V0.2.3  整理了代码
V0.2.4  增加了对上级目录的支持
V0.3.0  加入了对树状网络的支持，对此有疑问可以私聊我的QQ：2492515121
V0.3.1  更新了文件系统，取消了dir_tree和currentd_ir，使用root和dir_now
V0.3.2  加入了指令rm，cp，ls和cat，作为指令del，copy，dir和type的别名
V0.4.0  加入了savegame指令保存游戏，以及加载游戏的功能
V1.0.0  加入剧情，整理代码
V1.0.4  解决了若干Bug
V1.1.0  加入了自动保存
V1.2.0	解决了若干bug,并修改了剧情系统
V1.2.1	将预加载的指针放到了一个list里，便于代码修改。无新增特性
V1.3.0	修改了启动界面
V1.4.0	增加了跳关功能（通过内置了若干存档实现）
*/

//有很多人反应，黑客游戏的代码可读性太差。
//于是我制作了这个有注释的版本
//这个版本中，我把所有定义摆到了前头
//而且加上了注释

//另外，为了方便各位开发
//该版本引入了事件的概念


//常数表和编译选项
//这是一堆常量
//#define COMPUTER_NUMBER 10//一共有10台电脑
#define LOCAL_HOST "188.131.133.248"//本地主机的IP为这个
#define COMPUTER1 "29.53.103.3"//几台电脑的IP
#define COMPUTER2 "178.53.100.24"
#define COMPUTER3 "117.51.143.120"
#define PROGRAM_INFO "黑客游戏 Release V1.4.0"//游戏的版本信息
#define SAVING_VERSION 4//存档版本
#define DEBUG//调试标志，打开这个后会执行一些代码，直接测试最后一关
//#define MAKE_SAVINGS//每进入一关就自动保存一个存档，调试用

//#define TEST_WINDOWS//用于在Linux上测试Windows模式
#define NO_DELAY//无延时，测试用
#define FOR_XES//用于生成单文件代码，即文件code.cpp

#if defined(_WIN32) || defined(_WIN64)//判断是否是windows
#define WINDOWS_OS//Windows模式
#else
#define LINUX_OS//Linux模式，适合在linux上编译
#endif

#ifdef DEBUG//是否是调试模式
#define DEBUG_FLAG true//代码中有的地方会写if(DEBUG_FLAG){..}之类的代码
//这样的话这些代码就只有在定义了DEBUG的时候才执行
#define ON_DEBUG(A) A//这个虽然定义了实际上没使用
#else
#define DEBUG_FLAG false
#define ON_DEBUG(A)
#endif

//头文件
//这个不用解释吧！（还是解释一下吧）
#include <iostream>//不用解释
#include <string.h>//不用解释
#include <list>//同上
#include <vector>//同上
#include <stdio.h>//同上
#include <stdlib.h>//同上
#include <time.h>//用这个头文件是因为，生成日志的时候会写上时间
#include <sstream>//这是为了处理字符串所以用的
#include <fstream>//存档系统要读写文件
#include <map>//不解释
#include <stack>//不解释
#include <algorithm>//不解释
//#include <thread>
//以前最后一关有一个额外的线程来生成日志
//后来发现学而思的在线执行似乎不支持多线程
//于是就改掉了
using namespace std;

//颜色表
//这个是Linux用的，不懂就别问了，反正你也不打算开发linux版本
#ifdef LINUX_OS
#define RESET "\033[0m"
#define BLACK "\033[30m" /* Black */
#define RED "\033[31m" /* Red */
#define GREEN "\033[32m" /* Green */
#define YELLOW "\033[33m" /* Yellow */
#define BLUE "\033[34m" /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m" /* Cyan */
#define WHITE "\033[37m" /* White */
#define BG_BLACK "\033[40m" /* Black */
#define BG_RED "\033[41m" /* Red */
#define BG_GREEN "\033[42m" /* Green */
#define BG_YELLOW "\033[43m" /* Yellow */
#define BG_BLUE "\033[44m" /* Blue */
#define BG_MAGENTA "\033[45m" /* Magenta */
#define BG_CYAN "\033[46m" /* Cyan */
#define BG_WHITE "\033[47m" /* White*/
#elif defined(WIN32) || defined(WIN64) // changed: fixed the color bug.
//解释一下，上面那条英文注释是维护者Ink写的。下面的代码也是他写的
//他是中国人
//写成英文恐怕是因为代码是发布在github上的吧
//感谢Ink写的代码
#include <windows.h>//不解释
struct Color {//一个结构体，用于描述一个颜色
  Color(WORD value) : value(value) {}
  short value;
};
#define SET(X) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)X);
//定一个一个宏函数，用于设置cmd的颜色
void operator <<(const ostream& stm, const Color& color) {//运算符重载，目的是以后可以用cout<<RED这一类本属于linux的写法
  SET(color.value);
  return;
}
#define RESET Color((WORD)0x0007)
#define BLACK Color((WORD)0x0000) /* Black */
#define RED Color((WORD)0x0004) /* Red */
#define GREEN Color((WORD)0x0002) /* Green */
#define YELLOW Color((WORD)0x0006) /* Yellow */
#define BLUE Color((WORD)0x0001) /* Blue */
#define MAGENTA Color((WORD)0x0005) /* Magenta */
#define CYAN Color((WORD)0x0003) /* Cyan */
#define WHITE Color((WORD)0x0007) /* White */
#define BG_BLACK Color((WORD)0x0007) /* Black */
#define BG_RED Color((WORD)0x0040) /* Red */
#define BG_GREEN Color((WORD)0x0020) /* Green */
#define BG_YELLOW Color((WORD)0x0060) /* Yellow */
#define BG_BLUE Color((WORD)0x0010) /* Blue */
#define BG_MAGENTA Color((WORD)0x0050) /* Magenta */
#define BG_CYAN Color((WORD)0x0030) /* Cyan */
#define BG_WHITE Color((WORD)0x0070) /* White*/
#endif



void delay(double time);//这个函数用于延时。它的实现在linux和windows中不一样
//time的单位是秒
void clearScreen();//用于清屏，linux中会执行clear，windows中会执行cls
void localtime_s_f(struct tm *ts,time_t *timep);//这是一个关于时间的函数
//从库里来的，但是因为Linux中和Windows中定义不一样，所以我自己定义了一个通用的。
//它的实现在os_function.cpp中
#ifdef LINUX_OS
#define sprintf_s snprintf//这是因为同一个函数在linux和windows中名字不一样
#endif

class Computer;//先定义一下这个类。这是一台电脑的类。这个类在GHG中是极端重要的，不过它的定义在后面

//这是一个namespace（不要问我啥是namespace）
//用于模拟文件系统
//加一句，这是一个树状结构
//没事，看了下面的你就明白了
namespace FileSystem
{
    typedef int (*exe_adr)(int,const char**,Computer*);//这是一个类型，是指向一个函数的指针，用于描述一个exe文件程序的函数
    //第一二个参数的含义和C++的main函数的参数意思一样
    //第三个参数表示这个exe是哪个虚拟电脑打开的
    //第三个参数你之后就明白了
    //你看我说吧，Computer这个类很重要
    
    class file//这是一个类，用来模拟一个文件
    {
    public:
        string name;//文件名
        size_t size;//文件大小（真实的内存里的大小）
        void *data;//执行文件数据的指针，请不要把它设置成NULL
        file(string n, size_t s, void *d);//按名称，大小，数据生成文件
        file(string n, string s);//生成一个包含一串字符的文件
        file(string n, exe_adr adr);//生成一个指向某函数的exe文件
        ~file();//析构函数，会free掉data
        bool run_exe(int i,const char **strlist,Computer* c);//执行这个文件，如果这是一个exe格式的文件。执行成功与否与文件名是不是.exe结尾 无 关 ！
    }; 
    
    class dir//模拟一个文件夹。这个游戏里，dir都表示文件夹
    {
    public:
        dir *father;//上一级文件夹
        string name;//文件夹名
        //int file_number;//文件的数量
        //int subdir_number;//子目录的数量
        vector<file *> content;//包含的文件
        vector<dir *>subdir;//包含的子目录
        //所以你看，dir类其实是一个树节点
        dir(string n);//根据目录名生成一个目录，空的（废话）
        ~dir();//析构函数，会递归地析构所有子文件夹和文件
        bool add_file(file *f);//添加一个文件。注意，只是把指针放进来，并不复制文件！（所以我不推荐调用这个函数）
        bool add_dir(dir *d);//添加一个目录。不复制。不推荐
        bool delete_file(string filename);//删除一个文件。注意！只删除指针，文件还存在！
        bool delete_dir(string dirname);//删除一个文件夹。注意！只删除指针！
        int search_file(string filename)const;//按文件名搜索文件，返回文件在content中的索引
        int search_subdir(string dirname)const;//按目录名搜索文件夹。返回文件夹在subdir中的索引
        file* locate_file(string s)const;//定位一个文件。字符串可以是文件名如abc.txt，也可以是目录加文件名如abc/abc.txt
        dir* locate_dir(string s)const;//定位一个文件夹。字符串可以是文件夹名如abc，也可以是目录加文件夹名如abc/abc。注意，结尾没有/
        bool add_new_txt(string name, string content);//加入一个新的txt。先new一个txt在加进来。
        bool add_new_dir(string name);//加入一个新的。先new一个txt在加进来。
        dir* turn_file_ps_into_dir_ps(string fp);//这个就是传入一个文件位置，如abc/def.txt，返回一个目录
        void show(int format=0);//打印这个文件夹的结构。这就是tree.exe调用的那个函数
    };
}

class ipport_cutter//这是一个实用的类，把像123.123.123.123:55这样的东西分成123.123.123.123和55
//当然我知道，有些人会很讨厌我把这样一个平淡无奇的功能封装成一个类，还弄一个单独的文件
//不过这可以让文件数量显得很多
{
public:
    string ip;
    int port;
    ipport_cutter(string ip_port,int port_for_no_port);
    //这个类可能会抛出一个字符串异常，如果输入格式有问题的话
};

class Style//这个类描述一台虚拟电脑用户界面的样子
{
public:
    string login_output;//很好理解
    string pass_output;//亦不难理解
    string login_successful;//同上
    string login_wrong;//同上
    string login_fail;//同上
    string before_ip;
    string between_ip;
    string after_ip;
    string getinput;
    //如果有人执意要弄明白上面四个是干嘛的，请直接去看computer::echo_before_command()
    Style();//构造一个默认的style
    Style(string s1,string s2,string s3,string s4,string s5,
        string s6,string s7,string s8,string s9);//构造函数的各个系数按顺序复制给各个变量
};



//这个类很简单
//描述一个端口的名字和端口号
//因为这个太简单我不解释了
//加一句，忘了我把它的代码放到哪里了
class port_info
{
public:
    string name;
    int port;
    port_info(){}
    port_info(string n, int p)
    {
        name=n;
        port=p;
    }
};




namespace SuperShell//这是一个namespace，用来处理用户的输入
//这玩意的代码是从一个我以前的作品来的
//加一句，这个模块的代码从ghg的第一个版本开始就有
//不过后来修改过
//不过因为太低层，你估计用不着
//（当你看到我说“你用不着”的时候，是这样的情况，我因为自己从来不用它所以已经忘了咋回事了。所以说白了就是我也不知道:>）
{
    typedef int (Computer::*program_adr)(int,const char**);//这是一个数据类型，是一个指向Computer类的成员函数的指针
    //（看不到别看了，内核的东西，你也用不着）
    
    typedef const char *inner_text;//我也忘了干啥子来的了
    
    class program//这个你别管了，你真用不着
    {
    public:
        string name;
        program_adr address;
        int call(int cmd_number,const char** cmd_list,Computer *C);
    };

    class Performer//估计你也用不着
    {
    public:
        string command;
        inner_text *listed_cmd_c_str;
        list<program> program_list;
        int number_of_cmd;
        Computer *belong_to;
        void cut();
        void turn_to_c();
        list<string> listed_cmd;
        Performer();
        void set_belonger(Computer *C);
        void import_command(string cmd);
        string get_command();
        bool process();
        void add(string name, program_adr adr);
    };
}

//下面的这些网络更新的内容不想看可以跳过去
//非要看的话最好对着network.cpp的代码看

class connection_request//网络更新后新加入的，表示一个请求
{
public:
    string from;//请求来自哪个主机
    string to;//请求寻找哪个主机
    string type;//请求的类型。暂时没有任何用
    connection_request(string from,string to,string type);//构造函数，方便构造
};

class net_node;

class connection_reply//网络更新后新加入的，表示一个回复
{
public:
    Computer *computer;//找到的网络节点对应主机的指针
    net_node *node;//找到的网络节点的指针
    string status;//状态，success表示成功
    connection_reply(Computer *c,net_node *n,string status);//构造函数
};

class data_package//其实没啥用
{
public:
    string from;
    string to;
    string data;
    string type;
    int status;//状态码，0为正在发送，1为正在传递，2为送达
    data_package(string from_i, string to_i, string data_i, string type_i, int status_i=0);
};

class net_node//网络更新后新加入的，表示一个网络节点
{
public:
    Computer *binded_computer;//对应的主机
    net_node *father;//上级节点。跟节点这里为NULL
    map<string,net_node*> net_map;//网络映射
    vector<net_node*> subnet;//直接连接的节点
    string ip;//该节点的IP
    void (*on_package_arrive)(data_package package, net_node *caller);//接收到数据包时触发
    void (*on_package_pass)(data_package package, net_node *caller);//数据包经过时触发，包括发送数据包时

    net_node(string ip);//构造函数
    connection_reply connect(string ip,string type="TCP");//发送一个请求，返回一个reply。type暂时没用
    connection_reply connect(connection_request,bool up_failed=false);//另一种发送请求的方式
    bool add_node(net_node *node,bool add_up=true);//添加一个节点，如果addup是true，会自动在所有上级节点上添加映射
    bool add_map(string ip,net_node *node);//添加一个映射
    bool bind_computer(Computer* computer);//绑定到一个主机
    void show(int format=0);//递归地显示
    void send_package(data_package package, bool up_failed=false);
    void send_package(string to, string data, string type);
};

//网络更新代码结束

typedef void (*event)();//定义了一个数据类型
//这是一个叫event的数据类型
//是一个指向一个像void abc（）这样的函数的指针
//说白了就是函数指针

class Computer{//这个很关键！很重要！很重要！重要的事情说三遍
    friend int exe_savegame(int i,const char **t, Computer *c);
    //忘了为啥要定义友元了（又来了）
private:
    bool login();//登陆的效果
    bool dc_signal;//断开连接的标志。用于实现dc
    //你把它设为true之后，run（）执行的时候检测到就会return
public:
    event event_before_input;//在显示东西让用户输入之前，先执行这个指针指向的函数
    //这玩意唯一的用处就是让剧情系统可以被调用
    //不过就是这个功能就让它成为tasks.cpp中最常见的东西之一
    //想详细了解，建议看computer.cpp中的run（）函数
    bool process_event_before_input();//调用刚刚那个事件
    
    string username;//用户名
    string password;//密码
    //string ip_address;//ip地址
    string name;//电脑的名字。如果这是空字符串，界面会显示ip
    //如果不是空的，显示这个名字
    
    string from;//谁连接了这台电脑。一般没用
    SuperShell::Performer pfm;//处理输入命令的东西。可以不管
    Style style;//界面的样式
    vector<port_info> port_list;//各种开放的端口，这个很实用
    net_node *netnode;
    
    net_node* net_conversation_caller;
    
    //这四个是V0.2.1文件系统更新后代替原有dir_tree和current_dir的文件系统
    FileSystem::dir *dir_now;//当前目录
    FileSystem::dir *root;//根目录
    FileSystem::dir* locate_dir(string path);//定位文件夹。/打头则从根目录开始，支持上级目录
    FileSystem::file* locate_file(string path);//定位文件。/打头则从根目录开始，支持上级目录
    bool delete_file(string path);//删除文件。/打头则从根目录开始，支持上级目录
    bool delete_dir(string path);//删除文件夹。/打头则从根目录开始，支持上级目录
    
    Computer(string ip);//用于建立新游戏。进行一系列初始化
    Computer();//用于从文件加载，只进行很少的初始化
    
    void run(bool need_login, string f, net_node *caller = NULL);
    //启动这台电脑
    //第一个参数是是否需要登陆
    //如果是false就不需要登陆
    //f表示是谁在连接。一般是连接者的ip
    //caller是发起会话者的网络节点

    void send_here(string data, string type="TCP");//通过网络从localhost发一个数据包到这个电脑
    
    void write_log(string name, string content);
    //写一条日志。日志会写到log文件夹里
    //name是日志名字。如果name是abc，那么日志文件名就是abc.log
    //文件在游戏里的/log文件夹    

    void write_exe(string name, FileSystem::exe_adr);
    //新建一个exe
    //exe会保存到bin文件夹

    bool process_command(string cmd);
    //处理一行目录
    //成功返回true，失败返回false

    void echo_before_command();
    //显示提示符
    
    int get_port(string name);//返回某个端口的端口号。如果没有这个端口，就返回-1
    void open_port(string name, int port);//开放一个端口
    FileSystem::dir* get_http_dir(int port);//返回http文件夹。没有就返回NULL
    
    //一堆内置的命令
    int dc(int i,const char **t);
    //int telnet(int i,const char **t);
    int dir(int i,const char **t);
    int cd(int i,const char **t);
    int pwd(int i,const char **t);
    //int mail(int i,const char **t);
    int type(int i,const char **t);
    int copy(int i,const char **t);
    int mkdir(int i,const char **t);
    int del(int i,const char **t);
    int scp(int i,const char **t);
    int install(int i,const char **t);
};

class ptr2id//存档系统利用的
//加一句，ptr2id念做ptr to id，那个2是to的谐音。这是计算机里常见的简写
{
private:
    map<void*,int> table;//一个表
    int now;
public:
    ptr2id();
    string getid(void* ptr);
    //它的功能是这样的
    //你传入一个新的ptr，它就会给这个指针一个id，存到table里面，把id转换成文本返回。存档文件里那些id0000008之类的就是它生成的
    //传入的指针如果已经有id了就返回那个id
};

class id2ptr//这个也是存档里用的。2的含义同上
//在读取存档时，各个对象里面的指针变量要一个一个填回去
//这个类就是干这个的
{
private:
    map<string,list<void**> > not_ready_vars;
    map<string,list<pair<vector<void*>*,int> > > not_ready_vectors;
    map<string,list<pair<map<string,void*>*,string> > > not_ready_maps;
    map<string,void*> ready;
public:
    void bind(string id, void** ptr_pos);
    //把一个id，与一个应该放这个指针的变量，关联。
    //如果这个id所指的指针已经通过load（id，ptr）加载了，那么会直接填上
    //如果不是，则会被记录下了，以后当这个id被load（id，ptr）之后会填进去
    void bind(string id, vector<void*> *vec, int pos);//同上，vec是要填指针的vector，pos是要填指针的变量在vector中的位置
    void bind(string id, map<string,void*> *mp, string key);//同上
    void load(string id, void* ptr);//加载一个id对应的指针
    void show();//显示还有哪些id没有被加载。调试用
};
//假如你修改完代码后发现有bug导致崩溃
//一个很大的可能就是你在游戏里放了新的指针
//但是指针没有被加载
//为了避免这种情况，请打开preload_ptr_table.cpp，加入一个新的指针
//举个例子，add_preload_ptr((void*)&exe_passguesser,"exe_passguesser")


class file_writer//存档时用于生成符合存档格式的文件
{
private:
    int format;//新的一行前面有几个tab键
    string fname;//文件名
    string buff;//缓冲。在调用save（）前，已经“写入”的部分会存在这里
    void write_string(string str);//忘了干嘛用的。非要知道看代码去
public:
    file_writer();
    bool open(string filename);//“打开文件”，其实就是传入文件名
    string write(string str1,string str2="");//写一行
    //你可以打开一个ghg存档看看。str1是每行=前的部分，str2是=后的部分
    void save();//写入存档
    void left();//下一行少一个tab
    void right();//下一行多一个tab
};

//一个命名空间，用于存储树状数据
//存档文件会通过loader.cpp中的load_tree_from_string(string game)，转化成一个DataTree。这是它的用处
namespace DataTree
{
    class data_node;//节点
    data_node* new_root();//创建一个跟节点。默认是不能用构造函数创建节点的
    //为了安全

    class data_node//树节点
    {
        friend data_node* new_root();//便于初建节点
    private:
        data_node(string key,string value="");//构造函数。默认不允许访问
        int search_subdata_node(string data_nodename)const;//搜索节点
    public:
        string key;//键。就是存档里等号前的部分
        string value;//值。就是存档里等号后的部分
        data_node *father;//父节点
        vector<data_node *>subdata_node;//子节点列表
        
        void remove_all();//递归地删除所有子节点
        data_node* add(string key,string value="");//新建节点
        bool del(string key);//删除节点。不free
        data_node* locate(string s)const;//定位节点。支持..和.格式如node1/node2/node3，这个代码是从filesystem那里借用过来的
        void show(int format=0);//递归地显示这个树。调试用
        string get_value_of(string key);//获取某个key对应的value
    };
}

extern int task_number;//关卡进度，一个全局变量。如果是1就说明是第一关，以此类推
//但是修改这个变量不能跳关
extern int sub_task_number;//子进度，另一个全局变量。这个变量没有固定的用处，但是会被保存，因此很实用。但是这个变量总是tasks.cpp中的代码在使用
extern net_node *Internet;//网络根节点。所有公网可访问的主机都挂在这里
extern Computer *localhost;//一个指针，指向本地主机。用于方便访问
//两种样式
extern Style basic_style;//这是本机上的style。存于global_objects.cpp
extern Style telnet;//telnet时的style。存于global_objects.cpp
extern Style style_1;

extern string default_filename;//保存时的默认文件名。也可以用savegame filename指令保存到别的文件
extern bool autosave;//是否自动保存

extern list<pair<void*,string> > preload_ptr_table;//这个里面放着所有存档时的指针及其名字。由preload_ptr_table.cpp初始化
//extern list<string> mail_list;

vector<string> split(const string &s, const string &seperator,bool if_count_first_blank=false);//分隔字符串，懒得解释了
//算了解释一下吧
//split("abc def ghi jkl"," ")会得到含有四个字符串的vector
//最后那个参数忘了干嘛的了

void init_new_game();//初始化新游戏。本函数被main（）直接调用（！！）
void init_from_file(string filename);//从文件加载游戏
void loadgame();//加载游戏（含有用户交互）
DataTree::data_node* load_tree_from_file(string filename);//从文件加载键值树
bool load_game_from_tree(DataTree::data_node *root);//从键值树加载游戏
bool load_game_from_string(string game);//从文本加载游戏
DataTree::data_node* load_tree_from_string(string game);//从文本加载键值树
void save_game_to_file(string filename);//保存游戏到文件
int get_version(DataTree::data_node *root);//获取存档版本号
DataTree::data_node* up_date_game(DataTree::data_node *root,int old_version);//升级游戏版本
bool check_and_autosave();//判断是否需要自动保存并执行自动保存。进行了保存返回true，没进行保存返回false
void init_preload_ptr_table();//初始化指针表。见preload_ptr_table的注释
void add_preload_ptr(void* ptr, string name);//加入新的指针




//一些剧情函数的定义
void task_1_1();
void task_1_2();
void task_2_1();
void task_2_2();
void task_3_1();
void task_3_2();
void task_4_1();
void task_4_2();
void task_5_1();
void task_5_2();
void task_6_1();
void task_6_2();
void task_7_1();
void task_7_2();
void task_7_3();
void task_8_1();
//void task_8_1_task();
void task_8_2();
void task_9_1();
void task_9_2();
//这堆函数的指针都放到preload_ptr_table里面。见preload_ptr_table的注释

void jump_through_task(int index);//跳过某关(index>=9)

//一堆exe的定义
int exe_wget(int n,const char **t, Computer *c);
int exe_help(int i,const char **t, Computer *c);
int exe_passguesser(int i,const char **t, Computer *c);
int exe_portscan(int i,const char *t[], Computer *c);
int exe_hvm(int i,const char *t[], Computer *c);
int exe_mail(int n,const char **t,Computer *c);
int exe_mail2(int n,const char **t,Computer *c);
int exe_telnet(int i,const char **t,Computer *c);
int exe_tree(int i,const char **t,Computer *c);
int exe_ipconfig(int i,const char **t,Computer *c);
int exe_ping(int i,const char **t,Computer *c);
int exe_savegame(int i,const char **t,Computer *c);
int exe_tracer(int n,const char **t, Computer *c);
int exe_myexe(int i,const char **t,Computer *c);
int exe_sniffer(int argc, const char *argv[], Computer *sender);
//int exe_mujs(int argc, const char *argv[], Computer *sender);
int exe_undefined(int argc, const char *argv[], Computer *sender);
int exe_style(int argc, const char *argv[], Computer *sender);
//这堆函数的指针都放到preload_ptr_table里面。见preload_ptr_table的注释

void logo();//显示logo

//编码系统
struct data_block//这个结构体描述了一个二进制数据块
{
    void * data;
    size_t size;
};
extern char base64_table_a[64];//从64个数映射到字符。base64编码的字典
extern char base64_table_b[255];//从字符映射到64个数。0表示无效
string encoding(string str);//把字符串中的特殊字符（比如说等号，它会干扰存档文件）给编码掉
//斜杠被编码为“\\”，等号被编码成“\e”，回车变编码成"\n"
string decoding(string str);//反过来，按上述规则解码
string bin2string(void *data, size_t size);//把二进制数据块转化成base64编码
//加一句，ptr2id念做ptr to id，那个2是to的谐音。这是计算机里常见的简写
data_block string2bin(string str);//把base64编码转化为二进制数据
//存档里，游戏中文件的内容会被视为二进制文件
//然后exe文件因为有指针会被特殊处理


//八个内置存档，跳关用，编码过
extern string saving_end;
extern string saving_task1;
extern string saving_task2;
extern string saving_task3;
extern string saving_task4;
extern string saving_task5;
extern string saving_task6;
extern string saving_task7;
extern string saving_task8;
//见inbuild_savings.cpp，特别长
//加一句，这几个是用encode（）编码的

//mujs4ghg的定义
//以前我想把一个开源的js解释器嵌入到ghg
//最后放弃了
/*
#include "mujs.h"
class mujs4cpp//包装一下mujs
{
private:
    js_State *J;
public:
    mujs4cpp();
    ~mujs4cpp();
    void bind_function(js_CFunction func, string name, int arg_num);
    void dostring(string s);
};
*/

//声明就完了
//不过各个文件里有的地方还有注释
//一半以上的注释是2021年7月12日写的
//我是故意写的特别啰嗦的:)

class mailManager{
//实现邮件管理，是mail.exe的组成部分。代码见mail.cpp
private:
    Computer * computerBinded;//绑定的电脑
public:
    class mail{//一个类，邮件
    public:
        string subject,receiver,sender,content;
        mail(string subject,string receiver,string sender,string content);
        mail(FileSystem::file * f);
    };
    mailManager(Computer * c);//构造函数
    void showContentOf(int index);//显示某个邮件的内容
    void showList();//显示邮件列表
    mail getContentOf(int index);//获取某个邮件的内容
    vector<string> getList();//获取邮件列表
    void sendMailToSelf(mail m);
};

void send_mail(string subject,string sender,string content,bool if_echo=true);//向玩家发送邮件

#endif
