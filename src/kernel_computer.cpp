/*
 *文件名:   computer.cpp
 *作者:     Nictheboy
 *内容:     实现Computer类
 *
 */
#ifndef FOR_XES
#include "hackgame.h"
#endif
// Computer的构造函数，进行主机的初始化
Computer::Computer()
{
    // 绑定指令，把这个类的各个内置命令对应的方法加载到pfm里面
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
    pfm.set_belonger(this); // 绑定pfm。用于方便pfm找到对应命令后调用方法

    net_conversation_caller = NULL;              // 设置caller是NULL
    event_before_input = NULL;                   // 设置事件为空
    dc_signal = false;                           // 初始化
    connection_from = "";                                   // 初始化
    username = "root";                           // 默认用户
    password = "weoferhsohogowergorewhogheowgh"; // 默认口令，别嘻嘻嘻，主机口令在创建后肯定是要改的

    name = "";           // 无主机名
    style = basic_style; // 默认样式
}

// Computer的构造函数，进行主机的初始化
Computer::Computer(string ip) // 参数是主机的ip
{
    // 同上
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
    pfm.set_belonger(this);                      // 同上
    net_conversation_caller = NULL;              // 同上
    event_before_input = NULL;                   // 设置事件为空
    dc_signal = false;                           // 初始化
    connection_from = "";                                   // 初始化
    username = "root";                           // 默认用户
    password = "weoferhsohogowergorewhogheowgh"; // 默认口令

    netnode = new net_node(ip);   // 新建一个网络节点，ip为参数中的IP
    netnode->bind_computer(this); // 将主机绑定到新节点

    name = "";                          // 无主机名
    style = basic_style;                // 默认样式
    root = new FileSystem::dir("root"); // 创建根目录
    dir_now = root;                     // 默认位于根目录
    // 创建文件夹和文件
    root->add_new_dir("system");
    locate_dir("/system")->add_new_txt("kernel32.dll", "这不是一个有效的文本文件！");
    locate_dir("/system")->add_new_txt("rundll32.dll", "这不是一个有效的文本文件！");
    locate_dir("/system")->add_new_txt("sysmain.sys", "这不是一个有效的文本文件！");
    locate_dir("/system")->add_new_txt("service.dll", "这不是一个有效的文本文件！");
    locate_dir("/system")->add_new_txt("cmd.exe", "这不是一个有效的文本文件！");
    root->add_new_dir("log");
    root->add_new_dir("data");
    root->add_new_dir("bin");

    // 创建初始exe文件
    write_exe("help", &exe_help);
    write_exe("mail", &exe_mail);
    write_exe("telnet", &exe_telnet);
    write_exe("tree", &exe_tree);
    write_exe("ipconfig", &exe_ipconfig);
    write_exe("ping", &exe_ping);
    write_exe("savegame", &exe_savegame);
    write_exe("myexe", &exe_myexe);
}

