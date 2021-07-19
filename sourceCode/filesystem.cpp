#ifndef FOR_XES
#include "hackgame.h"
#endif
FileSystem::file::file( string n, size_t s, void *d )
{
	name	= n;
	size	= s;
	data	= d;
}
FileSystem::file::file( string n, string s )
{
	name	= n;
	size	= sizeof(char) * (s.length() + 1);
	data	= malloc( size );
	memcpy( data, s.c_str(), size );
}
FileSystem::file::file( string n, exe_adr adr )
{
	name	= n;
	size	= sizeof(char) * 3 + sizeof(exe_adr);
	data	= malloc( size );
	char *tmp = (char *) data;
	tmp[0]	= 'E';
	tmp[1]	= 'X';
	tmp[2]	= 'E';
	exe_adr *e = (exe_adr *) ( (char *) data + 3);
	*e = adr;
}
FileSystem::file::~file()
{
	free(data);
}
bool FileSystem::file::run_exe( int i, const char **c, Computer *computer)
{
	char *tmp = (char *) data;
	if ( tmp[0] == 'E' && tmp[1] == 'X' && tmp[2] == 'E' )
	{
		exe_adr e	= *(exe_adr *) ( (char *) data + 3);
		int	ret	= -1;
		ret = e( i, c, computer );
		if ( ret == 0 )
		{
			return(true);
		}else{
			cout << "exe文件"<<this->name<<"未正常退出，返回值"<<ret<<endl;
			return(false);
		}
	}else{
		return(false);
	}
}


FileSystem::dir::dir( string n )
{
	//file_number	= 0;
	//subdir_number	= 0;
	name		= n;
}

FileSystem::dir::~dir()
{
	for (int i=0; i<content.size(); i++){
		delete content[i];
	}
	for (int i=0; i<subdir.size(); i++){
		delete subdir[i];
	}
}

bool FileSystem::dir::add_file( file *f )
{
    //cout<<"Ok";
    /*
	if ( content.size() > 99 )
	{
		return(false);
	}
	*/
	if ( f == NULL )
	{
		throw "FileSystem::dir::add_file():文件为NULL";
		//return(false);
	}
	//cout<<"Ok";
	//cout<<content.size();
	//cout<<"Ok";
	for ( int i = 0; i < content.size(); i++ )
	{
		if ( content[i]->name == f->name )
		{
			throw "FileSystem::dir::add_file():文件"+f->name+"已存在创建!";
			//cout << "文件已存在创建!\n";
			//return(false);
		}
	}
	content.resize(content.size() + 1);
	content[content.size() - 1] = f;
	//cout<<"OK";
	return(true);
}


bool FileSystem::dir::add_dir( dir *d )
{
    //cout<<"OK";
    /*
	if ( subdir.size() > 99 )
	{
		return(false);
	}
	*/
	if ( d == NULL )
	{
		throw "FileSystem::dir::add_dir():传入文件夹为NULL";
		//cout << "内部错误：文件夹为NULL";
		//return(false);
	}
	for ( int i = 0; i < subdir.size(); i++ )
	{
		if ( subdir[i]->name == d->name )
		{
			throw "FileSystem::dir::add_dir():文件夹"+d->name+"已经存在";
			//return(false);
		}
	}
	//subdir_number++;
	subdir.resize(subdir.size() + 1);
	subdir[subdir.size() - 1] = d;
	d->father=this;
	//cout<<"OK";
	return(true);
}


bool FileSystem::dir::delete_file( string filename )
{
	int where;
	where = search_file( filename );
	if ( where == -1 )
	{
		return(false);
	}
	for ( int i = where; i < content.size() - 1; i++ )
	{
		content[i] = content[i + 1];
	}
	content.resize(content.size()-1);
	return(true);
}


bool FileSystem::dir::delete_dir( string dirname )
{
	int where;
	where = search_subdir( dirname );
	if ( where == -1 )
	{
		return(false);
	}
	for ( int i = where; i < subdir.size() - 1; i++ )
	{
		subdir[i] = subdir[i + 1];
	}
	subdir.resize(subdir.size()-1);
	return(true);
}


