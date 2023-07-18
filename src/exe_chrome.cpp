/*
 *文件名:   exe_chrome.cpp
 *作者:     Nictheboy
 *内容:     实现chrome.exe
 *
 *最后一次修改: 2023/1/11
 */
#ifndef FOR_XES
#include "hackgame.h"
#endif

int exe_chrome(int n,const char **t, Computer *c)
{
    if (n < 2)
    {
        cout << "Chrome 是一个强大的浏览器" << endl;
        cout << "语法:chrome [网站域名或ip](:[端口号]) [页面名]" << endl;
        return 0;
    }
    int port;
    string url;
    string ip;
    try
    {
        ipport_cutter cutter(t[1], 80);
        port = cutter.port;
        url = cutter.ip;
    }
    catch (string)
    {
        return 0;
    }
    ip = net_node::get_dns_reply(url);

    // 填写请求数据
    connection_request req(c->netnode->ip, ip, "http");
    if (n > 2){
        req.argument = t[2];
        for (int i=3; i<n; i++){
            req.argument = req.argument + " " + t[i];
        }
    }
    

    connection_reply reply = c->netnode->connect(req);
    c->netnode->send_package(ip, "浏览器请求页面", "http");
    if (reply.computer && reply.computer->get_port("http") == port)
    {
        delay(0.2);
        c->netnode->send_package(ip, "校验握手", "telnet");
        c->write_log("chrome", string("访问了网站 ") + url + " ，参数为 " + req.argument);
        reply.computer->handle_http_request(port, reply.received);
        return 0;
    }
    delay(1);
    cout << "无法访问此网站，请检查拼写。\n";
    return 0;
    return 0;
}



