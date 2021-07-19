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
//#define DEBUG//调试标志，打开这个后会执行一些代码，直接测试最后一关
//#define MAKE_SAVINGS//每进入一关就自动保存一个存档，调试用

#if defined(_WIN32) || defined(_WIN64)//判断是否是windows
#define WINDOWS_OS//Windows模式
#else
#define LINUX_OS//Linux模式，适合在linux上编译
#endif

//#define TEST_WINDOWS//用于在Linux上测试Windows模式
#define NO_DELAY//无延时，测试用
#define FOR_XES//用于生成单文件代码，即文件code.cpp

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
//void add_mail(string mail);



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

void send_mail(string subject,string sender,string content);//向玩家发送邮件

#endif
#ifndef FOR_XES
#include "hackgame.h"
#endif
int exe_undefined(int argc, const char *argv[], Computer *sender)
{
    cout << "这是一个没有实现的exe文件！" << endl;
    return 0;
}
#ifndef FOR_XES
#include "hackgame.h"
#endif

connection_request::connection_request(string f,string t,string ty)
{
    from=f;
    to=t;
    type=ty;
}

net_node::net_node(string i)
{
    binded_computer = NULL;
    ip=i;
    on_package_arrive = NULL;
    on_package_pass = NULL;
}

connection_reply::connection_reply(Computer *c,net_node *n,string s)
{
    computer=c;
    node=n;
    status=s;
}

connection_reply net_node::connect(string remote_ip,string type)
{
    //cout<<"connect()"<<remote_ip<<"\t"<<type<<endl;
    //Internet.show();
    return connect(connection_request(ip,remote_ip,type));
}

connection_reply net_node::connect(connection_request request,bool up_failed)
{
    //cout<<"connect()@"<<ip<<endl;
    if (request.to==ip)
    {
        return connection_reply(binded_computer,this,"successful");
    }
    else
    {
        if (net_map.count(request.to) != 0)
        {
            return net_map[request.to]->connect(request,true);
        }
        else
        {
            if (up_failed)
            {
                return connection_reply(NULL,NULL,"location failed");
            }
            else
            {
                if (father)
                {
                    return father->connect(request);
                }
                else
                {
                    return connection_reply(NULL,NULL,"location failed");
                }
            }
        }
    }
}

bool net_node::add_node(net_node *node,bool add_up)
{
    //cout<<"add_node@"<<ip<<":\t"<<node->ip<<endl;
    add_map(node->ip,node);
    subnet.push_back(node);
    node->father=this;
    if (add_up)
    {
        net_node *down=this;
        net_node *up=father;
        while(up!=nullptr)
        {
            up->add_map(node->ip,down);
            down=up;
            up=up->father;
        }
    }
    return true;
}

bool net_node::add_map(string ip,net_node *node)
{
    return net_map.insert(pair<string,net_node*>(ip,node)).second;
}

bool net_node::bind_computer(Computer* computer)
{
    binded_computer=computer;
    return true;
}

void net_node::show(int format)
//void pre_show(node *p,int format=0) 
{
    map<string,net_node*>::reverse_iterator iter;
    /*
    if (format==0)
    {
        for(iter = net_map.rbegin(); iter != net_map.rend(); iter++)
        {
            cout<<iter->second->ip<<endl;
        }
    }
    */
    
    if (format != 0)
    {
        for (int i=0; i<(format-2)/2; i++)
        {
            cout<<"| ";
        }
        cout<<"|-";
    }
    cout<<ip<</*":subnet.size()="<<subnet.size()<<*/endl;
    for(int i=0;i<subnet.size();i++)
    {
        subnet[i]->show(format+2);
    }
}

void net_node::send_package(data_package package, bool up_failed)
{
    if (package.to==ip)
    {
        package.status = 2;
        if (on_package_arrive)
            (*on_package_arrive)(package ,this);
        return;
    }
    else
    {
        if (net_map.count(package.to) != 0)
        {
            if (on_package_pass)
                (*on_package_pass)(package ,this);
            package.status = 1;
            net_map[package.to]->send_package(package,true);
            return;
        }
        else
        {
            if (up_failed)
            {
                return;
            }
            else
            {
                if (father)
                {
                    if (on_package_pass)
                        (*on_package_pass)(package ,this);
                    package.status = 1;
                    father->send_package(package);
                    return;
                }
                else
                {
                    return;
                }
            }
        }
    }
}

data_package::data_package(string from_i, string to_i, string data_i, string type_i, int status_i)
{
    from = from_i;
    to = to_i;
    data = data_i;
    type = type_i;
    status = status_i;
}

void net_node::send_package(string to, string data, string type)
{
    send_package(
        data_package(
            ip,
            to,
            data,
            type,
            0
        )
    );
}
#ifndef FOR_XES
#include "hackgame.h"
#endif
//#define DEBUG_OUTPUT//是否输出调试信息

ptr2id::ptr2id()
{
    now=0;
}

string ptr2id::getid(void *ptr)
{
    int id;
    char buff[100];
    string ret;
    if (table.count(ptr) != 0)
    {
        id=table[ptr];
    }
    else
    {
        now++;
        id=now;
        table.insert(pair<void*,int>(ptr,id));
    }
    sprintf_s(buff,100,"id%06d",id);
    ret=buff;
    return ret;
}

/*
class file_writer
{
private:
    int format;
    string fname;
    string buff;
    void write_string(string str);
    string encoding(string str);
public:
    file_writer();
    bool open(string filename);
    string write(string str1,string str2="");
    void save();
    void left();
    void right();
};
*/

file_writer::file_writer()
{
    format=0;
}

void file_writer::write_string(string str)
{
    buff += str;
}



void file_writer::save()
{
    fstream f(fname, ios::out);
    if(f.bad())
    {
        cout << "打开文件出错" << endl;
        return;
    }
    f<<buff;
    f.close();
}

bool file_writer::open(string filename)
{
    fname=filename;
    return true;
}

string file_writer::write(string str1,string str2)
{
    string ret;
    for (int i=0; i<format; i++)
    {
        ret += '\t';
    }
    ret += encoding(str1);
    ret += "=";
    ret += encoding(str2);
    ret += "\n";
    write_string(ret);
    return ret;
}

void file_writer::left()
{
    format--;
}
void file_writer::right()
{
    format++;
}






/*------------------------------------------------------*/





void id2ptr::bind(string id, void** ptr_pos)
{
    if (ready.count(id) == 0)
    {
#ifdef DEBUG_OUTPUT
        cout<<"地址槽address["<<ptr_pos<<"]已与id["<<id<<"]绑定"<<endl;
#endif
        if (not_ready_vars.count(id) == 0)
        {
            not_ready_vars.insert(pair<string,list<void**> >(id,list<void**>()));
            not_ready_vars[id].push_back(ptr_pos);
        }
        else
        {
            not_ready_vars[id].push_back(ptr_pos);
        }
    }
    else
    {
        *ptr_pos = ready[id];
    }
}

void id2ptr::bind(string id, vector<void*> *vec, int pos)
{
    if (ready.count(id) == 0)
    {
#ifdef DEBUG_OUTPUT
        cout<<"地址槽vector:"<<vec<<"["<<pos<<"]已与id["<<id<<"]绑定"<<endl;
#endif
        if (not_ready_vectors.count(id) == 0)
        {
            not_ready_vectors.insert(pair<string,list<pair<vector<void*>*,int> > >(id,list<pair<vector<void*>*,int> >()));
            not_ready_vectors[id].push_back(pair<vector<void*>*,int>(vec,pos));
        }
        else
        {
            not_ready_vectors[id].push_back(pair<vector<void*>*,int>(vec,pos));
        }
    }
    else
    {
        (*vec)[pos] = ready[id];
    }
}

void id2ptr::bind(string id, map<string,void*> *mp, string key)
{
    if (ready.count(id) == 0)
    {
#ifdef DEBUG_OUTPUT
        cout<<"地址槽map:"<<mp<<"["<<key<<"]已与id["<<id<<"]绑定"<<endl;
#endif
        if (not_ready_maps.count(id) == 0)
        {
            not_ready_maps.insert(pair<string,list<pair<map<string,void*>*,string> > >(id,list<pair<map<string,void*>*,string> >()));
            not_ready_maps[id].push_back(pair<map<string,void*>*,string>(mp,key));
        }
        else
        {
            not_ready_maps[id].push_back(pair<map<string,void*>*,string>(mp,key));
        }
    }
    else
    {
        (*mp)[key] = ready[id];
    }
}

void id2ptr::load(string id, void* ptr)
{
#ifdef DEBUG_OUTPUT
    cout<<id<<"被加载为"<<ptr<<endl;
#endif
    ready.insert(pair<string,void*>(id,ptr));
    auto nrl = not_ready_vars[id];
    auto itor = nrl.begin();
	while(itor != nrl.end())
    {
#ifdef DEBUG_OUTPUT
        cout<<"向槽address["<<*itor<<"]填入地址"<<ptr<<endl;
#endif
        **itor = ptr;
        itor++;
    }
    not_ready_vars.erase(id);
    auto nrl2 = not_ready_vectors[id];
    auto itor2 = nrl2.begin();
	while(itor2 != nrl2.end())
    {
#ifdef DEBUG_OUTPUT
        cout<<"向槽vector:"<<(itor2->first)<<"["<<(itor2->second)<<"]填入地址"<<ptr<<endl;
#endif
        (*(itor2->first))[itor2->second] = ptr;
        itor2++;
    }
    not_ready_vectors.erase(id);
    auto nrl3 = not_ready_maps[id];
    auto itor3 = nrl3.begin();
	while(itor3 != nrl3.end())
    {
#ifdef DEBUG_OUTPUT
        cout<<"向槽map:"<<(itor3->first)<<"["<<(itor3->second)<<"]填入地址"<<ptr<<endl;
#endif
        (*(itor3->first))[itor3->second] = ptr;
        itor3++;
    }
    not_ready_maps.erase(id);
}

