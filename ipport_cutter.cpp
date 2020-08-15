#include "hackgame.h"

ipport_cutter::ipport_cutter( string ip_port, int port_for_no_port )
{
	vector<string>	ipport	= split( ip_port, ":" );
	vector<string>	ipportc = split( ip_port, "：" );
	if ( ipportc.size() > ipport.size() )
	{
		ipport = ipportc;
	}
	if ( ipport.size() == 2 )
	{
		/* cout<<1; */
		ip = *ipport.begin();
		/*
		 * cout<<ip;
		 * cout<<3;
		 */
		port = atoi( ipport.at( ipport.size() - 1 ).c_str() );
		/*
		 * cout<<port;
		 * cout<<4;
		 */
	}else if ( ipport.size() == 1 )
	{
		ip	= ip_port;
		port	= port_for_no_port;
	}else{
		cout << "IP格式错误！正确格式为 [IP] 或 [IP]:[端口]\n";
		throw string( "IP_ERR" );
	}
}