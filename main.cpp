#include "exec.h"

#if defined(_WIN32)
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )
#endif

int main(int argc, char *argv[]) {
    Exec program(argc,argv);
    return program.Run();
}
