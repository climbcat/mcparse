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
    s32 registered_cnt = 0;
    s32 parsed_cnt = 0;

    while (fpaths) {
        char *filename = StrLstNext(&fpaths);
        char *text = (char*) LoadFileFSeek(&a_files, filename);
        if (text == NULL) {
            continue;
        }

        printf("parsing  #%.3d: %s \n", parsed_cnt, filename);
        Instrument *instr = ParseInstrument(a_parse, text);
        parsed_cnt++;

        if (RegisterInstrument(instr, &map_instrs)) {
            registered_cnt++;
        }
    }

    return map_instrs;
}

HashMap ParseComponents(MArena *a_parse, StrLst *fpaths, bool dbg_print = false) {
    MArena a_files = ArenaCreate();

    HashMap map_comps = InitMap(a_parse, StrListLen(fpaths) * 3);
    s32 registered_cnt = 0;
    s32 parsed_cnt = 0;

    while (fpaths) {
        char *filename = StrLstNext(&fpaths);
        char *text = (char*) LoadFileFSeek(&a_files, filename);
        if (text == NULL) {
            continue;
        }

        if (dbg_print) printf("parsing  #%.3d: %s \n", parsed_cnt, filename);
        Component *comp = ParseComponent(a_parse, text);
        parsed_cnt++;

        if (RegisterComponentType(comp, &map_comps)) {
            registered_cnt++;
        }
    }

    if (dbg_print) {
        printf("\n");
        printf("Parsed %d, registered %d components; total data size %lu bytes\n", parsed_cnt, registered_cnt, a_parse->used + a_files.used);
        printf("\n");
    }

    return map_comps;
}


bool TypeCheckInstrument(Instrument *instr, HashMap *comps) {
    bool has_error = false;
    for (s32 i = 0; i < instr->comps.len; ++i) {
        ComponentCall c = instr->comps.arr[i];

        // TODO: here we have to deal wih COPY declarations; 
        //      A copy component has no type defined; it is implicit
        //      and defined by the name of another comp declaration
        //      in the instrument 

        u64 comp_exists = MapGet(comps, c.type);

        if (comp_exists == 0) {
            has_error = true;

            printf("\n    Missing component type: ");
            StrPrint(c.type);
        }
    }
    return has_error;
}


int main (int argc, char **argv) {
    TimeProgram;

    if (CLAContainsArg("--dbg", argc, argv)) {
        dbg_print = true;
    }

    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("[comp_lib]:      component files path\n");
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
        char *comp_lib_path = argv[1];
        char *instr_path = argv[2];

        StringInit();
        StrLst *comp_paths = GetFiles(comp_lib_path, "comp", true);
        StrLst *instr_paths = GetFiles(instr_path, "instr", true);

        MArena a_work = ArenaCreate();
        HashMap components = ParseComponents(&a_work, comp_paths);
        HashMap instruments = ParseInstruments(&a_work, instr_paths);

        printf("\nParsed %d Components\n", components.noccupants);
        MapIter iter = {};

        //while (Component *comp = (Component*) MapNextVal(&components, &iter)) {
        //    printf("COMPONENT: "); StrPrint(comp->type); printf("\n");
        //}

        printf("\nParsed %d Instruments\n\n", instruments.noccupants);
        iter = {};
        while (Instrument *instr = (Instrument*) MapNextVal(&instruments, &iter)) {
            printf("INSTRUMENT: "); StrPrint(instr->name);
            bool has_error = TypeCheckInstrument(instr, &components);
            if (has_error == false) {
                printf(" - OK");
            }
            printf("\n");
        }
    }
}