void id2ptr::show()
{
    auto itor = ready.begin();
    while (itor != ready.end())
    {
#ifdef DEBUG_OUTPUT
        cout<<itor->first<<" : "<<itor->second<<endl;
#endif
        itor++;
    }

    auto itor1 = not_ready_vars.begin();
    while (itor1 != not_ready_vars.end())
    {
        cout<<"id["<<itor1->first<<"]未被加载！"<<endl;
        itor1++;
    }

    auto itor2 = not_ready_vectors.begin();
    while (itor2 != not_ready_vectors.end())
    {
        cout<<"id["<<itor2->first<<"]未被加载！"<<endl;
        itor2++;
    }

    auto itor3 = not_ready_maps.begin();
    while (itor3 != not_ready_maps.end())
    {
        cout<<"id["<<itor3->first<<"]未被加载！"<<endl;
        itor3++;
    }
    /*
    cout<<"not_ready_vars"<<endl;
    auto nrl = not_ready_vars[id];
    auto itor = nrl.begin();
	while(itor != nrl.end())
    {
        cout<<itor<<endl
        itor++;
    }
    cout<<"not_ready_vectors"<<endl;
    auto nrl2 = not_ready_vectors[id];
    auto itor2 = nrl2.begin();
	while(itor2 != nrl2.end())
    {
        cout<<itor2->first<<" : "<<itor2->second<<endl;
        itor++;
    }
    cout<<"not_ready_maps"<<endl;
    auto nrl3 = not_ready_maps[id];
    auto itor3 = nrl3.begin();
	while(itor3 != nrl3.end())
    {
        cout<<itor3->first<<" : "<<itor3->second<<endl;
        itor++;
    }
    */
}
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
    cout<<"\n";
    cout<<"没了，就这么几个，不过这个简单的文件系统我真做了老半天，命令行系统也是我自己做的。命令太少，请谅解。\n";
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
            reply.computer->password=="password0abc")
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
    cout<<"Ping "<<t[1]<<" with 32 bytes of data:"<<endl;
    for (int i=0;i<4;i++)
    {
        sent++;
        reply=c->netnode->connect(t[1]);
        if (reply.computer)
        {
            received++;
            cout<<"Reply from "<<t[1]<<": bytes=32 time="<<rand()%5+20<<" TTL=100"<<endl;
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
    cout<<"Ping statistics for "<<t[1]<<":"<<endl;
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
/*
 *文件名:   computer.cpp
 *作者:     Nictheboy
 *内容:     实现Computer类
 *
 *最后一次修改: 2020/6/20
 */
#ifndef FOR_XES
#include "hackgame.h"
#endif
//Computer的构造函数，进行主机的初始化
Computer::Computer()
{
    //绑定指令，把这个类的各个内置命令对应的方法加载到pfm里面
    pfm.add("dc", &Computer::dc);
    pfm.add("dir", &Computer::dir);
    pfm.add("ls", &Computer::dir);
    pfm.add("cd", &Computer::cd);
    pfm.add("pwd", &Computer::pwd);
    pfm.add("type", &Computer::type);
    pfm.add("cat", &Computer::type);
    pfm.add("copy", &Computer::copy);
    pfm.add("cp", &Computer::copy);
    pfm.add("mkdir", &Computer::mkdir);
    pfm.add("del", &Computer::del);
    pfm.add("rm", &Computer::del);
    pfm.add("scp", &Computer::scp);
    pfm.add("install", &Computer::install);
    pfm.set_belonger(this);//绑定pfm。用于方便pfm找到对应命令后调用方法

    net_conversation_caller = NULL;//设置caller是NULL
    event_before_input = NULL;//设置事件为空
    dc_signal = false;//初始化
    from = "";//初始化
    username = "root";//默认用户
    password = "weoferhsohogowergorewhogheowgh";//默认口令，别嘻嘻嘻，主机口令在创建后肯定是要改的

    name = "";//无主机名
    style = basic_style;//默认样式
}

//Computer的构造函数，进行主机的初始化
Computer::Computer(string ip)//参数是主机的ip
{
    //同上
    pfm.add("dc", &Computer::dc);
    pfm.add("dir", &Computer::dir);
    pfm.add("ls", &Computer::dir);
    pfm.add("cd", &Computer::cd);
    pfm.add("pwd", &Computer::pwd);
    pfm.add("type", &Computer::type);
    pfm.add("cat", &Computer::type);
    pfm.add("copy", &Computer::copy);
    pfm.add("cp", &Computer::copy);
    pfm.add("mkdir", &Computer::mkdir);
    pfm.add("del", &Computer::del);
    pfm.add("rm", &Computer::del);
    pfm.add("scp", &Computer::scp);
    pfm.add("install", &Computer::install);
    pfm.set_belonger(this);//同上
    net_conversation_caller = NULL;//同上
    event_before_input = NULL;//设置事件为空
    dc_signal = false;//初始化
    from = "";//初始化
    username = "root";//默认用户
    password = "weoferhsohogowergorewhogheowgh";//默认口令

    netnode = new net_node(ip);//新建一个网络节点，ip为参数中的IP
    netnode->bind_computer(this);//将主机绑定到新节点

    name = "";//无主机名
    style = basic_style;//默认样式
    root = new FileSystem::dir("");//创建根目录
    dir_now = root;//默认位于根目录

    //创建文件夹和文件
    root->add_new_dir("system");
    locate_dir("/system")->add_new_txt("kernel32.dll", "这不是一个有效的文本文件！");
    locate_dir("/system")->add_new_txt("rundll32.dll", "这不是一个有效的文本文件！");
    locate_dir("/system")->add_new_txt("sysmain.sys", "这不是一个有效的文本文件！");
    locate_dir("/system")->add_new_txt("service.dll", "这不是一个有效的文本文件！");
    locate_dir("/system")->add_new_txt("cmd.exe", "这不是一个有效的文本文件！");
    root->add_new_dir("log");
    root->add_new_dir("data");
    root->add_new_dir("bin");


    //创建初始exe文件
    write_exe("help", &exe_help);
    write_exe("mail", &exe_mail);
    write_exe("telnet", &exe_telnet);
    write_exe("tree", &exe_tree);
    write_exe("ipconfig", &exe_ipconfig);
    write_exe("ping", &exe_ping);
    write_exe("savegame", &exe_savegame);
    write_exe("myexe", &exe_myexe);
}

//实现登陆的效果
bool Computer::login()
{
    string tmp;
    string tmp2;
    for (int i = 0;i < 3;i++)//重复三次
    {
        cout << style.login_output;
        string tmp;
        getline(cin, tmp);//获取用户名输入
        cout << style.pass_output;
        string tmp2;
        getline(cin, tmp2);//获取密码输入
        send_here ("登陆:"+tmp+" 密码:"+tmp2);//让连接节点发送一个数据包。用与截获密码的效果
        if (tmp == username && tmp2 == password)
        {
            //登陆成功
            cout << style.login_successful << endl;
            return true;
        }
        else 
        {
            //登陆失败
            if (i == 2)
            {
                cout << style.login_fail << endl;
            }
            else 
            {
                cout << style.login_wrong << endl;
            }
        }
    }
    return false;
}

//获取制定端口名称对应的端口号
int Computer::get_port(string name/*端口名称*/)
{
    //遍历port_list
    for (int i = 0;i < port_list.size();i++)
    {
        if (port_list.at(i).name == name)
        {
            return port_list.at(i).port;
        }
    }
    return -1;//-1表示没有这个端口
}

//断开连接。run（）的循环一开始就会检查dc_signal，所以可以通过设置 dc_signal退出run（）函数
int Computer::dc(int i, const char** t)
{
    dc_signal = true;
    return 0;
}

//列出文件和文件夹
int Computer::dir(int i, const char** t)
{
    string str;
    FileSystem::dir * d = locate_dir((i==1)?"":t[1]);
    if (!d){
        cout << t[1] <<"不存在或不是文件夹" << endl;
        return 0;
    }
    //遍历当前文件夹的子文件夹
    for (i = 0; i < d->subdir.size(); i++)
    {
        str = d->subdir[i]->name;
        cout << str;
        //补齐
        for (int i = 0;strlen(str.c_str()) <= 29 && i < (30 - strlen(str.c_str())); i++)
        {
            cout << " ";
        }
        cout << "<DIR>\n";
    }
    //遍历当前文件夹的文件
    for (i = 0; i < d->content.size(); i++)
    {
        str = d->content[i]->name;
        cout << str;
        cout << endl;
    }
    return 0;
}

//进入目录
int Computer::cd(int i, const char** t)
{
    //判断是否缺少参数
    if (i < 2)
    {
        cout << "语法错误！\n";
        return 0;
    }
    //temp指向参数对应的文件夹
    FileSystem::dir* temp = locate_dir(t[1]);
    if (temp)//如果不为NULL
    {
        dir_now = temp;
    }
    else//如果是NULL
    {
        cout << "找不到该目录!" << endl;
    }
    return 0;
}

//根据文件夹路径定位文件夹
FileSystem::dir* Computer::locate_dir(string path)
{
    vector<string> listed_ps = split(path, "/", true);//分割
    FileSystem::dir* temp;
    int i=0;
    if (listed_ps.size() == 0)//path是空则返回当前目录
    {
        return dir_now;
    }
    else
    {
        if (listed_ps[0] == "")//如果路径是斜杠打头
        {
            temp = root;//从根目录开始
            i = 1;//跳过第一项
        }
        else
        {
            temp = dir_now;//从当前目录开始
            i = 0;
        }
    }
    for (/*前面设置过i了*/;i < listed_ps.size();i++)
    {
        if (listed_ps[i] == "." || listed_ps[i] == "..")//假如是点
        {
            if (!(temp->father))//没有上级目录则失败
            {
                return nullptr;
            }
            temp = temp->father;//到上级目录
        }
        else
        {
            int ps = temp->search_subdir(listed_ps[i]);//查找目录的索引
            if (ps == -1)//-1表示没找着
            {
                return nullptr;//失败
            }
            else
            {
                temp = temp->subdir[ps];//到下一个目录
            }
        }
    }
    return temp;//返回目录指针
}

//定位文件
FileSystem::file* Computer::locate_file(string path)
{
    //和上面类似
    vector<string> listed_ps = split(path, "/", true);
    FileSystem::dir* temp;
    int i;
    if (listed_ps.size() == 0)
    {
        return nullptr;
    }
    if (listed_ps.size() == 1)
    {
        temp = dir_now;
    }
    else
    {
        if (listed_ps[0] == "")
        {
            temp = root;
            i = 1;
        }
        else
        {
            temp = dir_now;
            i = 0;
        }
    }
    for (;i < listed_ps.size() - 1;i++)
    {
        if (listed_ps[i] == "." || listed_ps[i] == "..")
        {
            //cout<<"OK";
            if (!(temp->father))
            {
                return nullptr;
            }
            temp = temp->father;
            //cout<<"OK";
        }
        else
        {
            //cout<<dir_now->search_subdir(listed_ps[i]);
            //cout<<"OK";
            int ps = temp->search_subdir(listed_ps[i]);
            if (ps == -1)
            {
                return nullptr;
            }
            else
            {
                temp = temp->subdir[ps];
            }
            //cout<<"OK";
        }
    }
    //上面相当于找到了文件夹，下面是找到文件返回
    return temp->locate_file(listed_ps[listed_ps.size() - 1]);
}

bool Computer::delete_file(string path){//删除文件。/打头则从根目录开始，支持上级目录
    //和上面类似
    vector<string> listed_ps = split(path, "/", true);
    FileSystem::dir* temp;
    int i;
    if (listed_ps.size() == 0)
    {
        return false;
    }
    if (listed_ps.size() == 1)
    {
        temp = dir_now;
    }
    else
    {
        if (listed_ps[0] == "")
        {
            temp = root;
            i = 1;
        }
        else
        {
            temp = dir_now;
            i = 0;
        }
    }
    for (;i < listed_ps.size() - 1;i++)
    {
        if (listed_ps[i] == "." || listed_ps[i] == "..")
        {
            //cout<<"OK";
            if (!(temp->father))
            {
                return false;
            }
            temp = temp->father;
            //cout<<"OK";
        }
        else
        {
            //cout<<dir_now->search_subdir(listed_ps[i]);
            //cout<<"OK";
            int ps = temp->search_subdir(listed_ps[i]);
            if (ps == -1)
            {
                return false;
            }
            else
            {
                temp = temp->subdir[ps];
            }
            //cout<<"OK";
        }
    }
    //上面相当于找到了文件夹，下面是删除文件
    FileSystem::file * f = temp->locate_file(listed_ps[listed_ps.size() - 1]);
    if (!f){
        return false;
    }
    temp->delete_file(f->name);
    //delete f->data;
    delete f;
    return true;
}
bool Computer::delete_dir(string path){//删除文件夹。/打头则从根目录开始，支持上级目录
    vector<string> listed_ps = split(path, "/", true);//分割
    FileSystem::dir* temp;
    int i=0;
    if (listed_ps.size() == 0)//path是空则返回当前目录
    {
        return false;
    }
    else
    {
        if (listed_ps[0] == "")//如果路径是斜杠打头
        {
            temp = root;//从根目录开始
            i = 1;//跳过第一项
        }
        else
        {
            temp = dir_now;//从当前目录开始
            i = 0;
        }
    }
    for (/*前面设置过i了*/;i < listed_ps.size();i++)
    {
        if (listed_ps[i] == "." || listed_ps[i] == "..")//假如是点
        {
            if (!(temp->father))//没有上级目录则失败
            {
                return false;
            }
            temp = temp->father;//到上级目录
        }
        else
        {
            int ps = temp->search_subdir(listed_ps[i]);//查找目录的索引
            if (ps == -1)//-1表示没找着
            {
                return false;//失败
            }
            else
            {
                temp = temp->subdir[ps];//到下一个目录
            }
        }
    }
    //return temp;//返回目录指针
    FileSystem::dir * d = temp;
    if (!(d->father)){
        return false;
    }
    (d->father)->delete_dir(d->name);
    delete d;
    return true;
}

//显示当前目录
int Computer::pwd(int i, const char** t)
{
    stack<FileSystem::dir*> stk;//栈结构
    FileSystem::dir* temp = dir_now;
    do
    {
        stk.push(temp);
        temp = temp->father;
    } while (temp != nullptr);//倒着从当前目录到根目录
    stk.pop();//pop出最后一个，也就是根目录
    if (stk.empty())//假如是空的，也就是当前就是根目录
    {
        cout << "/";
    }
    while (!stk.empty())//一个一个出栈然后显示
    {
        cout << "/" << stk.top()->name;
        stk.pop();
    }
    cout << endl;
    return 0;
}

//输出一个文本文件的内容
int Computer::type(int i, const char* t[])
{
    using namespace FileSystem;

    if (i < 2)
    {
        cout << "缺少参数!语法:type [文件名]\n";
        return 0;
    }
    string filename = t[1];
    file* f = locate_file(filename);
    if (f)
    {
        string temp;
        temp = (char*)f->data;
        cout << temp << endl;
    }
    else {
        cout << "文件不存在!\n";
    }
    return 0;
}

//复制文件
int Computer::copy(int i, const char* c[])
{
    using namespace FileSystem;


    if (i < 3)
    {
        cout << "缺少参数!语法:copy [被复制文件名] [目标文件名]\n";
        return 0;
    }
    vector<string> listed_to = split(c[2], "/");
    string name = listed_to.at(listed_to.size() - 1);

    file* file_from = locate_file(c[1]);
    class dir* dir_to = dir_now->turn_file_ps_into_dir_ps(c[2]);

    if (file_from == NULL)
    {
        cout << "被复制文件不存在!\n";
        return 0;
    }
    if (dir_to == NULL)
    {
        cout << "目标目录不存在!\n";
        return 0;
    }
    void* newdata = malloc(file_from->size);
    file* newfile = new file(name, file_from->size, newdata);
    memcpy(newdata, file_from->data, file_from->size);
    dir_to->add_file(newfile);
    return 0;
}

//新建文件夹
int Computer::mkdir(int n, const char* c[])
{
    using namespace FileSystem;

    if (n < 2)
    {
        cout << "缺少参数!语法:mkdir [创建的文件夹名]\n";
        return 0;
    }
    for (int j = 0; j != strlen(c[1]); j++){
        if (c[1][j] == '/'){
            cout << "文件夹名不合法!" << endl;
            return 0;
        }
    }
    if (locate_dir(c[1])){
        cout << "文件夹已存在!" <<endl;
        return 0;
    }
    dir_now->add_new_dir(c[1]);
    return 0;
}

//删除文件
int Computer::del(int n, const char* c[])
{
    using namespace FileSystem;
    if (n < 2)
    {
        cout << "缺少参数!语法:del [删除的文件或文件夹名]\n";
        return 0;
    }
    if(!(delete_file(c[1])||delete_dir(c[1]))){
        cout << "文件或文件夹不存在!\n";
    }

    /*
    if (!(dir_now->delete_file(c[1]) || dir_now->delete_dir(c[1])))
    {
        class dir* fath = dir_now->turn_file_ps_into_dir_ps(c[1]);
        file* f = dir_now->locate_file(c[1]);
        //cout<<fath<<endl;
        //cout<<f<<endl;
        if (fath && f)
        {
            fath->delete_file(f->name);
        }
        else
        {
            cout << "文件或文件夹不存在!\n";
        }
        
    }
    */
    /*
    dir * d = locate_dir (dir_now->turn_file_ps_into_dir_ps (c[1]));
    file * f = locate_file (c[1]);
    if ((!d)||(!f)){
        cout << "文件或文件夹不存在!" << endl;
    }
    f->delete_file ();
    */
    return 0;
}

//写日志
void Computer::write_log(string name, string content)
{
    using namespace FileSystem;

    time_t timep;
    struct tm ts;
    time(&timep);
    localtime_s_f(&ts,&timep);
    struct tm *t = &ts;
    stringstream ss;
    ss << "[" << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "]";
    string time_str = ss.str();

    class dir* d = locate_dir("/log");
    file* f = d->locate_file(name + ".log");
    if (f)
    {
        string old;
        old = (char*)f->data;
        old = old + time_str + content + "\n";
        d->delete_file(name + ".log");
        d->add_new_txt(name + ".log", old);
    }
    else {
        locate_dir("/log")->add_new_txt(name + ".log", time_str + content + "\n");
    }
}

//写exe
void Computer::write_exe(string name, FileSystem::exe_adr adr)
{
    using namespace FileSystem;
    locate_dir("/bin")->add_file(new file(name + ".exe", adr));
}

//远程复制文件
int Computer::scp(int n, const char* c[])
{
    using namespace FileSystem;
    if (n < 2)
    {
        cout << "缺少参数!语法:\n";
        cout << "scp [远程文件名]"<<endl;
        cout << "scp -u [本地文件名] [远程ip](:[端口])"<<endl;
        cout << "scp -d [远程文件名] [远程ip](:[端口])"<<endl;
        return 0;
    }
    file* from;
    class dir* to;
    if (strcmp(c[1],"-u") == 0)
    {
        if (n < 3)
        {
            cout << "缺少参数!语法:\n";
            cout << "scp [远程文件名]"<<endl;
            cout << "scp -u [本地文件名] [远程ip](:[端口])"<<endl;
            cout << "scp -d [远程文件名] [远程ip](:[端口])"<<endl;
            return 0;
        }
        string ip;
        int port;
        try
        {
            ipport_cutter cutter(c[3],23);
            ip = cutter.ip;
            port = cutter.port;
        }
        catch (string)
        {
            cout<<"输入格式错误!"<<endl;
        }
        connection_reply reply = netnode->connect(ip);//连接，以获取远程主机的指针
        if (reply.computer == NULL/*没找到主机*/ || 
            reply.computer->get_port("telnet") != port/*端口关闭*/)
        {
            cout<<"无法连接到远程主机的telnet服务!上传文件失败"<<endl;
            return 0;
        }
        cout<<"正在与远程主机建立会话..."<<endl;
        delay(0.5);
        if (!reply.computer->login())
        {
            cout<<"权限不足"<<endl;
            return 0;
        }
        to = reply.computer->locate_dir("/data");
        if (!to)
        {
            reply.computer->root->add_new_dir("data");
            to = reply.computer->locate_dir("/data");
        }
        from = dir_now->locate_file(c[2]);
        if (from == NULL)
        {
            from = locate_file(c[2]);
            if (from == NULL)
            {
                cout << "文件不存在！\n";
                return 0;
            }
        }
        void* data = malloc(from->size);
        memcpy(data, from->data, from->size);
        file* new_f = new file(from->name, from->size, data);
        file* old = to->locate_file(new_f->name);
        while (old && old->name == new_f->name)
        {
            new_f->name += "(2)";
        }
        to->add_file(new_f);
        cout << "正在将本地文件" + from->name + "复制到远程目录/data\n";
        delay(2);
        cout << "复制成功！\n";
        return 0;
    }
    else if (strcmp(c[1],"-d") == 0)
    {
        if (n < 3)
        {
            cout << "缺少参数!语法:\n";
            cout << "scp [远程文件名]"<<endl;
            cout << "scp -u [本地文件名] [远程ip](:[端口])"<<endl;
            cout << "scp -d [远程文件名] [远程ip](:[端口])"<<endl;
            return 0;
        }
        string ip;
        int port;
        try
        {
            ipport_cutter cutter(c[3],23);
            ip = cutter.ip;
            port = cutter.port;
        }
        catch (string)
        {
            cout<<"输入格式错误!"<<endl;
        }
        //类似
        connection_reply reply = netnode->connect(ip);
        if (reply.computer == NULL || 
            reply.computer->get_port("telnet") != port)
        {
            cout<<"无法连接到远程主机的telnet服务!下载文件失败"<<endl;
            return 0;
        }
        cout<<"正在与远程主机建立会话..."<<endl;
        delay(0.5);
        if (!reply.computer->login())//登陆失败的话
        {
            cout<<"权限不足"<<endl;
            return 0;
        }
        to = localhost->locate_dir("/data");
        if (!to)
        {
            localhost->root->add_new_dir("data");
            to = localhost->locate_dir("/data");
        }
        from = reply.computer->locate_file(c[2]);
        if (from == NULL)
        {
            cout << "文件不存在！\n";
            return 0;
        }
        void* data = malloc(from->size);
        memcpy(data, from->data, from->size);
        file* new_f = new file(from->name, from->size, data);
        file* old = to->locate_file(new_f->name);
        while (old && old->name == new_f->name)
        {
            new_f->name += "(2)";
        }
        to->add_file(new_f);
        cout << "正在将远程文件" + from->name + "复制到远程目录/data\n";
        delay(2);
        cout << "复制成功！\n";
        return 0;
    }
    else
    {
        if (this == localhost)
        {
            cout << "scp用于从远程计算机复制文件，请不要在本地计算机上使用scp\n";
            return 0;
        }
        to = localhost->locate_dir("/data");
        if (!to)
        {
            localhost->root->add_new_dir("data");
            to = localhost->locate_dir("/data");
        }
        from = dir_now->locate_file(c[1]);
        if (from == NULL)
        {
            from = locate_file(c[1]);
            if (from == NULL)
            {
                cout << "文件不存在！\n";
                return 0;
            }
        }
        void* data = malloc(from->size);
        memcpy(data, from->data, from->size);
        file* new_f = new file(from->name, from->size, data);

        file* old = to->locate_file(new_f->name);
        while (old && old->name == new_f->name)
        {
            new_f->name += "(2)";
        }
        to->add_file(new_f);
        cout << "正在将文件" + from->name + "复制到本地目录/data\n";
        delay(2);
        cout << "复制成功！\n";
        return 0;
    }
    return 0;
}

//安装程序，实际上是复制文件到bin文件夹
int Computer::install(int n, const char* c[])
{
    using namespace FileSystem;
    //参数不足
    if (n < 2)
    {
        cout << "缺少参数!语法:install [想要安装的程序的位置]\n";
        return 0;
    }

    file* from;
    class dir* to;
    //定位原来的文件
    from = dir_now->locate_file(c[1]);
    if (from == NULL)
    {
        cout << "文件不存在！\n";
        return 0;
    }
    to = locate_dir("/bin");//定位bin文件夹
    if (!to)//没有bin文件夹，创建文件夹
    {
        localhost->root->add_new_dir("bin");
        to = localhost->locate_dir("/bin");
    }
    file* old = to->locate_file(from->name);//检测有没有重名文件
    if (old)
    {
        cout << from->name << "已经安装!请勿重复安装\n";
        return 0;
    }
    //复制文件
    void* data = malloc(from->size);
    memcpy(data, from->data, from->size);
    file* new_f = new file(from->name, from->size, data);
    to->add_file(new_f);
    cout << "正在安装程序" + from->name + "中...";
    delay(2);
    cout << "安装成功！\n";
    return 0;
}

//获取公开共享的文件夹
FileSystem::dir* Computer::get_http_dir(int port)
{
    if (port == get_port("http")||port == get_port("www"))//检测端口
    //www是老版游戏中对http的称呼
    {
        FileSystem::dir* http = locate_dir("/web");
        FileSystem::dir* www = locate_dir("/www");
        if (www){//解决兼容性问题
            return www;
        }else{
            return http;
        }
    }
    else {
        return NULL;
    }
}

//从连接者发送一个数据包到本地
void Computer::send_here(string data, string type)
{
    if (net_conversation_caller)//假如连接者指针不是NULL
        net_conversation_caller->send_package(netnode->ip, data, type);//发送数据包
}

//打开一个端口
void Computer::open_port(string name, int port)
{
    port_list.push_back(port_info(name,port));//在port_list中加一个对象
}

bool Computer::process_event_before_input()
{
    //这个是检查有函数指针是不是NULL，这个指针一般执行剧情函数
    if (event_before_input)
    {
        (*event_before_input)();
        return true;
        //event_before_input=NULL;
    }
    else
    {
        return false;
    }
}

bool Computer::process_command(string cmd)
{
    using namespace FileSystem;
    //这个是把输入给一个pfm对象。这个是Performer类的实例。用途是看看命令是不是内置命令
    string input = cmd;
    pfm.import_command(input);
    if (!pfm.process())//如果 pfm.process()是false表示没执行成功
    {
        //pfm执行完process（）之后会把分过块的命令文本存到listed_cmd里面
        //这是在bin文件夹里找有没有对应的exe文件
        string cmd_name = *(pfm.listed_cmd.begin());
        bool ok = false;
        file *f = locate_file(cmd_name);
        if (!f)
        {
            f = locate_file(cmd_name + ".exe");
            if (!f)
            {
                f = locate_file("/bin/" + cmd_name);
                if (!f)
                {
                    f = locate_file("/bin/" + cmd_name + ".exe");
                    if (!f)
                    {
                        return false;
                    }
                }
            }
        }
        f->run_exe(pfm.number_of_cmd, pfm.listed_cmd_c_str, this);
    }
    return true;
}

void Computer::echo_before_command()
{
    cout << style.before_ip;
    cout << username;
    cout << style.between_ip;
    if (name != "")
    {
        cout << name;
    }
    else
    {
        cout << netnode->ip;
    }
    cout << " ";
    if (dir_now == root)
        cout << "/";
    else
        cout << dir_now->name;
    cout <<style.after_ip<< style.getinput;
}

//运行这台电脑
void Computer::run( bool need_login/*是否需要登陆*/, 
                    string f/*连接者的名字，用于写日志*/, 
                    net_node *caller/*连接者的网络节点，默认为NULL，用于发送数据包实现截获数据的效果*/)
{
    using namespace FileSystem;
    from = f;
    net_conversation_caller = caller;
    write_log("connect", from + " 连接了本地终端");//写日志
    send_here ("连接成功");//从连接者发送数据包到自己
    string input;
    //这部分是登陆
    if (need_login)
    {
        if (!login())//登陆失败
        {
            write_log("connect", from + " 因密码错误登陆失败！");//日志
            return;
        }
        write_log("connect", from + " 登录成功");//日志
    }
    //while（true）里面是获取用户输入然后执行命令
    while (true)//循环。出口在下面的return退出函数
    {
        //这个是判断是否要断开
        //如果有断开的标志就return
        if (dc_signal)
        {
            dc_signal = false;
            return;
        }
        process_event_before_input();//处理事件
        echo_before_command();//根据style的数据输出前面那些提示符。这些不是很重要
        check_and_autosave();//检查自动保存并保存
        getline(cin, input);
        if (input != "")
        {
            try
            {
                if (process_command(input))
                {
                    //check_and_autosave();//执行了命令后检查自动保存并保存
                }
                else
                {
                    cout << input << "不是任何内部或外部指令！！输入help获取指令列表\n";
                }
            }
            catch (string s){
                cout<<BG_RED<<"程序抛出异常:"<<s<<RESET<<endl;
            }
            catch (...){
                cout<<BG_RED<<"执行命令'"<<input<<"'的时候发生了未处理的异常!"<<RESET<<endl;
            }
        }
    }
}
#ifndef FOR_XES
#include "hackgame.h"
#endif
void add_preload_ptr(void* ptr, string name)
{
    preload_ptr_table.push_back(pair<void*,string>(ptr,name));
}

void  init_preload_ptr_table()
{
    add_preload_ptr(NULL,"NULL");
    add_preload_ptr((void*)&task_1_1,"task_1_1");
    add_preload_ptr((void*)&task_1_2,"task_1_2");
    add_preload_ptr((void*)&task_2_1,"task_2_1");
    add_preload_ptr((void*)&task_2_2,"task_2_2");
    add_preload_ptr((void*)&task_3_1,"task_3_1");
    add_preload_ptr((void*)&task_3_2,"task_3_2");
    add_preload_ptr((void*)&task_4_1,"task_4_1");
    add_preload_ptr((void*)&task_4_2,"task_4_2");
    add_preload_ptr((void*)&task_5_1,"task_5_1");
    add_preload_ptr((void*)&task_5_2,"task_5_2");
    add_preload_ptr((void*)&task_6_1,"task_6_1");
    add_preload_ptr((void*)&task_6_2,"task_6_2");
    add_preload_ptr((void*)&task_7_1,"task_7_1");
    add_preload_ptr((void*)&task_7_2,"task_7_2");
    add_preload_ptr((void*)&task_7_3,"task_7_3");
    add_preload_ptr((void*)&task_8_1,"task_8_1");
    add_preload_ptr((void*)&task_8_2,"task_8_2");
    add_preload_ptr((void*)&task_9_1,"task_9_1");
    add_preload_ptr((void*)&task_9_2,"task_9_2");
    add_preload_ptr((void*)&exe_wget,"exe_wget");
    add_preload_ptr((void*)&exe_help,"exe_help");
    add_preload_ptr((void*)&exe_passguesser,"exe_passguesser");
    add_preload_ptr((void*)&exe_portscan,"exe_portscan");
    add_preload_ptr((void*)&exe_hvm,"exe_hvm");
    add_preload_ptr((void*)&exe_mail,"exe_mail");
    add_preload_ptr((void*)&exe_telnet,"exe_telnet");
    add_preload_ptr((void*)&exe_tree,"exe_tree");
    add_preload_ptr((void*)&exe_ipconfig,"exe_ipconfig");
    add_preload_ptr((void*)&exe_ping,"exe_ping");
    add_preload_ptr((void*)&exe_savegame,"exe_savegame");
    add_preload_ptr((void*)&exe_myexe,"exe_myexe");
    add_preload_ptr((void*)&exe_tracer,"exe_tracer");
    add_preload_ptr((void*)&exe_sniffer,"exe_sniffer");
    add_preload_ptr((void*)&exe_mail2,"exe_mail2");
//        fw.write(table.getid((void*)&exe_mujs),"exe_mujs");
}
#ifndef FOR_XES
#include "hackgame.h"
#endif
string saving_end = string("version_code=4\npreload=\n	id000001=NULL\n	id000002=task_1_1\n	id000003=task_1_2\n	id000004=task_2_1\n	id000005=task_2_2\n	id000006=task_3_1\n	id000007=task_3_2\n	id000008=task_4_1\n	id000009=task_4_2\n	id000010=task_5_1\n	id000011=task_5_2\n	id000012=task_6_1\n	id000013=task_6_2\n	id000014=task_7_1\n	id000015=task_7_2\n	id000016=task_7_3\n	id000017=task_8_1\n	id000018=task_8_2\n	id000019=task_9_1\n	id000020=task_9_2\n	id000021=exe_wget\n	id000022=exe_help\n	id000023=exe_passguesser\n	id000024=exe_portscan\n	id000025=exe_hvm\n	id000026=exe_mail\n	id000027=exe_telnet\n	id000028=exe_tree\n	id000029=exe_ipconfig\n	id000030=exe_ping\n	id000031=exe_savegame\n	id000032=exe_myexe\n	id000033=exe_tracer\n	id000034=exe_sniffer\nglobal=\n	task_number=8\n	sub_task_number=1\n	Internet=id000035\n	localhost=id000036\n	default_filename=temp\n	autosave=true\nnet_node=\n	id000037=\n		ip=192.168.0.0\n		binded_computer=id000036\n		father=id000038\n		net_map=\n		subnet=\n	id000039=\n		ip=192.168.0.1\n		binded_computer=id000040\n		father=id000038\n		net_map=\n		subnet=\n	id000041=\n		ip=192.168.0.2\n		binded_computer=id000042\n		father=id000038\n		net_map=\n		subnet=\n	id000038=\n		ip=188.131.133.248\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=192.168.0.0$id000037\n			1=192.168.0.1$id000039\n			2=192.168.0.2$id000041\n		subnet=\n			0=id000037\n			1=id000039\n			2=id000041\n	id000043=\n		ip=29.53.103.3\n		binded_computer=id000044\n		father=id000035\n		net_map=\n		subnet=\n	id000045=\n		ip=178.53.100.24\n		binded_computer=id000046\n		father=id000035\n		net_map=\n		subnet=\n	id000047=\n		ip=117.51.143.120\n		binded_computer=id000048\n		father=id000035\n		net_map=\n		subnet=\n	id000049=\n		ip=180.21.10.220\n		binded_computer=id000050\n		father=id000035\n		net_map=\n		subnet=\n	id000051=\n		ip=24.120.41.10\n		binded_computer=id000052\n		father=id000053\n		net_map=\n		subnet=\n	id000054=\n		ip=24.120.41.20\n		binded_computer=id000055\n		father=id000053\n		net_map=\n		subnet=\n	id000053=\n		ip=24.120.41.0\n		binded_computer=id000056\n		father=id000057\n		net_map=\n			0=24.120.41.10$id000051\n			1=24.120.41.20$id000054\n		subnet=\n			0=id000051\n			1=id000054\n	id000057=\n		ip=24.120.41.2\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=24.120.41.0$id000053\n			1=24.120.41.10$id000053\n			2=24.120.41.20$id000053\n		subnet=\n			0=id000053\n	id000035=\n		ip=Internet\n		binded_computer=id000001\n		father=id000001\n		net_map=\n			0=117.51.143.120$id000047\n			1=178.53.100.24$id000045\n			2=180.21.10.220$id000049\n			3=188.131.133.248$id000038\n			4=24.120.41.0$id000057\n			5=24.120.41.10$id000057\n			6=24.120.41.2$id000057\n			7=24.120.41.20$id000057\n			8=29.53.103.3$id000043\n		subnet=\n			0=id000038\n			1=id000043\n			2=id000045\n			3=id000047\n			4=id000049\n			5=id000057\ncomputer=\n	id000056=\n		event_before_input=id000018\n		username=root\n		password=password0abc\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$91\n		netnode=id000053\n		root=id000058\n	id000055=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000054\n		root=id000059\n	id000052=\n		event_before_input=id000001\n		username=root\n		password=pass_lispt202\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000051\n		root=id000060\n	id000050=\n		event_before_input=id000001\n		username=root\n		password=root\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$256\n		netnode=id000049\n		root=id000061\n	id000048=\n		event_before_input=id000001\n		username=root\n		password=password123\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000047\n		root=id000062\n	id000046=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=www$21\n		netnode=id000045\n		root=id000063\n	id000044=\n		event_before_input=id000001\n		username=root\n		password=123456\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000043\n		root=id000064\n	id000042=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000041\n		root=id000065\n	id000040=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000039\n		root=id000066\n	id000036=\n		event_before_input=id000019\n		username=_USER_NAME_\n		password=_PASSWORD_\n		name=_HOST_NAME_\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000037\n		root=id000067\ndir=\n	id000068=\n		father=id000067\n		name=system\n		content=\n			0=id000069\n			1=id000070\n			2=id000071\n			3=id000072\n			4=id000073\n		subdir=\n	id000074=\n		father=id000067\n		name=log\n		content=\n			0=id000075\n			1=id000076\n		subdir=\n	id000077=\n		father=id000067\n		name=data\n		content=\n			0=id000078\n			1=id000079\n			2=id000080\n			3=id000081\n			4=id000082\n			5=id000083\n		subdir=\n	id000084=\n		father=id000067\n		name=bin\n		content=\n			0=id000085\n			1=id000086\n			2=id000087\n			3=id000088\n			4=id000089\n			5=id000090\n			6=id000091\n			7=id000092\n			8=id000093\n			9=id000094\n			10=id000095\n			11=id000096\n		subdir=\n	id000067=\n		father=id000001\n		name=\n		content=\n			0=id000097\n		subdir=\n			0=id000068\n			1=id000074\n			2=id000077\n			3=id000084\n	id000098=\n		father=id000066\n		name=system\n		content=\n			0=id000099\n			1=id000100\n			2=id000101\n			3=id000102\n			4=id000103\n		subdir=\n	id000104=\n		father=id000066\n		name=log\n		content=\n		subdir=\n	id000105=\n		father=id000066\n		name=data\n		content=\n		subdir=\n	id000106=\n		father=id000066\n		name=bin\n		content=\n			0=id000107\n			1=id000108\n			2=id000109\n			3=id000110\n			4=id000111\n			5=id000112\n			6=id000113\n			7=id000114\n		subdir=\n	id000066=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000098\n			1=id000104\n			2=id000105\n			3=id000106\n	id000115=\n		father=id000065\n		name=system\n		content=\n			0=id000116\n			1=id000117\n			2=id000118\n			3=id000119\n			4=id000120\n		subdir=\n	id000121=\n		father=id000065\n		name=log\n		content=\n		subdir=\n	id000122=\n		father=id000065\n		name=data\n		content=\n		subdir=\n	id000123=\n		father=id000065\n		name=bin\n		content=\n			0=id000124\n			1=id000125\n			2=id000126\n			3=id000127\n			4=id000128\n			5=id000129\n			6=id000130\n			7=id000131\n		subdir=\n	id000065=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000115\n			1=id000121\n			2=id000122\n			3=id000123\n	id000132=\n		father=id000064\n		name=system\n		content=\n			0=id000133\n			1=id000134\n			2=id000135\n			3=id000136\n			4=id000137\n		subdir=\n	id000138=\n		father=id000064\n		name=log\n		content=\n			0=id000139\n		subdir=\n	id000140=\n		father=id000064\n		name=data\n		content=\n		subdir=\n	id000141=\n		father=id000064\n		name=bin\n		content=\n			0=id000142\n			1=id000143\n			2=id000144\n			3=id000145\n			4=id000146\n			5=id000147\n			6=id000148\n			7=id000149\n			8=id000150\n		subdir=\n	id000064=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000132\n			1=id000138\n			2=id000140\n			3=id000141\n	id000151=\n		father=id000063\n		name=system\n		content=\n			0=id000152\n			1=id000153\n			2=id000154\n			3=id000155\n			4=id000156\n		subdir=\n	id000157=\n		father=id000063\n		name=log\n		content=\n			0=id000158\n		subdir=\n	id000159=\n		father=id000063\n		name=data\n		content=\n		subdir=\n	id000160=\n		father=id000063\n		name=bin\n		content=\n			0=id000161\n			1=id000162\n			2=id000163\n			3=id000164\n			4=id000165\n			5=id000166\n			6=id000167\n			7=id000168\n		subdir=\n	id000169=\n		father=id000063\n		name=www\n		content=\n			0=id000170\n			1=id000171\n		subdir=\n	id000063=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000151\n			1=id000157\n			2=id000159\n			3=id000160\n			4=id000169\n	id000172=\n		father=id000062\n		name=system\n		content=\n			0=id000173\n			1=id000174\n			2=id000175\n			3=id000176\n			4=id000177\n		subdir=\n	id000178=\n		father=id000062\n		name=log\n		content=\n		subdir=\n	id000179=\n		father=id000062\n		name=data\n		content=\n		subdir=\n	id000180=\n		father=id000062\n		name=bin\n		content=\n			0=id000181\n			1=id000182\n			2=id000183\n			3=id000184\n			4=id000185\n			5=id000186\n			6=id000187\n			7=id000188\n			8=id000189\n		subdir=\n	id000062=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000172\n			1=id000178\n			2=id000179\n			3=id000180\n	id000190=\n		father=id000061\n		name=system\n		content=\n			0=id000191\n			1=id000192\n			2=id000193\n			3=id000194\n			4=id000195\n		subdir=\n	id000196=\n		father=id000061\n		name=log\n		content=\n		subdir=\n	id000197=\n		father=id000061\n		name=data\n		content=\n		subdir=\n	id000198=\n		father=id000061\n		name=bin\n		co") + "ntent=\n			0=id000199\n			1=id000200\n			2=id000201\n			3=id000202\n			4=id000203\n			5=id000204\n			6=id000205\n			7=id000206\n			8=id000207\n		subdir=\n	id000061=\n		father=id000208\n		name=\n		content=\n		subdir=\n			0=id000190\n			1=id000196\n			2=id000197\n			3=id000198\n	id000209=\n		father=id000060\n		name=system\n		content=\n			0=id000210\n			1=id000211\n			2=id000212\n			3=id000213\n			4=id000214\n		subdir=\n	id000215=\n		father=id000060\n		name=log\n		content=\n		subdir=\n	id000216=\n		father=id000060\n		name=data\n		content=\n			0=id000217\n		subdir=\n	id000218=\n		father=id000060\n		name=bin\n		content=\n			0=id000219\n			1=id000220\n			2=id000221\n			3=id000222\n			4=id000223\n			5=id000224\n			6=id000225\n			7=id000226\n		subdir=\n	id000060=\n		father=id000208\n		name=\n		content=\n		subdir=\n			0=id000209\n			1=id000215\n			2=id000216\n			3=id000218\n	id000227=\n		father=id000059\n		name=system\n		content=\n			0=id000228\n			1=id000229\n			2=id000230\n			3=id000231\n			4=id000232\n		subdir=\n	id000233=\n		father=id000059\n		name=log\n		content=\n		subdir=\n	id000234=\n		father=id000059\n		name=data\n		content=\n		subdir=\n	id000235=\n		father=id000059\n		name=bin\n		content=\n			0=id000236\n			1=id000237\n			2=id000238\n			3=id000239\n			4=id000240\n			5=id000241\n			6=id000242\n			7=id000243\n		subdir=\n	id000059=\n		father=id000208\n		name=\n		content=\n		subdir=\n			0=id000227\n			1=id000233\n			2=id000234\n			3=id000235\n	id000244=\n		father=id000058\n		name=system\n		content=\n			0=id000245\n			1=id000246\n			2=id000247\n			3=id000248\n			4=id000249\n		subdir=\n	id000250=\n		father=id000058\n		name=log\n		content=\n		subdir=\n	id000251=\n		father=id000058\n		name=data\n		content=\n		subdir=\n	id000252=\n		father=id000058\n		name=bin\n		content=\n			0=id000253\n			1=id000254\n			2=id000255\n			3=id000256\n			4=id000257\n			5=id000258\n			6=id000259\n			7=id000260\n		subdir=\n	id000058=\n		father=id000208\n		name=\n		content=\n		subdir=\n			0=id000244\n			1=id000250\n			2=id000251\n			3=id000252\nfile=\n	id000260=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000259=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000258=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000257=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000256=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000255=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000254=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000253=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000249=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000248=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000247=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000246=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000245=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000243=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000242=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000241=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000240=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000239=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000238=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000237=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000236=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000232=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000231=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000230=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000229=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000228=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000226=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000225=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000224=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000223=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000222=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000221=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000220=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000219=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000217=\n		name=code.cpp\n		is_exe=false\n		data=aW50IGV4ZV9pcGNvbmZpZyhpbnQgaSxjb25zdCBjaGFyICoqdCxDb21wdXRlciAqYykKewogICAgY291dDw8IuacrOWcsOe9kee7nOmFjee9ruWZqCI8PGVuZGw7CiAgICBjb3V0PDwiLS0tLS0tLS0tLS0tLS0tLS0tLS0tIjw8ZW5kbDsKICAgIGNvdXQ8PCJJUHY05Zyw5Z2AOiI8PGMtPm5ldG5vZGUtPmlwPDxlbmRsOwogICAgY291dDw8Ium7mOiupOe9keWFszoiPDxjLT5uZXRub2RlLT5mYXRoZXItPmlwPDxlbmRsOwogICAgaWYgKGMtPnBvcnRfbGlzdC5zaXplKCk+MCkA\n	id000214=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000213=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000212=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000211=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000210=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000207=\n		name=tracer.exe\n		is_exe=true\n		address=id000033\n	id000206=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000205=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000204=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000203=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000202=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000201=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000200=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000199=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000195=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000194=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000193=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000192=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000191=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000189=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000188=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000187=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000186=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000185=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000184=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000183=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000182=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000181=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000177=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000176=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000175=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000174=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000173=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000171=\n		name=sniffer.exe\n		is_exe=true\n		address=id000034\n	id000170=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000168=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000167=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000166=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000165=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000164=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000163=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000162=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000161=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000158=\n		name=www.log\n		is_exe=false\n		data=Wzk6MTc6MzFdMTkyLjE2OC4wLjAg6L+c56iL5ou36LSd5LqG5paH5Lu2IHBhc3NndWVzc2VyLmV4ZQpbOToyOTozNF0xOTIuMTY4LjAuMCDov5znqIvmi7fotJ3kuobmlofku7Ygc25pZmZlci5leGUKAA\\e\\e\n	id000156=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000155=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000154=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000153=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000152=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000150=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000149=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000148=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000147=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000146=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000145=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000144=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000143=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000142=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000139=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTU6M10xOTIuMTY4LjAuMCDov57mjqXkuobmnKzlnLDnu4jnq68KWzk6MTU6OF0xOTIuMTY4LjAuMCDnmbvlvZXmiJDlip8KWzk6MTU6NDNdMTkyLjE2OC4wLjAg6L+e5o6l5LqG5pys5Zyw57uI56uvCls5OjE1OjQ4XTE5Mi4xNjguMC4wIOeZu+W9leaIkOWKnwoA\n	id000137=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000136=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000135=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000134=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000133=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000131=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000130=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000129=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000128=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000127=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000126=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000125=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000124=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000120=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000119=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000118=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000117=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000116=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000114=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000113=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000112=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000111=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000110=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000109=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000108=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000107=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000103=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000102=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000101=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000100=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000099=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000097=\n		name=1.txt\n		is_exe=false\n		data=MTIzAA\\e\\e\n	id000096=\n		name=tracer.exe\n		is_exe=true\n		address=id000033\n	id000095=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000094=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000093=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000092=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000091=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000090=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000089=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000088=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000087=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000086=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000085=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000083=\n		name=code.cpp\n		is_exe=false\n		data=aW50IGV4ZV9pcGNvbmZpZyhpbnQgaSxjb25zdCBjaGFyICoqdCxDb21wdXRlciAqYykKewogICAgY291dDw8IuacrOWcsOe9kee7nOmFjee9ruWZqCI8PGVuZGw7CiAgICBjb3V0PDwiLS0tLS0tLS0tLS0tLS0tLS0tLS0tIjw8ZW5kbDsKICAgIGNvdXQ8PCJJUHY05Zyw5Z2AOiI8PGMtPm5ldG5vZGUtPmlwPDxlbmRsOwogICAgY291dDw8Ium7mOiupOe9keWFszoiPDxjLT5uZXRub2RlLT5mYXRoZXItPmlwPDxlbmRsOwogICAgaWYgKGMtPnBvcnRfbGlzdC5zaXplKCk+MCkA\n	id000082=\n		name=sniffer.exe\n		is_exe=true\n		address=id000034\n	id000081=\n		name=tracer.exe\n		is_exe=true\n		address=id000033\n	id000080=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000079=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000078=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000076=\n		name=telnet.log\n		is_exe=false\n		data=Wzk6MTU6M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToxNTo0M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToyMToxNl3miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoxMTcuNTEuMTQzLjEyMDoyMwpbOToyNjoxOF3miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoxODAuMjEuMTAuMjIwOjI1NgpbOTo0MjoyMl3miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyNC4xMjAuNDEuMDo5MQpbOTo0NjowXeaIkOWKn+i/nuaOpei/nOeoi+iuoeeul+acujI0LjEyMC40MS4xMDoyMwoA\n	id000075=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTQ6OV3mnKzlnLDmjqfliLblj7Ag6L+e5o6l5LqG5pys5Zyw57uI56uvCgA\\e\n	id000073=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000072=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000071=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000070=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000069=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n\n";
string saving_task1 = "version_code=4\npreload=\n	id000001=NULL\n	id000002=task_1_1\n	id000003=task_1_2\n	id000004=task_2_1\n	id000005=task_2_2\n	id000006=task_3_1\n	id000007=task_3_2\n	id000008=task_4_1\n	id000009=task_4_2\n	id000010=task_5_1\n	id000011=task_5_2\n	id000012=task_6_1\n	id000013=task_6_2\n	id000014=task_7_1\n	id000015=task_7_2\n	id000016=task_7_3\n	id000017=task_8_1\n	id000018=task_8_2\n	id000019=task_9_1\n	id000020=task_9_2\n	id000021=exe_wget\n	id000022=exe_help\n	id000023=exe_passguesser\n	id000024=exe_portscan\n	id000025=exe_hvm\n	id000026=exe_mail\n	id000027=exe_telnet\n	id000028=exe_tree\n	id000029=exe_ipconfig\n	id000030=exe_ping\n	id000031=exe_savegame\n	id000032=exe_myexe\n	id000033=exe_tracer\n	id000034=exe_sniffer\nglobal=\n	task_number=0\n	sub_task_number=0\n	Internet=id000035\n	localhost=id000036\n	default_filename=temp\n	autosave=true\nnet_node=\n	id000037=\n		ip=192.168.0.0\n		binded_computer=id000036\n		father=id000038\n		net_map=\n		subnet=\n	id000039=\n		ip=192.168.0.1\n		binded_computer=id000040\n		father=id000038\n		net_map=\n		subnet=\n	id000041=\n		ip=192.168.0.2\n		binded_computer=id000042\n		father=id000038\n		net_map=\n		subnet=\n	id000038=\n		ip=188.131.133.248\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=192.168.0.0$id000037\n			1=192.168.0.1$id000039\n			2=192.168.0.2$id000041\n		subnet=\n			0=id000037\n			1=id000039\n			2=id000041\n	id000043=\n		ip=29.53.103.3\n		binded_computer=id000044\n		father=id000035\n		net_map=\n		subnet=\n	id000045=\n		ip=178.53.100.24\n		binded_computer=id000046\n		father=id000035\n		net_map=\n		subnet=\n	id000047=\n		ip=117.51.143.120\n		binded_computer=id000048\n		father=id000035\n		net_map=\n		subnet=\n	id000035=\n		ip=Internet\n		binded_computer=id000001\n		father=id000001\n		net_map=\n			0=117.51.143.120$id000047\n			1=178.53.100.24$id000045\n			2=188.131.133.248$id000038\n			3=29.53.103.3$id000043\n		subnet=\n			0=id000038\n			1=id000043\n			2=id000045\n			3=id000047\ncomputer=\n	id000048=\n		event_before_input=id000001\n		username=root\n		password=password123\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000047\n		root=id000049\n	id000046=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=www$21\n		netnode=id000045\n		root=id000050\n	id000044=\n		event_before_input=id000001\n		username=root\n		password=123456\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000043\n		root=id000051\n	id000042=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000041\n		root=id000052\n	id000040=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000039\n		root=id000053\n	id000036=\n		event_before_input=id000002\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=localhost\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000037\n		root=id000054\ndir=\n	id000055=\n		father=id000054\n		name=system\n		content=\n			0=id000056\n			1=id000057\n			2=id000058\n			3=id000059\n			4=id000060\n		subdir=\n	id000061=\n		father=id000054\n		name=log\n		content=\n			0=id000062\n		subdir=\n	id000063=\n		father=id000054\n		name=data\n		content=\n		subdir=\n	id000064=\n		father=id000054\n		name=bin\n		content=\n			0=id000065\n			1=id000066\n			2=id000067\n			3=id000068\n			4=id000069\n			5=id000070\n			6=id000071\n			7=id000072\n		subdir=\n	id000054=\n		father=id000001\n		name=\n		content=\n			0=id000073\n		subdir=\n			0=id000055\n			1=id000061\n			2=id000063\n			3=id000064\n	id000074=\n		father=id000053\n		name=system\n		content=\n			0=id000075\n			1=id000076\n			2=id000077\n			3=id000078\n			4=id000079\n		subdir=\n	id000080=\n		father=id000053\n		name=log\n		content=\n		subdir=\n	id000081=\n		father=id000053\n		name=data\n		content=\n		subdir=\n	id000082=\n		father=id000053\n		name=bin\n		content=\n			0=id000083\n			1=id000084\n			2=id000085\n			3=id000086\n			4=id000087\n			5=id000088\n			6=id000089\n			7=id000090\n		subdir=\n	id000053=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000074\n			1=id000080\n			2=id000081\n			3=id000082\n	id000091=\n		father=id000052\n		name=system\n		content=\n			0=id000092\n			1=id000093\n			2=id000094\n			3=id000095\n			4=id000096\n		subdir=\n	id000097=\n		father=id000052\n		name=log\n		content=\n		subdir=\n	id000098=\n		father=id000052\n		name=data\n		content=\n		subdir=\n	id000099=\n		father=id000052\n		name=bin\n		content=\n			0=id000100\n			1=id000101\n			2=id000102\n			3=id000103\n			4=id000104\n			5=id000105\n			6=id000106\n			7=id000107\n		subdir=\n	id000052=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000091\n			1=id000097\n			2=id000098\n			3=id000099\n	id000108=\n		father=id000051\n		name=system\n		content=\n			0=id000109\n			1=id000110\n			2=id000111\n			3=id000112\n			4=id000113\n		subdir=\n	id000114=\n		father=id000051\n		name=log\n		content=\n		subdir=\n	id000115=\n		father=id000051\n		name=data\n		content=\n		subdir=\n	id000116=\n		father=id000051\n		name=bin\n		content=\n			0=id000117\n			1=id000118\n			2=id000119\n			3=id000120\n			4=id000121\n			5=id000122\n			6=id000123\n			7=id000124\n			8=id000125\n		subdir=\n	id000051=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000108\n			1=id000114\n			2=id000115\n			3=id000116\n	id000126=\n		father=id000050\n		name=system\n		content=\n			0=id000127\n			1=id000128\n			2=id000129\n			3=id000130\n			4=id000131\n		subdir=\n	id000132=\n		father=id000050\n		name=log\n		content=\n		subdir=\n	id000133=\n		father=id000050\n		name=data\n		content=\n		subdir=\n	id000134=\n		father=id000050\n		name=bin\n		content=\n			0=id000135\n			1=id000136\n			2=id000137\n			3=id000138\n			4=id000139\n			5=id000140\n			6=id000141\n			7=id000142\n		subdir=\n	id000143=\n		father=id000050\n		name=www\n		content=\n			0=id000144\n		subdir=\n	id000050=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000126\n			1=id000132\n			2=id000133\n			3=id000134\n			4=id000143\n	id000145=\n		father=id000049\n		name=system\n		content=\n			0=id000146\n			1=id000147\n			2=id000148\n			3=id000149\n			4=id000150\n		subdir=\n	id000151=\n		father=id000049\n		name=log\n		content=\n		subdir=\n	id000152=\n		father=id000049\n		name=data\n		content=\n		subdir=\n	id000153=\n		father=id000049\n		name=bin\n		content=\n			0=id000154\n			1=id000155\n			2=id000156\n			3=id000157\n			4=id000158\n			5=id000159\n			6=id000160\n			7=id000161\n			8=id000162\n		subdir=\n	id000049=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000145\n			1=id000151\n			2=id000152\n			3=id000153\nfile=\n	id000162=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000161=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000160=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000159=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000158=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000157=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000156=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000155=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000154=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000150=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000149=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000148=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000147=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000146=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000144=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000142=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000141=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000140=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000139=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000138=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000137=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000136=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000135=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000131=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000130=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000129=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000128=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000127=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000125=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000124=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000123=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000122=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000121=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000120=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000119=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000118=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000117=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000113=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000112=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000111=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000110=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000109=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000107=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000106=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000105=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000104=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000103=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000102=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000101=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000100=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000096=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000095=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000094=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000093=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000092=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000090=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000089=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000088=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000087=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000086=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000085=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000084=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000083=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000079=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000078=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000077=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000076=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000075=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000073=\n		name=1.txt\n		is_exe=false\n		data=MTIzAA\\e\\e\n	id000072=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000071=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000070=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000069=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000068=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000067=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000066=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000065=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000062=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTQ6OV3mnKzlnLDmjqfliLblj7Ag6L+e5o6l5LqG5pys5Zyw57uI56uvCgA\\e\n	id000060=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000059=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000058=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000057=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000056=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n\n";
string saving_task2 = "version_code=4\npreload=\n	id000001=NULL\n	id000002=task_1_1\n	id000003=task_1_2\n	id000004=task_2_1\n	id000005=task_2_2\n	id000006=task_3_1\n	id000007=task_3_2\n	id000008=task_4_1\n	id000009=task_4_2\n	id000010=task_5_1\n	id000011=task_5_2\n	id000012=task_6_1\n	id000013=task_6_2\n	id000014=task_7_1\n	id000015=task_7_2\n	id000016=task_7_3\n	id000017=task_8_1\n	id000018=task_8_2\n	id000019=task_9_1\n	id000020=task_9_2\n	id000021=exe_wget\n	id000022=exe_help\n	id000023=exe_passguesser\n	id000024=exe_portscan\n	id000025=exe_hvm\n	id000026=exe_mail\n	id000027=exe_telnet\n	id000028=exe_tree\n	id000029=exe_ipconfig\n	id000030=exe_ping\n	id000031=exe_savegame\n	id000032=exe_myexe\n	id000033=exe_tracer\n	id000034=exe_sniffer\nglobal=\n	task_number=1\n	sub_task_number=1\n	Internet=id000035\n	localhost=id000036\n	default_filename=temp\n	autosave=true\nnet_node=\n	id000037=\n		ip=192.168.0.0\n		binded_computer=id000036\n		father=id000038\n		net_map=\n		subnet=\n	id000039=\n		ip=192.168.0.1\n		binded_computer=id000040\n		father=id000038\n		net_map=\n		subnet=\n	id000041=\n		ip=192.168.0.2\n		binded_computer=id000042\n		father=id000038\n		net_map=\n		subnet=\n	id000038=\n		ip=188.131.133.248\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=192.168.0.0$id000037\n			1=192.168.0.1$id000039\n			2=192.168.0.2$id000041\n		subnet=\n			0=id000037\n			1=id000039\n			2=id000041\n	id000043=\n		ip=29.53.103.3\n		binded_computer=id000044\n		father=id000035\n		net_map=\n		subnet=\n	id000045=\n		ip=178.53.100.24\n		binded_computer=id000046\n		father=id000035\n		net_map=\n		subnet=\n	id000047=\n		ip=117.51.143.120\n		binded_computer=id000048\n		father=id000035\n		net_map=\n		subnet=\n	id000035=\n		ip=Internet\n		binded_computer=id000001\n		father=id000001\n		net_map=\n			0=117.51.143.120$id000047\n			1=178.53.100.24$id000045\n			2=188.131.133.248$id000038\n			3=29.53.103.3$id000043\n		subnet=\n			0=id000038\n			1=id000043\n			2=id000045\n			3=id000047\ncomputer=\n	id000048=\n		event_before_input=id000001\n		username=root\n		password=password123\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000047\n		root=id000049\n	id000046=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=www$21\n		netnode=id000045\n		root=id000050\n	id000044=\n		event_before_input=id000001\n		username=root\n		password=123456\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000043\n		root=id000051\n	id000042=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000041\n		root=id000052\n	id000040=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000039\n		root=id000053\n	id000036=\n		event_before_input=id000004\n		username=_USER_NAME_\n		password=_PASSWORD_\n		name=_HOST_NAME_\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000037\n		root=id000054\ndir=\n	id000055=\n		father=id000054\n		name=system\n		content=\n			0=id000056\n			1=id000057\n			2=id000058\n			3=id000059\n			4=id000060\n		subdir=\n	id000061=\n		father=id000054\n		name=log\n		content=\n			0=id000062\n			1=id000063\n		subdir=\n	id000064=\n		father=id000054\n		name=data\n		content=\n		subdir=\n	id000065=\n		father=id000054\n		name=bin\n		content=\n			0=id000066\n			1=id000067\n			2=id000068\n			3=id000069\n			4=id000070\n			5=id000071\n			6=id000072\n			7=id000073\n		subdir=\n	id000054=\n		father=id000001\n		name=\n		content=\n			0=id000074\n		subdir=\n			0=id000055\n			1=id000061\n			2=id000064\n			3=id000065\n	id000075=\n		father=id000053\n		name=system\n		content=\n			0=id000076\n			1=id000077\n			2=id000078\n			3=id000079\n			4=id000080\n		subdir=\n	id000081=\n		father=id000053\n		name=log\n		content=\n		subdir=\n	id000082=\n		father=id000053\n		name=data\n		content=\n		subdir=\n	id000083=\n		father=id000053\n		name=bin\n		content=\n			0=id000084\n			1=id000085\n			2=id000086\n			3=id000087\n			4=id000088\n			5=id000089\n			6=id000090\n			7=id000091\n		subdir=\n	id000053=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000075\n			1=id000081\n			2=id000082\n			3=id000083\n	id000092=\n		father=id000052\n		name=system\n		content=\n			0=id000093\n			1=id000094\n			2=id000095\n			3=id000096\n			4=id000097\n		subdir=\n	id000098=\n		father=id000052\n		name=log\n		content=\n		subdir=\n	id000099=\n		father=id000052\n		name=data\n		content=\n		subdir=\n	id000100=\n		father=id000052\n		name=bin\n		content=\n			0=id000101\n			1=id000102\n			2=id000103\n			3=id000104\n			4=id000105\n			5=id000106\n			6=id000107\n			7=id000108\n		subdir=\n	id000052=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000092\n			1=id000098\n			2=id000099\n			3=id000100\n	id000109=\n		father=id000051\n		name=system\n		content=\n			0=id000110\n			1=id000111\n			2=id000112\n			3=id000113\n			4=id000114\n		subdir=\n	id000115=\n		father=id000051\n		name=log\n		content=\n			0=id000116\n		subdir=\n	id000117=\n		father=id000051\n		name=data\n		content=\n		subdir=\n	id000118=\n		father=id000051\n		name=bin\n		content=\n			0=id000119\n			1=id000120\n			2=id000121\n			3=id000122\n			4=id000123\n			5=id000124\n			6=id000125\n			7=id000126\n			8=id000127\n		subdir=\n	id000051=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000109\n			1=id000115\n			2=id000117\n			3=id000118\n	id000128=\n		father=id000050\n		name=system\n		content=\n			0=id000129\n			1=id000130\n			2=id000131\n			3=id000132\n			4=id000133\n		subdir=\n	id000134=\n		father=id000050\n		name=log\n		content=\n		subdir=\n	id000135=\n		father=id000050\n		name=data\n		content=\n		subdir=\n	id000136=\n		father=id000050\n		name=bin\n		content=\n			0=id000137\n			1=id000138\n			2=id000139\n			3=id000140\n			4=id000141\n			5=id000142\n			6=id000143\n			7=id000144\n		subdir=\n	id000145=\n		father=id000050\n		name=www\n		content=\n			0=id000146\n		subdir=\n	id000050=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000128\n			1=id000134\n			2=id000135\n			3=id000136\n			4=id000145\n	id000147=\n		father=id000049\n		name=system\n		content=\n			0=id000148\n			1=id000149\n			2=id000150\n			3=id000151\n			4=id000152\n		subdir=\n	id000153=\n		father=id000049\n		name=log\n		content=\n		subdir=\n	id000154=\n		father=id000049\n		name=data\n		content=\n		subdir=\n	id000155=\n		father=id000049\n		name=bin\n		content=\n			0=id000156\n			1=id000157\n			2=id000158\n			3=id000159\n			4=id000160\n			5=id000161\n			6=id000162\n			7=id000163\n			8=id000164\n		subdir=\n	id000049=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000147\n			1=id000153\n			2=id000154\n			3=id000155\nfile=\n	id000164=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000163=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000162=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000161=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000160=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000159=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000158=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000157=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000156=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000152=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000151=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000150=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000149=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000148=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000146=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000144=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000143=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000142=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000141=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000140=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000139=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000138=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000137=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000133=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000132=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000131=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000130=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000129=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000127=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000126=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000125=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000124=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000123=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000122=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000121=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000120=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000119=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000116=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTU6M10xOTIuMTY4LjAuMCDov57mjqXkuobmnKzlnLDnu4jnq68KWzk6MTU6OF0xOTIuMTY4LjAuMCDnmbvlvZXmiJDlip8KAA\\e\\e\n	id000114=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000113=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000112=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000111=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000110=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000108=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000107=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000106=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000105=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000104=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000103=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000102=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000101=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000097=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000096=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000095=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000094=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000093=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000091=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000090=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000089=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000088=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000087=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000086=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000085=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000084=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000080=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000079=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000078=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000077=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000076=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000074=\n		name=1.txt\n		is_exe=false\n		data=MTIzAA\\e\\e\n	id000073=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000072=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000071=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000070=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000069=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000068=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000067=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000066=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000063=\n		name=telnet.log\n		is_exe=false\n		data=Wzk6MTU6M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwoA\n	id000062=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTQ6OV3mnKzlnLDmjqfliLblj7Ag6L+e5o6l5LqG5pys5Zyw57uI56uvCgA\\e\n	id000060=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000059=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000058=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000057=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000056=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n\n";
string saving_task3 = "version_code=4\npreload=\n	id000001=NULL\n	id000002=task_1_1\n	id000003=task_1_2\n	id000004=task_2_1\n	id000005=task_2_2\n	id000006=task_3_1\n	id000007=task_3_2\n	id000008=task_4_1\n	id000009=task_4_2\n	id000010=task_5_1\n	id000011=task_5_2\n	id000012=task_6_1\n	id000013=task_6_2\n	id000014=task_7_1\n	id000015=task_7_2\n	id000016=task_7_3\n	id000017=task_8_1\n	id000018=task_8_2\n	id000019=task_9_1\n	id000020=task_9_2\n	id000021=exe_wget\n	id000022=exe_help\n	id000023=exe_passguesser\n	id000024=exe_portscan\n	id000025=exe_hvm\n	id000026=exe_mail\n	id000027=exe_telnet\n	id000028=exe_tree\n	id000029=exe_ipconfig\n	id000030=exe_ping\n	id000031=exe_savegame\n	id000032=exe_myexe\n	id000033=exe_tracer\n	id000034=exe_sniffer\nglobal=\n	task_number=2\n	sub_task_number=1\n	Internet=id000035\n	localhost=id000036\n	default_filename=temp\n	autosave=true\nnet_node=\n	id000037=\n		ip=192.168.0.0\n		binded_computer=id000036\n		father=id000038\n		net_map=\n		subnet=\n	id000039=\n		ip=192.168.0.1\n		binded_computer=id000040\n		father=id000038\n		net_map=\n		subnet=\n	id000041=\n		ip=192.168.0.2\n		binded_computer=id000042\n		father=id000038\n		net_map=\n		subnet=\n	id000038=\n		ip=188.131.133.248\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=192.168.0.0$id000037\n			1=192.168.0.1$id000039\n			2=192.168.0.2$id000041\n		subnet=\n			0=id000037\n			1=id000039\n			2=id000041\n	id000043=\n		ip=29.53.103.3\n		binded_computer=id000044\n		father=id000035\n		net_map=\n		subnet=\n	id000045=\n		ip=178.53.100.24\n		binded_computer=id000046\n		father=id000035\n		net_map=\n		subnet=\n	id000047=\n		ip=117.51.143.120\n		binded_computer=id000048\n		father=id000035\n		net_map=\n		subnet=\n	id000035=\n		ip=Internet\n		binded_computer=id000001\n		father=id000001\n		net_map=\n			0=117.51.143.120$id000047\n			1=178.53.100.24$id000045\n			2=188.131.133.248$id000038\n			3=29.53.103.3$id000043\n		subnet=\n			0=id000038\n			1=id000043\n			2=id000045\n			3=id000047\ncomputer=\n	id000048=\n		event_before_input=id000001\n		username=root\n		password=password123\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000047\n		root=id000049\n	id000046=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=www$21\n		netnode=id000045\n		root=id000050\n	id000044=\n		event_before_input=id000001\n		username=root\n		password=123456\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000043\n		root=id000051\n	id000042=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000041\n		root=id000052\n	id000040=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000039\n		root=id000053\n	id000036=\n		event_before_input=id000005\n		username=_USER_NAME_\n		password=_PASSWORD_\n		name=_HOST_NAME_\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000037\n		root=id000054\ndir=\n	id000055=\n		father=id000054\n		name=system\n		content=\n			0=id000056\n			1=id000057\n			2=id000058\n			3=id000059\n			4=id000060\n		subdir=\n	id000061=\n		father=id000054\n		name=log\n		content=\n			0=id000062\n			1=id000063\n		subdir=\n	id000064=\n		father=id000054\n		name=data\n		content=\n			0=id000065\n		subdir=\n	id000066=\n		father=id000054\n		name=bin\n		content=\n			0=id000067\n			1=id000068\n			2=id000069\n			3=id000070\n			4=id000071\n			5=id000072\n			6=id000073\n			7=id000074\n		subdir=\n	id000054=\n		father=id000001\n		name=\n		content=\n			0=id000075\n		subdir=\n			0=id000055\n			1=id000061\n			2=id000064\n			3=id000066\n	id000076=\n		father=id000053\n		name=system\n		content=\n			0=id000077\n			1=id000078\n			2=id000079\n			3=id000080\n			4=id000081\n		subdir=\n	id000082=\n		father=id000053\n		name=log\n		content=\n		subdir=\n	id000083=\n		father=id000053\n		name=data\n		content=\n		subdir=\n	id000084=\n		father=id000053\n		name=bin\n		content=\n			0=id000085\n			1=id000086\n			2=id000087\n			3=id000088\n			4=id000089\n			5=id000090\n			6=id000091\n			7=id000092\n		subdir=\n	id000053=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000076\n			1=id000082\n			2=id000083\n			3=id000084\n	id000093=\n		father=id000052\n		name=system\n		content=\n			0=id000094\n			1=id000095\n			2=id000096\n			3=id000097\n			4=id000098\n		subdir=\n	id000099=\n		father=id000052\n		name=log\n		content=\n		subdir=\n	id000100=\n		father=id000052\n		name=data\n		content=\n		subdir=\n	id000101=\n		father=id000052\n		name=bin\n		content=\n			0=id000102\n			1=id000103\n			2=id000104\n			3=id000105\n			4=id000106\n			5=id000107\n			6=id000108\n			7=id000109\n		subdir=\n	id000052=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000093\n			1=id000099\n			2=id000100\n			3=id000101\n	id000110=\n		father=id000051\n		name=system\n		content=\n			0=id000111\n			1=id000112\n			2=id000113\n			3=id000114\n			4=id000115\n		subdir=\n	id000116=\n		father=id000051\n		name=log\n		content=\n			0=id000117\n		subdir=\n	id000118=\n		father=id000051\n		name=data\n		content=\n		subdir=\n	id000119=\n		father=id000051\n		name=bin\n		content=\n			0=id000120\n			1=id000121\n			2=id000122\n			3=id000123\n			4=id000124\n			5=id000125\n			6=id000126\n			7=id000127\n			8=id000128\n		subdir=\n	id000051=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000110\n			1=id000116\n			2=id000118\n			3=id000119\n	id000129=\n		father=id000050\n		name=system\n		content=\n			0=id000130\n			1=id000131\n			2=id000132\n			3=id000133\n			4=id000134\n		subdir=\n	id000135=\n		father=id000050\n		name=log\n		content=\n		subdir=\n	id000136=\n		father=id000050\n		name=data\n		content=\n		subdir=\n	id000137=\n		father=id000050\n		name=bin\n		content=\n			0=id000138\n			1=id000139\n			2=id000140\n			3=id000141\n			4=id000142\n			5=id000143\n			6=id000144\n			7=id000145\n		subdir=\n	id000146=\n		father=id000050\n		name=www\n		content=\n			0=id000147\n		subdir=\n	id000050=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000129\n			1=id000135\n			2=id000136\n			3=id000137\n			4=id000146\n	id000148=\n		father=id000049\n		name=system\n		content=\n			0=id000149\n			1=id000150\n			2=id000151\n			3=id000152\n			4=id000153\n		subdir=\n	id000154=\n		father=id000049\n		name=log\n		content=\n		subdir=\n	id000155=\n		father=id000049\n		name=data\n		content=\n		subdir=\n	id000156=\n		father=id000049\n		name=bin\n		content=\n			0=id000157\n			1=id000158\n			2=id000159\n			3=id000160\n			4=id000161\n			5=id000162\n			6=id000163\n			7=id000164\n			8=id000165\n		subdir=\n	id000049=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000148\n			1=id000154\n			2=id000155\n			3=id000156\nfile=\n	id000165=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000164=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000163=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000162=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000161=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000160=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000159=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000158=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000157=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000153=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000152=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000151=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000150=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000149=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000147=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000145=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000144=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000143=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000142=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000141=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000140=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000139=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000138=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000134=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000133=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000132=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000131=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000130=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000128=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000127=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000126=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000125=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000124=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000123=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000122=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000121=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000120=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000117=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTU6M10xOTIuMTY4LjAuMCDov57mjqXkuobmnKzlnLDnu4jnq68KWzk6MTU6OF0xOTIuMTY4LjAuMCDnmbvlvZXmiJDlip8KWzk6MTU6NDNdMTkyLjE2OC4wLjAg6L+e5o6l5LqG5pys5Zyw57uI56uvCls5OjE1OjQ4XTE5Mi4xNjguMC4wIOeZu+W9leaIkOWKnwoA\n	id000115=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000114=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000113=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000112=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000111=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000109=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000108=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000107=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000106=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000105=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000104=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000103=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000102=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000098=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000097=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000096=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000095=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000094=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000092=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000091=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000090=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000089=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000088=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000087=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000086=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000085=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000081=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000080=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000079=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000078=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000077=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000075=\n		name=1.txt\n		is_exe=false\n		data=MTIzAA\\e\\e\n	id000074=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000073=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000072=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000071=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000070=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000069=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000068=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000067=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000065=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000063=\n		name=telnet.log\n		is_exe=false\n		data=Wzk6MTU6M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToxNTo0M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwoA\n	id000062=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTQ6OV3mnKzlnLDmjqfliLblj7Ag6L+e5o6l5LqG5pys5Zyw57uI56uvCgA\\e\n	id000060=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000059=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000058=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000057=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000056=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n\n";
string saving_task4 = "version_code=4\npreload=\n	id000001=NULL\n	id000002=task_1_1\n	id000003=task_1_2\n	id000004=task_2_1\n	id000005=task_2_2\n	id000006=task_3_1\n	id000007=task_3_2\n	id000008=task_4_1\n	id000009=task_4_2\n	id000010=task_5_1\n	id000011=task_5_2\n	id000012=task_6_1\n	id000013=task_6_2\n	id000014=task_7_1\n	id000015=task_7_2\n	id000016=task_7_3\n	id000017=task_8_1\n	id000018=task_8_2\n	id000019=task_9_1\n	id000020=task_9_2\n	id000021=exe_wget\n	id000022=exe_help\n	id000023=exe_passguesser\n	id000024=exe_portscan\n	id000025=exe_hvm\n	id000026=exe_mail\n	id000027=exe_telnet\n	id000028=exe_tree\n	id000029=exe_ipconfig\n	id000030=exe_ping\n	id000031=exe_savegame\n	id000032=exe_myexe\n	id000033=exe_tracer\n	id000034=exe_sniffer\nglobal=\n	task_number=3\n	sub_task_number=1\n	Internet=id000035\n	localhost=id000036\n	default_filename=temp\n	autosave=true\nnet_node=\n	id000037=\n		ip=192.168.0.0\n		binded_computer=id000036\n		father=id000038\n		net_map=\n		subnet=\n	id000039=\n		ip=192.168.0.1\n		binded_computer=id000040\n		father=id000038\n		net_map=\n		subnet=\n	id000041=\n		ip=192.168.0.2\n		binded_computer=id000042\n		father=id000038\n		net_map=\n		subnet=\n	id000038=\n		ip=188.131.133.248\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=192.168.0.0$id000037\n			1=192.168.0.1$id000039\n			2=192.168.0.2$id000041\n		subnet=\n			0=id000037\n			1=id000039\n			2=id000041\n	id000043=\n		ip=29.53.103.3\n		binded_computer=id000044\n		father=id000035\n		net_map=\n		subnet=\n	id000045=\n		ip=178.53.100.24\n		binded_computer=id000046\n		father=id000035\n		net_map=\n		subnet=\n	id000047=\n		ip=117.51.143.120\n		binded_computer=id000048\n		father=id000035\n		net_map=\n		subnet=\n	id000035=\n		ip=Internet\n		binded_computer=id000001\n		father=id000001\n		net_map=\n			0=117.51.143.120$id000047\n			1=178.53.100.24$id000045\n			2=188.131.133.248$id000038\n			3=29.53.103.3$id000043\n		subnet=\n			0=id000038\n			1=id000043\n			2=id000045\n			3=id000047\ncomputer=\n	id000048=\n		event_before_input=id000001\n		username=root\n		password=password123\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000047\n		root=id000049\n	id000046=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=www$21\n		netnode=id000045\n		root=id000050\n	id000044=\n		event_before_input=id000001\n		username=root\n		password=123456\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000043\n		root=id000051\n	id000042=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000041\n		root=id000052\n	id000040=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000039\n		root=id000053\n	id000036=\n		event_before_input=id000008\n		username=_USER_NAME_\n		password=_PASSWORD_\n		name=_HOST_NAME_\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000037\n		root=id000054\ndir=\n	id000055=\n		father=id000054\n		name=system\n		content=\n			0=id000056\n			1=id000057\n			2=id000058\n			3=id000059\n			4=id000060\n		subdir=\n	id000061=\n		father=id000054\n		name=log\n		content=\n			0=id000062\n			1=id000063\n		subdir=\n	id000064=\n		father=id000054\n		name=data\n		content=\n			0=id000065\n		subdir=\n	id000066=\n		father=id000054\n		name=bin\n		content=\n			0=id000067\n			1=id000068\n			2=id000069\n			3=id000070\n			4=id000071\n			5=id000072\n			6=id000073\n			7=id000074\n			8=id000075\n		subdir=\n	id000054=\n		father=id000001\n		name=\n		content=\n			0=id000076\n		subdir=\n			0=id000055\n			1=id000061\n			2=id000064\n			3=id000066\n	id000077=\n		father=id000053\n		name=system\n		content=\n			0=id000078\n			1=id000079\n			2=id000080\n			3=id000081\n			4=id000082\n		subdir=\n	id000083=\n		father=id000053\n		name=log\n		content=\n		subdir=\n	id000084=\n		father=id000053\n		name=data\n		content=\n		subdir=\n	id000085=\n		father=id000053\n		name=bin\n		content=\n			0=id000086\n			1=id000087\n			2=id000088\n			3=id000089\n			4=id000090\n			5=id000091\n			6=id000092\n			7=id000093\n		subdir=\n	id000053=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000077\n			1=id000083\n			2=id000084\n			3=id000085\n	id000094=\n		father=id000052\n		name=system\n		content=\n			0=id000095\n			1=id000096\n			2=id000097\n			3=id000098\n			4=id000099\n		subdir=\n	id000100=\n		father=id000052\n		name=log\n		content=\n		subdir=\n	id000101=\n		father=id000052\n		name=data\n		content=\n		subdir=\n	id000102=\n		father=id000052\n		name=bin\n		content=\n			0=id000103\n			1=id000104\n			2=id000105\n			3=id000106\n			4=id000107\n			5=id000108\n			6=id000109\n			7=id000110\n		subdir=\n	id000052=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000094\n			1=id000100\n			2=id000101\n			3=id000102\n	id000111=\n		father=id000051\n		name=system\n		content=\n			0=id000112\n			1=id000113\n			2=id000114\n			3=id000115\n			4=id000116\n		subdir=\n	id000117=\n		father=id000051\n		name=log\n		content=\n			0=id000118\n		subdir=\n	id000119=\n		father=id000051\n		name=data\n		content=\n		subdir=\n	id000120=\n		father=id000051\n		name=bin\n		content=\n			0=id000121\n			1=id000122\n			2=id000123\n			3=id000124\n			4=id000125\n			5=id000126\n			6=id000127\n			7=id000128\n			8=id000129\n		subdir=\n	id000051=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000111\n			1=id000117\n			2=id000119\n			3=id000120\n	id000130=\n		father=id000050\n		name=system\n		content=\n			0=id000131\n			1=id000132\n			2=id000133\n			3=id000134\n			4=id000135\n		subdir=\n	id000136=\n		father=id000050\n		name=log\n		content=\n		subdir=\n	id000137=\n		father=id000050\n		name=data\n		content=\n		subdir=\n	id000138=\n		father=id000050\n		name=bin\n		content=\n			0=id000139\n			1=id000140\n			2=id000141\n			3=id000142\n			4=id000143\n			5=id000144\n			6=id000145\n			7=id000146\n		subdir=\n	id000147=\n		father=id000050\n		name=www\n		content=\n			0=id000148\n		subdir=\n	id000050=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000130\n			1=id000136\n			2=id000137\n			3=id000138\n			4=id000147\n	id000149=\n		father=id000049\n		name=system\n		content=\n			0=id000150\n			1=id000151\n			2=id000152\n			3=id000153\n			4=id000154\n		subdir=\n	id000155=\n		father=id000049\n		name=log\n		content=\n		subdir=\n	id000156=\n		father=id000049\n		name=data\n		content=\n		subdir=\n	id000157=\n		father=id000049\n		name=bin\n		content=\n			0=id000158\n			1=id000159\n			2=id000160\n			3=id000161\n			4=id000162\n			5=id000163\n			6=id000164\n			7=id000165\n			8=id000166\n		subdir=\n	id000049=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000149\n			1=id000155\n			2=id000156\n			3=id000157\nfile=\n	id000166=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000165=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000164=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000163=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000162=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000161=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000160=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000159=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000158=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000154=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000153=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000152=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000151=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000150=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000148=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000146=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000145=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000144=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000143=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000142=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000141=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000140=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000139=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000135=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000134=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000133=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000132=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000131=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000129=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000128=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000127=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000126=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000125=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000124=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000123=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000122=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000121=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000118=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTU6M10xOTIuMTY4LjAuMCDov57mjqXkuobmnKzlnLDnu4jnq68KWzk6MTU6OF0xOTIuMTY4LjAuMCDnmbvlvZXmiJDlip8KWzk6MTU6NDNdMTkyLjE2OC4wLjAg6L+e5o6l5LqG5pys5Zyw57uI56uvCls5OjE1OjQ4XTE5Mi4xNjguMC4wIOeZu+W9leaIkOWKnwoA\n	id000116=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000115=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000114=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000113=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000112=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000110=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000109=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000108=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000107=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000106=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000105=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000104=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000103=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000099=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000098=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000097=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000096=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000095=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000093=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000092=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000091=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000090=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000089=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000088=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000087=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000086=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000082=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000081=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000080=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000079=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000078=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000076=\n		name=1.txt\n		is_exe=false\n		data=MTIzAA\\e\\e\n	id000075=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000074=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000073=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000072=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000071=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000070=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000069=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000068=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000067=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000065=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000063=\n		name=telnet.log\n		is_exe=false\n		data=Wzk6MTU6M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToxNTo0M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwoA\n	id000062=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTQ6OV3mnKzlnLDmjqfliLblj7Ag6L+e5o6l5LqG5pys5Zyw57uI56uvCgA\\e\n	id000060=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000059=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000058=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000057=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000056=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n\n";
string saving_task5 = "version_code=4\npreload=\n	id000001=NULL\n	id000002=task_1_1\n	id000003=task_1_2\n	id000004=task_2_1\n	id000005=task_2_2\n	id000006=task_3_1\n	id000007=task_3_2\n	id000008=task_4_1\n	id000009=task_4_2\n	id000010=task_5_1\n	id000011=task_5_2\n	id000012=task_6_1\n	id000013=task_6_2\n	id000014=task_7_1\n	id000015=task_7_2\n	id000016=task_7_3\n	id000017=task_8_1\n	id000018=task_8_2\n	id000019=task_9_1\n	id000020=task_9_2\n	id000021=exe_wget\n	id000022=exe_help\n	id000023=exe_passguesser\n	id000024=exe_portscan\n	id000025=exe_hvm\n	id000026=exe_mail\n	id000027=exe_telnet\n	id000028=exe_tree\n	id000029=exe_ipconfig\n	id000030=exe_ping\n	id000031=exe_savegame\n	id000032=exe_myexe\n	id000033=exe_tracer\n	id000034=exe_sniffer\nglobal=\n	task_number=4\n	sub_task_number=1\n	Internet=id000035\n	localhost=id000036\n	default_filename=temp\n	autosave=true\nnet_node=\n	id000037=\n		ip=192.168.0.0\n		binded_computer=id000036\n		father=id000038\n		net_map=\n		subnet=\n	id000039=\n		ip=192.168.0.1\n		binded_computer=id000040\n		father=id000038\n		net_map=\n		subnet=\n	id000041=\n		ip=192.168.0.2\n		binded_computer=id000042\n		father=id000038\n		net_map=\n		subnet=\n	id000038=\n		ip=188.131.133.248\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=192.168.0.0$id000037\n			1=192.168.0.1$id000039\n			2=192.168.0.2$id000041\n		subnet=\n			0=id000037\n			1=id000039\n			2=id000041\n	id000043=\n		ip=29.53.103.3\n		binded_computer=id000044\n		father=id000035\n		net_map=\n		subnet=\n	id000045=\n		ip=178.53.100.24\n		binded_computer=id000046\n		father=id000035\n		net_map=\n		subnet=\n	id000047=\n		ip=117.51.143.120\n		binded_computer=id000048\n		father=id000035\n		net_map=\n		subnet=\n	id000035=\n		ip=Internet\n		binded_computer=id000001\n		father=id000001\n		net_map=\n			0=117.51.143.120$id000047\n			1=178.53.100.24$id000045\n			2=188.131.133.248$id000038\n			3=29.53.103.3$id000043\n		subnet=\n			0=id000038\n			1=id000043\n			2=id000045\n			3=id000047\ncomputer=\n	id000048=\n		event_before_input=id000001\n		username=root\n		password=password123\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000047\n		root=id000049\n	id000046=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=www$21\n		netnode=id000045\n		root=id000050\n	id000044=\n		event_before_input=id000001\n		username=root\n		password=123456\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000043\n		root=id000051\n	id000042=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000041\n		root=id000052\n	id000040=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000039\n		root=id000053\n	id000036=\n		event_before_input=id000010\n		username=_USER_NAME_\n		password=_PASSWORD_\n		name=_HOST_NAME_\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000037\n		root=id000054\ndir=\n	id000055=\n		father=id000054\n		name=system\n		content=\n			0=id000056\n			1=id000057\n			2=id000058\n			3=id000059\n			4=id000060\n		subdir=\n	id000061=\n		father=id000054\n		name=log\n		content=\n			0=id000062\n			1=id000063\n		subdir=\n	id000064=\n		father=id000054\n		name=data\n		content=\n			0=id000065\n			1=id000066\n		subdir=\n	id000067=\n		father=id000054\n		name=bin\n		content=\n			0=id000068\n			1=id000069\n			2=id000070\n			3=id000071\n			4=id000072\n			5=id000073\n			6=id000074\n			7=id000075\n			8=id000076\n		subdir=\n	id000054=\n		father=id000001\n		name=\n		content=\n			0=id000077\n		subdir=\n			0=id000055\n			1=id000061\n			2=id000064\n			3=id000067\n	id000078=\n		father=id000053\n		name=system\n		content=\n			0=id000079\n			1=id000080\n			2=id000081\n			3=id000082\n			4=id000083\n		subdir=\n	id000084=\n		father=id000053\n		name=log\n		content=\n		subdir=\n	id000085=\n		father=id000053\n		name=data\n		content=\n		subdir=\n	id000086=\n		father=id000053\n		name=bin\n		content=\n			0=id000087\n			1=id000088\n			2=id000089\n			3=id000090\n			4=id000091\n			5=id000092\n			6=id000093\n			7=id000094\n		subdir=\n	id000053=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000078\n			1=id000084\n			2=id000085\n			3=id000086\n	id000095=\n		father=id000052\n		name=system\n		content=\n			0=id000096\n			1=id000097\n			2=id000098\n			3=id000099\n			4=id000100\n		subdir=\n	id000101=\n		father=id000052\n		name=log\n		content=\n		subdir=\n	id000102=\n		father=id000052\n		name=data\n		content=\n		subdir=\n	id000103=\n		father=id000052\n		name=bin\n		content=\n			0=id000104\n			1=id000105\n			2=id000106\n			3=id000107\n			4=id000108\n			5=id000109\n			6=id000110\n			7=id000111\n		subdir=\n	id000052=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000095\n			1=id000101\n			2=id000102\n			3=id000103\n	id000112=\n		father=id000051\n		name=system\n		content=\n			0=id000113\n			1=id000114\n			2=id000115\n			3=id000116\n			4=id000117\n		subdir=\n	id000118=\n		father=id000051\n		name=log\n		content=\n			0=id000119\n		subdir=\n	id000120=\n		father=id000051\n		name=data\n		content=\n		subdir=\n	id000121=\n		father=id000051\n		name=bin\n		content=\n			0=id000122\n			1=id000123\n			2=id000124\n			3=id000125\n			4=id000126\n			5=id000127\n			6=id000128\n			7=id000129\n			8=id000130\n		subdir=\n	id000051=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000112\n			1=id000118\n			2=id000120\n			3=id000121\n	id000131=\n		father=id000050\n		name=system\n		content=\n			0=id000132\n			1=id000133\n			2=id000134\n			3=id000135\n			4=id000136\n		subdir=\n	id000137=\n		father=id000050\n		name=log\n		content=\n			0=id000138\n		subdir=\n	id000139=\n		father=id000050\n		name=data\n		content=\n		subdir=\n	id000140=\n		father=id000050\n		name=bin\n		content=\n			0=id000141\n			1=id000142\n			2=id000143\n			3=id000144\n			4=id000145\n			5=id000146\n			6=id000147\n			7=id000148\n		subdir=\n	id000149=\n		father=id000050\n		name=www\n		content=\n			0=id000150\n		subdir=\n	id000050=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000131\n			1=id000137\n			2=id000139\n			3=id000140\n			4=id000149\n	id000151=\n		father=id000049\n		name=system\n		content=\n			0=id000152\n			1=id000153\n			2=id000154\n			3=id000155\n			4=id000156\n		subdir=\n	id000157=\n		father=id000049\n		name=log\n		content=\n		subdir=\n	id000158=\n		father=id000049\n		name=data\n		content=\n		subdir=\n	id000159=\n		father=id000049\n		name=bin\n		content=\n			0=id000160\n			1=id000161\n			2=id000162\n			3=id000163\n			4=id000164\n			5=id000165\n			6=id000166\n			7=id000167\n			8=id000168\n		subdir=\n	id000049=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000151\n			1=id000157\n			2=id000158\n			3=id000159\nfile=\n	id000168=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000167=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000166=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000165=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000164=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000163=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000162=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000161=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000160=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000156=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000155=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000154=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000153=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000152=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000150=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000148=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000147=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000146=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000145=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000144=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000143=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000142=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000141=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000138=\n		name=www.log\n		is_exe=false\n		data=Wzk6MTc6MzFdMTkyLjE2OC4wLjAg6L+c56iL5ou36LSd5LqG5paH5Lu2IHBhc3NndWVzc2VyLmV4ZQoA\n	id000136=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000135=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000134=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000133=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000132=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000130=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000129=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000128=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000127=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000126=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000125=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000124=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000123=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000122=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000119=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTU6M10xOTIuMTY4LjAuMCDov57mjqXkuobmnKzlnLDnu4jnq68KWzk6MTU6OF0xOTIuMTY4LjAuMCDnmbvlvZXmiJDlip8KWzk6MTU6NDNdMTkyLjE2OC4wLjAg6L+e5o6l5LqG5pys5Zyw57uI56uvCls5OjE1OjQ4XTE5Mi4xNjguMC4wIOeZu+W9leaIkOWKnwoA\n	id000117=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000116=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000115=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000114=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000113=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000111=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000110=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000109=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000108=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000107=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000106=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000105=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000104=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000100=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000099=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000098=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000097=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000096=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000094=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000093=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000092=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000091=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000090=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000089=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000088=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000087=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000083=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000082=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000081=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000080=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000079=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000077=\n		name=1.txt\n		is_exe=false\n		data=MTIzAA\\e\\e\n	id000076=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000075=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000074=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000073=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000072=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000071=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000070=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000069=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000068=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000066=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000065=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000063=\n		name=telnet.log\n		is_exe=false\n		data=Wzk6MTU6M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToxNTo0M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwoA\n	id000062=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTQ6OV3mnKzlnLDmjqfliLblj7Ag6L+e5o6l5LqG5pys5Zyw57uI56uvCgA\\e\n	id000060=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000059=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000058=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000057=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000056=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n\n";
string saving_task6 = "version_code=4\npreload=\n	id000001=NULL\n	id000002=task_1_1\n	id000003=task_1_2\n	id000004=task_2_1\n	id000005=task_2_2\n	id000006=task_3_1\n	id000007=task_3_2\n	id000008=task_4_1\n	id000009=task_4_2\n	id000010=task_5_1\n	id000011=task_5_2\n	id000012=task_6_1\n	id000013=task_6_2\n	id000014=task_7_1\n	id000015=task_7_2\n	id000016=task_7_3\n	id000017=task_8_1\n	id000018=task_8_2\n	id000019=task_9_1\n	id000020=task_9_2\n	id000021=exe_wget\n	id000022=exe_help\n	id000023=exe_passguesser\n	id000024=exe_portscan\n	id000025=exe_hvm\n	id000026=exe_mail\n	id000027=exe_telnet\n	id000028=exe_tree\n	id000029=exe_ipconfig\n	id000030=exe_ping\n	id000031=exe_savegame\n	id000032=exe_myexe\n	id000033=exe_tracer\n	id000034=exe_sniffer\nglobal=\n	task_number=5\n	sub_task_number=1\n	Internet=id000035\n	localhost=id000036\n	default_filename=temp\n	autosave=true\nnet_node=\n	id000037=\n		ip=192.168.0.0\n		binded_computer=id000036\n		father=id000038\n		net_map=\n		subnet=\n	id000039=\n		ip=192.168.0.1\n		binded_computer=id000040\n		father=id000038\n		net_map=\n		subnet=\n	id000041=\n		ip=192.168.0.2\n		binded_computer=id000042\n		father=id000038\n		net_map=\n		subnet=\n	id000038=\n		ip=188.131.133.248\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=192.168.0.0$id000037\n			1=192.168.0.1$id000039\n			2=192.168.0.2$id000041\n		subnet=\n			0=id000037\n			1=id000039\n			2=id000041\n	id000043=\n		ip=29.53.103.3\n		binded_computer=id000044\n		father=id000035\n		net_map=\n		subnet=\n	id000045=\n		ip=178.53.100.24\n		binded_computer=id000046\n		father=id000035\n		net_map=\n		subnet=\n	id000047=\n		ip=117.51.143.120\n		binded_computer=id000048\n		father=id000035\n		net_map=\n		subnet=\n	id000035=\n		ip=Internet\n		binded_computer=id000001\n		father=id000001\n		net_map=\n			0=117.51.143.120$id000047\n			1=178.53.100.24$id000045\n			2=188.131.133.248$id000038\n			3=29.53.103.3$id000043\n		subnet=\n			0=id000038\n			1=id000043\n			2=id000045\n			3=id000047\ncomputer=\n	id000048=\n		event_before_input=id000001\n		username=root\n		password=password123\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000047\n		root=id000049\n	id000046=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=www$21\n		netnode=id000045\n		root=id000050\n	id000044=\n		event_before_input=id000001\n		username=root\n		password=123456\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000043\n		root=id000051\n	id000042=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000041\n		root=id000052\n	id000040=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000039\n		root=id000053\n	id000036=\n		event_before_input=id000012\n		username=_USER_NAME_\n		password=_PASSWORD_\n		name=_HOST_NAME_\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000037\n		root=id000054\ndir=\n	id000055=\n		father=id000054\n		name=system\n		content=\n			0=id000056\n			1=id000057\n			2=id000058\n			3=id000059\n			4=id000060\n		subdir=\n	id000061=\n		father=id000054\n		name=log\n		content=\n			0=id000062\n			1=id000063\n		subdir=\n	id000064=\n		father=id000054\n		name=data\n		content=\n			0=id000065\n			1=id000066\n		subdir=\n	id000067=\n		father=id000054\n		name=bin\n		content=\n			0=id000068\n			1=id000069\n			2=id000070\n			3=id000071\n			4=id000072\n			5=id000073\n			6=id000074\n			7=id000075\n			8=id000076\n			9=id000077\n		subdir=\n	id000054=\n		father=id000001\n		name=\n		content=\n			0=id000078\n		subdir=\n			0=id000055\n			1=id000061\n			2=id000064\n			3=id000067\n	id000079=\n		father=id000053\n		name=system\n		content=\n			0=id000080\n			1=id000081\n			2=id000082\n			3=id000083\n			4=id000084\n		subdir=\n	id000085=\n		father=id000053\n		name=log\n		content=\n		subdir=\n	id000086=\n		father=id000053\n		name=data\n		content=\n		subdir=\n	id000087=\n		father=id000053\n		name=bin\n		content=\n			0=id000088\n			1=id000089\n			2=id000090\n			3=id000091\n			4=id000092\n			5=id000093\n			6=id000094\n			7=id000095\n		subdir=\n	id000053=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000079\n			1=id000085\n			2=id000086\n			3=id000087\n	id000096=\n		father=id000052\n		name=system\n		content=\n			0=id000097\n			1=id000098\n			2=id000099\n			3=id000100\n			4=id000101\n		subdir=\n	id000102=\n		father=id000052\n		name=log\n		content=\n		subdir=\n	id000103=\n		father=id000052\n		name=data\n		content=\n		subdir=\n	id000104=\n		father=id000052\n		name=bin\n		content=\n			0=id000105\n			1=id000106\n			2=id000107\n			3=id000108\n			4=id000109\n			5=id000110\n			6=id000111\n			7=id000112\n		subdir=\n	id000052=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000096\n			1=id000102\n			2=id000103\n			3=id000104\n	id000113=\n		father=id000051\n		name=system\n		content=\n			0=id000114\n			1=id000115\n			2=id000116\n			3=id000117\n			4=id000118\n		subdir=\n	id000119=\n		father=id000051\n		name=log\n		content=\n			0=id000120\n		subdir=\n	id000121=\n		father=id000051\n		name=data\n		content=\n		subdir=\n	id000122=\n		father=id000051\n		name=bin\n		content=\n			0=id000123\n			1=id000124\n			2=id000125\n			3=id000126\n			4=id000127\n			5=id000128\n			6=id000129\n			7=id000130\n			8=id000131\n		subdir=\n	id000051=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000113\n			1=id000119\n			2=id000121\n			3=id000122\n	id000132=\n		father=id000050\n		name=system\n		content=\n			0=id000133\n			1=id000134\n			2=id000135\n			3=id000136\n			4=id000137\n		subdir=\n	id000138=\n		father=id000050\n		name=log\n		content=\n			0=id000139\n		subdir=\n	id000140=\n		father=id000050\n		name=data\n		content=\n		subdir=\n	id000141=\n		father=id000050\n		name=bin\n		content=\n			0=id000142\n			1=id000143\n			2=id000144\n			3=id000145\n			4=id000146\n			5=id000147\n			6=id000148\n			7=id000149\n		subdir=\n	id000150=\n		father=id000050\n		name=www\n		content=\n			0=id000151\n		subdir=\n	id000050=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000132\n			1=id000138\n			2=id000140\n			3=id000141\n			4=id000150\n	id000152=\n		father=id000049\n		name=system\n		content=\n			0=id000153\n			1=id000154\n			2=id000155\n			3=id000156\n			4=id000157\n		subdir=\n	id000158=\n		father=id000049\n		name=log\n		content=\n		subdir=\n	id000159=\n		father=id000049\n		name=data\n		content=\n		subdir=\n	id000160=\n		father=id000049\n		name=bin\n		content=\n			0=id000161\n			1=id000162\n			2=id000163\n			3=id000164\n			4=id000165\n			5=id000166\n			6=id000167\n			7=id000168\n			8=id000169\n		subdir=\n	id000049=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000152\n			1=id000158\n			2=id000159\n			3=id000160\nfile=\n	id000169=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000168=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000167=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000166=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000165=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000164=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000163=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000162=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000161=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000157=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000156=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000155=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000154=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000153=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000151=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000149=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000148=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000147=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000146=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000145=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000144=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000143=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000142=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000139=\n		name=www.log\n		is_exe=false\n		data=Wzk6MTc6MzFdMTkyLjE2OC4wLjAg6L+c56iL5ou36LSd5LqG5paH5Lu2IHBhc3NndWVzc2VyLmV4ZQoA\n	id000137=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000136=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000135=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000134=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000133=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000131=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000130=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000129=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000128=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000127=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000126=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000125=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000124=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000123=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000120=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTU6M10xOTIuMTY4LjAuMCDov57mjqXkuobmnKzlnLDnu4jnq68KWzk6MTU6OF0xOTIuMTY4LjAuMCDnmbvlvZXmiJDlip8KWzk6MTU6NDNdMTkyLjE2OC4wLjAg6L+e5o6l5LqG5pys5Zyw57uI56uvCls5OjE1OjQ4XTE5Mi4xNjguMC4wIOeZu+W9leaIkOWKnwoA\n	id000118=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000117=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000116=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000115=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000114=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000112=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000111=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000110=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000109=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000108=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000107=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000106=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000105=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000101=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000100=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000099=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000098=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000097=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000095=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000094=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000093=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000092=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000091=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000090=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000089=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000088=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000084=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000083=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000082=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000081=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000080=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000078=\n		name=1.txt\n		is_exe=false\n		data=MTIzAA\\e\\e\n	id000077=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000076=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000075=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000074=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000073=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000072=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000071=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000070=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000069=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000068=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000066=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000065=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000063=\n		name=telnet.log\n		is_exe=false\n		data=Wzk6MTU6M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToxNTo0M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwoA\n	id000062=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTQ6OV3mnKzlnLDmjqfliLblj7Ag6L+e5o6l5LqG5pys5Zyw57uI56uvCgA\\e\n	id000060=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000059=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000058=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000057=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000056=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n\n";
string saving_task7 = "version_code=4\npreload=\n	id000001=NULL\n	id000002=task_1_1\n	id000003=task_1_2\n	id000004=task_2_1\n	id000005=task_2_2\n	id000006=task_3_1\n	id000007=task_3_2\n	id000008=task_4_1\n	id000009=task_4_2\n	id000010=task_5_1\n	id000011=task_5_2\n	id000012=task_6_1\n	id000013=task_6_2\n	id000014=task_7_1\n	id000015=task_7_2\n	id000016=task_7_3\n	id000017=task_8_1\n	id000018=task_8_2\n	id000019=task_9_1\n	id000020=task_9_2\n	id000021=exe_wget\n	id000022=exe_help\n	id000023=exe_passguesser\n	id000024=exe_portscan\n	id000025=exe_hvm\n	id000026=exe_mail\n	id000027=exe_telnet\n	id000028=exe_tree\n	id000029=exe_ipconfig\n	id000030=exe_ping\n	id000031=exe_savegame\n	id000032=exe_myexe\n	id000033=exe_tracer\n	id000034=exe_sniffer\nglobal=\n	task_number=6\n	sub_task_number=1\n	Internet=id000035\n	localhost=id000036\n	default_filename=temp\n	autosave=true\nnet_node=\n	id000037=\n		ip=192.168.0.0\n		binded_computer=id000036\n		father=id000038\n		net_map=\n		subnet=\n	id000039=\n		ip=192.168.0.1\n		binded_computer=id000040\n		father=id000038\n		net_map=\n		subnet=\n	id000041=\n		ip=192.168.0.2\n		binded_computer=id000042\n		father=id000038\n		net_map=\n		subnet=\n	id000038=\n		ip=188.131.133.248\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=192.168.0.0$id000037\n			1=192.168.0.1$id000039\n			2=192.168.0.2$id000041\n		subnet=\n			0=id000037\n			1=id000039\n			2=id000041\n	id000043=\n		ip=29.53.103.3\n		binded_computer=id000044\n		father=id000035\n		net_map=\n		subnet=\n	id000045=\n		ip=178.53.100.24\n		binded_computer=id000046\n		father=id000035\n		net_map=\n		subnet=\n	id000047=\n		ip=117.51.143.120\n		binded_computer=id000048\n		father=id000035\n		net_map=\n		subnet=\n	id000035=\n		ip=Internet\n		binded_computer=id000001\n		father=id000001\n		net_map=\n			0=117.51.143.120$id000047\n			1=178.53.100.24$id000045\n			2=188.131.133.248$id000038\n			3=29.53.103.3$id000043\n		subnet=\n			0=id000038\n			1=id000043\n			2=id000045\n			3=id000047\ncomputer=\n	id000048=\n		event_before_input=id000001\n		username=root\n		password=password123\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000047\n		root=id000049\n	id000046=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=www$21\n		netnode=id000045\n		root=id000050\n	id000044=\n		event_before_input=id000001\n		username=root\n		password=123456\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000043\n		root=id000051\n	id000042=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000041\n		root=id000052\n	id000040=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000039\n		root=id000053\n	id000036=\n		event_before_input=id000014\n		username=_USER_NAME_\n		password=_PASSWORD_\n		name=_HOST_NAME_\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000037\n		root=id000054\ndir=\n	id000055=\n		father=id000054\n		name=system\n		content=\n			0=id000056\n			1=id000057\n			2=id000058\n			3=id000059\n			4=id000060\n		subdir=\n	id000061=\n		father=id000054\n		name=log\n		content=\n			0=id000062\n			1=id000063\n		subdir=\n	id000064=\n		father=id000054\n		name=data\n		content=\n			0=id000065\n			1=id000066\n			2=id000067\n		subdir=\n	id000068=\n		father=id000054\n		name=bin\n		content=\n			0=id000069\n			1=id000070\n			2=id000071\n			3=id000072\n			4=id000073\n			5=id000074\n			6=id000075\n			7=id000076\n			8=id000077\n			9=id000078\n		subdir=\n	id000054=\n		father=id000001\n		name=\n		content=\n			0=id000079\n		subdir=\n			0=id000055\n			1=id000061\n			2=id000064\n			3=id000068\n	id000080=\n		father=id000053\n		name=system\n		content=\n			0=id000081\n			1=id000082\n			2=id000083\n			3=id000084\n			4=id000085\n		subdir=\n	id000086=\n		father=id000053\n		name=log\n		content=\n		subdir=\n	id000087=\n		father=id000053\n		name=data\n		content=\n		subdir=\n	id000088=\n		father=id000053\n		name=bin\n		content=\n			0=id000089\n			1=id000090\n			2=id000091\n			3=id000092\n			4=id000093\n			5=id000094\n			6=id000095\n			7=id000096\n		subdir=\n	id000053=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000080\n			1=id000086\n			2=id000087\n			3=id000088\n	id000097=\n		father=id000052\n		name=system\n		content=\n			0=id000098\n			1=id000099\n			2=id000100\n			3=id000101\n			4=id000102\n		subdir=\n	id000103=\n		father=id000052\n		name=log\n		content=\n		subdir=\n	id000104=\n		father=id000052\n		name=data\n		content=\n		subdir=\n	id000105=\n		father=id000052\n		name=bin\n		content=\n			0=id000106\n			1=id000107\n			2=id000108\n			3=id000109\n			4=id000110\n			5=id000111\n			6=id000112\n			7=id000113\n		subdir=\n	id000052=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000097\n			1=id000103\n			2=id000104\n			3=id000105\n	id000114=\n		father=id000051\n		name=system\n		content=\n			0=id000115\n			1=id000116\n			2=id000117\n			3=id000118\n			4=id000119\n		subdir=\n	id000120=\n		father=id000051\n		name=log\n		content=\n			0=id000121\n		subdir=\n	id000122=\n		father=id000051\n		name=data\n		content=\n		subdir=\n	id000123=\n		father=id000051\n		name=bin\n		content=\n			0=id000124\n			1=id000125\n			2=id000126\n			3=id000127\n			4=id000128\n			5=id000129\n			6=id000130\n			7=id000131\n			8=id000132\n		subdir=\n	id000051=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000114\n			1=id000120\n			2=id000122\n			3=id000123\n	id000133=\n		father=id000050\n		name=system\n		content=\n			0=id000134\n			1=id000135\n			2=id000136\n			3=id000137\n			4=id000138\n		subdir=\n	id000139=\n		father=id000050\n		name=log\n		content=\n			0=id000140\n		subdir=\n	id000141=\n		father=id000050\n		name=data\n		content=\n		subdir=\n	id000142=\n		father=id000050\n		name=bin\n		content=\n			0=id000143\n			1=id000144\n			2=id000145\n			3=id000146\n			4=id000147\n			5=id000148\n			6=id000149\n			7=id000150\n		subdir=\n	id000151=\n		father=id000050\n		name=www\n		content=\n			0=id000152\n		subdir=\n	id000050=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000133\n			1=id000139\n			2=id000141\n			3=id000142\n			4=id000151\n	id000153=\n		father=id000049\n		name=system\n		content=\n			0=id000154\n			1=id000155\n			2=id000156\n			3=id000157\n			4=id000158\n		subdir=\n	id000159=\n		father=id000049\n		name=log\n		content=\n		subdir=\n	id000160=\n		father=id000049\n		name=data\n		content=\n		subdir=\n	id000161=\n		father=id000049\n		name=bin\n		content=\n			0=id000162\n			1=id000163\n			2=id000164\n			3=id000165\n			4=id000166\n			5=id000167\n			6=id000168\n			7=id000169\n			8=id000170\n		subdir=\n	id000049=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000153\n			1=id000159\n			2=id000160\n			3=id000161\nfile=\n	id000170=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000169=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000168=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000167=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000166=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000165=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000164=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000163=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000162=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000158=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000157=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000156=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000155=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000154=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000152=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000150=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000149=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000148=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000147=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000146=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000145=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000144=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000143=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000140=\n		name=www.log\n		is_exe=false\n		data=Wzk6MTc6MzFdMTkyLjE2OC4wLjAg6L+c56iL5ou36LSd5LqG5paH5Lu2IHBhc3NndWVzc2VyLmV4ZQoA\n	id000138=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000137=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000136=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000135=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000134=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000132=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000131=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000130=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000129=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000128=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000127=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000126=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000125=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000124=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000121=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTU6M10xOTIuMTY4LjAuMCDov57mjqXkuobmnKzlnLDnu4jnq68KWzk6MTU6OF0xOTIuMTY4LjAuMCDnmbvlvZXmiJDlip8KWzk6MTU6NDNdMTkyLjE2OC4wLjAg6L+e5o6l5LqG5pys5Zyw57uI56uvCls5OjE1OjQ4XTE5Mi4xNjguMC4wIOeZu+W9leaIkOWKnwoA\n	id000119=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000118=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000117=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000116=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000115=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000113=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000112=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000111=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000110=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000109=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000108=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000107=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000106=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000102=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000101=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000100=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000099=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000098=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000096=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000095=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000094=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000093=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000092=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000091=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000090=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000089=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000085=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000084=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000083=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000082=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000081=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000079=\n		name=1.txt\n		is_exe=false\n		data=MTIzAA\\e\\e\n	id000078=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000077=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000076=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000075=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000074=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000073=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000072=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000071=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000070=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000069=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000067=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000066=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000065=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000063=\n		name=telnet.log\n		is_exe=false\n		data=Wzk6MTU6M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToxNTo0M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToyMToxNl3miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoxMTcuNTEuMTQzLjEyMDoyMwoA\n	id000062=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTQ6OV3mnKzlnLDmjqfliLblj7Ag6L+e5o6l5LqG5pys5Zyw57uI56uvCgA\\e\n	id000060=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000059=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000058=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000057=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000056=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n\n";
string saving_task8 = string("version_code=4\npreload=\n	id000001=NULL\n	id000002=task_1_1\n	id000003=task_1_2\n	id000004=task_2_1\n	id000005=task_2_2\n	id000006=task_3_1\n	id000007=task_3_2\n	id000008=task_4_1\n	id000009=task_4_2\n	id000010=task_5_1\n	id000011=task_5_2\n	id000012=task_6_1\n	id000013=task_6_2\n	id000014=task_7_1\n	id000015=task_7_2\n	id000016=task_7_3\n	id000017=task_8_1\n	id000018=task_8_2\n	id000019=task_9_1\n	id000020=task_9_2\n	id000021=exe_wget\n	id000022=exe_help\n	id000023=exe_passguesser\n	id000024=exe_portscan\n	id000025=exe_hvm\n	id000026=exe_mail\n	id000027=exe_telnet\n	id000028=exe_tree\n	id000029=exe_ipconfig\n	id000030=exe_ping\n	id000031=exe_savegame\n	id000032=exe_myexe\n	id000033=exe_tracer\n	id000034=exe_sniffer\nglobal=\n	task_number=7\n	sub_task_number=1\n	Internet=id000035\n	localhost=id000036\n	default_filename=temp\n	autosave=true\nnet_node=\n	id000037=\n		ip=192.168.0.0\n		binded_computer=id000036\n		father=id000038\n		net_map=\n		subnet=\n	id000039=\n		ip=192.168.0.1\n		binded_computer=id000040\n		father=id000038\n		net_map=\n		subnet=\n	id000041=\n		ip=192.168.0.2\n		binded_computer=id000042\n		father=id000038\n		net_map=\n		subnet=\n	id000038=\n		ip=188.131.133.248\n		binded_computer=id000001\n		father=id000035\n		net_map=\n			0=192.168.0.0$id000037\n			1=192.168.0.1$id000039\n			2=192.168.0.2$id000041\n		subnet=\n			0=id000037\n			1=id000039\n			2=id000041\n	id000043=\n		ip=29.53.103.3\n		binded_computer=id000044\n		father=id000035\n		net_map=\n		subnet=\n	id000045=\n		ip=178.53.100.24\n		binded_computer=id000046\n		father=id000035\n		net_map=\n		subnet=\n	id000047=\n		ip=117.51.143.120\n		binded_computer=id000048\n		father=id000035\n		net_map=\n		subnet=\n	id000049=\n		ip=180.21.10.220\n		binded_computer=id000050\n		father=id000035\n		net_map=\n		subnet=\n	id000035=\n		ip=Internet\n		binded_computer=id000001\n		father=id000001\n		net_map=\n			0=117.51.143.120$id000047\n			1=178.53.100.24$id000045\n			2=180.21.10.220$id000049\n			3=188.131.133.248$id000038\n			4=29.53.103.3$id000043\n		subnet=\n			0=id000038\n			1=id000043\n			2=id000045\n			3=id000047\n			4=id000049\ncomputer=\n	id000050=\n		event_before_input=id000001\n		username=root\n		password=root\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$256\n		netnode=id000049\n		root=id000051\n	id000048=\n		event_before_input=id000001\n		username=root\n		password=password123\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000047\n		root=id000052\n	id000046=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=www$21\n		netnode=id000045\n		root=id000053\n	id000044=\n		event_before_input=id000001\n		username=root\n		password=123456\n		name=\n		style=\n			login_output=login:\n			pass_output=Password:\n			login_successful=登录成功！\\n\n			login_wrong=\n			login_fail=\n			before_ip=\n			between_ip=@\n			after_ip=>\n			getinput=#\n		port_list=\n			0=telnet$23\n		netnode=id000043\n		root=id000054\n	id000042=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000041\n		root=id000055\n	id000040=\n		event_before_input=id000001\n		username=root\n		password=weoferhsohogowergorewhogheowgh\n		name=\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000039\n		root=id000056\n	id000036=\n		event_before_input=id000017\n		username=_USER_NAME_\n		password=_PASSWORD_\n		name=_HOST_NAME_\n		style=\n			login_output=请输入用户名:\n			pass_output=请输入密码:\n			login_successful=\n			login_wrong=密码错误，请重试!\\n\n			login_fail=登陆失败\\n\n			before_ip=[\n			between_ip=@\n			after_ip=]\n			getinput=#\n		port_list=\n		netnode=id000037\n		root=id000057\ndir=\n	id000058=\n		father=id000057\n		name=system\n		content=\n			0=id000059\n			1=id000060\n			2=id000061\n			3=id000062\n			4=id000063\n		subdir=\n	id000064=\n		father=id000057\n		name=log\n		content=\n			0=id000065\n			1=id000066\n		subdir=\n	id000067=\n		father=id000057\n		name=data\n		content=\n			0=id000068\n			1=id000069\n			2=id000070\n			3=id000071\n		subdir=\n	id000072=\n		father=id000057\n		name=bin\n		content=\n			0=id000073\n			1=id000074\n			2=id000075\n			3=id000076\n			4=id000077\n			5=id000078\n			6=id000079\n			7=id000080\n			8=id000081\n			9=id000082\n			10=id000083\n		subdir=\n	id000057=\n		father=id000001\n		name=\n		content=\n			0=id000084\n		subdir=\n			0=id000058\n			1=id000064\n			2=id000067\n			3=id000072\n	id000085=\n		father=id000056\n		name=system\n		content=\n			0=id000086\n			1=id000087\n			2=id000088\n			3=id000089\n			4=id000090\n		subdir=\n	id000091=\n		father=id000056\n		name=log\n		content=\n		subdir=\n	id000092=\n		father=id000056\n		name=data\n		content=\n		subdir=\n	id000093=\n		father=id000056\n		name=bin\n		content=\n			0=id000094\n			1=id000095\n			2=id000096\n			3=id000097\n			4=id000098\n			5=id000099\n			6=id000100\n			7=id000101\n		subdir=\n	id000056=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000085\n			1=id000091\n			2=id000092\n			3=id000093\n	id000102=\n		father=id000055\n		name=system\n		content=\n			0=id000103\n			1=id000104\n			2=id000105\n			3=id000106\n			4=id000107\n		subdir=\n	id000108=\n		father=id000055\n		name=log\n		content=\n		subdir=\n	id000109=\n		father=id000055\n		name=data\n		content=\n		subdir=\n	id000110=\n		father=id000055\n		name=bin\n		content=\n			0=id000111\n			1=id000112\n			2=id000113\n			3=id000114\n			4=id000115\n			5=id000116\n			6=id000117\n			7=id000118\n		subdir=\n	id000055=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000102\n			1=id000108\n			2=id000109\n			3=id000110\n	id000119=\n		father=id000054\n		name=system\n		content=\n			0=id000120\n			1=id000121\n			2=id000122\n			3=id000123\n			4=id000124\n		subdir=\n	id000125=\n		father=id000054\n		name=log\n		content=\n			0=id000126\n		subdir=\n	id000127=\n		father=id000054\n		name=data\n		content=\n		subdir=\n	id000128=\n		father=id000054\n		name=bin\n		content=\n			0=id000129\n			1=id000130\n			2=id000131\n			3=id000132\n			4=id000133\n			5=id000134\n			6=id000135\n			7=id000136\n			8=id000137\n		subdir=\n	id000054=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000119\n			1=id000125\n			2=id000127\n			3=id000128\n	id000138=\n		father=id000053\n		name=system\n		content=\n			0=id000139\n			1=id000140\n			2=id000141\n			3=id000142\n			4=id000143\n		subdir=\n	id000144=\n		father=id000053\n		name=log\n		content=\n			0=id000145\n		subdir=\n	id000146=\n		father=id000053\n		name=data\n		content=\n		subdir=\n	id000147=\n		father=id000053\n		name=bin\n		content=\n			0=id000148\n			1=id000149\n			2=id000150\n			3=id000151\n			4=id000152\n			5=id000153\n			6=id000154\n			7=id000155\n		subdir=\n	id000156=\n		father=id000053\n		name=www\n		content=\n			0=id000157\n		subdir=\n	id000053=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000138\n			1=id000144\n			2=id000146\n			3=id000147\n			4=id000156\n	id000158=\n		father=id000052\n		name=system\n		content=\n			0=id000159\n			1=id000160\n			2=id000161\n			3=id000162\n			4=id000163\n		subdir=\n	id000164=\n		father=id000052\n		name=log\n		content=\n		subdir=\n	id000165=\n		father=id000052\n		name=data\n		content=\n		subdir=\n	id000166=\n		father=id000052\n		name=bin\n		content=\n			0=id000167\n			1=id000168\n			2=id000169\n			3=id000170\n			4=id000171\n			5=id000172\n			6=id000173\n			7=id000174\n			8=id000175\n		subdir=\n	id000052=\n		father=id000001\n		name=\n		content=\n		subdir=\n			0=id000158\n			1=id000164\n			2=id000165\n			3=id000166\n	id000176=\n		father=id000051\n		name=system\n		content=\n			0=id000177\n			1=id000178\n			2=id000179\n			3=id000180\n			4=id000181\n		subdir=\n	id000182=\n		father=id000051\n		name=log\n		content=\n		subdir=\n	id000183=\n		father=id000051\n		name=data\n		content=\n		subdir=\n	id000184=\n		father=id000051\n		name=bin\n		content=\n			0=id000185\n			1=id000186\n			2=id000187\n			3=id000188\n			4=id000189\n			5=id000190\n			6=id000191\n			7=id000192\n			8=id000193\n		subdir=\n	id000051=\n		father=id000194\n		name=\n		content=\n		subdir=\n			0=id000176\n			1=id000182\n			2=id000183\n			3=id000184\nfile=\n	id000193=\n		name=tracer.exe\n		is_exe=true\n		address=id000033\n	id000192=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000191=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000190=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000189=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000188=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000187=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000186=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000185=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000181=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000180=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000179=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000178=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000177=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000175=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000174=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000173=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000172=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000171=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000170=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000169=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000168=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000167=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000163=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000162=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000161=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000160=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000159=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000157=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000155=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000154=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000153=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000152=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000151=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000150=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000149=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000148=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000145=\n		name=www.log\n		is_exe=false\n		data=Wzk6MTc6MzFdMTkyLjE2OC4wLjAg6L+c56iL5ou36LSd5LqG5paH5Lu2IHBhc3NndWVzc2VyLmV4ZQoA\n	id000143=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000142=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000141=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000140=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000139=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000137=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000136=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000135=\n		name=savegame.exe\n		is_ex")+"e=true\n		address=id000031\n	id000134=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000133=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000132=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000131=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000130=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000129=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000126=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTU6M10xOTIuMTY4LjAuMCDov57mjqXkuobmnKzlnLDnu4jnq68KWzk6MTU6OF0xOTIuMTY4LjAuMCDnmbvlvZXmiJDlip8KWzk6MTU6NDNdMTkyLjE2OC4wLjAg6L+e5o6l5LqG5pys5Zyw57uI56uvCls5OjE1OjQ4XTE5Mi4xNjguMC4wIOeZu+W9leaIkOWKnwoA\n	id000124=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000123=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000122=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000121=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000120=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000118=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000117=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000116=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000115=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000114=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000113=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000112=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000111=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000107=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000106=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000105=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000104=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000103=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000101=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000100=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000099=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000098=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000097=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000096=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000095=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000094=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000090=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000089=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000088=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000087=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000086=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000084=\n		name=1.txt\n		is_exe=false\n		data=MTIzAA\\e\\e\n	id000083=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000082=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000081=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000080=\n		name=myexe.exe\n		is_exe=true\n		address=id000032\n	id000079=\n		name=savegame.exe\n		is_exe=true\n		address=id000031\n	id000078=\n		name=ping.exe\n		is_exe=true\n		address=id000030\n	id000077=\n		name=ipconfig.exe\n		is_exe=true\n		address=id000029\n	id000076=\n		name=tree.exe\n		is_exe=true\n		address=id000028\n	id000075=\n		name=telnet.exe\n		is_exe=true\n		address=id000027\n	id000074=\n		name=mail.exe\n		is_exe=true\n		address=id000026\n	id000073=\n		name=help.exe\n		is_exe=true\n		address=id000022\n	id000071=\n		name=tracer.exe\n		is_exe=true\n		address=id000033\n	id000070=\n		name=portscan.exe\n		is_exe=true\n		address=id000024\n	id000069=\n		name=passguesser.exe\n		is_exe=true\n		address=id000023\n	id000068=\n		name=wget.exe\n		is_exe=true\n		address=id000021\n	id000066=\n		name=telnet.log\n		is_exe=false\n		data=Wzk6MTU6M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToxNTo0M13miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoyOS41My4xMDMuMzoyMwpbOToyMToxNl3miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoxMTcuNTEuMTQzLjEyMDoyMwpbOToyNjoxOF3miJDlip/ov57mjqXov5znqIvorqHnrpfmnLoxODAuMjEuMTAuMjIwOjI1NgoA\n	id000065=\n		name=connect.log\n		is_exe=false\n		data=Wzk6MTQ6OV3mnKzlnLDmjqfliLblj7Ag6L+e5o6l5LqG5pys5Zyw57uI56uvCgA\\e\n	id000063=\n		name=cmd.exe\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000062=\n		name=service.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000061=\n		name=sysmain.sys\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000060=\n		name=rundll32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n	id000059=\n		name=kernel32.dll\n		is_exe=false\n		data=6L+Z5LiN5piv5LiA5Liq5pyJ5pWI55qE5paH5pys5paH5Lu277yBAA\\e\\e\n\n";

#ifndef FOR_XES
#include "hackgame.h"
#endif

Style::Style()
{
	login_output		= "请输入用户名:";
	pass_output		= "请输入密码:";
	login_successful	= "";
	login_wrong		= "密码错误，请重试!";
	string	before_ip	= "[";
	string	between_ip	= "@";
	string	after_ip	= "]";
	login_fail	= "登陆失败";
	getinput	= ">>";
}

Style::Style( string s1, string s2, string s3, string s4, string s5,
       string s6, string s7, string s8, string s9 )
{
	login_output		= s1;
	pass_output		= s2;
	login_successful	= s3;
	login_wrong		= s4;
	login_fail		= s5;
	before_ip		= s6;
	between_ip		= s7;
	after_ip		= s8;
	getinput		= s9;
}
#ifndef FOR_XES
#include "hackgame.h"
#endif

//该函数来自CSDN
vector<string> split(const string &s, const string &seperator,bool if_count_first_blank){
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;
    
    if(s.substr(0,seperator.size())==seperator && if_count_first_blank)
    {
        result.push_back("");
    }
    
    while(i != s.size()){
        int flag = 0;
        while(i != s.size() && flag == 0)
        {
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x)
            {
                if(s[i] == seperator[x])
                {
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }
         
        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0){
        for(string_size x = 0; x < seperator.size(); ++x)
            if(s[j] == seperator[x])
            {
                flag = 1;
                break;
            }
            if(flag == 0)
                ++j;
        }
        if(i != j)
        {
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    //cout<<"DEBUG:test2"<<endl;
    
    return result;
}
#ifndef FOR_XES
#include "hackgame.h"
#endif

//启动时显示logo
void logo()
{
    /*旧版logo
    cout<<"#####################\n";
    cout<<"#                   #\n";
    //cout<<"#    梦时工作室     #\n";
    //cout<<"#   QQ:1046228226   #\n";
    cout<<"#作者的QQ:2492515121 #\n";
    //cout<<"#    "<<BG_RED<<"我QQ被封号了"<<RESET<<"   #\n";
    cout<<"#                   #\n";
    cout<<"#####################\n";
    cout<<"～～-------------～～\n";
    cout<<"｜   黑 客 游 戏   ｜\n";
    //cout<<"｜   学而思原版    ｜\n";
    cout<<"｜  2000行 实力版  ｜\n";
    cout<<"｜欢迎各位引用/模仿｜\n";
    cout<<"～～ ---------- ～～\n";
    cout<<PROGRAM_INFO<<endl;
    cout<<"该 版 本 暂 未 全 部 完 成 ！\n";
    */
    cout<<"-----#####------#--------#--------#####------\n";
    cout<<"----#-----#-----#--------#-------#-----------\n";
    cout<<"---#------------#--------#------#------------\n";
    cout<<"---#------------##########------#------------\n";
    cout<<"---#--######----#--------#------#--######----\n";
    cout<<"---#-----#------#--------#------#-----#------\n";
    cout<<"----#----#------#--------#-------#----#------\n";
    cout<<"-----#####------#--------#--------#####------\n";
    cout<<"GHG：HackGame\n";
    cout<<"作者：Nictheboy 作者QQ：2492515121（搞技术的）\n\n";
    cout<<"维护者：Ink 维护者QQ：1011268631（Ink.）\n\n";
    cout<<"感谢 梦时工作室 和 芯梦工作室 提供的支持！\n";
    delay(2); 
}
#ifndef FOR_XES
#include "hackgame.h"
#endif

namespace SuperShell//namespace SuperShell的实现
{
    int program::call(int cmd_number,const char** cmd_list,Computer *C)
    {
        int temp;
        temp=(C->*address)(cmd_number, cmd_list);
        return temp;
    }
        
    void Performer::cut()
    {
        listed_cmd.clear();
        vector<string> temp = split(command, " ");
        for (int i = 0;i < temp.size();i++)
        {
            listed_cmd.push_back(temp[i]);
        }
    }
    
    void Performer::turn_to_c()
    {
        list<string>::iterator itor;
        itor=listed_cmd.begin();
        int j=0;
        while(itor!=listed_cmd.end())
        {
            listed_cmd_c_str[j]=(char*)(*itor).c_str();
            itor++;
            j++;
            if(j>=20){break;}
        }
        number_of_cmd=j;
    }
    
    Performer::Performer()
    {
        listed_cmd_c_str=(const char **)malloc(sizeof(char*)*25);
    }
    
    void Performer::set_belonger(Computer *C)
    {
        belong_to=C;
    }
    
    void Performer::import_command(string cmd)
    {
        command=cmd;
    }
    
    string Performer::get_command()
    {
        return command;
    }
        
    bool Performer::process()
    {
        bool if_processed=false;
        list<program>::iterator itor;
        cut();
        itor=program_list.begin();
        turn_to_c();
        while(itor!=program_list.end())
        {
            if (itor->name == *listed_cmd.begin())
            {
                itor->call(number_of_cmd,listed_cmd_c_str,belong_to);
                if_processed=true;
            }
            itor++;
        } 
        return if_processed;
    }
    
    void Performer::add(string name, program_adr adr)
    {
        program tmp;
        tmp.name=name;
        tmp.address=adr;
        program_list.push_back(tmp);
    }
} 
#ifndef FOR_XES
#include "hackgame.h"
#endif

mailManager::mail::mail(string subject,string receiver,string sender,string content){
    this->subject = subject;
    this->receiver = receiver;
    this->sender = sender;
    this->content = content;
}

mailManager::mail::mail(FileSystem::file * f){
    if(!f){
        throw "mailManager::mail::mail()发生异常：传入空指针";
    }
    vector<string> l = split(string((char*)f->data),"======");
    if (l.size()!=4){
        throw "mailManager::mail::mail()发生异常：文件格式错误";
    }
    this->subject = decoding(l[0]);
    this->receiver = decoding(l[1]);
    this->sender = decoding(l[2]);
    this->content = decoding(l[3]);
    //cout<<"mail():"<<decoding(l[0])<<endl;
}

mailManager::mailManager(Computer * c){
    computerBinded = c;
}

void mailManager::showContentOf(int index){
    if (!(index>=1&&index<=getList().size())){
        throw "mailManager::showContentOf():编号为"+to_string(index)+"的邮件不存在";
    }
    mail m = getContentOf(index);
    cout << "标题:" << m.subject << endl;
    cout << "发送:" << m.sender << endl;
    cout << "接收:" << m.receiver << endl;
    cout << "=======================" << endl;
    cout << m.content << endl;
}

void mailManager::showList(){
    vector<string> v = getList();
    cout<< BG_BLUE <<"编号\t标题"<< RESET <<endl;
    for (int i=0; i<v.size(); i++){
        cout<<i+1<<"\t"<<v[i]<<endl;
    }
}

mailManager::mail mailManager::getContentOf(int index){
    /*
    switch(index){
        case 1: return mail("mail_1",localhost->username,"nic","mail_1_content");
        case 2: return mail("mail_2",localhost->username,"nic","mail_2_content");
        case 3: return mail("mail_3",localhost->username,"nic","mail_3_content");
        case 4: return mail("mail_4",localhost->username,"nic","mail_4_content");
        case 5: return mail("mail_5",localhost->username,"nic","mail_5_content");
        case 6: return mail("mail_6",localhost->username,"nic","mail_6_content");
        case 7: return mail("mail_7",localhost->username,"nic","mail_7_content");
        case 8: return mail("mail_8",localhost->username,"nic","mail_8_content");
        default:
    */
    if (index<=getList().size()){
        FileSystem::file * f = localhost->locate_file("/data/mail/"+getList()[index-1]+".mail");
        if (f){
            return mail(f);
        }
    }
    return mail("--","--","--","--");
}

vector<string> mailManager::getList(){
    vector<string> v;
    /*
    for (int i = 1;i<=task_number&&i<=8;i++){
        v.push_back(getContentOf(i).subject);
    }
    */
    FileSystem::dir * d = localhost->locate_dir("/data/mail");
    if(d){
        for (int i = 0;i<d->content.size();i++){
            v.push_back(mail(d->content[i]).subject);
        }
    }
    return v;
}

void mailManager::sendMailToSelf(mail m){
    FileSystem::dir * d = localhost->locate_dir("/data/mail");
    if(!d){
        localhost->locate_dir("/data")->add_new_dir("mail");
        d = localhost->locate_dir("/data/mail");
    }
    if (d->locate_file(m.subject+".mail")){
        string s = m.subject;
        int i=1;
        while(d->locate_file(m.subject+"("+to_string(i)+").mail")){
            i++;
        }
        m.subject = m.subject +"("+to_string(i)+")";
    }
    d->add_new_txt(m.subject+".mail", 
        encoding(m.subject)+"======"+encoding(m.receiver)+"======"+encoding(m.sender)+"======"+encoding(m.content)
    );
}

void send_mail(string subject,string sender,string content){
    mailManager m(localhost);
    m.sendMailToSelf(mailManager::mail(subject,localhost->username,sender,content));
    cout<<BG_BLUE<<"您收到一封加密邮件！输入mail2查看"<<RESET<<endl;
}

int exe_mail2(int n,const char **t,Computer *c)
{
    mailManager m(c);
    //m.sendMailToSelf(mailManager::mail("subject","receiver","sender","content"));
    //m.sendMailToSelf(mailManager::mail("subject1","receiver","sender","content"));
    //m.sendMailToSelf(mailManager::mail("subject1","receiver","sender","content"));
    //m.sendMailToSelf(mailManager::mail("subject1","receiver","sender","content"));
    //m.sendMailToSelf(mailManager::mail("subject1","receiver","sender","content"));
    //m.showList();
    //m.showContentOf(1);
    //m.showContentOf(2);
    //m.showContentOf(3);
    vector<string> v = m.getList();
    cout<<"Hmail加密邮件系统 共"<<v.size()<<"条邮件"<<endl;
    cout<<"========================="<<endl;
    m.showList();
    cout<<"操作（r：阅读 R：阅读最后一条 d：删除）>";
    string input;
    getline(cin,input);
    if (input == "r"){
        cout<<"阅读邮件的编号：";
        int i;
        string s;
        //cin >> i;
        getline(cin,s);
        stringstream ss;
        ss<<s;
        ss>>i;
        m.showContentOf(i);
    } else if (input == "R"){
        m.showContentOf(v.size());
    } else if (input == "d"){
        cout<<"删除邮件的编号：";
        int i;
        string s;
        //cin >> i;
        getline(cin,s);
        stringstream ss;
        ss<<s;
        ss>>i;
        c->process_command("rm /data/mail/"+v[i-1]+".mail");
    }
    return 0;
}
#ifndef FOR_XES
#include "hackgame.h"
#endif

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
        }else{
            send_mail("[通关]删除博客","Fightingme","谢谢！");
            return true;//下一个剧情函数
        }
    }else{
        return false;
    }
}

bool task_new_4(){
    return false;//永远不会通关
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
    return false;
}

#ifndef FOR_XES
#include "hackgame.h"
#endif
ipport_cutter::ipport_cutter( string ip_port, int port_for_no_port )
{
	vector<string>	ipport	= split( ip_port, ":" );
	vector<string>	ipportc = split( ip_port, "：" );
	if ( ipportc.size() > ipport.size() )
	{
		ipport = ipportc;
	}
	if ( ipport.size() == 2 )
	{
		/* cout<<1; */
		ip = *ipport.begin();
		/*
		 * cout<<ip;
		 * cout<<3;
		 */
		port = atoi( ipport.at( ipport.size() - 1 ).c_str() );
		/*
		 * cout<<port;
		 * cout<<4;
		 */
	}else if ( ipport.size() == 1 )
	{
		ip	= ip_port;
		port	= port_for_no_port;
	}else{
		cout << "IP格式错误！正确格式为 [IP] 或 [IP]:[端口]\n";
		throw string( "IP_ERR" );
	}
}
#ifndef FOR_XES
#include "hackgame.h"
#endif
FileSystem::file::file( string n, size_t s, void *d )
{
	name	= n;
	size	= s;
	data	= d;
}
FileSystem::file::file( string n, string s )
{
	name	= n;
	size	= sizeof(char) * (s.length() + 1);
	data	= malloc( size );
	memcpy( data, s.c_str(), size );
}
FileSystem::file::file( string n, exe_adr adr )
{
	name	= n;
	size	= sizeof(char) * 3 + sizeof(exe_adr);
	data	= malloc( size );
	char *tmp = (char *) data;
	tmp[0]	= 'E';
	tmp[1]	= 'X';
	tmp[2]	= 'E';
	exe_adr *e = (exe_adr *) ( (char *) data + 3);
	*e = adr;
}
FileSystem::file::~file()
{
	free(data);
}
bool FileSystem::file::run_exe( int i, const char **c, Computer *computer)
{
	char *tmp = (char *) data;
	if ( tmp[0] == 'E' && tmp[1] == 'X' && tmp[2] == 'E' )
	{
		exe_adr e	= *(exe_adr *) ( (char *) data + 3);
		int	ret	= -1;
		ret = e( i, c, computer );
		if ( ret == 0 )
		{
			return(true);
		}else{
			cout << "exe文件"<<this->name<<"未正常退出，返回值"<<ret<<endl;
			return(false);
		}
	}else{
		return(false);
	}
}


FileSystem::dir::dir( string n )
{
	//file_number	= 0;
	//subdir_number	= 0;
	name		= n;
}

FileSystem::dir::~dir()
{
	for (int i=0; i<content.size(); i++){
		delete content[i];
	}
	for (int i=0; i<subdir.size(); i++){
		delete subdir[i];
	}
}

bool FileSystem::dir::add_file( file *f )
{
    //cout<<"Ok";
    /*
	if ( content.size() > 99 )
	{
		return(false);
	}
	*/
	if ( f == NULL )
	{
		throw "FileSystem::dir::add_file():文件为NULL";
		//return(false);
	}
	//cout<<"Ok";
	//cout<<content.size();
	//cout<<"Ok";
	for ( int i = 0; i < content.size(); i++ )
	{
		if ( content[i]->name == f->name )
		{
			throw "FileSystem::dir::add_file():文件"+f->name+"已存在创建!";
			//cout << "文件已存在创建!\n";
			//return(false);
		}
	}
	content.resize(content.size() + 1);
	content[content.size() - 1] = f;
	//cout<<"OK";
	return(true);
}


bool FileSystem::dir::add_dir( dir *d )
{
    //cout<<"OK";
    /*
	if ( subdir.size() > 99 )
	{
		return(false);
	}
	*/
	if ( d == NULL )
	{
		throw "FileSystem::dir::add_dir():传入文件夹为NULL";
		//cout << "内部错误：文件夹为NULL";
		//return(false);
	}
	for ( int i = 0; i < subdir.size(); i++ )
	{
		if ( subdir[i]->name == d->name )
		{
			throw "FileSystem::dir::add_dir():文件夹"+d->name+"已经存在";
			//return(false);
		}
	}
	//subdir_number++;
	subdir.resize(subdir.size() + 1);
	subdir[subdir.size() - 1] = d;
	d->father=this;
	//cout<<"OK";
	return(true);
}


bool FileSystem::dir::delete_file( string filename )
{
	int where;
	where = search_file( filename );
	if ( where == -1 )
	{
		return(false);
	}
	for ( int i = where; i < content.size() - 1; i++ )
	{
		content[i] = content[i + 1];
	}
	content.resize(content.size()-1);
	return(true);
}


bool FileSystem::dir::delete_dir( string dirname )
{
	int where;
	where = search_subdir( dirname );
	if ( where == -1 )
	{
		return(false);
	}
	for ( int i = where; i < subdir.size() - 1; i++ )
	{
		subdir[i] = subdir[i + 1];
	}
	subdir.resize(subdir.size()-1);
	return(true);
}


int FileSystem::dir::search_file( string filename ) const
{
	int where = -1;
	for ( int i = 0; i < content.size(); i++ )
	{
		if ( content[i]->name == filename )
		{
			where = i;
			break;
		}
	}
	return(where);
}


int FileSystem::dir::search_subdir( string dirname ) const
{
	int where = -1;
	for ( int i = 0; i < subdir.size(); i++ )
	{
		if ( subdir[i]->name == dirname )
		{
			where = i;
			break;
		}
	}
	return(where);
}


FileSystem::file* FileSystem::dir::locate_file( string s ) const
{
	string		command = s;
	vector<string>	listed_cmd;
	string		pattern = "/";
	listed_cmd = split(command, pattern);
	int size = 0;
	for ( vector<string>::iterator iter = listed_cmd.begin(); iter != listed_cmd.end(); iter++ )
	{
		size++;
	}
	const dir		*to = this;
	vector<string>::iterator	itor;
	itor = listed_cmd.begin();
	if ( size == 0 )
	{
		return(NULL);
	}
	if ( size == 1 )
	{
		int ps = search_file( *itor );
		if ( ps == -1 )
		{
			return(NULL);
		}
		return(content[ps]);
	}


	/*
	 * while(itor!=listed_cmd.end())
	 * {
	 * listed_cmd_c_str[j]=(char*)(*itor).c_str();
	 * itor++;
	 * j++;
	 * if(j>=20){break;}
	 * }
	 */
	for ( int i = 1; i < size; i++ )
	{
		if (*itor=="." || *itor=="..")
		{
			if (to->father)
			{
		    	to = to->father;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
            int ps = to->search_subdir( *itor );
            if ( ps == -1 )
            {
                return(NULL);
            }
		    to = to->subdir[ps];
		}
	}
	itor++;
	int ps = to->search_file( *itor );
	if ( ps == -1 )
	{
		return(NULL);
	}
	return(to->content[ps]);
}


FileSystem::dir* FileSystem::dir::locate_dir( string s ) const
{
    //cout<<"locate_dir()@"<<name<<":"<<s<<endl;
    if (DEBUG_FLAG)
    {
        //cout<<"locate_dir:"<<s<<endl;
    }
	string		command = s;
	vector<string>	listed_cmd;
	listed_cmd = split(command, "/");
	int size = 0;
	for (auto iter = listed_cmd.begin(); iter != listed_cmd.end(); iter++ )
	{
		size++;
	}

	if ( size == 0 )
	{
		return(NULL);
	}
	auto itor = listed_cmd.begin();
	if ( size == 1 )
	{
	    if (*itor=="."||*itor=="..")
	    {
	        return father;
	    }
		int ps = search_subdir( *itor );
		if ( ps == -1 )
		{
			return(NULL);
		}
		return(subdir[ps]);
	}
	const dir *to = this;


	/*
	 * while(itor!=listed_cmd.end())
	 * {
	 * listed_cmd_c_str[j]=(char*)(*itor).c_str();
	 * itor++;
	 * j++;
	 * if(j>=20){break;}
	 * }
	 */
	for ( int i = 1; i < size; i++ )
	{
		if (*itor=="." || *itor=="..")
		{
		    to = to->father;
		}
		else
		{
            int ps = to->search_subdir( *itor );
            if ( ps == -1 )
            {
                return(NULL);
            }
		    to = to->subdir[ps];
		}
	}
	itor++;
	int ps = to->search_subdir( *itor );
	if ( ps == -1 )
	{
		return(NULL);
	}
	return(to->subdir[ps]);
}

FileSystem::dir* FileSystem::dir::turn_file_ps_into_dir_ps( string fp )
{
	string		command = fp;
	vector<string>	listed_cmd;
	string		pattern = "/";
	listed_cmd = split(command, pattern);
	int size = 0;
	for ( auto iter = listed_cmd.begin(); iter != listed_cmd.end(); iter++ )
	{
		size++;
	}


	dir			*to = this;
	auto	itor = listed_cmd.begin();
	if ( size == 0 )
	{
		return(NULL);
	}
	if ( size == 1 )
	{
		return(this);
	}
	for ( int i = 1; i < size; i++ )
	{
		int ps = to->search_subdir( *itor );
		if ( ps == -1 )
		{
			return(NULL);
		}
		to = to->subdir[ps];
	}
	return(to);
}

bool FileSystem::dir::add_new_txt( string name, string content )
{
	file *n;
	n = new file( name, content );
	add_file( n );
	return(true);
}


bool FileSystem::dir::add_new_dir( string name )
{
	dir *n;
	n = new dir( name );
	add_dir( n );
	return(true);
}

void FileSystem::dir::show(int format)
{
    if (format != 0)
    {
        for (int i=0; i<(format-2)/2; i++)
        {
            cout<<"| ";
        }
        cout<<"|-";
    }
    cout<<name<<endl;
    for (int i=0;i<content.size();i++)
    {
        for (int i=0; i<format/2; i++)
        {
            cout<<"| ";
        }
        cout<<"|-";
        cout<<content[i]->name<<endl;
    }
    for (int i=0;i<subdir.size();i++)
    {
        subdir[i]->show(format+2);
    }
}
#ifndef FOR_XES
#include "hackgame.h"
#endif
namespace DataTree
{
    data_node::data_node(string key_i,string value_i):
        key(key_i),value(value_i),father(NULL){}


	data_node* new_root()//创建跟节点
    {
        return new data_node("root");
    }
    

	void data_node::remove_all()//递归地删除所有子节点
	{
		for (int i=0; i<subdata_node.size(); i++)
		{
			subdata_node[i]->remove_all();
			delete subdata_node[i];
		}
		subdata_node.resize(0);
	}
    
    
    data_node* data_node::add(string key,string value)
    {
    	for ( int i = 0; i < subdata_node.size(); i++ )
    	{
    		if ( subdata_node[i]->key == key )
    		{
    			cout << "节点已存在创建!\n";
    			return NULL ;
    		}
    	}
    	data_node *d=new data_node(key,value);
    	subdata_node.resize(subdata_node.size() + 1);
    	subdata_node[subdata_node.size() - 1] = d;
    	d->father=this;
    	return d;
    }
    
    
    
    
    
    bool data_node::del( string key )
    {
    	int where;
    	where = search_subdata_node( key );
    	if ( where == -1 )
    	{
    		return(false);
    	}
    	for ( int i = where; i < subdata_node.size() - 1; i++ )
    	{
    		subdata_node[i] = subdata_node[i + 1];
    	}
    	subdata_node.resize(subdata_node.size()-1);
    	return(true);
    }
    
    
    
    
    
    int data_node::search_subdata_node( string data_nodename ) const
    {
    	int where = -1;
    	for ( int i = 0; i < subdata_node.size(); i++ )
    	{
    		if ( subdata_node[i]->key == data_nodename )
    		{
    			where = i;
    			break;
    		}
    	}
    	return(where);
    }
    
    
    
    
    data_node* data_node::locate( string s ) const
    {
    	string		command = s;
    	vector<string>	listed_cmd;
    	string		pattern = "/";
        listed_cmd = split(command, pattern);
    
    	int size = 0;
    	for ( vector<string>::iterator iter = listed_cmd.begin(); iter != listed_cmd.end(); iter++ )
    	{
    		size++;
    	}
    
    	if ( size == 0 )
    	{
    		return(NULL);
    	}
    	vector<string>::iterator itor;
    	itor = listed_cmd.begin();
    	if ( size == 1 )
    	{
    	    if (*itor=="."||*itor=="..")
    	    {
    	        return father;
    	    }
    		int ps = search_subdata_node( *itor );
    		if ( ps == -1 )
    		{
    			return(NULL);
    		}
    		return(subdata_node[ps]);
    	}
    	const data_node *to = this;
    
    
    	/*
    	 * while(itor!=listed_cmd.end())
    	 * {
    	 * listed_cmd_c_str[j]=(char*)(*itor).c_str();
    	 * itor++;
    	 * j++;
    	 * if(j>=20){break;}
    	 * }
    	 */
    	for ( int i = 1; i < size; i++ )
    	{
    		if (*itor=="." || *itor=="..")
    		{
    		    to = to->father;
    		}
    		else
    		{
                int ps = to->search_subdata_node( *itor );
                if ( ps == -1 )
                {
                    return(NULL);
                }
    		    to = to->subdata_node[ps];
    		}
    	}
    	itor++;
    	int ps = to->search_subdata_node( *itor );
    	if ( ps == -1 )
    	{
    		return(NULL);
    	}
    	return(to->subdata_node[ps]);
    }
    
    
    
    
    
    
    void data_node::show(int format)
    {
        if (format != 0)
        {
            for (int i=0; i<(format-2)/2; i++)
            {
                cout<<"| ";
            }
            cout<<"|-";
        }
        cout<<key<<"="<<value<<endl;
        for (int i=0;i<subdata_node.size();i++)
        {
            subdata_node[i]->show(format+2);
        }
    }

	string data_node::get_value_of(string key)
    {
		data_node *child = locate (key);
		if (child != nullptr)
		{
			return child->value;
		}
		else
		{
			cout<<"未查找到子节点"<<key<<endl;
			return string("");
		}
	}
}
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

string default_filename = "autobackup.tree";//保存时的默认文件名
bool autosave = true;//是否自动保存

list<pair<void*,string> > preload_ptr_table;
list<string> mail_list;
#ifndef FOR_XES
#include "hackgame.h"
#endif
#include <assert.h>
using namespace std;

typedef unsigned char     uint8;
typedef unsigned int    uint32;

static uint8 alphabet_map[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static uint8 reverse_map[] =
{
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
     255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62, 255, 255, 255, 63,
     52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255, 255, 255, 255, 255,
     255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 255, 255, 255, 255, 255,
     255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
     41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 255, 255, 255, 255, 255
};

string bin2string(void *data, size_t size)
{
    const uint8 *text = (const uint8 *)data;
    uint32 text_len = (uint32)size;
    uint32 i, j;
    uint8* encode;
    encode = (uint8*)malloc(text_len/3*4 + 10);
    memset(encode, '\0', sizeof(encode));
    for (i = 0, j = 0; i+3 <= text_len; i+=3)
    {
        encode[j++] = alphabet_map[text[i]>>2];                             //取出第一个字符的前6位并找出对应的结果字符
        encode[j++] = alphabet_map[((text[i]<<4)&0x30)|(text[i+1]>>4)];     //将第一个字符的后2位与第二个字符的前4位进行组合并找到对应的结果字符
        encode[j++] = alphabet_map[((text[i+1]<<2)&0x3c)|(text[i+2]>>6)];   //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
        encode[j++] = alphabet_map[text[i+2]&0x3f];                         //取出第三个字符的后6位并找出结果字符
    }

    if (i < text_len)
    {
        uint32 tail = text_len - i;
        if (tail == 1)
        {
            encode[j++] = alphabet_map[text[i]>>2];
            encode[j++] = alphabet_map[(text[i]<<4)&0x30];
            encode[j++] = '=';
            encode[j++] = '=';
        }
        else //tail==2
        {
            encode[j++] = alphabet_map[text[i]>>2];
            encode[j++] = alphabet_map[((text[i]<<4)&0x30)|(text[i+1]>>4)];
            encode[j++] = alphabet_map[(text[i+1]<<2)&0x3c];
            encode[j++] = '=';
        }
    }
    encode[j] = '\0';
    string ret = string((char*)&(encode[0]));
    free(encode);
    return ret;
}
data_block string2bin(string str)
{
    const uint8 *code = (const uint8 *)str.c_str();
    uint32 code_len = (uint32)str.length();
    assert((code_len&0x03) == 0);  //如果它的条件返回错误，则终止程序执行。4的倍数。
    uint8* plain;
    plain = (uint8*)malloc(code_len / 4 * 3 + 10);
    memset(plain, '\0', sizeof(plain));
    uint32 i, j = 0;
    uint8 quad[4];
    for (i = 0; i < code_len; i+=4)
    {
        for (uint32 k = 0; k < 4; k++)
        {
            quad[k] = reverse_map[code[i+k]];//分组，每组四个分别依次转换为base64表内的十进制数
        }

        assert(quad[0]<64 && quad[1]<64);

        plain[j++] = (quad[0]<<2)|(quad[1]>>4); //取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的前2位进行组合

        if (quad[2] >= 64)
            break;
        else if (quad[3] >= 64)
        {
            plain[j++] = (quad[1]<<4)|(quad[2]>>2); //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应base64表的十进制数的前4位进行组合
            break;
        }
        else
        {
            plain[j++] = (quad[1]<<4)|(quad[2]>>2);
            plain[j++] = (quad[2]<<6)|quad[3];//取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合
        }
    }
    data_block ret;
    ret.data = malloc(j);
    memcpy(ret.data, plain, j);
    ret.size = j;
    free(plain);
    return ret;
}

string encoding(string str)
{
    const char *c=str.c_str();
    string buff1;
    for (int i=0;i<str.size();i++)
    {
        if (c[i]=='\\')
        {
            buff1 += "\\\\";
        }
        else
        {
            if (c[i]=='=')
            {
                buff1 += "\\e";
            }
            else
            {
                if (c[i]=='\n')
                {
                    buff1 += "\\n";
                }
                else
                {
                    buff1 += c[i];
                }
            }
        }
    }
    return buff1;
}

string decoding(string str)
{
    const char *c=str.c_str();
    string buff1;
    for (int i=0;i<str.size();)
    {
        if (c[i]=='\\' && c[i+1]=='\\')
        {
            buff1 += "\\";
            i += 2;
        }
        else
        {
            if (c[i]=='\\' && c[i+1]=='e')
            {
                buff1 += "=";
                i += 2;
            }
            else
            {
                if (c[i]=='\\' && c[i+1]=='n')
                {
                    buff1 += "\n";
                    i += 2;
                }
                else
                {
                    buff1 += c[i];
                    i += 1;
                }
            }
        }
    }
    return buff1;
}
#ifndef FOR_XES
#include "hackgame.h"
#endif
//εйl
#ifdef LINUX_OS
void localtime_s_f(struct tm *ts,time_t *timep)
{
    localtime_r(timep,ts);
}
#include <unistd.h>
#ifdef NO_DELAY
void delay(double time){}
#else
void delay(double time)//
{
    usleep(1000000*time);
}
#endif
void clearScreen()
{
    system("clear");
}
#else
#ifdef WINDOWS_OS
void localtime_s_f(struct tm *ts,time_t *timep)
{
    localtime_s(ts,timep);
}
#include <windows.h>
#ifdef NO_DELAY
void delay(double time){}
#else
void delay(double time)//
{
    Sleep(time*1000);
}
#endif
void clearScreen()
{
    system("cls");
}
#else
#ifdef TEST_WINDOWS
void clearScreen()
{
    system("clear");
}
void delay(double time)//
{
    usleep(1000000*time);
}
#else
#error Error opitions!
#endif
#endif
#endif
#ifndef FOR_XES
#include "hackgame.h"
#endif

//初始化游戏
//调用init_new_game()或init_from_file()
void loadgame()
{
    string input,a;
    //cout<<"输入new创建新游戏"<<endl;
    //cout<<"输入存档名称打开游戏"<<endl;
    while (true)
    {
        cout<<"------------------------"<<endl;
        cout<<"1.创建新游戏"<<endl;
        cout<<"2.从某个关卡开始游戏"<<endl;
        cout<<"3.加载存档"<<endl;
        cout<<"请输入编号选择:";
        getline(cin,input);
        if (input == "1")
        {
            init_new_game();
            break;
        }
     	else if (input == "2")
        {
            bool skip_set_username = false;
            string encoded_saving;
            cout<<"1.第1关"<<endl;
            cout<<"2.第2关"<<endl;
            cout<<"3.第3关"<<endl;
            cout<<"4.第4关"<<endl;
            cout<<"5.第5关"<<endl;
            cout<<"6.第6关"<<endl;
            cout<<"7.第7关"<<endl;
            cout<<"8.第8关"<<endl;
            cout<<"9.通关邮件"<<endl;
            getline(cin,a);
            if (a == "1")
            {
                encoded_saving = saving_task1;                
                skip_set_username = true;
            }
            else if (a == "2")
            {
                encoded_saving = saving_task2;
            }
            else if (a == "3")
            {
                encoded_saving = saving_task3;
            }
            else if (a == "4")
            {
                encoded_saving = saving_task4;
            }
            else if (a == "5")
            {
                encoded_saving = saving_task5;
            }
            else if (a == "6")
            {
                encoded_saving = saving_task6;
            }
            else if (a == "7")
            {
                encoded_saving = saving_task7;
            }
            else if (a == "8")
            {
                encoded_saving = saving_task8;
            }
            else if (a == "9")
            {
                encoded_saving = saving_end;
            }
            else
            {
                cout<<"请输入1至9的数字!"<<endl;
                continue;
            }
            //cout<<decoding(encoded_saving)<<endl<<endl;
            //cout<<decoding(encoded_saving);
            load_game_from_string(encoded_saving);
            if (!skip_set_username)
            {
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
            }
            clearScreen();
            break;
        }
        else if (input == "3")
        {
            cout<<"请输入存档名:";
            getline(cin,input);
            init_from_file(input);
            clearScreen();
            cout<<"欢迎回来！"<<endl;
            break;
        }
        cout<<"请输入1或2!"<<endl;
    }
}

void init_from_file(string filename)//从文件加载游戏
{
    DataTree::data_node *root = load_tree_from_file(filename);
    if (root == NULL)
    {
        cout<<"读入文件失败！程序退出";
        exit(0);
    }

    int version = get_version(root);
    cout<<"Saved Version:\t"<<to_string(version)<<endl;
    if (version > SAVING_VERSION)
    {
        cout<<"您的游戏版本过低，无法进行游戏！请联系开发者获取最新版本"<<endl;
        cout<<"开发者QQ:2492515121"<<endl;
        delay(999);
        exit(0);
    }
    if (version < SAVING_VERSION)
    {
        for (int i = version; i<SAVING_VERSION; i++)
        {
            root = up_date_game(root,i);
        }
    }
    
    if (load_game_from_tree(root) == false)
    {
        cout<<"加载失败！程序退出";
        exit(0);
    }
    default_filename = filename;
    root->remove_all();
    delete root;
    cout<<"加载程序顺利退出"<<endl;
    return;
}

//一个重要的函数
//初始化
//游戏启动时会被调用
void init_new_game()
{
    using namespace FileSystem;
    if (!DEBUG_FLAG)
    {
//        logo();//显示logo
    }
    //clearScreen();//清屏

    
    Internet = new net_node("Internet");//第一个网络节点。所有公网可访问的主机都挂在这里
    
    //临时指针
    Computer *temp;
    net_node *tempnode;
    
    //新建一个节点
    tempnode=new net_node(LOCAL_HOST);
    Internet->add_node(tempnode);
    
    //新建一个主机
    temp=new Computer("192.168.0.0");
    tempnode->add_node(temp->netnode,false);//连接这个主机的节点
    //temp->root->add_new_txt("1.txt","123");//创建文件
    temp->event_before_input=&task_1_1;//绑定设置剧情函数
    temp->name="localhost";
//    temp->locate_dir("/bin")->add_file(new file("mujs.exe",&exe_mujs));
    localhost=temp;
    
    temp=new Computer("192.168.0.1");
    tempnode->add_node(temp->netnode,false);
    
    temp=new Computer("192.168.0.2");
    tempnode->add_node(temp->netnode,false);
    
    temp=new Computer(COMPUTER1);
    Internet->add_node(temp->netnode);
    temp->style=telnet;
    temp->password="123456";//设密码
    temp->open_port("telnet",23);//打开端口
    temp->locate_dir("/bin")->add_file(new file("wget.exe",&exe_wget));//放文件
    
    temp=new Computer(COMPUTER2);
    Internet->add_node(temp->netnode);
    temp->style=telnet;
    temp->open_port("http",21);
    temp->root->add_new_dir("web");
    temp->locate_dir("/web")->add_file(new file("passguesser.exe",&exe_passguesser));
    if (DEBUG_FLAG)
    {
        temp->password="123456";
        temp->open_port("telnet",23);
    }
    
    temp=new Computer(COMPUTER3);
    Internet->add_node(temp->netnode);
    temp->style=telnet;
    temp->password="password123";
    temp->open_port("telnet",23);
    temp->locate_dir("/bin")->add_file(new file("portscan.exe",&exe_portscan));

    if (DEBUG_FLAG)
    {
        localhost->locate_dir("/bin")->add_file(new file("passguesser.exe",&exe_passguesser));
        localhost->locate_dir("/bin")->add_file(new file("portscan.exe",&exe_portscan));
        localhost->locate_dir("/bin")->add_file(new file("hvm.exe",&exe_hvm));
        localhost->locate_dir("/bin")->add_file(new file("wget.exe",&exe_wget));
        localhost->locate_dir("/bin")->add_file(new file("tracer.exe",&exe_tracer));
        //localhost->locate_dir("/bin")->add_file(new file("sniffer.exe",&exe_sniffer));
        localhost->event_before_input=&task_8_1;
    }else{
        localhost->event_before_input=&task_1_1;
    }
    string input;
    cout<<"您是否希望自动保存存档？（1:是 2:否） :";
    cin>>input;
    if (input == "1" || input == "是" || input == "y" || input == "Y" || input == "0")
    {
        autosave = true;
    }
    else
    {
        autosave = false;
    }
    cout<<"您希望保存时的文件名是什么？ :";
    cin>>input;
    default_filename = input;
    getline(cin, input);//把回车读进来
}

void save_game_to_file(string filename)
{
    ptr2id table;
    file_writer fw;
    fw.open(filename);
    fw.write("version_code",to_string(SAVING_VERSION));
    fw.write("preload");
    fw.right();
	/*
        fw.write(table.getid(NULL),"NULL");
        fw.write(table.getid((void*)&task_1_1),"task_1_1");
        fw.write(table.getid((void*)&task_1_2),"task_1_2");
        fw.write(table.getid((void*)&task_2_1),"task_2_1");
        fw.write(table.getid((void*)&task_2_2),"task_2_2");
        fw.write(table.getid((void*)&task_3_1),"task_3_1");
        fw.write(table.getid((void*)&task_3_2),"task_3_2");
        fw.write(table.getid((void*)&task_4_1),"task_4_1");
        fw.write(table.getid((void*)&task_4_2),"task_4_2");
        fw.write(table.getid((void*)&task_5_1),"task_5_1");
        fw.write(table.getid((void*)&task_5_2),"task_5_2");
        fw.write(table.getid((void*)&task_6_1),"task_6_1");
        fw.write(table.getid((void*)&task_6_2),"task_6_2");
        fw.write(table.getid((void*)&task_7_1),"task_7_1");
        fw.write(table.getid((void*)&task_7_2),"task_7_2");
        fw.write(table.getid((void*)&task_7_3),"task_7_3");
        fw.write(table.getid((void*)&task_8_1),"task_8_1");
        fw.write(table.getid((void*)&task_8_2),"task_8_2");
        fw.write(table.getid((void*)&task_9_1),"task_9_1");
        fw.write(table.getid((void*)&task_9_2),"task_9_2");
        fw.write(table.getid((void*)&exe_wget),"exe_wget");
        fw.write(table.getid((void*)&exe_help),"exe_help");
        fw.write(table.getid((void*)&exe_passguesser),"exe_passguesser");
        fw.write(table.getid((void*)&exe_portscan),"exe_portscan");
        fw.write(table.getid((void*)&exe_hvm),"exe_hvm");
        fw.write(table.getid((void*)&exe_mail),"exe_mail");
        fw.write(table.getid((void*)&exe_telnet),"exe_telnet");
        fw.write(table.getid((void*)&exe_tree),"exe_tree");
        fw.write(table.getid((void*)&exe_ipconfig),"exe_ipconfig");
        fw.write(table.getid((void*)&exe_ping),"exe_ping");
        fw.write(table.getid((void*)&exe_savegame),"exe_savegame");
        fw.write(table.getid((void*)&exe_myexe),"exe_myexe");
        fw.write(table.getid((void*)&exe_tracer),"exe_tracer");
        fw.write(table.getid((void*)&exe_sniffer),"exe_sniffer");
//        fw.write(table.getid((void*)&exe_mujs),"exe_mujs");
        */
    for(auto i=preload_ptr_table.begin();i!=preload_ptr_table.end();i++)
    {
        fw.write(table.getid(i->first),i->second);
    }
    fw.left();

    fw.write("global");
    fw.right();
        fw.write("task_number",to_string(task_number));
        fw.write("sub_task_number",to_string(sub_task_number));
        fw.write("Internet",table.getid(Internet));
        fw.write("localhost",table.getid(localhost));
        fw.write("default_filename",default_filename);
        if (autosave == true)
        {
            fw.write("autosave","true");
        }
        else
        {
            fw.write("autosave","false");
        }
    fw.left();

    
    stack<int> stk;
    stack<Computer*> computers;
    stack<FileSystem::dir*> roots;
    stack<FileSystem::file*> files;
    net_node *now=Internet;
    stk.push(0);
    fw.write("net_node");
    fw.right();
    while(true){
        if (stk.top() < now->subnet.size())
        {
            now=now->subnet[stk.top()];
            stk.push(0);
        }
        else
        {
            //do
            if (now->binded_computer)
            {
                computers.push(now->binded_computer);
            }
            fw.write(table.getid((void*)now));
            fw.right();
                fw.write("ip",now->ip);
                fw.write("binded_computer",table.getid((void*)now->binded_computer));
                fw.write("father",table.getid((void*)now->father));
                fw.write("net_map");
                fw.right();
                    map<string,net_node*>::iterator iter;
                    int k=0;
                    for(iter = now->net_map.begin(); iter != now->net_map.end(); iter++)
                    {
                        fw.write(to_string (k),iter->first+"$"+table.getid((void*)iter->second));
                        k++;
                    }
                fw.left();
                fw.write("subnet");
                fw.right();
                    for (int i=0; i<now->subnet.size(); i++)
                    {
                        fw.write(to_string (i),table.getid((void*)now->subnet[i]));
                    }
                fw.left();
                /*
                fw.right();
                fw.left();
                */
                //fw.write("father",table.getid((void*)now->father));
                //fw.write("father",table.getid((void*)now->father));
            fw.left();
            
            stk.pop();
            if (stk.empty())
            {
                break;
            }
            now=now->father;
            stk.top() +=1;
        }
    }
    fw.left();
    fw.write("computer");
    fw.right();
    while(!computers.empty())
    {
        Computer *temp=computers.top();
        computers.pop();
        roots.push(temp->root);
        fw.write(table.getid((void*)temp));
        fw.right();
            //fw.write("dc_signal",to_string(temp->dc_signal));
            fw.write("event_before_input",table.getid((void*)temp->event_before_input));
            fw.write("username",temp->username);
            fw.write("password",temp->password);
            fw.write("name",temp->name);
            fw.write("style");
            fw.right();
                fw.write("login_output", temp->style.login_output);
                fw.write("pass_output", temp->style.pass_output);
                fw.write("login_successful", temp->style.login_successful);
                fw.write("login_wrong", temp->style.login_wrong);
                fw.write("login_fail", temp->style.login_fail);
                fw.write("before_ip", temp->style.before_ip);
                fw.write("between_ip", temp->style.between_ip);
                fw.write("after_ip", temp->style.after_ip);
                fw.write("getinput", temp->style.getinput);
            fw.left();
            fw.write("port_list");
            fw.right();
            for (int i=0;i<temp->port_list.size();i++)
            {
                fw.write(to_string(i),temp->port_list[i].name+"$"+to_string(temp->port_list[i].port));
            }
            fw.left();
            fw.write("netnode",table.getid((void*)temp->netnode));
            fw.write("root",table.getid((void*)temp->root));
        fw.left();
    }
    fw.left();
    fw.write("dir");
    fw.right();
    while(!roots.empty())
    {
        FileSystem::dir *temp=roots.top();
        roots.pop();
        stk.push(0);
        while(true)
        {
            if (stk.top() < temp->subdir.size())
            {
                temp=temp->subdir[stk.top()];
                stk.push(0);
            }
            else
            {
                //do
                fw.write(table.getid((void*)temp));
                fw.right();
                    fw.write("father",table.getid((void*)temp->father));
                    fw.write("name",temp->name);
                    fw.write("content");
                    fw.right();
                    for (int i=0;i<temp->content.size();i++)
                    {
                        files.push(temp->content[i]);
                        fw.write(to_string(i),table.getid((void*)temp->content[i]));
                    }
                    fw.left();
                    fw.write("subdir");
                    fw.right();
                    for (int i=0;i<temp->subdir.size();i++)
                    {
                        fw.write(to_string(i),table.getid((void*)temp->subdir[i]));
                    }
                    fw.left();
                fw.left();
                
                stk.pop();
                if (stk.empty())
                {
                    break;
                }
                temp=temp->father;
                stk.top() +=1;
            }
        }
    }
    fw.left();
    fw.write("file");
    fw.right();
    while (!files.empty())
    {
        FileSystem::file *temp=files.top();
        files.pop();
        fw.write(table.getid((void*)temp));
        fw.right();
            fw.write("name",temp->name);
            if (((char*)(temp->data))[0]=='E' && 
                ((char*)(temp->data))[1]=='X' && 
                ((char*)(temp->data))[2]=='E' )
            {
                void *addr=temp->data;
                addr=(char*)addr+3;
                fw.write("is_exe","true");
                fw.write("address",table.getid(*(void**)addr));
            }
            else
            {
                fw.write("is_exe","false");   
                fw.write("data",bin2string(temp->data,temp->size/sizeof(char)));
            }
        fw.left();
    }
    fw.left();
    fw.save();
    return;
}

DataTree::data_node* load_tree_from_file(string filename)
{
    DataTree::data_node *root = DataTree::new_root();
    DataTree::data_node *now = root;
    ifstream fcin(filename);
    string input;
    string key;
    string value;
    vector<string> cutted_input;
    if (! fcin.is_open())
    {
        cout << "打开文件失败"<<endl;
        return NULL;
    }
    int format_before = 0;
    int format_now = -1;
    //getline(fcin,input);
    //cout<<"Debug:b"<<endl;
    while (true)
    {
        if (fcin.eof())
        {
            break;
        }
        getline(fcin,input);
        format_before = format_now;
        format_now=0;
        while(input.c_str()[format_now]=='\t')
        {
            format_now++;
        }
        input = input.substr(format_now,input.length()-format_now);
        cutted_input = split(input,"=");
    //cout<<"Debug:1"<<endl;
        if (cutted_input.size()<1)
        {
            cout<<input<<"可能有格式错误！"<<endl;
        }
        else if (cutted_input.size()==1)
        {
            key =  decoding(cutted_input[0]);
            value = "";
        }
        else
        {
            key = decoding(cutted_input[0]);
            value = decoding(cutted_input[1]);
        }
        if (format_now > format_before)
        {
            if ((format_now-format_before)>1)
            {
                cout<<"输入的tab过多！加载失败"<<endl;
                return NULL;
            }
            now = now->add(key,value);
        }
        else if (format_now < format_before)
        {
            for (int i=0; i<(format_before-format_now+1); i++)
            {
                now = now->father;
            }
            now = now->add(key,value);
        }
        else
        {
            now = now->father;
            now = now->add(key,value);
        }
    //cout<<"Debug:2"<<endl;
    }
    //cout<<"Debug:e"<<endl;
    return root;
}

/*
bool load_game_from_tree(DataTree::data_node *root)
{
    id2ptr table;
    DataTree::data_node *now = root;
    DataTree::data_node *temp1 = root;
    DataTree::data_node *temp2 = root;
    DataTree::data_node *temp3 = root;
    DataTree::data_node *temp4 = root;

    now = root->locate("preload");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        if (temp1->value == "NULL")
        {
            table.load(temp1->key,NULL);
        }
        else if (temp1->value == "task_1_1")
        {
            table.load(temp1->key,(void*)task_1_1);
        }
        else if (temp1->value == "task_1_2")
        {
            table.load(temp1->key,(void*)task_1_2);
        }
        else if (temp1->value == "task_2_1")
        {
            table.load(temp1->key,(void*)task_2_1);
        }
        else if (temp1->value == "task_2_2")
        {
            table.load(temp1->key,(void*)task_2_2);
        }
        else if (temp1->value == "task_3_1")
        {
            table.load(temp1->key,(void*)task_3_1);
        }
        else if (temp1->value == "task_3_2")
        {
            table.load(temp1->key,(void*)task_3_2);
        }
        else if (temp1->value == "task_4_1")
        {
            table.load(temp1->key,(void*)task_4_1);
        }
        else if (temp1->value == "task_4_2")
        {
            table.load(temp1->key,(void*)task_4_2);
        }
        else if (temp1->value == "task_5_1")
        {
            table.load(temp1->key,(void*)task_5_1);
        }
        else if (temp1->value == "task_5_2")
        {
            table.load(temp1->key,(void*)task_5_2);
        }
        else if (temp1->value == "task_6_1")
        {
            table.load(temp1->key,(void*)task_6_1);
        }
        else if (temp1->value == "task_6_2")
        {
            table.load(temp1->key,(void*)task_6_2);
        }
        else if (temp1->value == "task_7_1")
        {
            table.load(temp1->key,(void*)task_7_1);
        }
        else if (temp1->value == "exe_wget")
        {
            table.load(temp1->key,(void*)exe_wget);
        }
        else if (temp1->value == "exe_help")
        {
            table.load(temp1->key,(void*)exe_help);
        }
        else if (temp1->value == "exe_passguesser")
        {
            table.load(temp1->key,(void*)exe_passguesser);
        }
        else if (temp1->value == "exe_portscan")
        {
            table.load(temp1->key,(void*)exe_portscan);
        }
        else if (temp1->value == "exe_hvm")
        {
            table.load(temp1->key,(void*)exe_hvm);
        }
        else if (temp1->value == "exe_mail")
        {
            table.load(temp1->key,(void*)exe_mail);
        }
        else if (temp1->value == "exe_telnet")
        {
            table.load(temp1->key,(void*)exe_telnet);
        }
        else if (temp1->value == "exe_tree")
        {
            table.load(temp1->key,(void*)exe_tree);
        }
        else if (temp1->value == "exe_ipconfig")
        {
            table.load(temp1->key,(void*)exe_ipconfig);
        }
        else if (temp1->value == "exe_ping")
        {
            table.load(temp1->key,(void*)exe_ping);
        }
        else if (temp1->value == "exe_savegame")
        {
            table.load(temp1->key,(void*)exe_savegame);
        }
        else if (temp1->value == "exe_myexe")
        {
            table.load(temp1->key,(void*)exe_myexe);
        }
        else
        {
            cout<<"不存在的预定义指针"<<temp1->value<<endl;
        }

    }

    now = root->locate("global");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        if (temp1->key == "task_number")
        {
            task_number = atoi(temp1->value.c_str());
        }
        else if (temp1->key == "sub_task_number")
        {
            sub_task_number = atoi(temp1->value.c_str());
        }
        else if (temp1->key == "Internet")
        {
            table.bind(temp1->value,(void**)&Internet);
        }
        else if (temp1->key == "localhost")
        {
            table.bind(temp1->value,(void**)&localhost);
        }
        else
        {
            cout<<"不存在全局变量"<<temp1->value<<endl;
        }
    }

    now = root->locate("net_node");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        net_node *nnd = new net_node(temp1->locate("ip")->value);
        table.load(temp1->key,(void*)nnd);
        table.bind(temp1->locate("binded_computer")->value,(void**)&(nnd->binded_computer));
        table.bind(temp1->locate("father")->value,(void**)&(nnd->father));
        temp2 = temp1->locate("net_map");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
            temp3 = temp2->subdata_node[g];
   // cout<<"DEBUG:test1"<<endl;
            vector<string> temp5 = split(temp3->value,"$");
  //  cout<<"DEBUG:testtemp"<<endl;
            nnd->add_map(temp5[0],NULL);
            //table.bind(temp5[1],(void**)&(nnd->net_map[temp5[0]]));
            table.bind(temp5[1],(map<string,void*>*)&(nnd->net_map),temp5[0]);
        }
        temp2 = temp1->locate("subnet");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
 //   cout<<"DEBUG:test3"<<endl;
            temp3 = temp2->subdata_node[g];
            nnd->subnet.push_back(NULL);
            //table.bind(temp5[1],(void**)&(nnd->subnet[nnd->subnet.size()-1]));
            table.bind(temp3->value,(vector<void*>*)&(nnd->subnet),nnd->subnet.size()-1);
  //  cout<<"DEBUG:test4"<<endl;
        }
    }

    
    now = root->locate("computer");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        Computer *nct = new Computer();
        table.load(temp1->key,(void**)nct);
        table.bind(temp1->get_value_of("event_before_input") , 
                    (void**)&(nct->event_before_input));
        nct->username = temp1->get_value_of("username");
        nct->password = temp1->get_value_of("password");
        nct->name = temp1->get_value_of("name");
        nct->style.login_output     = temp1->get_value_of("style/login_output");
        nct->style.pass_output      = temp1->get_value_of("style/pass_output");
        nct->style.login_successful = temp1->get_value_of("style/login_successful");
        nct->style.login_wrong      = temp1->get_value_of("style/login_wrong");
        nct->style.login_fail       = temp1->get_value_of("style/login_fail");
        nct->style.before_ip        = temp1->get_value_of("style/before_ip");
        nct->style.between_ip       = temp1->get_value_of("style/between_ip");
        nct->style.after_ip         = temp1->get_value_of("style/after_ip");
        nct->style.getinput         = temp1->get_value_of("style/getinput");
        temp2 = temp1->locate("port_list");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
            temp3 = temp2->subdata_node[g];
            vector<string> temp5 = split(temp3->value,"$");
            nct->open_port(temp5[0],atoi(temp5[1].c_str()));
        }
        table.bind(temp1->get_value_of("netnode") , 
                    (void**)&(nct->netnode));
        table.bind(temp1->get_value_of("root") , 
                    (void**)&(nct->root));
        table.bind(temp1->get_value_of("root") , 
                    (void**)&(nct->dir_now));
    }
    

    
    now = root->locate("dir");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        FileSystem::dir *ndr = new FileSystem::dir(temp1->get_value_of("name"));
        table.load(temp1->key,(void**)ndr);
        table.bind(temp1->get_value_of("father") , 
                    (void**)&(ndr->father));
        temp2 = temp1->locate("content");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
            temp3 = temp2->subdata_node[g];
            ndr->content.push_back(nullptr);
            table.bind(temp3->value,(vector<void*>*)&(ndr->content),ndr->content.size()-1);
        }
        temp2 = temp1->locate("subdir");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
            temp3 = temp2->subdata_node[g];
            ndr->subdir.push_back(nullptr);
            table.bind(temp3->value,(vector<void*>*)&(ndr->subdir),ndr->subdir.size()-1);
        }
    }

    now = root->locate("file");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        if (temp1->get_value_of("is_exe") == "true")
        {
            FileSystem::file *nfl = new FileSystem::file(
                temp1->get_value_of("name"),
                nullptr
            );
            table.load(temp1->key,(void**)nfl);
            table.bind(
                temp1->get_value_of("address"),
                (void**)&(
                    ((char*)nfl->data)[3]
                )
            );
        }
        else
        {
            data_block data = string2bin(temp1->get_value_of("data"));
            FileSystem::file *nfl = new FileSystem::file(
                temp1->get_value_of("name"),
                data.size,
                (void*)data.data
            );
            table.load(temp1->key,(void**)nfl);
        }
    }
    table.show();
    return true;
}
*/

bool load_game_from_tree(DataTree::data_node *root)
{
    id2ptr table;
    DataTree::data_node *now = root;
    DataTree::data_node *temp1 = root;
    DataTree::data_node *temp2 = root;
    DataTree::data_node *temp3 = root;
    DataTree::data_node *temp4 = root;
    
    if (!root)
    {
        cout << "load_game_from_tree():根结点为nullptr，无法加载！"<<endl;
        return false;
    }
    
    now = root->locate("preload");
    if (now == nullptr)
    {
        cout<<"缺少必要的preload数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        /*
        if (temp1->value == "NULL")
        {
            table.load(temp1->key,NULL);
        }
        else if (temp1->value == "task_1_1")
        {
            table.load(temp1->key,(void*)task_1_1);
        }
        else if (temp1->value == "task_1_2")
        {
            table.load(temp1->key,(void*)task_1_2);
        }
        else if (temp1->value == "task_2_1")
        {
            table.load(temp1->key,(void*)task_2_1);
        }
        else if (temp1->value == "task_2_2")
        {
            table.load(temp1->key,(void*)task_2_2);
        }
        else if (temp1->value == "task_3_1")
        {
            table.load(temp1->key,(void*)task_3_1);
        }
        else if (temp1->value == "task_3_2")
        {
            table.load(temp1->key,(void*)task_3_2);
        }
        else if (temp1->value == "task_4_1")
        {
            table.load(temp1->key,(void*)task_4_1);
        }
        else if (temp1->value == "task_4_2")
        {
            table.load(temp1->key,(void*)task_4_2);
        }
        else if (temp1->value == "task_5_1")
        {
            table.load(temp1->key,(void*)task_5_1);
        }
        else if (temp1->value == "task_5_2")
        {
            table.load(temp1->key,(void*)task_5_2);
        }
        else if (temp1->value == "task_6_1")
        {
            table.load(temp1->key,(void*)task_6_1);
        }
        else if (temp1->value == "task_6_2")
        {
            table.load(temp1->key,(void*)task_6_2);
        }
        else if (temp1->value == "task_7_1")
        {
            table.load(temp1->key,(void*)task_7_1);
        }
        else if (temp1->value == "task_7_2")
        {
            table.load(temp1->key,(void*)task_7_2);
        }
        else if (temp1->value == "task_7_3")
        {
            table.load(temp1->key,(void*)task_7_3);
        }
        else if (temp1->value == "task_8_1")
        {
            table.load(temp1->key,(void*)task_8_1);
        }
        else if (temp1->value == "task_8_2")
        {
            table.load(temp1->key,(void*)task_8_2);
        }
        else if (temp1->value == "task_9_1")
        {
            table.load(temp1->key,(void*)task_9_1);
        }
        else if (temp1->value == "task_9_2")
        {
            table.load(temp1->key,(void*)task_9_2);
        }
        else if (temp1->value == "exe_wget")
        {
            table.load(temp1->key,(void*)exe_wget);
        }
        else if (temp1->value == "exe_help")
        {
            table.load(temp1->key,(void*)exe_help);
        }
        else if (temp1->value == "exe_passguesser")
        {
            table.load(temp1->key,(void*)exe_passguesser);
        }
        else if (temp1->value == "exe_portscan")
        {
            table.load(temp1->key,(void*)exe_portscan);
        }
        else if (temp1->value == "exe_hvm")
        {
            table.load(temp1->key,(void*)exe_hvm);
        }
        else if (temp1->value == "exe_mail")
        {
            table.load(temp1->key,(void*)exe_mail);
        }
        else if (temp1->value == "exe_telnet")
        {
            table.load(temp1->key,(void*)exe_telnet);
        }
        else if (temp1->value == "exe_tree")
        {
            table.load(temp1->key,(void*)exe_tree);
        }
        else if (temp1->value == "exe_ipconfig")
        {
            table.load(temp1->key,(void*)exe_ipconfig);
        }
        else if (temp1->value == "exe_ping")
        {
            table.load(temp1->key,(void*)exe_ping);
        }
        else if (temp1->value == "exe_savegame")
        {
            table.load(temp1->key,(void*)exe_savegame);
        }
        else if (temp1->value == "exe_myexe")
        {
            table.load(temp1->key,(void*)exe_myexe);
        }
        else if (temp1->value == "exe_tracer")
        {
            table.load(temp1->key,(void*)exe_tracer);
        }
        else if (temp1->value == "exe_sniffer")
        {
            table.load(temp1->key,(void*)exe_sniffer);
        }
        else if (temp1->value == "exe_mujs")
        {
            table.load(temp1->key,(void*)exe_undefined);
        }
        else
        {
            cout<<"不存在的预定义指针"<<temp1->value<<endl;
        }
        */
        bool error = true;
        for(auto i=preload_ptr_table.begin();i!=preload_ptr_table.end();i++)
        {
            if (temp1->value == i->second)
            {
                table.load(temp1->key,i->first);
                error = false;
                break;
            }
        }
        if (error)
        {
            cout<<"不存在的预定义指针"<<temp1->value<<endl;
        }
    }

    now = root->locate("global");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        if (temp1->key == "task_number")
        {
            task_number = atoi(temp1->value.c_str());
        }
        else if (temp1->key == "sub_task_number")
        {
            sub_task_number = atoi(temp1->value.c_str());
        }
        else if (temp1->key == "Internet")
        {
            table.bind(temp1->value,(void**)&Internet);
        }
        else if (temp1->key == "localhost")
        {
            table.bind(temp1->value,(void**)&localhost);
        }
        /*
        else if (temp1->key == "default_filename")
        {
            default_filename = temp1->value;
        }
        */
        else if (temp1->key == "autosave")
        {
            if (temp1->value == "true")
            {
                autosave = true;
            }
            else if (temp1->value == "false")
            {
                autosave = false;
            }
            else
            {
                cout<<"项目global/autosave应该为布尔值，但'"<<encoding(temp1->value)<<"'既不是true也不是false"<<endl;
            }
        }
        else
        {
            cout<<"不存在全局变量"<<temp1->key<<endl;
        }
    }

    now = root->locate("net_node");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        net_node *nnd = new net_node(temp1->locate("ip")->value);
        table.load(temp1->key,(void*)nnd);
        table.bind(temp1->locate("binded_computer")->value,(void**)&(nnd->binded_computer));
        table.bind(temp1->locate("father")->value,(void**)&(nnd->father));
        temp2 = temp1->locate("net_map");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
            temp3 = temp2->subdata_node[g];
   // cout<<"DEBUG:test1"<<endl;
            vector<string> temp5 = split(temp3->value,"$");
  //  cout<<"DEBUG:testtemp"<<endl;
            nnd->add_map(temp5[0],NULL);
            //table.bind(temp5[1],(void**)&(nnd->net_map[temp5[0]]));
            table.bind(temp5[1],(map<string,void*>*)&(nnd->net_map),temp5[0]);
        }
        temp2 = temp1->locate("subnet");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
 //   cout<<"DEBUG:test3"<<endl;
            temp3 = temp2->subdata_node[g];
            nnd->subnet.push_back(NULL);
            //table.bind(temp5[1],(void**)&(nnd->subnet[nnd->subnet.size()-1]));
            table.bind(temp3->value,(vector<void*>*)&(nnd->subnet),nnd->subnet.size()-1);
  //  cout<<"DEBUG:test4"<<endl;
        }
    }

    
    now = root->locate("computer");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        Computer *nct = new Computer();
        table.load(temp1->key,(void**)nct);
        table.bind(temp1->get_value_of("event_before_input") , 
                    (void**)&(nct->event_before_input));
        nct->username = temp1->get_value_of("username");
        nct->password = temp1->get_value_of("password");
        nct->name = temp1->get_value_of("name");
        nct->style.login_output     = temp1->get_value_of("style/login_output");
        nct->style.pass_output      = temp1->get_value_of("style/pass_output");
        nct->style.login_successful = temp1->get_value_of("style/login_successful");
        nct->style.login_wrong      = temp1->get_value_of("style/login_wrong");
        nct->style.login_fail       = temp1->get_value_of("style/login_fail");
        nct->style.before_ip        = temp1->get_value_of("style/before_ip");
        nct->style.between_ip       = temp1->get_value_of("style/between_ip");
        nct->style.after_ip         = temp1->get_value_of("style/after_ip");
        nct->style.getinput         = temp1->get_value_of("style/getinput");
        temp2 = temp1->locate("port_list");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
            temp3 = temp2->subdata_node[g];
            vector<string> temp5 = split(temp3->value,"$");
            nct->open_port(temp5[0],atoi(temp5[1].c_str()));
        }
        table.bind(temp1->get_value_of("netnode") , 
                    (void**)&(nct->netnode));
        table.bind(temp1->get_value_of("root") , 
                    (void**)&(nct->root));
        table.bind(temp1->get_value_of("root") , 
                    (void**)&(nct->dir_now));
    }
    

    
    now = root->locate("dir");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        FileSystem::dir *ndr = new FileSystem::dir(temp1->get_value_of("name"));
        table.load(temp1->key,(void**)ndr);
        table.bind(temp1->get_value_of("father") , 
                    (void**)&(ndr->father));
        temp2 = temp1->locate("content");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
            temp3 = temp2->subdata_node[g];
            ndr->content.push_back(nullptr);
            table.bind(temp3->value,(vector<void*>*)&(ndr->content),ndr->content.size()-1);
        }
        temp2 = temp1->locate("subdir");
        for (int g=0; g<temp2->subdata_node.size(); g++)
        {
            temp3 = temp2->subdata_node[g];
            ndr->subdir.push_back(nullptr);
            table.bind(temp3->value,(vector<void*>*)&(ndr->subdir),ndr->subdir.size()-1);
        }
    }

    now = root->locate("file");
    if (now == nullptr)
    {
        cout<<"缺少必要的数据！"<<endl;
        return false;
    }
    for (int k=0; k<now->subdata_node.size(); k++)
    {
        temp1 = now->subdata_node[k];
        if (temp1->get_value_of("is_exe") == "true")
        {
            FileSystem::file *nfl = new FileSystem::file(
                temp1->get_value_of("name"),
                nullptr
            );
            table.load(temp1->key,(void**)nfl);
            table.bind(
                temp1->get_value_of("address"),
                (void**)&(
                    ((char*)nfl->data)[3]
                )
            );
        }
        else
        {
            data_block data = string2bin(temp1->get_value_of("data"));
            FileSystem::file *nfl = new FileSystem::file(
                temp1->get_value_of("name"),
                data.size,
                (void*)data.data
            );
            table.load(temp1->key,(void**)nfl);
        }
    }
    table.show();
    return true;
}

