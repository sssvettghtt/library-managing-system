#include <windows.h>
#include "Library.hpp"

int main(){
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

   Library lib;
   lib.run();

    return 0;

}