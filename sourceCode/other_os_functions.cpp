#ifndef FOR_XES
#include "hackgame.h"
#endif
//εйl
#ifdef LINUX_OS
void localtime_s_f(struct tm *ts,time_t *timep)
{
    localtime_r(timep,ts);
}
#include <unistd.h>
#ifdef NO_DELAY
void delay(double time){}
#else
void delay(double time)//
{
    usleep(1000000*time);
}
#endif
void clearScreen()
{
    system("clear");
}
#else
#ifdef WINDOWS_OS
void localtime_s_f(struct tm *ts,time_t *timep)
{
    localtime_s(ts,timep);
}
#include <windows.h>
#ifdef NO_DELAY
void delay(double time){}
#else
void delay(double time)//
{
    Sleep(time*1000);
}
#endif
void clearScreen()
{
    system("cls");
}
#else
#ifdef TEST_WINDOWS
void clearScreen()
{
    system("clear");
}
void delay(double time)//
{
    usleep(1000000*time);
}
#else
#error Error opitions!
#endif
#endif
#endif
