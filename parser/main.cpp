#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "jg_baselayer.h"
#include "parsecore.h"
#include "phelpers.h"
#include "pcomp.h"
#include "pinstr.h"


bool RegisterComponentType(Component *comp, HashMap *map) {
    u64 val = MapGet(map, comp->type);
    bool type_was_unique = (val == 0);
    if (type_was_unique) {
        MapPut(map, comp->type, comp);
    }

    return type_was_unique;
}

bool RegisterInstrument(Instrument *instr, HashMap *map) {
    u64 val = MapGet(map, instr->name);
    bool name_is_unique = (val == 0);
    if (name_is_unique) {
        MapPut(map, instr->name, instr);
    }

    return name_is_unique;
}


HashMap ParseInstruments(MArena *a_parse, StrLst *fpaths) {
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


HashMap ParseComponents(MArena *a_parse, StrLst *fpaths) {
    MArena a_files = ArenaCreate();

    HashMap map_comps = InitMap(a_parse, StrListLen(fpaths) * 3);
    s32 comp_count_registered = 0;
    s32 comp_count_parsed = 0;
    
    while (fpaths) {
        char *filename = StrLstNext(&fpaths);
        char *text = (char*) LoadFileFSeek(&a_files, filename);
        if (text == NULL) {
            continue;
        }

        printf("parsing  #%.3d: %s \n", comp_count_parsed, filename);
        Component *comp = ParseComponent(a_parse, text);
        comp_count_parsed++;

        if (RegisterComponentType(comp, &map_comps)) {
            comp_count_registered++;
        }
    }

    printf("\n");
    printf("Parsed %d, registered %d components; total data size %lu bytes\n", comp_count_parsed, comp_count_registered, a_parse->used + a_files.used);
    printf("\n");

    return map_comps;
}


int main (int argc, char **argv) {
    TimeProgram;

    if (CLAContainsArg("--dbg", argc, argv)) {
        dbg_print = true;
    }

    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("[instr]:         folder or instrument file\n");
        printf("--help:          display help (this text)\n");
        printf("--dbg:           enable debug print\n");
        printf("--test:          run test functions\n");
        exit(0);
    }
    else if (CLAContainsArg("--test", argc, argv)) {
        printf("No registered tests ...\n");
    }
    else {
        StringInit();
        StrLst *comp_paths = GetFiles(argv[1], "comp", true);
        StrLst *instr_paths = GetFiles(argv[1], "instr", true);

        MArena a_work = ArenaCreate();
        HashMap components = ParseComponents(&a_work, comp_paths);
        HashMap instruments = ParseInstruments(&a_work, instr_paths);

        printf("\nParsed %d Components:\n\n", components.noccupants);
        MapIter iter = {};
        while (Component *comp = (Component*) MapNextVal(&components, &iter)) {
            printf("COMPONENT: "); StrPrint(comp->type); printf("\n");
        }

        printf("\nParsed %d Instruments:\n\n", instruments.noccupants);
        iter = {};
        while (Instrument *instr = (Instrument*) MapNextVal(&instruments, &iter)) {
            printf("INSTRUMENT: "); StrPrint(instr->name); printf("\n");
        }
    }
}
