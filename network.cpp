#include "hackgame.h"

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
