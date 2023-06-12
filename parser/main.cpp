#include "lib.c"


void Test() {
    printf("Running tests ...\n");
}


void DoStuff() {
    printf("Executing program ...\n");
}

int main (int argc, char **argv) {
    if (ContainsArg("--help", argc, argv) || ContainsArg("-h", argc, argv)) {
        printf("--help:          display help (this text)\n");
        exit(0);
    }
    if (ContainsArg("--test", argc, argv)) {
        Test();
        exit(0);
    }

    DoStuff();
}
