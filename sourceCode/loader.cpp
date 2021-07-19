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
            cout<<"9.[新]第9关"<<endl;
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