int get_version(DataTree::data_node *root)
{
    DataTree::data_node *version_node = root->locate("version_code");
    if (version_node == NULL)
    {
        return 0;
    }
    else
    {
        return atoi(version_node->value.c_str());
    }
}

DataTree::data_node* up_date_game(DataTree::data_node *root,int old_version)
{
    DataTree::data_node *temp;
    DataTree::data_node *temp2;
    string input;
    switch (old_version)
    {
    case 0:
        cout<<"Update Game Saving From Version 0 to Version 1"<<endl;
        root->locate("preload")->add("id_update_temp_1","task_7_2");
        root->locate("preload")->add("id_update_temp_2","task_7_3");
        root->locate("preload")->add("id_update_temp_3","task_8_1");
        root->locate("preload")->add("id_update_temp_4","exe_tracer");
        temp = root->locate("computer");
        for (int i=0; i<temp->subdata_node.size(); i++)
        {
            temp->subdata_node[i]->locate("style/between_ip")->value = "@";
        }
        if (atoi(root->get_value_of("global/task_number").c_str())  >= 7)
        {
            string id_of_localhost = root->get_value_of("global/localhost");
            root->locate("computer/"+id_of_localhost+"/event_before_input")->value = "id000014";
        }
        temp = root->locate("file");
        for (int i=0; i<temp->subdata_node.size(); i++)
        {
            temp2 = temp->subdata_node[i];
            if (temp2->get_value_of("name") == "portscan.exe")
            {
                if (temp2->get_value_of("is_exe") != "true")
                {
                    temp2->locate("is_exe")->value = "true";
                    temp2->add("address","id000018");
                }
            }
        }
        //load_game_from_tree (root);
        //save_game_to_file ("~temp_file_for_GHG_updating_to_v1.temp");
        //root = load_tree_from_file("~temp_file_for_GHG_updating_to_v1.temp");
        break;
    case 1:
        cout<<"Update Game Saving From Version 1 to Version 2"<<endl;
        root->locate("preload")->add("id_update_temp_11","task_8_2");
        root->locate("preload")->add("id_update_temp_12","task_9_1");
        root->locate("preload")->add("id_update_temp_13","task_9_2");
        root->locate("preload")->add("id_update_temp_14","exe_sniffer");
        if (atoi(root->get_value_of("global/task_number").c_str())  >= 7)
        {
            string id_of_localhost = root->get_value_of("global/localhost");
            cout<<"computer/"+id_of_localhost+"/event_before_input"<<endl;
            root->locate("computer/"+id_of_localhost)->locate("event_before_input")->value = "id000017";
        }
        break;
    case 2:
        cout<<"您是否希望自动保存存档？（1:是 2:否） :";
        cin>>input;
        if (input == "1" || input == "是" || input == "y" || input == "Y" || input == "0")
        {
            root->locate("global")->add("autosave","true");
        }
        else
        {
            root->locate("global")->add("autosave","false");
        }
        /*
        cout<<"您希望保存时的文件名是什么？ :";
        cin>>input;
        default_filename = input;
        */
        getline(cin, input);//把回车读进来
        break;
    /*
    case 3:
        
        break;
    */
    default:
        cout<<"up_data_game():\tCase Undefined!!!\n";
        break;
    }
    return root;
}

