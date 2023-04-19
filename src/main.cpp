#include <iostream>
#include <windows.h>
#include "windows_hard_drives.h"

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    Hard_Drives hard_drives;

    hard_drives.print_table();

    return 0;
}
