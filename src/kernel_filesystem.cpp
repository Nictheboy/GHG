#ifndef FOR_XES
#include "hackgame.h"
#endif

bool FileSystem::is_legal_name(string name)
{
	DEBUG_FS("is_legal_name(" << name << ")")
		if (name.length() < 1)
		{
			return false;
		}
	for (int i = 0; i < name.length(); i++)
	{
		char c = name.c_str()[i];
		if (c == '\"' || c == '*' || c == '<' || c == '>' || c == '?' || c == '\\' || c == '|' || c == '/' || c == ':' || c == ' ')
		{
			return false;
		}
	}
	return true;
}

FileSystem::file::file(string n, size_t s, void* d)
{
	DEBUG_FS("file(" << n << ", ..., ...)")
		if (!is_legal_name(n))
		{
			throw illegal_argument();
		}
	name = n;
	size = 0;
	data = nullptr;
	load_data(s, d);
}
FileSystem::file::file(string n, string s)
{
	DEBUG_FS("file(" << n << ", ...)")
		if (!is_legal_name(n))
		{
			throw illegal_argument();
		}
	name = n;
	data = nullptr;
	size = 0;
	load_string(s);
}
FileSystem::file::file(string n, exe_adr adr)
{
	DEBUG_FS("file(" << n << ", ...)")
		if (!is_legal_name(n))
		{
			throw illegal_argument();
		}
	name = n;
	load_exe(adr);
}
FileSystem::file::~file()
{
	DEBUG_FS("~file()")
		if (data)
			free(data);
	data = nullptr;
}
bool FileSystem::file::run_exe(int n, const char** c, Computer* computer)
{
	DEBUG_FS("run_exe()")
		char* tmp = (char*)data;
	if (tmp[0] == 'E' && tmp[1] == 'X' && tmp[2] == 'E')
	{
		exe_adr e = *(exe_adr*)((char*)data + 3);
		int ret = -1;

		//复制参数
		char** c_new = new char* [n];
		for (int i = 0; i < n; i++)
		{
			size_t size_temp = strlen(c[i]) + 1;
			c_new[i] = new char[size_temp];
			strcpy_s(c_new[i], size_temp, c[i]);
		}

		ret = e(n, (const char**)c_new, computer);

		//释放参数
		for (int i = 0; i < n; i++)
		{
			delete[] c_new[i];
		}
		delete[] c_new;

		if (ret == 0)
		{
			return (true);
		}
		else
		{
			cout << "exe文件" << this->name << "未正常退出，返回值" << ret << endl;
			return (false);
		}
	}
	else
	{
		return (false);
	}
}

FileSystem::dir::dir(string n)
{
	DEBUG_FS("dir(" << n << ")")
		if (!is_legal_name(n))
		{
			throw illegal_argument();
		}
	name = n;
}

FileSystem::dir::~dir()
{
	DEBUG_FS("~dir()")
		for (int i = 0; i < content.size(); i++)
		{
			delete content[i];
		}
	for (int i = 0; i < subdir.size(); i++)
	{
		delete subdir[i];
	}
}

void FileSystem::dir::add_file(file* f, bool auto_rename)
{
	DEBUG_FS("add_file(" << f->name << ")")
		if (!f)
		{
			throw illegal_argument();
		}
	if (search_file(f->name) != -1)
	{
		if (auto_rename)
		{
			int i = 1;
			while (search_file(f->name + "(" + to_string(i) + ")") != -1) i++;
			f->rename(f->name + "(" + to_string(i) + ")");
			content.push_back(f);
		}
		else
		{
			throw already_exist();
		}
	}
	else
	{
		content.push_back(f);
	}
}

void FileSystem::dir::add_dir(dir* d)
{
	DEBUG_FS("add_dir(" << d->name << ")")
		if (!d)
		{
			throw illegal_argument();
		}
	if (search_file(d->name) != -1)
	{
		throw already_exist();
	}
	subdir.push_back(d);
	d->father = this;
}

void FileSystem::dir::delete_file(string filename)
{
	DEBUG_FS("delete_file(" << filename << ")")
		int where = search_file(filename);
	if (where == -1)
	{
		throw not_exist();
	}
	for (int i = where; i < content.size() - 1; i++)
	{
		content[i] = content[i + 1];
	}
	content.resize(content.size() - 1);
}

