#include "winfix.hpp"

#include <windows.h>
#include <winnls.h>

namespace helpers::winfix {

void FixWinConsole(){
    SetConsoleOutputCP(CP_UTF8);
}
}