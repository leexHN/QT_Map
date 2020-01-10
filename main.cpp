#include "exec.h"


int main(int argc, char *argv[]) {
    Exec program(argc,argv);
    return program.Run();
}