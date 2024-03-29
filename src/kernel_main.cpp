
#ifndef FOR_XES
#include "hackgame.h"
#endif

//主程序
int main()
{
	try {
		clearScreen();
		logo();
		init_preload_ptr_table();
		loadgame();
		localhost->run(false, "本地控制台");//启动电脑！
	}
	catch (...) {
		if (localhost) {
			cout << "游戏抛出了未处理的异常，已经崩溃！幸而您可以导出当前的存档。" << endl;
			localhost->process_command("/bin/savegame.exe");
			cout << "请联系开发者（QQ：2492515121）获取帮助。我们将感谢您的反馈！" << endl;
		}
		else {
			cout << "游戏在加载时崩溃！请联系开发者（QQ：2492515121）获取帮助。我们将感谢您的反馈！" << endl;
		}
	}
	cout << "请保存游戏以便下次再来" << endl;
	localhost->process_command("/bin/savegame.exe");
	return 0;
}

#ifdef WINDOWS_OS
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
) {
	//这里是Windows下的主程序入口

	//打开一个控制台窗口
	AllocConsole();
	FILE* out;
	FILE* in;
	freopen_s(&out, "CONOUT$", "w", stdout);
	freopen_s(&in, "CONIN$", "r", stdin);

	// 设置控制台窗口的代码页为UTF-8
	// 以防止中文显示为乱码
	SetConsoleOutputCP(65001);
	SetConsoleCP(65001);


	// 设置控制台窗口的字体为SimSun-ExtB
	// 以防止中文无法显示
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 16;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"SimSun-ExtB");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	//调用主程序
	int ret = main();

	//关闭控制台窗口
	FreeConsole();
	return ret;
}
#endif
