#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "jg_baselayer.h"
#include "parser.h"
#include "phelpers.h"
#include "pinstr.h"


int ParseInstrMain(int argc, char **argv) {

/*
    char *input = argv[1];

    if (argc != 2) {
        printf("Input folder or file.\n");
        exit(0);
    }

    MArena stack_files = ArenaCreate();
    MArena stack_work = ArenaCreate();
    Array<char*> filepaths = InitArray<char*>(&stack_files, 1000);

    bool print_detailed = false;

    if (IsInstrFile(input)) {
        filepaths.Add(&input);
        print_detailed = true;
    }
    else {
        filepaths = GetFilesInFolderPaths(input, &stack_files);
    }

    for (int i = 0; i < filepaths.len; ++i) {
        stack_work.Clear();

        char *filename = *filepaths.At(i);
        if (!IsInstrFile(filename)) {
            printf("skipping #%.3d: %s\n", i, filename);
            continue;
        }

        char *text = LoadFile(filename, false, &stack_files);
        if (text == NULL) {
            continue;
        }
        Tokenizer tokenizer = {};
        tokenizer.Init(text);

        printf("parsing  #%.3d: %s  ", i, filename);

        InstrDef instr = ParseInstrument(&tokenizer, &stack_work);
        printf("  %s\n", instr.name);

        if (print_detailed) {
            PrintInstrumentParse(instr);
            exit(0);
        }
    }
*/
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

    // TODO: reintroduce
    /*
    // memory
    MArena _a = ArenaCreate();
    MArena *a = &_a;

    // load instr
    Str instr;
    instr.str = (char*) LoadFileFSeek(a, argv[1]);
    instr.len = strlen(instr.str);
    printf("Parsing %s ...\n", argv[1]);

    // parse
    ParseInstr(a, instr);
    */

    return ParseInstrMain(argc, argv);
}
