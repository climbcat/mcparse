#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "jg_baselayer.h"
#include "parsecore.h"
#include "phelpers.h"
#include "pinstr.h"


int ParseInstrMain(int argc, char **argv) {

    char *input = argv[1];

    if (argc != 2) {
        printf("Input folder or file.\n");
        exit(0);
    }

    MArena stack_files = ArenaCreate();
    MArena stack_work = ArenaCreate();
    StringInit();

    StrLst *fpaths = NULL;
    bool print_detailed = false;

    if (IsInstrFile(input)) {
        fpaths = StrLstPush(fpaths, input);
        print_detailed = true;
    }
    else {
        fpaths = GetFilesInFolderPaths(&stack_files, input);
    }

    s32 i = 0;
    while (fpaths) {
        ArenaClear(&stack_work);
        char *filename = StrLstNext(&fpaths);

        if (!IsInstrFile(filename)) {
            printf("skipping #%.3d: %s\n", i, filename);
            continue;
        }

        char *text = (char*) LoadFileFSeek(&stack_files, filename);
        if (text == NULL) {
            continue;
        }
        Tokenizer tokenizer = {};
        tokenizer.Init(text);

        printf("parsing  #%.3d: %s  ", i, filename);
        InstrDef instr = ParseInstrument(&tokenizer, &stack_work);
        printf(" -> %s: OK\n", instr.name);

        if (print_detailed) {
            PrintInstrumentParse(instr);
            exit(0);
        }

        ++i;
    }
    return 0;
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
        printf("No registered tests ...\n");
        exit(0);
    }
    if (argc != 2) {
        printf("Supply instrument file (.instr) to parse.\n");
        exit(0);
    }

    return ParseInstrMain(argc, argv);
}
