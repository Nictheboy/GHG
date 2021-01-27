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
