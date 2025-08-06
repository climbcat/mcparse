#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "lib/jg_baselayer.h"
#include "src/parsecore.h"
#include "src/parsehelpers.h"
#include "src/parse_comp.h"
#include "src/parse_instr.h"
#include "src/cogen_comp.h"
#include "src/cogen_instr.h"


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

HashMap ParseInstruments(MArena *a_parse, StrLst *fpaths, bool dbg_print) {
    MArena a_files = ArenaCreate();

    HashMap map_instrs = InitMap(a_parse, StrListLen(fpaths) * 3);
    s32 total_cnt = 0;
    s32 parsed_cnt = 0;
    s32 registered_cnt = 0;
    s32 duplicate_cnt = 0;
    s32 error_cnt = 0;

    while (fpaths) {
        Str filename = StrLstNext(&fpaths);
        Str text = LoadTextFileFSeek(&a_files, filename);
        if (text.len == 0) {
            continue;
        }

        if (dbg_print) printf("parsing  #%.3d: %.*s", total_cnt, filename.len, filename.str);

        Instrument *instr = ParseInstrument(a_parse, text);
        instr->path = filename;
        instr->check_idx = total_cnt;

        if (instr->parse_error) {
            error_cnt++;
        }
        else {
            parsed_cnt++;

            if (RegisterInstrument(instr, &map_instrs)) {
                registered_cnt++;
            }
            else {
                parsed_cnt++;
                duplicate_cnt++;
            }
        }
        if (dbg_print) printf("\n");

        total_cnt++;
    }

    if (dbg_print) {
        printf("\n");
        printf("Instruments: %d, parsed & registered: %d (errors: %d, duplicates: %d)\n",
            total_cnt, registered_cnt, error_cnt, duplicate_cnt);
        printf("\n");
    }

    return map_instrs;
}

HashMap ParseComponents(MArena *a_parse, StrLst *fpaths, bool dbg_print) {
    MArena a_files = ArenaCreate();

    HashMap map_comps = InitMap(a_parse, StrListLen(fpaths) * 3);
    s32 registered_cnt = 0;
    s32 parsed_cnt = 0;
    s32 error_cnt = 0;
    s32 duplicate_cnt = 0;
    s32 total_cnt = 0;

    while (fpaths) {
        Str filename = StrLstNext(&fpaths);
        Str text = LoadTextFileFSeek(&a_files, filename);
        if (text.len == 0) {
            continue;
        }

        if (dbg_print) printf("parsing  #%.3d: %.*s", total_cnt, filename.len, filename.str);
        Component *comp = ParseComponent(a_parse, text);
        comp->file_path = filename;

        if (comp->parse_error == true) {
            error_cnt++;
        }
        else {
            parsed_cnt++;

            if (RegisterComponentType(comp, &map_comps)) {
                registered_cnt++;
            }
            else {
                duplicate_cnt++;
            }
        }
        if (dbg_print) printf("\n");

        total_cnt++;
    }

    if (dbg_print) {
        printf("\n");
        printf("Components: %d, parsed & registered: %d (errors: %d, duplicates: %d)\n",
            total_cnt, registered_cnt, error_cnt, duplicate_cnt);
        printf("\n");
    }

    return map_comps;
}


ComponentCall *_FindByName(Array<ComponentCall> comps, Str name) {
    for (s32 i = 0; i < comps.len; ++i) {
        if (StrEqual(name, comps.arr[i].name)) {
            return comps.arr + i;
        }
    }
    return NULL;
}


bool CheckInstrument(MArena *a_tmp, Instrument *instr, HashMap *comps, bool dbg_print_missing_types = false) {
    s32 max_copy_comps = 1000;
    HashMap map_cpys = InitMap(a_tmp, max_copy_comps);

    bool type_error = false;
    bool nameref_error = false;

    printf("checking  #%d: ", instr->check_idx);
    StrPrint(instr->name);

    for (s32 i = 0; i < instr->comps.len; ++i) {
        ComponentCall *c = instr->comps.arr + i;

        // Handle "... = COPY (copy_type)":
        //      Meaning, we eliminate any COPY notes and reference the .type and .args.
        if (c->copy_type.len) {
            if (StrEqual(c->copy_type, "PREVIOUS")) {

                // TODO: on error, set error flag and skip
                if (i == 0) {
                    nameref_error = true;
                    printf("\nERROR: COPY(PREVIOUS) can not be the first component call\n\n");
                }
                else {
                    Str prev_type = instr->comps.arr[i-1].type;
                    c->type = prev_type;
                }
            }
            else {
                ComponentCall *org_comp = _FindByName(instr->comps, c->copy_type);
                if (org_comp == NULL) {
                    StrPrint("\nERROR: Referenced component name not found: ", c->copy_type, "\n\n");

                    nameref_error = true;
                }

                // reference the type and args of the copied component
                c->type = org_comp->type;
                c->args = org_comp->args;
            }
        }

        // Handle "COPY (copy_name) = ...":
        //      Meaning, we eliminate any COPY notes and coyp the .name.
        if (c->copy_name.len) {
            // We don't need to check that a component by name exists unless we are dealing with PREVIOUS.
            // We just need the index to put in the component name.
            u64 copy_name_index = MapGet(&map_cpys, c->copy_name);
            copy_name_index++;
            MapPut(&map_cpys, c->copy_name, (void*) copy_name_index);

            c->name = StrAlloc(c->copy_name.len + 4);
            StrCopy(c->copy_name, c->name);

            char subscript[4];
            sprintf(subscript, "_%lu", copy_name_index);
            strcat(c->name.str, subscript);
        }

        u64 comp_exists = MapGet(comps, c->type);
        if (comp_exists == 0) {
            type_error = true;

            if (dbg_print_missing_types) {
                printf("    Missing component type (idx %d): ", i);
                StrPrint(c->type);
                printf("\n");
            }
        }
    }

    instr->type_checked = ! type_error;
    instr->namerefs_checked = ! nameref_error;

    if ((instr->type_checked == true) && (instr->namerefs_checked == true)) { printf(" - OK"); }
    if ((instr->type_checked == false)) { printf("\n    ERROR: Missing component types"); }
    if ((instr->namerefs_checked == false)) { printf("\n    ERROR: Component instance name reference"); }
    if (type_error || nameref_error) { printf("\n"); } 
    printf("\n");

    return (! type_error) && (! nameref_error);
}


