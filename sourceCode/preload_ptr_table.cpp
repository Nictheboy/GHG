#ifndef FOR_XES
#include "hackgame.h"
#endif
void add_preload_ptr(void* ptr, string name)
{
    preload_ptr_table.push_back(pair<void*,string>(ptr,name));
}

void  init_preload_ptr_table()
{
    add_preload_ptr(NULL,"NULL");
    add_preload_ptr((void*)&task_1_1,"task_1_1");
    add_preload_ptr((void*)&task_1_2,"task_1_2");
    add_preload_ptr((void*)&task_2_1,"task_2_1");
    add_preload_ptr((void*)&task_2_2,"task_2_2");
    add_preload_ptr((void*)&task_3_1,"task_3_1");
    add_preload_ptr((void*)&task_3_2,"task_3_2");
    add_preload_ptr((void*)&task_4_1,"task_4_1");
    add_preload_ptr((void*)&task_4_2,"task_4_2");
    add_preload_ptr((void*)&task_5_1,"task_5_1");
    add_preload_ptr((void*)&task_5_2,"task_5_2");
    add_preload_ptr((void*)&task_6_1,"task_6_1");
    add_preload_ptr((void*)&task_6_2,"task_6_2");
    add_preload_ptr((void*)&task_7_1,"task_7_1");
    add_preload_ptr((void*)&task_7_2,"task_7_2");
    add_preload_ptr((void*)&task_7_3,"task_7_3");
    add_preload_ptr((void*)&task_8_1,"task_8_1");
    add_preload_ptr((void*)&task_8_2,"task_8_2");
    add_preload_ptr((void*)&task_9_1,"task_9_1");
    add_preload_ptr((void*)&task_9_2,"task_9_2");
    add_preload_ptr((void*)&exe_wget,"exe_wget");
    add_preload_ptr((void*)&exe_help,"exe_help");
    add_preload_ptr((void*)&exe_passguesser,"exe_passguesser");
    add_preload_ptr((void*)&exe_portscan,"exe_portscan");
    add_preload_ptr((void*)&exe_hvm,"exe_hvm");
    add_preload_ptr((void*)&exe_mail,"exe_mail");
    add_preload_ptr((void*)&exe_telnet,"exe_telnet");
    add_preload_ptr((void*)&exe_tree,"exe_tree");
    add_preload_ptr((void*)&exe_ipconfig,"exe_ipconfig");
    add_preload_ptr((void*)&exe_ping,"exe_ping");
    add_preload_ptr((void*)&exe_savegame,"exe_savegame");
    add_preload_ptr((void*)&exe_myexe,"exe_myexe");
    add_preload_ptr((void*)&exe_tracer,"exe_tracer");
    add_preload_ptr((void*)&exe_sniffer,"exe_sniffer");
    add_preload_ptr((void*)&exe_mail2,"exe_mail2");
//        fw.write(table.getid((void*)&exe_mujs),"exe_mujs");
}
