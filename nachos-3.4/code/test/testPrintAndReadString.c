#include "syscall.h"

char name[100];

int main() {
    PrintString("Input your name: ");

    ReadString(name, 50);

    PrintString("Your name is: ");

    PrintString(name);

    Halt();
    return 0;
}