void FileSystem::dir::delete_dir(string dirname)
{
	DEBUG_FS("delete_dir(" << dirname << ")")
		int where = search_subdir(dirname);
	if (where == -1)
	{
		throw not_exist();
	}
	for (int i = where; i < subdir.size() - 1; i++)
	{
		subdir[i] = subdir[i + 1];
	}
	subdir.resize(subdir.size() - 1);
}

int FileSystem::dir::search_file(string filename) const
{
	DEBUG_FS("search_file(" << filename << ")")
		int where = -1;
	for (int i = 0; i < content.size(); i++)
	{
		if (content[i]->name == filename)
		{
			where = i;
			break;
		}
	}
	return where;
}

int FileSystem::dir::search_subdir(string dirname) const
{
	DEBUG_FS("search_subdir(" << dirname << ")")
		int where = -1;
	for (int i = 0; i < subdir.size(); i++)
	{
		if (subdir[i]->name == dirname)
		{
			where = i;
			break;
		}
	}
	return where;
}

FileSystem::file* FileSystem::dir::locate_file(string path, bool auto_add)
{
	DEBUG_FS("locate_file(" << path << ")")
		vector<string> list = split(path, "/", true);
	dir* now = turn_file_ps_into_dir_ps(path, auto_add);
	string fname = list[list.size() - 1];
	if (!is_legal_name(fname))
	{
		throw illegal_argument();
	}
	int ps2 = now->search_file(fname);
	if (ps2 == -1)
	{
		if (auto_add)
		{
			return now->add_new_txt(fname, "");
		}
		else
		{
			throw not_exist();
		}
	}
	else
	{
		return now->content[ps2];
	}
}

FileSystem::dir* FileSystem::dir::locate_dir(string s, bool auto_add)
{
	DEBUG_FS("locate_dir(" << s << ")")
		//this->find_root()->show();
		vector<string> list = split(s, "/", true);
	size_t size = list.size();
	if (size == 0)
	{
		throw not_exist();
	}
	if (list.back() == "")
	{
		list.pop_back();
	}
	if (size == 1)
	{
		string current_name = list[0];
		if (current_name == "") {
			return this;
		}
		if (current_name == ".") {
			return this;
		}
		if (current_name == "..") {
			if (father) {
				return father;
			}
			else {
				throw not_exist();
			}
		}
		int ps3 = search_subdir(current_name);
		if (ps3 == -1)
		{
			if (auto_add)
			{
				return add_new_dir(current_name);
			}
			else
			{
				throw not_exist();
			}
		}
		else
		{
			return subdir[ps3];
		}
	}
	dir* now = this;
	if (list[0] == "")
	{
		now = find_root();
		for (int i = 1; i < size; i++)
		{
			list[i - 1] = list[i];
		}
		list.resize(size - 1);
		--size;
	}
	for (int i = 0; i < size; i++)
	{
		string current_name = list[i];
		if (current_name == ".")
		{
			// do nothing
		}
		else if (current_name == "..")
		{
			if (now->father)
			{
				now = now->father;
			}
			else
			{
				throw not_exist();
			}
		}
		else
		{
			if (!is_legal_name(current_name))
			{
				throw illegal_argument();
			}
			int ps = now->search_subdir(current_name);
			if (ps == -1)
			{
				if (auto_add)
				{
					now = now->add_new_dir(current_name);
				}
				else
				{
					throw not_exist();
				}
			}
			else
			{
				now = now->subdir[ps];
			}
		}
	}
	return now;
}

FileSystem::dir* FileSystem::dir::turn_file_ps_into_dir_ps(string path, bool auto_add)
{
	DEBUG_FS("turn_file_ps_into_dir_ps(" << path << ")")
		vector<string> list = split(path, "/", true);
	size_t size = list.size();
	if (size == 0)
	{
		throw not_exist();
	}
	if (size == 1)
	{
		return this;
	}
	dir* now = this;
	if (list[0] == "")
	{
		now = find_root();
		for (int i = 1; i < size; i++)
		{
			list[i - 1] = list[i];
		}
		list.resize(size - 1);
		--size;
	}
	for (int i = 0; i < size - 1; i++)
	{
		string current_name = list[i];
		if (current_name == ".")
		{
			// do nothing
		}
		else if (current_name == "..")
		{
			if (now->father)
			{
				now = now->father;
			}
			else
			{
				throw not_exist();
			}
		}
		else
		{
			if (!is_legal_name(current_name))
			{
				throw illegal_argument();
			}
			int ps = now->search_subdir(current_name);
			if (ps == -1)
			{
				if (auto_add)
				{
					now = now->add_new_dir(current_name);
				}
				else
				{
					throw not_exist();
				}
			}
			else
			{
				now = now->subdir[ps];
			}
		}
	}
	return now;
}

