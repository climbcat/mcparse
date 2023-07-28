#include <cstdlib>
#include <cstdio>
#include <cstddef>

#include "../../baselayer/base.c"
#include "../../baselayer/memory.c"
#include "../../baselayer/string.c"
#include "../../baselayer/utils.c"
#include "../../baselayer/profile.c"


void Test() {
    printf("Running tests ...\n");

    MArena arena = ArenaCreate();
    MArena *a = &arena;
    StrLst files = GetFilesInFolderPaths(a, (char*) "/home");
    StrLstPrint(files);
    exit(0);
}

void ParseInstr(MArena *a, Str instr) {
    StrPrint("\n%s\n", instr);
}

int main (int argc, char **argv) {
    CLAInit(argc, argv);
    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("[instr]:         instrument file\n");
        printf("--help:          display help (this text)\n");
        printf("--test:          run test function\n");
        exit(0);
    }
    if (CLAContainsArg("--test", argc, argv)) {
        Test();
        exit(0);
    }
    if (argc != 2) {
        printf("Supply instrument file (.instr) to parse.\n");
        exit(0);
    }

    // memory
    MArena arena = ArenaCreate();

    // load instr
    Str instr;
    instr.str = LoadFileMMAP(argv[1]);
    instr.len = strlen(instr.str);
    printf("Parsing %s ...\n", argv[1]);

    // parse
    ParseInstr(&arena, instr);
}
