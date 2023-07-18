#ifndef FOR_XES
#include "hackgame.h"
#endif

#ifdef LINUX_OS
int strcpy_s(char *strDestination,size_t numberOfElements,const char *strSource)
{
    strncpy(strDestination, strSource, numberOfElements);
    return 0;
}
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
    Sleep(static_cast<int>(time*1000));
}
#endif
void clearScreen()
{
    system("cls");
}
ostream& operator <<(ostream& stm, const Color& color) {//运算符重载，目的是以后可以用cout<<RED这一类本属于linux的写法
    SET(color.value);
    return stm;
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