// 实现登陆的效果
bool Computer::login()
{
    string tmp;
    string tmp2;
    for (int i = 0; i < 3; i++) // 重复三次
    {
        cout << style.login_output;
        getline(cin, tmp); // 获取用户名输入
        cout << style.pass_output;
        getline(cin, tmp2);                         // 获取密码输入
        send_here("登陆:" + tmp + " 密码:" + tmp2); // 让连接节点发送一个数据包。用与截获密码的效果
        if (tmp == username && tmp2 == password)
        {
            // 登陆成功
            cout << style.login_successful << endl;
            return true;
        }
        else
        {
            // 登陆失败
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

// 获取制定端口名称对应的端口号
int Computer::get_port(string name_arg /*端口名称*/)
{
    // 遍历port_list
    for (int i = 0; i < port_list.size(); i++)
    {
        if (port_list.at(i).name == name_arg)
        {
            return port_list.at(i).port;
        }
    }
    return -1; //-1表示没有这个端口
}

// 断开连接。run（）的循环一开始就会检查dc_signal，所以可以通过设置 dc_signal退出run（）函数
int Computer::dc(int i, const char **t)
{
    dc_signal = true;
    return 0;
}

// 列出文件和文件夹
int Computer::dir(int n, const char **t)
{
    string str;
    FileSystem::dir *d = nullptr;
    try
    {
        d = locate_dir((n == 1) ? "." : t[1]);
    }
    catch (FileSystem::not_exist)
    {
        cout << "文件夹不存在！" << endl;
        return -1;
    }
    if (!d)
    {
        cout<<"Impossible Error!"<<endl;
        return -1;
    }
    // 遍历当前文件夹的子文件夹
    for (int i = 0; i < d->subdir.size(); i++)
    {
        str = d->subdir[i]->name;
        cout << str;
        // 补齐
        for (int j = 0; strlen(str.c_str()) <= 29 && j < (30 - strlen(str.c_str())); j++)
        {
            cout << " ";
        }
        cout << "<DIR>\n";
    }
    // 遍历当前文件夹的文件
    for (int i = 0; i < d->content.size(); i++)
    {
        str = d->content[i]->name;
        cout << str;
        cout << endl;
    }
    return 0;
}

// 进入目录
int Computer::cd(int i, const char **t)
{
    // 判断是否缺少参数
    if (i < 2)
    {
        cout << "语法错误！\n";
        return 0;
    }
    try
    {
        dir_now = locate_dir(t[1]);
    }
    catch (FileSystem::not_exist)
    {
        cout << "找不到该目录!" << endl;
    }
    return 0;
}

// 根据文件夹路径定位文件夹
FileSystem::dir *Computer::locate_dir(string path, bool auto_add)
{
    return dir_now->locate_dir(path, auto_add);
}

// 定位文件
FileSystem::file *Computer::locate_file(string path, bool auto_add)
{
    return dir_now->locate_file(path, auto_add);
}

void Computer::delete_file(string path)
{ // 删除文件。/打头则从根目录开始，支持上级目录
    vector<string> list = split(path, "/", true);
    FileSystem::dir *d = dir_now->turn_file_ps_into_dir_ps(path, false);
    d->delete_file(list[list.size() - 1]);
}

void Computer::delete_dir(string path)
{ // 删除文件夹。/打头则从根目录开始，支持上级目录
    vector<string> list = split(path, "/", true);
    FileSystem::dir *d = dir_now->turn_file_ps_into_dir_ps(path, false);
    d->delete_dir(list[list.size() - 1]);
}

bool Computer::if_file_exist(string path)
{
    return dir_now->if_file_exist(path);
}

bool Computer::if_dir_exist(string path)
{
    return dir_now->if_dir_exist(path);
}

// 显示当前目录
int Computer::pwd(int i, const char **t)
{
    stack<FileSystem::dir *> stk; // 栈结构
    FileSystem::dir *temp = dir_now;
    do
    {
        stk.push(temp);
        temp = temp->father;
    } while (temp != nullptr); // 倒着从当前目录到根目录
    stk.pop();                 // pop出最后一个，也就是根目录
    if (stk.empty())           // 假如是空的，也就是当前就是根目录
    {
        cout << "/";
    }
    while (!stk.empty()) // 一个一个出栈然后显示
    {
        cout << "/" << stk.top()->name;
        stk.pop();
    }
    cout << endl;
    return 0;
}

// 输出一个文本文件的内容
int Computer::type(int i, const char *t[])
{
    using namespace FileSystem;

    if (i < 2)
    {
        cout << "缺少参数!语法:type [文件名]\n";
        return 0;
    }
    string filename = t[1];
    try
    {
        cout << locate_file(filename)->read_string() << endl;
    }
    catch (FileSystem::not_exist)
    {
        cout << "文件不存在!\n";
    }
    return 0;
}

// 复制文件
int Computer::copy(int i, const char *c[])
{
    using namespace FileSystem;

    if (i < 3)
    {
        cout << "缺少参数!语法:copy [被复制文件名] [目标文件名]\n";
        return 0;
    }
    vector<string> listed_to = split(c[2], "/");
    if (!dir_now->if_file_exist(c[1]))
    {
        cout << "被复制文件不存在!\n";
        return -1;
    }
    file *file_from = locate_file(c[1]);
    dir_now->locate_file(c[2], true)->load_data(file_from->size, file_from->data);
    return 0;
}

// 新建文件夹
int Computer::mkdir(int n, const char *c[])
{
    using namespace FileSystem;
    if (n < 2)
    {
        cout << "缺少参数!语法:mkdir [创建的文件夹名]\n";
        return 0;
    }
    try
    {
        locate_dir(c[1], true);
    }
    catch (illegal_argument)
    {
        cout << "文件夹名不合法!" << endl;
        return 0;
    }
    catch (already_exist)
    {
        cout << "文件夹已存在!" << endl;
    }
    return 0;
}

// 删除文件
int Computer::del(int n, const char *c[])
{
    using namespace FileSystem;
    if (n < 2)
    {
        cout << "缺少参数!语法:del [删除的文件或文件夹名]\n";
        return 0;
    }
    try
    {
        delete_file(c[1]);
    }
    catch (not_exist)
    {
        try
        {
            delete_dir(c[1]);
        }
        catch (not_exist)
        {
            cout << "文件或文件夹不存在!\n";
        }
    }
    return 0;
}

// 写日志
void Computer::write_log(string log_filename, string content)
{
    using namespace FileSystem;

    time_t timep;
    struct tm ts;
    time(&timep);
    localtime_s_f(&ts, &timep);
    struct tm *t = &ts;
    stringstream ss;
    ss << "[" << t->tm_hour << ":" << t->tm_min << ":" << t->tm_sec << "]";
    string time_str = ss.str();

    string path = string("/log/") + log_filename + ".log";
    file *f = locate_file(path, true);
    string str = f->read_string() + time_str + content + "\n";
    f->load_string(str);
}

// 写exe
void Computer::write_exe(string exe_name, FileSystem::exe_adr adr)
{
    using namespace FileSystem;
    locate_dir("/bin", true)->add_file(new file(exe_name + ".exe", adr));
}

// 远程复制文件
int Computer::scp(int n, const char *c[])
{
    using namespace FileSystem;
    if (n < 2)
    {
        cout << "缺少参数!语法:\n";
        cout << "scp [远程文件名]" << endl;
        cout << "scp -u [本地文件名] [远程ip](:[端口])" << endl;
        cout << "scp -d [远程文件名] [远程ip](:[端口])" << endl;
        return 0;
    }
    file *from;
    class dir *to;
    if (strcmp(c[1], "-u") == 0)
    {
        if (n < 3)
        {
            cout << "缺少参数!语法:\n";
            cout << "scp [远程文件名]" << endl;
            cout << "scp -u [本地文件名] [远程ip](:[端口])" << endl;
            cout << "scp -d [远程文件名] [远程ip](:[端口])" << endl;
            return 0;
        }
        string ip;
        int port;
        try
        {
            ipport_cutter cutter(c[3], 23);
            ip = cutter.ip;
            port = cutter.port;
        }
        catch (string)
        {
            cout << "网络地址输入格式错误!" << endl;
            return -1;
        }
        connection_reply reply = netnode->connect(ip); // 连接，以获取远程主机的指针
        if (reply.computer == NULL /*没找到主机*/ ||
            reply.computer->get_port("telnet") != port /*端口关闭*/)
        {
            cout << "无法连接到远程主机的telnet服务!上传文件失败" << endl;
            return 0;
        }
        cout << "正在与远程主机建立会话..." << endl;
        delay(0.5);
        if (!reply.computer->login())
        {
            cout << "权限不足" << endl;
            return 0;
        }
        to = reply.computer->locate_dir("/data", true);
        if (!dir_now->if_file_exist(c[2]))
        {
            cout << "文件不存在！\n";
            return 0;
        }
        from = dir_now->locate_file(c[2]);
    }
    else if (strcmp(c[1], "-d") == 0)
    {
        if (n < 3)
        {
            cout << "缺少参数!语法:\n";
            cout << "下载文件到本地：   scp [远程文件名]" << endl;
            cout << "上传文件到远程主机：scp -u [本地文件名] [远程ip](:[端口])" << endl;
            cout << "下载远程主机的文件：scp -d [远程文件名] [远程ip](:[端口])" << endl;
            return 0;
        }
        string ip;
        int port;
        try
        {
            ipport_cutter cutter(c[3], 23);
            ip = cutter.ip;
            port = cutter.port;
        }
        catch (string)
        {
            cout << "网络输入格式错误!" << endl;
            return -1;
        }
        // 类似
        connection_reply reply = netnode->connect(ip);
        if (reply.computer == NULL ||
            reply.computer->get_port("telnet") != port)
        {
            cout << "无法连接到远程主机的telnet服务!下载文件失败" << endl;
            return 0;
        }
        cout << "正在与远程主机建立会话..." << endl;
        delay(0.5);
        if (!reply.computer->login()) // 登陆失败的话
        {
            cout << "权限不足" << endl;
            return 0;
        }
        to = localhost->locate_dir("/data", true);
        if (!reply.computer->if_file_exist(c[2]))
        {
            cout << "文件不存在！\n";
            return 0;
        }
        from = reply.computer->locate_file(c[2]);
    }
    else
    {
        if (this == localhost)
        {
            cout << "scp用于从远程计算机复制文件，请不要在本地计算机上使用scp\n";
            return 0;
        }
        to = localhost->locate_dir("/data", true);
        if (!if_file_exist(c[1]))
        {
            cout << "文件不存在！\n";
            return 0;
        }
        from = locate_file(c[1]);
    }
    assert(from);
    assert(to);
    to->add_file(new file(from), true);
    cout << "正在将文件" + from->name + "复制到本地目录/data\n";
    delay(2);
    cout << "复制成功！\n";
    return 0;
}

// 安装程序，实际上是复制文件到bin文件夹
int Computer::install(int n, const char *c[])
{
    using namespace FileSystem;
    // 参数不足
    if (n < 2)
    {
        cout << "缺少参数!语法:install [想要安装的程序的位置]\n";
        return 0;
    }

    file *from;
    class dir *to;
    // 定位原来的文件
    if (!if_file_exist(c[1]))
    {
        cout << "文件不存在！\n";
        return 0;
    }
    from = locate_file(c[1]);
    to = locate_dir("/bin", true); // 定位bin文件夹
    if (to->if_file_exist(from->name))
    {
        cout << from->name << "已经安装!请勿重复安装\n";
        return 1;
    }
    // 复制文件
    to->add_file(new file(from), false);
    cout << "正在安装程序" + from->name + "中...";
    delay(2);
    cout << "安装成功！\n";
    return 0;
}

// 获取公开共享的文件夹
FileSystem::dir *Computer::get_http_dir(int port)
{
    if (port == get_port("http")) // 检测端口
    {
        if (if_dir_exist("/web/static")) // 默认返回静态目录
        {
            return locate_dir("/web/static");
        }
        else // 兼容老版本
        {
            return locate_dir("/web", true);
        }
    }
    else if (port == get_port("www")) // 兼容老版本
    {
        return locate_dir("/www", true);
    }
    else
    {
        throw FileSystem::not_exist();
    }
}

bool Computer::handle_http_request(int port, connection_request req)
{
    if (port == get_port("http")) // 检测端口
    {
        if (if_dir_exist("/web/backend")) // 返回目录
        {
            if (if_file_exist("/web/backend/apache.config"))
            {
                string config = locate_file("/web/backend/apache.config", false)->read_string();
                DataTree::data_node *root_temp = load_tree_from_string(config);
                DataTree::data_node *index_service = root_temp->locate("index/service");
                if (index_service)
                {
                    write_log("apache", string("Handled a request from ") + req.from);
                    process_command(string("/web/backend/") + index_service->value + " " + req.argument);
                    delete root_temp;
                    return true;
                }
                else
                {
                    delete root_temp;
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

// 从连接者发送一个数据包到本地
void Computer::send_here(string data, string type)
{
    if (net_conversation_caller)                                        // 假如连接者指针不是NULL
        net_conversation_caller->send_package(netnode->ip, data, type); // 发送数据包
}

// 打开一个端口
void Computer::open_port(string port_name, int port)
{
    port_list.push_back(port_info(port_name, port)); // 在port_list中加一个对象
}

bool Computer::process_event_before_input()
{
    // 这个是检查有函数指针是不是NULL，这个指针一般执行剧情函数
    if (event_before_input)
    {
        (*event_before_input)();
        return true;
        // event_before_input=NULL;
    }
    else
    {
        return false;
    }
}

bool Computer::process_command(string cmd)
{
    using namespace FileSystem;
    // 这个是把输入给一个pfm对象。这个是Performer类的实例。用途是看看命令是不是内置命令
    string input = cmd;
    pfm.import_command(input);
    if (!pfm.process()) // 如果 pfm.process()是false表示没执行成功
    {
        // pfm执行完process（）之后会把分过块的命令文本存到listed_cmd里面
        // 这是在bin文件夹里找有没有对应的exe文件
        string cmd_name = *(pfm.listed_cmd.begin());
        file *f = nullptr;
        if (if_file_exist(cmd_name))
        {
            f = locate_file(cmd_name);
        }
        if (if_file_exist(cmd_name + ".exe"))
        {
            f = locate_file(cmd_name + ".exe");
        }
        if (if_file_exist("/bin/" + cmd_name))
        {
            f = locate_file("/bin/" + cmd_name);
        }
        if (if_file_exist("/bin/" + cmd_name + ".exe"))
        {
            f = locate_file("/bin/" + cmd_name + ".exe");
        }
        if (!f)
            return false;
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
    cout << style.after_ip << style.getinput;
}

// 运行这台电脑
void Computer::run(bool need_login /*是否需要登陆*/,
                   string f /*连接者的名字，用于写日志*/,
                   net_node *caller /*连接者的网络节点，默认为NULL，用于发送数据包实现截获数据的效果*/)
{
    using namespace FileSystem;
    connection_from = f;
    net_conversation_caller = caller;
    write_log("connect", connection_from + " 连接了本地终端"); // 写日志
    send_here("连接成功");                          // 从连接者发送数据包到自己
    string input;
    // 这部分是登陆
    if (need_login)
    {
        if (!login()) // 登陆失败
        {
            write_log("connect", connection_from + " 因密码错误登陆失败！"); // 日志
            return;
        }
        write_log("connect", connection_from + " 登录成功"); // 日志
    }
    // while（true）里面是获取用户输入然后执行命令
    while (true) // 循环。出口在下面的return退出函数
    {
        // 这个是判断是否要断开
        // 如果有断开的标志就return
        if (dc_signal)
        {
            dc_signal = false;
            return;
        }
        process_event_before_input(); // 处理事件
        echo_before_command();        // 根据style的数据输出前面那些提示符。这些不是很重要
        check_and_autosave();         // 检查自动保存并保存
        getline(cin, input);
        if (input != "")
        {
            try
            {
                if (process_command(input))
                {
                    // check_and_autosave();//执行了命令后检查自动保存并保存
                }
                else
                {
                    cout << input << "不是任何内部或外部指令！！输入help获取指令列表\n";
                }
            }
            catch (string s)
            {
                cout << BG_RED << "程序抛出异常:" << s << RESET << endl;
            }
            catch (...)
            {
                cout << BG_RED << "执行命令'" << input << "'的时候发生了未处理的异常!" << RESET << endl;
            }
        }
    }
}