bool check_and_autosave()
{
    if (autosave)
    {
        save_game_to_file(default_filename);
        return true;
    }
    else
    {
        return false;
    }
}

bool load_game_from_string(string game)//从文本加载游戏
{
    return load_game_from_tree(load_tree_from_string(game));
}
DataTree::data_node* load_tree_from_string(string game)//从文本加载键值树
{
    //代码从load_tree_from_file复制并修改
    DataTree::data_node *root = DataTree::new_root();
    DataTree::data_node *now = root;
    istringstream ss(game);
    string input;
    string key;
    string value;
    vector<string> cutted_input;
    int format_before = 0;
    int format_now = -1;
    //getline(fcin,input);
    //cout<<"Debug:b"<<endl;
    while (true)
    {
        if (ss.eof())
        {
            break;
        }
        getline(ss,input);
        format_before = format_now;
        format_now=0;
        while(input.c_str()[format_now]=='\t')
        {
            format_now++;
        }
        input = input.substr(format_now,input.length()-format_now);
        cutted_input = split(input,"=");
    //cout<<"Debug:1"<<endl;
        if (cutted_input.size()<1)
        {
            cout<<input<<"可能有格式错误！"<<endl;
        }
        else if (cutted_input.size()==1)
        {
            key =  decoding(cutted_input[0]);
            value = "";
        }
        else
        {
            key = decoding(cutted_input[0]);
            value = decoding(cutted_input[1]);
        }
        if (format_now > format_before)
        {
            if ((format_now-format_before)>1)
            {
                cout<<"输入的tab过多！加载失败"<<endl;
                return NULL;
            }
            now = now->add(key,value);
        }
        else if (format_now < format_before)
        {
            for (int i=0; i<(format_before-format_now+1); i++)
            {
                now = now->father;
            }
            now = now->add(key,value);
        }
        else
        {
            now = now->father;
            now = now->add(key,value);
        }
    //cout<<"Debug:2"<<endl;
    }
    //cout<<"Debug:e"<<endl;
    return root;
}


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
#ifndef FOR_XES
#include "hackgame.h"
#endif

