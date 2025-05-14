#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "jg_baselayer.h"
#include "parsecore.h"
#include "phelpers.h"
#include "pinstr.h"


HashMap ParseInstruments(MArena *a_parse, StrLst *fpaths, bool print_details) {
    MArena a_files = ArenaCreate();

    HashMap map_comps = InitMap(a_parse, 1000);
    s32 comp_count_registered = 0;
    s32 comp_count_parsed = 0;
    
    while (fpaths) {
        char *filename = StrLstNext(&fpaths);
        char *text = (char*) LoadFileFSeek(&a_files, filename);
        if (text == NULL) {
            continue;
        }

        printf("parsing  #%.3d: %s \n", comp_count_parsed, filename);

        Instrument *instr = ParseInstrument(a_parse, text);
        //InstrumentPrint(instr);

        comp_count_parsed++;
    }

    return map_comps;
}


void TestLookForToken(int argc, char **argv) {
    printf("TestLookForToken\n");

    StringInit();
    StrLst *fpaths = GetFiles(argv[2], "instr", true);
    MArena a_files = ArenaCreate();

    while (fpaths) {
        char *filename = StrLstNext(&fpaths);
        char *text = (char*) LoadFileFSeek(&a_files, filename);
        if (text == NULL) {
            continue;
        }

        TestPrintTokensOfType(text, TOK_MCSTAS_C_EXPRESSION);
    }
}


int main (int argc, char **argv) {
    TimeProgram;

    if (CLAContainsArg("--dbg", argc, argv)) {
        dbg_print = true;
    }

    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("[instr]:         folder or instrument file\n");
        printf("--help:          display help (this text)\n");
        printf("--dbg:           display help (this text)\n");
        printf("--test:          run test functions\n");
        exit(0);
    }
    else if (CLAContainsArg("--test", argc, argv)) {
        TestLookForToken(argc, argv);
    }
    else {
        StringInit();
        StrLst *fpaths = GetFiles(argv[1], "instr", true);

        MArena a_work = ArenaCreate();
        ParseInstruments(&a_work, fpaths, true);
    }
}