FileSystem::file* FileSystem::dir::add_new_txt(string file_name, string content_string, bool auto_rename)
{
	DEBUG_FS("add_new_txt(" << file_name << ")")
		file* n = new file(file_name, content_string);
	add_file(n, auto_rename);
	return n;
}

FileSystem::dir* FileSystem::dir::add_new_dir(string dir_name)
{
	DEBUG_FS("add_new_dir(" << dir_name << ")")
		dir* n = new dir(dir_name);
	add_dir(n);
	return n;
}

void FileSystem::dir::show(int format)
{
	if (format != 0)
	{
		for (int i = 0; i < (format - 2) / 2; i++)
		{
			cout << "| ";
		}
		cout << "|-";
	}
	cout << name << endl;
	for (int i = 0; i < content.size(); i++)
	{
		for (int j = 0; j < format / 2; j++)
		{
			cout << "| ";
		}
		cout << "|-";
		cout << content[i]->name << endl;
	}
	for (int i = 0; i < subdir.size(); i++)
	{
		subdir[i]->show(format + 2);
	}
}

FileSystem::dir* FileSystem::dir::find_root()
{
	DEBUG_FS("find_root()")
		dir* now = this;
	while (now->father)
	{
		now = now->father;
	}
	return now;
}

string FileSystem::file::read_string() {
	DEBUG_FS("read_string()")
		if (size == 0 || !data) {
			return "";
		}
	void* temp = malloc(size + 1);
	assert(temp);
	memcpy(temp, data, size);
	((char*)temp)[size + 1 - 1] = '\0';
	string ret = string((char*)temp);
	free(temp);
	return ret;
}

void FileSystem::file::load_string(string str) {
	DEBUG_FS("load_string(" << str << ")")
		if (data) free(data);
	size = str.size() + 1;
	//data = new char[size];
	data = (char*)malloc(size);
	assert(data);
	((char*)data)[size - 1] = '\0';
	memcpy(data, str.c_str(), size - 1);
}

void FileSystem::file::load_data(size_t s, void* d) {
	DEBUG_FS("load_data()")
		if (data) free(data);
	size = s;
	//data = new char[size];
	data = (char*)malloc(size);
	assert(data);
	memcpy(data, d, size);
}

bool FileSystem::dir::if_file_exist(string path)
{
	DEBUG_FS("if_file_exist(" << path << ")")
		try {
		locate_file(path);
		return true;
	}
	catch (not_exist) {
		return false;
	}
}

bool FileSystem::dir::if_dir_exist(string path)
{
	DEBUG_FS("if_dir_exist(" << path << ")")
		try {
		locate_dir(path);
		return true;
	}
	catch (not_exist) {
		return false;
	}
}

FileSystem::file::file(file* f) {
	name = f->name;
	size = 0;
	data = nullptr;
	load_data(f->size, f->data);
}

void FileSystem::file::rename(string n)
{
	if (!is_legal_name(n)) {
		throw illegal_argument();
	}
	name = n;
}

void FileSystem::dir::copy_a_file(file* from, bool auto_rename)
{
	DEBUG_FS("copy_a_file(" << from->name << ")")
		add_file(new file(from), auto_rename);
}

void FileSystem::file::load_exe(exe_adr adr)
{
	if (data) free(data);
	size = sizeof(char) * 3 + sizeof(exe_adr);
	//data = new char[size];
	data = (char*)malloc(size);
	char* tmp = (char*)data;
	tmp[0] = 'E';
	tmp[1] = 'X';
	tmp[2] = 'E';
	exe_adr* e = (exe_adr*)((char*)data + 3);
	*e = adr;
}
