#include "syscall.h"

void main() {
    int PingPID, PongPID;
    PrintString("Ping-Pong test starting...\n\n");
    PingPID = Exec("./test/Ping");
    PongPID = Exec("./test/Pong");
}
