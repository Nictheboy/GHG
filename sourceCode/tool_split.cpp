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
