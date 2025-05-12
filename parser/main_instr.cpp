#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "jg_baselayer.h"
#include "parsecore.h"
#include "pcomp.h"
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

        printf("parsing (dry)  #%.3d: %s \n", comp_count_parsed, filename);

        //Instrument *instr = ParseInstrument(a_parse, text);
        //comp_count_parsed++;
    }

    return map_comps;
}


int main (int argc, char **argv) {
    TimeProgram;

    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("[instr]:         folder or instrument file\n");
        printf("--help:          display help (this text)\n");
        exit(0);
    }
    else if (argc != 2) {
        printf("Provide an instrument file (.instr) or folder containing instrument files\n");
        exit(0);
    }
    else {
        StringInit();
        StrLst *fpaths = GetFiles(argv[1], "instr", true);

        MArena a_work = ArenaCreate();
        ParseInstruments(&a_work, fpaths, true);
    }
}
