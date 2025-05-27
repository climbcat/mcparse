#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "jg_baselayer.h"
#include "parsecore.h"
#include "phelpers.h"
#include "pinstr.h"


bool RegisterInstrument(Instrument *instr, HashMap *map) {
    u64 val = MapGet(map, instr->name);
    bool name_is_unique = (val == 0);
    if (name_is_unique) {
        MapPut(map, instr->name, instr);
    }

    return name_is_unique;
}


HashMap ParseInstruments(MArena *a_parse, StrLst *fpaths, bool print_details) {
    MArena a_files = ArenaCreate();

    HashMap map_instrs = InitMap(a_parse, StrListLen(fpaths) * 3);
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
        RegisterInstrument(instr, &map_instrs);

        comp_count_parsed++;
    }

    return map_instrs;
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
        HashMap map_instrs = ParseInstruments(&a_work, fpaths, true);

        MapIter iter = {};
        while (Instrument *instr = (Instrument*) MapNextVal(&map_instrs, &iter)) {
            InstrumentPrint(instr, true, true, true);
        }
    }
}
