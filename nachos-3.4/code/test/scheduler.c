#include "syscall.h"

void main() {
    int PingPID, PongPID;
    PrintString("Ping-Pong test starting...\n\n");
    PongPID = Exec("./test/Pong");
    PingPID = Exec("./test/Ping");
}
