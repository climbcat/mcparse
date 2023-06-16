#include "lib.c"


void Test() {
    printf("Running tests ...\n");

    printf("ArenaCreate\n");
    MArena arena = ArenaCreate();
    MArena *a = &arena;

    printf("StrLiteral\n");
    String s1 = StrLiteral(&arena, "hello");
    String s2 = StrLiteral(&arena, "hello_noteq");
    String s3 = StrLiteral(&arena, "hello");

    printf("StrPrint - ");
    StrPrint(s1);
    printf("\n");

    printf("StrEqual - ");
    printf("should be (0 1 0): %d %d %d\n", StrEqual(s1, s2), StrEqual(s1, s3), StrEqual(s2, s3));

    printf("StrCat - ");
    StrPrint(StrCat(&arena, s1, s3));
    printf("\n");

    printf("StrSplit / StrLstPrint - ");
    String to_split = StrLiteral(a, "...Hello.I..Have.Been.Split..");
    printf("splitting: ");
    StrPrint(to_split);
    printf(" into: ");
    StringList *lst = StrSplit(a, to_split, '.');
    StrLstPrint(lst);
    printf("\n");

    printf("StrJoin - ");
    String join = StrJoin(a, lst);
    StrPrint(join);
    printf("\n");
    join = StrJoinInsertChar(a, lst, '/');
    StrPrint(join);
    printf("\n");

    printf("CLAInit - ");
    if (CLAContainsArg("--test", g_argc, g_argv)) {
        printf("OK");
    }
    printf("\n");
}


void DoStuff() {
    printf("Executing program ...\n");
}

int main (int argc, char **argv) {
    CLAInit(argc, argv);
    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("--help:          display help (this text)\n");
        exit(0);
    }
    if (CLAContainsArg("--test", argc, argv)) {
        Test();
        exit(0);
    }

    DoStuff();
}
