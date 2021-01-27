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
    /*
    dir * d = locate_dir (dir_now->turn_file_ps_into_dir_ps (c[1]));
    file * f - locate_file (c[1]);
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
FileSystem::dir* Computer::get_www_dir(int port)
{
    if (port == get_port("www"))//检测端口
    {
        return locate_dir("/www");
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
            catch (...)
            {
                cout<<BG_RED<<"执行命令'"<<input<<"'的时候发生了未处理的异常!"<<RESET<<endl;
            }
        }
    }
}
