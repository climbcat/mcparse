#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "jg_baselayer.h"
#include "parsecore.h"
#include "phelpers.h"
#include "pinstr.h"
#include "pcomp.h"


void *ParseComponents(MArena *a_parse, StrLst *fpaths, bool print_details) {
    MArena stack_files = ArenaCreate();

    s32 i = 0;
    while (fpaths) {
        ArenaClear(&stack_files);
        char *filename = StrLstNext(&fpaths);

        if (!IsCompFile(filename)) {
            printf("skipping #%.3d: %s\n", i, filename);
            continue;
        }

        char *text = (char*) LoadFileFSeek(&stack_files, filename);
        if (text == NULL) {
            continue;
        }

        // DBG



        printf("parsing  #%.3d: %s  ", i, filename);
        //InstrDef instr = ParseInstrument(&tokenizer, a_parse);
        Component comp = ComponentParse(a_parse, text);
        ComponentPrint(comp);
        //printf(" -> %s: OK\n", instr.name);

        //
        //if (print_details) {
        //    PrintInstrumentParse(instr);
        //    exit(0);
        //}

        ++i;
    }

    // TODO: return the parsed data structure / AST thing
    return NULL;
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
    else if (argc != 2) {
        printf("Provide a component file (.comp) or folder containing component files\n");
        exit(0);
    }
    else {
        StringInit();

        StrLst *fpaths = GetFilesInFolderPaths_Rec(argv[1], NULL, NULL, "comp", true)->first;
        //StrLstPrint(fpaths);

        // parse
        MArena a_work = ArenaCreate();
        ParseComponents(&a_work, fpaths, true);
    }
}
