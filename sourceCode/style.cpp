#ifndef FOR_XES
#include "hackgame.h"
#endif

Style::Style()
{
	login_output		= "请输入用户名:";
	pass_output		= "请输入密码:";
	login_successful	= "";
	login_wrong		= "密码错误，请重试!";
	string	before_ip	= "[";
	string	between_ip	= "@";
	string	after_ip	= "]";
	login_fail	= "登陆失败";
	getinput	= ">>";
}

Style::Style( string s1, string s2, string s3, string s4, string s5,
       string s6, string s7, string s8, string s9 )
{
	login_output		= s1;
	pass_output		= s2;
	login_successful	= s3;
	login_wrong		= s4;
	login_fail		= s5;
	before_ip		= s6;
	between_ip		= s7;
	after_ip		= s8;
	getinput		= s9;
}
