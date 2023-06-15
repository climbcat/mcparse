#include "lib.c"


void Test() {
    printf("Running tests ...\n");

    printf("ArenaCreate\n");
    MArena arena = ArenaCreate();

    printf("StrLitteral\n");
    String s1 = StrLitteral(&arena, "hello");
    String s2 = StrLitteral(&arena, "hello_noteq");
    String s3 = StrLitteral(&arena, "hello");

    printf("StrPrint - ");
    StrPrint(s1);
    printf("\n");

    printf("StrEqual - ");
    printf("should be (0 1 0): %d %d %d\n", StrEqual(s1, s2), StrEqual(s1, s3), StrEqual(s2, s3));

    printf("StrCat - ");
    StrPrint(StrCat(&arena, s1, s3));
    printf("\n");
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
