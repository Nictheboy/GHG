#include "hackgame.h"

//string mail_name="";

int exe_mail(int n,const char **t,Computer *c)
{
    vector<string> all_mail_name;
    cout << "\nHmail邮件" << "\n-----------------------------\n";
    int number=task_number;
    if (n>1)
    {
        stringstream stm;
        stm<<t[1];
        stm>>number;
        if (DEBUG_FLAG==false&&task_number<number)
        {
          //::mail_name = "error";
            cout<<"找不到此邮件.\n\n";
            //::mail_name = all_mail_name[task_number - 1];
            return 0;
        }
    }
    //cout<<number;
    switch (number)
    {
    case 1:
      //::mail_name = "初次见面";
      //all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<",您好!\n";
        cout<<"我是Nic.\n";
        cout<<"你好，听说你想要当黑客，我决定教教你。不过，你得先通过我的几个小测试。\n";
        cout<<"你知道telnet吗？这是一个连接到远程计算机的工具。你需要学会如何使用它。\n现在需要你用Telnet连接到我的一个服务器上。我服务器的IP地址是29.53.103.3,用户名是root，密码是123456。连接成功后，请你立即用dc指令断开连接。\n你断开连接后，我会再给你寄一封邮件\n如果你有什么问题，可以使用系统的help指令。那里面有各种指令的使用说明。\n";
        cout<<"(提示：使用命令telnet 29.53.103.3)\n";
        cout<<"有关信息：\n";
        cout<<"     指令：telnet 29.53.103.3\n";
        cout<<"     用户名：root\n";
        cout<<"     密码：123456\n";
        cout<<"                                nic\n\n";
        break;
    case 2:
      //::mail_name = "恭喜通过测试";
     // all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<",您好!\n";
        //cout<<"\n";
        cout<<"恭喜，您成功通过了考验的第一关。\n";
        //cout<<"\n";
        cout<<"我忽然想起来你没有一个叫wget.exe的非常重要的程序。这是一个用来从网站上下载程序的软件，有很多用处。我打算让你把他弄过来。\n";
        //cout<<"\n";
        cout<<"在上次让你练习telnet的那台主机上，存有一份wget.exe。你得在连接一次29.53.103.3，然后用scp指令把它复制过来。\n";
        //cout<<"\n";
        cout<<"你只需要在登录成功后，用指令 cd bin 进入那台主机的bin文件夹，然后用 scp wget.exe 指令把文件复制到你的电脑上。\n";
        //cout<<"\n";
        cout<<"有关信息：\n";
        //cout<<"\n";
        cout<<"     IP：        29.53.103.3\n";
        cout<<"     用户名：    root\n";
        cout<<"     密码        123456\n";
        cout<<"     连接方式：  telnet\n";
        cout<<"     文件位置：  /bin/wget.exe\n";
        cout<<"                                   nic \n\n";
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
     // ::mail_name = "Re: 恭喜通过测试";
      //all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<",您好!\n";
        //cout<<"\n";
        cout<<"恭喜，您成功通过了考验的第二关。\n";
        //cout<<"\n";
        cout<<"刚刚，你成功把文件wget.exe复制到了本地的data文件夹。现在你需要安装它。指令是install data/wget.exe，其中data/wget.exe指的正是wget.exe的位置。\n";
        //cout<<"\n";
        cout<<"安装成功后，我会再给你发一封邮件。\n";
        //cout<<"\n";
        cout<<"有关信息：\n";
        //cout<<"\n";
        cout<<"指令:install data/wget.exe\n";
        cout<<"                                   nic \n\n";
        break;
    case 4:
        /*
        ,您好!

        恭喜，您成功通过了考验的第三关。

        刚刚，您已经成功安装了一个非常重要的程序：wget.exe

        你可以输入命令wget测试，现在它已经在你电脑上了。

        下面给你介绍一下如何使用这个软件。使用它，你可以从一个www服务器上下载那里公开的文件。如果你输入 wget [服务器的IP] ，你可以看到服务器上所有公开的文件。

        如何使用wget下载文件？你需要这样使用它：wget [服务器的IP] [想下载的文件名]

        我有一台www服务器，它的IP是178.53.100.24。现在上面有一个程序，叫passguesser.exe，它对你的黑客生涯非常非常重要。请你用wget把它下载下来。

        安装成功后，我会再给你发一封邮件。

        有关信息：

        IP      :178.53.100.24
        文件名  :passguesser.exe
                                        nic 
        */
     // ::mail_name = "初识wget";
     // all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<",您好!\n";
        //cout<<"\n";
        cout<<"恭喜，您成功通过了考验的第三关。\n";
        //cout<<"\n";
        cout<<"刚刚，您已经成功安装了一个非常重要的程序：wget.exe\n";
        //cout<<"\n";
        cout<<"你可以输入命令wget测试，现在它已经在你电脑上了。\n";
        //cout<<"\n";
        cout<<"下面给你介绍一下如何使用这个软件。使用它，你可以从一个www服务器上下载那里公开的文件。如果你输入 wget [服务器的IP] ，你可以看到服务器上所有公开的文件。\n";
        //cout<<"\n";
        cout<<"如何使用wget下载文件？你需要这样使用它：wget [服务器的IP] [想下载的文件名]\n";
        //cout<<"\n";
        cout<<"我有一台www服务器，它的IP是178.53.100.24。现在上面有一个程序，叫passguesser.exe，它对你的黑客生涯非常非常重要。请你用wget把它下载下来。\n";
        //cout<<"\n";
        cout<<"安装成功后，我会再给你发一封邮件。\n";
        //cout<<"\n";
        cout<<"有关信息：\n";
        //cout<<"\n";
        cout<<"IP      :178.53.100.24\n";
        cout<<"文件名  :passguesser.exe\n";
        cout<<"                                   nic \n\n";
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
      //::mail_name = "密码宝典";
     // all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<"，您好\n";
        //cout<<"\n";
        cout<<"恭喜你，你已经通过前四个考验了。\n";
        //cout<<"\n";
        cout<<"刚刚，你弄到了passguesser.exe，这是一个密码破解器。不过，在使用它前，你得先把它安装好。安装的方法和之前安装wget.exe时一样。\n";
        //cout<<"\n";
        cout<<"等你安装好了，我会再给你发一封电子邮件。\n";
        //cout<<"\n";
        cout<<"有关信息：\n";
        cout<<"指令：install data/passguesser.exe\n";
        cout<<"\n";
        cout<<"                                          nic\n\n";
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
     // ::mail_name = "Re: 密码宝典";
      //all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<"，您好\n";
        //cout<<"\n";
        cout<<"恭喜你，你已经通过了考验。\n";
        cout<<"\n";
        //cout<<"\n";
        //cout<<"\n";
        cout<<"刚刚，你安装好了passguesser.exe，这是一个密码破解器。\n";
        //cout<<"\n";
        cout<<"这是个很简单的程序。它里面内置了三百万个常见的密码，可以破解绝大多数常见的像123456或者abc666666这样的密码。\n";
        //cout<<"\n";
        cout<<"这次你将执行一次真正的黑客任务。目标主机的IP是117.51.143.120，它的密码需要你用passguesser程序破解。\n";
        //cout<<"\n";
        cout<<"目标主机的bin文件夹里有一个文件，叫portscan.exe，请你用scp把它复制回来。之后，请到log文件夹里删除connect.log文件。如果不删除这个日志文件，嘿嘿。\n";
        //cout<<"\n";
        cout<<"等你完成了，我会再给你发一封电子邮件。\n";
        //cout<<"\n";
        cout<<"有关信息：\n";
        //cout<<"\n";
        cout<<"     IP:117.51.143.120\n";
        cout<<"     协议:telnet\n";
        cout<<"     指令：\n";
        cout<<"         passguesser\n";
        cout<<"         telnet 117.51.143.120\n";
        cout<<"         scp /bin/portscan.exe\n";
        cout<<"     注意事项：\n";
        cout<<"         请记得删除日志\n";
        cout<<"\n";
        cout<<"                                          nic\n\n";
        break;
    case 7:
      //::mail_name = "瞒天过海――获取文件";
     // all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<"，您好\n";
        cout<<"恭喜你！你成功完成了任务\n";
        cout<<"请你先按照老方法安装好程序portscan.exe\n";
        cout<<"这是一个端口扫描器。使用它，你可以获得一台电脑上的开放端口的列表\n";
        cout<<"现在你需要入侵一台电脑。它的IP是180.21.10.220，端口需要你去扫描，密码应该可以破解\n";
        cout<<"目标主机里有个文件，叫tracer.exe。它在那台电脑的bin文件夹里\n";
        cout<<"记得删除日志。祝你好运\n";
        cout<<"有关信息：\n";
        cout<<"     IP:180.21.10.220\n";
        cout<<"     目标文件:/bin/tracer.exe\n";
        cout<<"                                          nic\n\n";
        break;
     case 8:
      //::mail_name = "神盗";
      //all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<"，您好\n";
        cout<<"这次，你遇到了一个十分复杂的任务。\n";
        cout<<"你得去偷一份某程序的源代码。\n";
        cout<<"这次的目标是24.120.41.10 。那里有一个code.cpp ，你需要把它复制回来\n";
        cout<<"但这个目标非常复杂，所以你必须使用一种很复杂的方式来入侵\n";
        cout<<"我测试过，这台主机的telnet端口就是23，而且是开放的，但密码破解不了\n";
        cout<<"所以你必须用一种叫密码嗅探的方式破解密码\n";
        cout<<"第一步，你先安装好上一次的tracer.exe\n";
        cout<<"第二步，你用wget到"<<COMPUTER2<<"那里下载一个sniffer.exe\n";
        cout<<"第三步，用tracer找出到目标主机过程中的所有路由\n";
        cout<<"第四步，用portscan扫描这些路由。可能有一个路由上有个端口，你可以用telnet连接\n";
        cout<<"第五步，用passguesser破解这台路由的密码\n";
        cout<<"第六步，用scp -u上传程序sniffer.exe到路由\n";
        cout<<"第七步，telnet到路由，启动sniffer\n";
        cout<<"第八步，多回车几次，直到log文件夹中多出一个sniffer.log 。那里有截获的数据包，包括目标主机的密码\n";
        cout<<"第九步，用telnet连接目标主机，用截获的密码登陆，然后下载目标文件\n";
        cout<<"\n";
        cout<<"有关信息：\n";
        cout<<"     记得删除日志，包括sniffer.log,还有，记得关闭sniffer.exe(使用指令sniffer stop)，它会暴露你的行踪\n";
        cout<<"                                          nic\n\n";
        break;
     case 9:
     // ::mail_name = "错误";
     // all_mail_name.push_back(::mail_name);
        cout<<"NULL\n";
        break;
    case 10:
      //::mail_name = "一时疏忽";
      //all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<"，您好\n";
        cout<<"我一时疏忽，忘记把一个电脑上的日志删除了。\n请你到那台电脑上把日志删除。请迅速完成\n";
        cout<<"有关信息：\n";
        cout<<"     IP:29.102.4.31\n";
        cout<<"     目标文件:所有的日志文件\n";
        cout<<"                                          nic\n\n";
        break;
    case 11:
      //::mail_name = "高级任务即将出现!";
     // all_mail_name.push_back(::mail_name);
        cout<<localhost->username<<"，您好\n";
        cout<<"请你连接到一个资源服务器，并在其的bin文件夹里找到一个端口破解工具，\n";
        cout<<"它的名字叫做FTPoverflow.exe，请你复制到本地后安装，下一次我给你发邮件\n";
        cout<<"时，你将面临更大的挑战。\n";
        cout<<"有关信息：\n";
        cout<<"     资源服务器  IP:187.56.222.10\n";
        cout<<"     请记得删除日志";
        cout<<"                                          nic\n\n";
        break;
    case 12:
        cout<<"NULL\n\n";
    default:
      //::mail_name = "error";
        cout<<"找不到此邮件.\n\n";
       // ::mail_name = all_mail_name[task_number - 1];
    }
    return 0;
}