int FileSystem::dir::search_file( string filename ) const
{
	int where = -1;
	for ( int i = 0; i < content.size(); i++ )
	{
		if ( content[i]->name == filename )
		{
			where = i;
			break;
		}
	}
	return(where);
}


int FileSystem::dir::search_subdir( string dirname ) const
{
	int where = -1;
	for ( int i = 0; i < subdir.size(); i++ )
	{
		if ( subdir[i]->name == dirname )
		{
			where = i;
			break;
		}
	}
	return(where);
}


FileSystem::file* FileSystem::dir::locate_file( string s ) const
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
	const dir		*to = this;
	vector<string>::iterator	itor;
	itor = listed_cmd.begin();
	if ( size == 0 )
	{
		return(NULL);
	}
	if ( size == 1 )
	{
		int ps = search_file( *itor );
		if ( ps == -1 )
		{
			return(NULL);
		}
		return(content[ps]);
	}


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
			if (to->father)
			{
		    	to = to->father;
			}
			else
			{
				return NULL;
			}
		}
		else
		{
            int ps = to->search_subdir( *itor );
            if ( ps == -1 )
            {
                return(NULL);
            }
		    to = to->subdir[ps];
		}
	}
	itor++;
	int ps = to->search_file( *itor );
	if ( ps == -1 )
	{
		return(NULL);
	}
	return(to->content[ps]);
}


FileSystem::dir* FileSystem::dir::locate_dir( string s ) const
{
    //cout<<"locate_dir()@"<<name<<":"<<s<<endl;
    if (DEBUG_FLAG)
    {
        //cout<<"locate_dir:"<<s<<endl;
    }
	string		command = s;
	vector<string>	listed_cmd;
	listed_cmd = split(command, "/");
	int size = 0;
	for (auto iter = listed_cmd.begin(); iter != listed_cmd.end(); iter++ )
	{
		size++;
	}

	if ( size == 0 )
	{
		return(NULL);
	}
	auto itor = listed_cmd.begin();
	if ( size == 1 )
	{
	    if (*itor=="."||*itor=="..")
	    {
	        return father;
	    }
		int ps = search_subdir( *itor );
		if ( ps == -1 )
		{
			return(NULL);
		}
		return(subdir[ps]);
	}
	const dir *to = this;


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
            int ps = to->search_subdir( *itor );
            if ( ps == -1 )
            {
                return(NULL);
            }
		    to = to->subdir[ps];
		}
	}
	itor++;
	int ps = to->search_subdir( *itor );
	if ( ps == -1 )
	{
		return(NULL);
	}
	return(to->subdir[ps]);
}

FileSystem::dir* FileSystem::dir::turn_file_ps_into_dir_ps( string fp )
{
	string		command = fp;
	vector<string>	listed_cmd;
	string		pattern = "/";
	listed_cmd = split(command, pattern);
	int size = 0;
	for ( auto iter = listed_cmd.begin(); iter != listed_cmd.end(); iter++ )
	{
		size++;
	}


	dir			*to = this;
	auto	itor = listed_cmd.begin();
	if ( size == 0 )
	{
		return(NULL);
	}
	if ( size == 1 )
	{
		return(this);
	}
	for ( int i = 1; i < size; i++ )
	{
		int ps = to->search_subdir( *itor );
		if ( ps == -1 )
		{
			return(NULL);
		}
		to = to->subdir[ps];
	}
	return(to);
}

bool FileSystem::dir::add_new_txt( string name, string content )
{
	file *n;
	n = new file( name, content );
	add_file( n );
	return(true);
}


bool FileSystem::dir::add_new_dir( string name )
{
	dir *n;
	n = new dir( name );
	add_dir( n );
	return(true);
}

void FileSystem::dir::show(int format)
{
    if (format != 0)
    {
        for (int i=0; i<(format-2)/2; i++)
        {
            cout<<"| ";
        }
        cout<<"|-";
    }
    cout<<name<<endl;
    for (int i=0;i<content.size();i++)
    {
        for (int i=0; i<format/2; i++)
        {
            cout<<"| ";
        }
        cout<<"|-";
        cout<<content[i]->name<<endl;
    }
    for (int i=0;i<subdir.size();i++)
    {
        subdir[i]->show(format+2);
    }
}
