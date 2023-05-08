#include "Pointer.h"
#include "LeakTester.h"

int main()
{
    Pointer<int> p = new int(19);
    p = new int(21);
    p = new int(28);

    return 0;
}

//g++ -std=c++1y -Wall -o main.exe main.cpp
