#include "hackgame.h"

//εйl
#ifdef LINUX_OS
void localtime_s_f(struct tm *ts,time_t *timep)
{
    localtime_r(timep,ts);
}
void clearScreen()
{
    system("clear");
}
void ChangeColor(string _Color) {
  cout << _Color;
}
#else
#ifdef WINDOWS_OS
void localtime_s_f(struct tm *ts,time_t *timep)
{
    localtime_s(ts,timep);
}
void ChangeColor(int _Color) {
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), _Color);
}
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
#endif // TEST_WINDOWS
#endif // WINDOWS_OS
#endif // LINUX_OS

#ifdef NO_DELAY
void delay(double time) {}
#else
void delay(double time) {
  clock_t tmp = clock();

  while (1) {
    if ((double)(clock() - tmp) / CLOCKS_PER_SEC >= time) break;
  }
}
#endif // NO_DELAY