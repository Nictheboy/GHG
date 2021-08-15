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