int main (int argc, char **argv) {
    TimeProgram;

    BaselayerAssertVersion(0, 2, 3);

    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("Usage: parser [--comps <comp-lib-root-folder>] [--instrs <instr-file>] [--cogen]\n");
        printf("\n");
        printf("--help                  display help (this text)\n");
        printf("--comps                 parse component file or folder hierarchy/library\n");
        printf("--instrs                parse instrument file or folder hierarchy/library\n");
        printf("--cogen                 generate code\n");
        exit(0);
    }

    else if (CLAContainsArg("--test", argc, argv)) {
        // any test code here

    }

    else {
        bool do_cogen = false;
        if (CLAContainsArg("--cogen", argc, argv)) { do_cogen = true; }


        // get input
        char *comp_lib_path = NULL;
        char *instr_lib_path = NULL;
        if (CLAContainsArg("--comps", argc, argv) || CLAContainsArg("-c", argc, argv)) {
            comp_lib_path = CLAGetArgValue("--comps", argc, argv);
        }
        if (CLAContainsArg("--instrs", argc, argv) || CLAContainsArg("-i", argc, argv)) {
            instr_lib_path = CLAGetArgValue("--instrs", argc, argv);
        }


        // init
        MArena a_tmp = ArenaCreate();
        MArena a_work = ArenaCreate();
        StrInit();
        StrBuff buff = StrBuffInit();
        MapIter iter = {};


        // components
        HashMap comp_map = {};
        if (comp_lib_path != NULL) {
            StrLst *comp_paths = GetFiles(comp_lib_path, "comp", true);
            comp_map = ParseComponents(&a_work, comp_paths, true);

            iter = {};
            while (Component *comp = (Component*) MapNextVal(&comp_map, &iter)) {

                // cogen components
                if (do_cogen) {
                    // print component names
                    StrPrint("Cogen: ", comp->type, " -> ");
                    StrBuffClear(&buff);
                    ComponentCogen(&buff, comp);

                    Str f_safe = StrPathBuild(StrDirPath(comp->file_path), StrBasename(comp->file_path), StrL("h"));
                    StrPrint(f_safe);
                    printf("\n");

                    SaveFile(StrZ(f_safe), buff.str, buff.len);
                }
            }
            if (do_cogen) {
                printf("\n");
                StrBuffClear(&buff);
                ComponentMetaCogen(&buff, &comp_map);

                // save component aggregate file
                Str dirpath = StrDirPath( StrL(comp_lib_path) );
                Str savefile = StrPathBuild(dirpath, StrL("comps_meta"), StrL("h"));
                StrPrint("Saving component meta file to: ", savefile, "\n\n");

                SaveFile(StrZ(savefile), buff.str, buff.len);
            }
        }


        // instruments
        if (instr_lib_path) {
            StrLst *instr_paths = GetFiles(instr_lib_path, "instr", true);
            HashMap instr_map = ParseInstruments(&a_work, instr_paths, true);

            // print instruments
            iter = {};
            while (Instrument *instr = (Instrument*) MapNextVal(&instr_map, &iter)) {
                if (instr->parse_error == true) { continue; }

                CheckInstrument(&a_tmp, instr, &comp_map);

                if (CLAContainsArg("--print_instr", argc, argv)) {
                    InstrumentPrint(instr, true, true, true);
                }

                if (do_cogen) {
                    StrBuffClear(&buff);
                    InstrumentCogen(&buff, instr);

                    // save instrument config file
                    Str dirpath = StrDirPath( StrL(instr_lib_path) );
                    Str basename = StrCat(instr->name, "_config");
                    Str savefile = StrPathBuild(dirpath, basename, StrL("h"));
                    StrPrint("Saving instument config file to: ", savefile, "\n");

                    SaveFile(StrZ(savefile), buff.str, buff.len);
                }
            }
        }

    }
}
