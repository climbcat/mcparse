#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "jg_baselayer.h"
#include "parsecore.h"
#include "phelpers.h"
#include "pcomp.h"


bool RegisterComponentType(Component *comp, HashMap *map) {
    u64 val = MapGet(map, comp->type);
    bool unreg = (val == 0);
    if (unreg) {
        MapPut(map, comp->type, comp);

        u64 check = MapGet(map, comp->type);
        assert(check == (u64) comp);
    }

    return unreg;
}

HashMap ParseComponents(MArena *a_parse, StrLst *fpaths, bool print_details) {
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
        Component *comp = ParseComponent(a_parse, text);
        comp_count_parsed++;

        if (RegisterComponentType(comp, &map_comps)) {
            comp_count_registered++;
        }
    }

    if (map_comps.slots.len < 2 * comp_count_registered) {
        printf("WARN: increase map slot count (ncomps: %d, nslots: %u)\n", comp_count_registered, map_comps.slots.len);
    }

    // print registered components
    s32 printed_cnt = 0;
    MapIter iter = {};
    while (Component *comp = (Component*) MapNextVal(&map_comps, &iter)) {
        ComponentPrint(comp);
        printed_cnt++;
    }

    printf("\n");
    printf("Parsed %d, registered %d components; total data size %lu bytes\n", comp_count_parsed, comp_count_registered, a_parse->used + a_files.used);
    printf("Printed %d components (%d slots and %d collisions)\n", printed_cnt, iter.occ_slots_cnt, iter.occ_colliders_cnt);

    return map_comps;
}


int main (int argc, char **argv) {
    TimeProgram;

    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("[component]:     folder or component file\n");
        printf("--help:          display help (this text)\n");
        exit(0);
    }
    else if (argc != 2) {
        printf("Provide a component file (.comp) or folder containing component files\n");
        exit(0);
    }
    else {
        StringInit();
        StrLst *fpaths = GetFiles(argv[1], "comp", true);

        MArena a_work = ArenaCreate();
        ParseComponents(&a_work, fpaths, true);
    }
}