int exe_mail(int n,const char **t,Computer *c){
    cout<<"Hmail邮件系统\n-----------------------------\n";
    int number=task_number;
    if (n>1)
    {
        stringstream stm;
        stm<<t[1];
        stm>>number;
        if (!DEBUG_FLAG&&task_number<number)
        {
            cout<<"邮件不存在！";
            return 0;
        }
    }
    //cout<<number;
    switch (number)
    {
    case 1:
        cout<<localhost->username<<",您好!\n\n";
        cout<<"我是你的朋友Nic\n\n";
        cout<<"昨天你说你想当黑客，我决定教教你。不过，你得先通过我的几个小测试。\n\n";
        cout<<"你知道telnet吗？这是一个连接到远程计算机的工具。你需要学会如何使用它。\n\n现在需要你用Telnet连接到我的一个服务器上。我服务器的IP地址是29.53.103.3,用户名是root，密码是123456。连接成功后，请你立即用dc指令断开连接。\n\n你断开连接后，我会再给你寄一封邮件\n\n如果你有什么问题，可以使用系统的help指令。那里面有各种指令的使用说明。\n\n";
        cout<<"(提示：使用命令telnet 29.53.103.3)\n\n";
        cout<<"有关信息：\n\n";
        cout<<"     指令：telnet 29.53.103.3\n\n";
        cout<<"     用户名：root\n\n";
        cout<<"     密码：123456\n\n";
        cout<<"                                nic\n\n";
        break;
    case 2:
        cout<<localhost->username<<",您好!\n";
        cout<<"\n";
        cout<<"恭喜，您成功通过了考验的第一关。\n";
        cout<<"\n";
        cout<<"我忽然想起来你没有一个叫wget.exe的非常重要的程序。这是一个用来从网站上下载程序的软件，有很多用处。我打算让你把他弄过来。\n";
        cout<<"\n";
        cout<<"在上次让你练习telnet的那台主机上，存有一份wget.exe。你得在连接一次29.53.103.3，然后用scp指令把它复制过来。\n";
        cout<<"\n";
        cout<<"你只需要在登录成功后，用指令 cd bin 进入那台主机的bin文件夹，然后用 scp wget.exe 指令把文件复制到你的电脑上。\n";
        cout<<"\n";
        cout<<"有关信息：\n";
        cout<<"\n";
        cout<<"     IP：        29.53.103.3\n";
        cout<<"     用户名：    root\n";
        cout<<"     密码        123456\n";
        cout<<"     连接方式：  telnet\n";
        cout<<"     文件位置：  /bin/wget.exe\n";
        cout<<"                                   nic \n";
        break;
    case 3:
        /*
        ,您好!

        恭喜，您成功通过了考验的第二关。

        刚刚，你成功把文件wget.exe复制到了本地的data文件夹。现在你需要安装它。指令是install data\wget.exe，其中data\wget.exe指的正是wget.exe的位置。

        安装成功后，我会再给你发一封邮件。

        有关信息：

        指令:install data\wget.exe
                                        nic 
        */
        cout<<localhost->username<<",您好!\n";
        cout<<"\n";
        cout<<"恭喜，您成功通过了考验的第二关。\n";
        cout<<"\n";
        cout<<"刚刚，你成功把文件wget.exe复制到了本地的data文件夹。现在你需要安装它。指令是install data/wget.exe，其中data/wget.exe指的正是wget.exe的位置。\n";
        cout<<"\n";
        cout<<"安装成功后，我会再给你发一封邮件。\n";
        cout<<"\n";
        cout<<"有关信息：\n";
        cout<<"\n";
        cout<<"指令:install data/wget.exe\n";
        cout<<"                                   nic \n";
        break;
    case 4:
        /*
        ,您好!

        恭喜，您成功通过了考验的第三关。

        刚刚，您已经成功安装了一个非常重要的程序：wget.exe

        你可以输入命令wget测试，现在它已经在你电脑上了。

        下面给你介绍一下如何使用这个软件。使用它，你可以从一个http服务器上下载那里公开的文件。如果你输入 wget [服务器的IP] ，你可以看到服务器上所有公开的文件。

        如何使用wget下载文件？你需要这样使用它：wget [服务器的IP] [想下载的文件名]

        我有一台http服务器，它的IP是178.53.100.24。现在上面有一个程序，叫passguesser.exe，它对你的黑客生涯非常非常重要。请你用wget把它下载下来。

        安装成功后，我会再给你发一封邮件。

        有关信息：

        IP      :178.53.100.24
        文件名  :passguesser.exe
                                        nic 
        */
        cout<<localhost->username<<",您好!\n";
        cout<<"\n";
        cout<<"恭喜，您成功通过了考验的第二关。\n";
        cout<<"\n";
        cout<<"刚刚，您已经成功安装了一个非常重要的程序：wget.exe\n";
        cout<<"\n";
        cout<<"你可以输入命令wget测试，现在它已经在你电脑上了。\n";
        cout<<"\n";
        cout<<"下面给你介绍一下如何使用这个软件。使用它，你可以从一个http服务器上下载那里公开的文件。如果你输入 wget [服务器的IP] ，你可以看到服务器上所有公开的文件。\n";
        cout<<"\n";
        cout<<"如何使用wget下载文件？你需要这样使用它：wget [服务器的IP] [想下载的文件名]\n";
        cout<<"\n";
        cout<<"我有一台http服务器，它的IP是178.53.100.24。现在上面有一个程序，叫passguesser.exe，它对你的黑客生涯非常非常重要。请你用wget把它下载下来。\n";
        cout<<"\n";
        cout<<"安装成功后，我会再给你发一封邮件。\n";
        cout<<"\n";
        cout<<"有关信息：\n";
        cout<<"\n";
        cout<<"IP      :178.53.100.24\n";
        cout<<"文件名  :passguesser.exe\n";
        cout<<"                                   nic \n";
        break;
    case 5:
        /*
        ，您好

        恭喜你，你已经通过前四个考验了。

        刚刚，你弄到了passguesser.exe，这是一个密码破解器。不过，在使用它前，你得先把它安装好。安装的方法和之前安装wget.exe时一样。

        等你安装好了，我会再给你发一封电子邮件。

        有关信息：
        指令：install data\passguesser.exe

                                                nic
        */
        cout<<localhost->username<<"，您好\n";
        cout<<"\n";
        cout<<"恭喜你，你已经通过前四个考验了。\n";
        cout<<"\n";
        cout<<"刚刚，你弄到了passguesser.exe，这是一个密码破解器。不过，在使用它前，你得先把它安装好。安装的方法和之前安装wget.exe时一样。\n";
        cout<<"\n";
        cout<<"等你安装好了，我会再给你发一封电子邮件。\n";
        cout<<"\n";
        cout<<"有关信息：\n";
        cout<<"指令：install data/passguesser.exe\n";
        cout<<"\n";
        cout<<"                                          nic\n";
        break;
    case 6:
        /*
        ，您好

        恭喜你，你已经通过了考验。



        刚刚，你安装好了passguesser.exe，这是一个密码破解器。

        这是个很简单的程序。它里面内置了三百万个常见的密码，可以破解绝大多数常见的像123456或者abc666666这样的密码。

        这次你将执行一次真正的黑客任务。目标主机的IP是117.51.143.120，它的密码需要你用passguesser程序破解。

        目标主机的bin文件夹里有一个文件，叫portscan.exe，请你用scp把它复制回来。之后，请到log文件夹里删除connect.log文件。如果不删除这个日志文件，嘿嘿。

        等你完成了，我会再给你发一封电子邮件。

        有关信息：

        IP:117.51.143.120
        协议:telnet
        指令：
            passguesser
            telnet 117.51.143.120
            scp /bin/portscan.exe
            del /log/connect.log

                                                nic
        */
        cout<<localhost->username<<"，您好\n";
        cout<<"\n";
        cout<<"恭喜你，你已经通过了考验。\n";
        cout<<"\n";
        cout<<"\n";
        cout<<"\n";
        cout<<"刚刚，你安装好了passguesser.exe，这是一个密码破解器。\n";
        cout<<"\n";
        cout<<"这是个很简单的程序。它里面内置了三百万个常见的密码，可以破解绝大多数常见的像123456或者abc666666这样的密码。\n";
        cout<<"\n";
        cout<<"这次你将执行一次真正的黑客任务。目标主机的IP是117.51.143.120，它的密码需要你用passguesser程序破解。\n";
        cout<<"\n";
        cout<<"目标主机的bin文件夹里有一个文件，叫portscan.exe，请你用scp把它复制回来。之后，请到log文件夹里删除connect.log文件。如果不删除这个日志文件，嘿嘿。\n";
        cout<<"\n";
        cout<<"等你完成了，我会再给你发一封电子邮件。\n";
        cout<<"\n";
        cout<<"有关信息：\n";
        cout<<"\n";
        cout<<"     IP:117.51.143.120\n";
        cout<<"     协议:telnet\n";
        cout<<"     指令：\n";
        cout<<"         passguesser\n";
        cout<<"         telnet 117.51.143.120\n";
        cout<<"         scp /bin/portscan.exe\n";
        cout<<"     注意事项：\n";
        cout<<"         记得删除日志啊\n";
        cout<<"\n";
        cout<<"                                          nic\n";
        break;
    case 7:
        cout<<localhost->username<<"，您好\n\n";
        cout<<"恭喜你！你成功完成了任务\n\n";
        cout<<"请你先按照老方法安装好程序portscan.exe\n\n";
        cout<<"这是一个端口扫描器。使用它，你可以获得一台电脑上的开放端口的列表\n\n";
        cout<<"现在你需要入侵一台电脑。它的IP是180.21.10.220，端口需要你去扫描，密码应该可以破解\n\n";
        cout<<"目标主机里有个文件，叫tracer.exe。它在那台电脑的bin文件夹里\n\n";
        cout<<"记得删除日志。祝你好运\n\n";
        cout<<"有关信息：\n\n";
        cout<<"     IP:180.21.10.220\n\n";
        cout<<"     目标文件:/bin/tracer.exe\n\n";
        cout<<"                                          nic\n";
        break;
     case 8:
        cout<<localhost->username<<"，您好\n\n";
        cout<<"这次，你遇到了一个十分复杂的任务。\n\n";
        cout<<"你得去偷一份某程序的源代码。\n\n";
        cout<<"这次的目标是24.120.41.10 。那里有一个code.cpp ，你需要把它复制回来\n\n";
        cout<<"但这个目标非常复杂，所以你必须使用一种很复杂的方式来入侵\n\n";
        cout<<"我测试过，这台主机的telnet端口就是23，而且是开放的，但密码破解不了\n\n";
        cout<<"所以你必须用一种叫密码嗅探的方式破解密码\n\n";
        cout<<"第一步，你先安装好上一次的tracer.exe\n\n";
        cout<<"第二步，你用wget到"<<COMPUTER2<<"那里下载一个sniffer.exe\n\n";
        cout<<"第三步，用tracer找出到目标主机过程中的所有路由\n\n";
        cout<<"第四步，用portscan扫描这些路由。可能有一个路由上有个端口，你可以用telnet连接\n\n";
        cout<<"第五步，用passguesser破解这台路由的密码\n\n";
        cout<<"第六步，用scp -u上传程序sniffer.exe到路由\n\n";
        cout<<"第七步，telnet到路由，启动sniffer\n\n";
        cout<<"第八步，多回车几次，直到log文件夹中多出一个sniffer.log 。那里有截获的数据包，包括目标主机的密码\n\n";
        cout<<"第九步，用telnet连接目标主机，用截获的密码登陆，然后下载目标文件\n\n";
        cout<<"\n\n";
        cout<<"有关信息：\n\n";
        cout<<"     记得删除日志，包括sniffer.log,还有，记得关闭sniffer.exe(使用指令sniffer stop)，它会暴露你的行踪\n\n";
        cout<<"                                          nic\n";
        break;
     case 9:
        cout<<localhost->username<<"，您好\n\n";
        cout<<"恭喜您，您成功完成了这一关！这是非常复杂的一关，我对您现在的能力十分有信心，所有我决定以后会直接让一些需要帮助的人给你写邮件求助。\n\n";
        cout<<"不过这需要保证通信的安全。所以我做了一个加密的专用邮件接收器，需要你下载下来安装一下。你用wget到"<<COMPUTER2<<"那里下载一个mail2.exe\n\n";
        cout<<"等你安装完之后我会用它给你发一些教程。\n\n";
        cout<<"                                          nic\n";
        break;
        /*
        cout<<localhost->username<<"，您好\n\n";
        cout<<"您成功完成了这一关！\n\n";
        cout<<"您真是太厉害了，可惜游戏没有关卡了，不然我相信你会是GHG最优秀的玩家。\n\n";
        cout<<"没错，你通关了\n\n";
        cout<<"唉，关卡太少了，不知道你是不是这么想的\n\n";
        cout<<"不过，你还有最后一个任务\n\n";
        cout<<"加一下作者QQ:2492515121\n\n";
        cout<<"加一句，我可不是啥黑客，这是个普通人，做了个游戏而已\n\n";
        cout<<"最后，\n\n";
        cout<<"真诚的感谢您，您玩游戏的过程就是对我最大的支持\n\n";
        cout<<"再见\n\n";
        cout<<"PS：看到这里你可以用savegame指令再保存一下。如果黑客游戏有下一版，您可以直接用这次的存档继续玩\n\n";
        cout<<"                                      游戏作者：\n\n";
        cout<<"                                      Nictheboy\n\n";
        break;
        */
        /*
    case 10:
        cout<<localhost->username<<"，您好\n\n";
        cout<<"我忘了把一个电脑上的日志删除了。请你到那台电脑上把日志删除。请迅速完成\n\n";
        cout<<"有关信息：\n\n";
        cout<<"     IP:29.102.4.31\n\n";
        cout<<"     目标文件:所有的日志文件\n\n";
        cout<<"                                          nic\n";
        break;
        */
    default:
        cout<<"内部错误:关卡不存在!!!\n";
    }
    return 0;
}
/*
void add_mail(string mail)
{
    mail_list.push_back(mail);
}
*/

