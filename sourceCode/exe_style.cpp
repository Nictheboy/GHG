#ifndef FOR_XES
#include "hackgame.h"
#endif

int exe_style(int argc, const char *argv[], Computer *sender)
{
    sender->style = style_1;
    return 0;
}
