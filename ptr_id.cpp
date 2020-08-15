#include "hackgame.h"
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
