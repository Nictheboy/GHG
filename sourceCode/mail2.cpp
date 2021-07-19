#ifndef FOR_XES
#include "hackgame.h"
#endif

mailManager::mail::mail(string subject,string receiver,string sender,string content){
    this->subject = subject;
    this->receiver = receiver;
    this->sender = sender;
    this->content = content;
}

mailManager::mail::mail(FileSystem::file * f){
    if(!f){
        throw "mailManager::mail::mail()发生异常：传入空指针";
    }
    vector<string> l = split(string((char*)f->data),"======");
    if (l.size()!=4){
        throw "mailManager::mail::mail()发生异常：文件格式错误";
    }
    this->subject = decoding(l[0]);
    this->receiver = decoding(l[1]);
    this->sender = decoding(l[2]);
    this->content = decoding(l[3]);
    //cout<<"mail():"<<decoding(l[0])<<endl;
}

mailManager::mailManager(Computer * c){
    computerBinded = c;
}

void mailManager::showContentOf(int index){
    if (!(index>=1&&index<=getList().size())){
        throw "mailManager::showContentOf():编号为"+to_string(index)+"的邮件不存在";
    }
    mail m = getContentOf(index);
    cout << "标题:" << m.subject << endl;
    cout << "发送:" << m.sender << endl;
    cout << "接收:" << m.receiver << endl;
    cout << "=======================" << endl;
    cout << m.content << endl;
}

void mailManager::showList(){
    vector<string> v = getList();
    cout<< BG_BLUE <<"编号\t标题"<< RESET <<endl;
    for (int i=0; i<v.size(); i++){
        cout<<i+1<<"\t"<<v[i]<<endl;
    }
}

mailManager::mail mailManager::getContentOf(int index){
    /*
    switch(index){
        case 1: return mail("mail_1",localhost->username,"nic","mail_1_content");
        case 2: return mail("mail_2",localhost->username,"nic","mail_2_content");
        case 3: return mail("mail_3",localhost->username,"nic","mail_3_content");
        case 4: return mail("mail_4",localhost->username,"nic","mail_4_content");
        case 5: return mail("mail_5",localhost->username,"nic","mail_5_content");
        case 6: return mail("mail_6",localhost->username,"nic","mail_6_content");
        case 7: return mail("mail_7",localhost->username,"nic","mail_7_content");
        case 8: return mail("mail_8",localhost->username,"nic","mail_8_content");
        default:
    */
    if (index<=getList().size()){
        FileSystem::file * f = localhost->locate_file("/data/mail/"+getList()[index-1]+".mail");
        if (f){
            return mail(f);
        }
    }
    return mail("--","--","--","--");
}

vector<string> mailManager::getList(){
    vector<string> v;
    /*
    for (int i = 1;i<=task_number&&i<=8;i++){
        v.push_back(getContentOf(i).subject);
    }
    */
    FileSystem::dir * d = localhost->locate_dir("/data/mail");
    if(d){
        for (int i = 0;i<d->content.size();i++){
            v.push_back(mail(d->content[i]).subject);
        }
    }
    return v;
}

void mailManager::sendMailToSelf(mail m){
    FileSystem::dir * d = localhost->locate_dir("/data/mail");
    if(!d){
        localhost->locate_dir("/data")->add_new_dir("mail");
        d = localhost->locate_dir("/data/mail");
    }
    if (d->locate_file(m.subject+".mail")){
        string s = m.subject;
        int i=1;
        while(d->locate_file(m.subject+"("+to_string(i)+").mail")){
            i++;
        }
        m.subject = m.subject +"("+to_string(i)+")";
    }
    d->add_new_txt(m.subject+".mail", 
        encoding(m.subject)+"======"+encoding(m.receiver)+"======"+encoding(m.sender)+"======"+encoding(m.content)
    );
}

void send_mail(string subject,string sender,string content){
    mailManager m(localhost);
    m.sendMailToSelf(mailManager::mail(subject,localhost->username,sender,content));
    cout<<BG_BLUE<<"您收到一封加密邮件！输入mail2查看"<<RESET<<endl;
}

int exe_mail2(int n,const char **t,Computer *c)
{
    mailManager m(c);
    //m.sendMailToSelf(mailManager::mail("subject","receiver","sender","content"));
    //m.sendMailToSelf(mailManager::mail("subject1","receiver","sender","content"));
    //m.sendMailToSelf(mailManager::mail("subject1","receiver","sender","content"));
    //m.sendMailToSelf(mailManager::mail("subject1","receiver","sender","content"));
    //m.sendMailToSelf(mailManager::mail("subject1","receiver","sender","content"));
    //m.showList();
    //m.showContentOf(1);
    //m.showContentOf(2);
    //m.showContentOf(3);
    vector<string> v = m.getList();
    cout<<"Hmail加密邮件系统 共"<<v.size()<<"条邮件"<<endl;
    cout<<"========================="<<endl;
    m.showList();
    cout<<"操作（r：阅读 R：阅读最后一条 d：删除）>";
    string input;
    getline(cin,input);
    if (input == "r"){
        cout<<"阅读邮件的编号：";
        int i;
        string s;
        //cin >> i;
        getline(cin,s);
        stringstream ss;
        ss<<s;
        ss>>i;
        m.showContentOf(i);
    } else if (input == "R"){
        m.showContentOf(v.size());
    } else if (input == "d"){
        cout<<"删除邮件的编号：";
        int i;
        string s;
        //cin >> i;
        getline(cin,s);
        stringstream ss;
        ss<<s;
        ss>>i;
        c->process_command("rm /data/mail/"+v[i-1]+".mail");
    }
    return 0;
}
