#include <iostream>
#include <string.h>
#include <list>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <map>
#include <stack>
#include <algorithm>
using namespace std;

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
            /*
            if (c[i]=='=')
            {   
                buff1 += "\\e";
            }
            else
            { 
            */  
                if (c[i]=='\n')
                {   
                    buff1 += "\\n";
                }
                else
                {
                    buff1 += c[i];
                }
            //}
        }
    }
    return buff1;
}

string getTxt(string filename)
{
    ifstream fcin(filename.c_str());
    if (!fcin.is_open())
    {
        cout << "打开文件失败"<<endl;
        return "";
    }
    string temp,input;
    while (!fcin.eof())
    {
        getline(fcin,temp);
        temp += "\n";
        input += temp;
    }
    return input;
}

int main(int argn, char ** argv)
{
    if (argn<2)
    {
        cout<<"参数不足"<<endl;
        return -1;
    }
    cout<<encoding(getTxt(string(argv[1])));
    return 0;
}


