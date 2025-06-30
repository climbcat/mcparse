#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>


#include "../lib/jg_baselayer.h"
#include "../lib/jg_cbui.h"
#include "parsecore.h"
#include "phelpers.h"
#include "pcomp.h"
#include "pinstr.h"
#include "cogencomp.h"


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
        comp->file_path = Str { filename, _strlen(filename) };
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


ComponentCall *_FindByName(Array<ComponentCall> comps, Str name) {
    for (s32 i = 0; i < comps.len; ++i) {
        if (StrEqual(name, comps.arr[i].name)) {
            return comps.arr + i;
        }
    }
    return NULL;
}

void StrCopy(Str src, Str dest) {
    assert(src.str && dest.str);

    for (s32 i = 0; i < MinS32( src.len, dest.len ); ++i) {
        dest.str[i] = src.str[i];
    }
}

Str ToStr(char *s) {
    Str result = {};
    result.str = s;
    result.len = _strlen(s);
    return result;
}


bool TypeCheckInstrument(MArena *a_tmp, Instrument *instr, HashMap *comps) {
    s32 max_copy_comps = 1000;
    HashMap map_cpys = InitMap(a_tmp, max_copy_comps);


    bool has_error = false;
    for (s32 i = 0; i < instr->comps.len; ++i) {
        ComponentCall *c = instr->comps.arr + i;

        // Handle "... = COPY (copy_type)":
        //      Meaning, we eliminate any COPY notes and reference the .type and .args.
        if (c->copy_type.len) {
            if (StrEqual(c->copy_type, "PREVIOUS")) {

                // TODO: on error, set error flag and skip
                assert(i > 0 && "copy_type: COPY(PREVIOUS) can not be the first component call");

                Str prev_type = instr->comps.arr[i-1].type;
                c->type = prev_type;
            }
            else {
                ComponentCall *org_comp = _FindByName(instr->comps, c->copy_type);
                if (org_comp == NULL) {
                    StrPrint("\n\n", c->copy_type, "\n\n");

                    // TODO: on error, set error flag and skip
                    assert(1 == 0 && "component by name not found");
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
            has_error = true;

            printf("\n    Missing component type (idx %d): ", i);
            StrPrint(c->type);
        }
    }
    return has_error;
}


void CLACountCheckExit_0(int min_argc, int argc, const char *msg = "Too few args, use --help for usage info.") {
    if (argc < min_argc) {
        printf("%s\n", msg);
        exit(0);
    }
}


int main (int argc, char **argv) {
    TimeProgram;

    bool dbg_print_names = false;
    bool dbg_print_instr_details = false;
    bool dbg_print_comp_details = false;
    bool do_cogen = false;

    if (CLAContainsArg("--print_expr", argc, argv)) {
        dbg_print_c_expressions = true;
    }
    if (CLAContainsArg("--print_names", argc, argv)) {
        dbg_print_names = true;
    }
    if (CLAContainsArg("--print_instr", argc, argv)) {
        dbg_print_instr_details = true;
    }
    if (CLAContainsArg("--print_comp", argc, argv)) {
        dbg_print_comp_details = true;
    }
    if (CLAContainsArg("--cogen", argc, argv)) {
        do_cogen = true;
    }

    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("Usage: parser <comp_lib> <instr> | [options]\n");
        printf("\n");
        printf("comp_lib            component files root path\n");
        printf("instr               instrument file or root path\n");
        printf("--help              display help (this text)\n");
        printf("--cogen             generate component code\n");
        printf("--print_names       debug print comp & instr names\n");
        printf("--print_instr       debug print instrument details\n");
        printf("--print_comp        debug print component details\n");
        printf("--print_expr        debug print parsed C expression parameter values\n");
        printf("--test              run test functions\n");
        exit(0);
    }
    else if (CLAContainsArg("--test", argc, argv)) {
        printf("Testing component cogen ...\n");

        MArena a_tmp = ArenaCreate();
        MArena a_work = ArenaCreate();
        StringInit();

        char *comp_lib_path = argv[1];
        StrLst *comp_paths = GetFiles(comp_lib_path, "comp", true);
        HashMap components = ParseComponents(&a_work, comp_paths);

        MapIter iter = {};
        Component *comp = (Component*) MapNextVal(&components, &iter);
        StrBuff buff = StrBuffInit();
        ComponentCogen(&buff, comp);
    }
    else {
        CLACountCheckExit_0(3, argc);

        MArena a_tmp = ArenaCreate();
        MArena a_work = ArenaCreate();
        StringInit();
        MapIter iter = {};

        char *comp_lib_path = argv[1];

        StrLst *comp_paths = GetFiles(comp_lib_path, "comp", true);
        HashMap components = ParseComponents(&a_work, comp_paths);
        printf("\nParsed %d Components\n", components.noccupants);

        StrBuff buff = StrBuffInit();
        iter = {};
        while (Component *comp = (Component*) MapNextVal(&components, &iter)) {
            // print component names
            if (dbg_print_names) {
                printf("COMPONENT: "); StrPrint(comp->type); printf("\n");
            }
            printf("\n");

            if (dbg_print_comp_details) {
                printf("comp print: \n");
                ComponentPrint(comp, false, false, true, false, false);
            }

            // cogen components
            if (do_cogen) {
                StrBuffClear(&buff);
                printf("Cogen: ");
                StrPrint(comp->file_path);
                printf("\n");

                ComponentCogen(&buff, comp);

                FInfo info = FInfoGet(StrZeroTerm(comp->file_path));
                Str f_safe = StrPathBuild(info.dirname, info.basename, StrL("h"));
                printf("Saving: ");
                StrPrint(f_safe);
                printf("\n");

                SaveFile(StrZeroTerm(f_safe), buff.str, buff.len);
            }
        }

        if (do_cogen) {
            printf("Meta: ");
            StrBuffClear(&buff);
            ComponentMetaCogen(&buff, &components);

            void *data = 0;
            SaveFile("meta_comps.h", buff.str, buff.len);
        }

        char *instr_path = argv[2];
        StrLst *instr_paths = GetFiles(instr_path, "instr", true);
        HashMap instruments = ParseInstruments(&a_work, instr_paths);
        printf("\nParsed %d Instruments\n\n", instruments.noccupants);

        // print instrument names
        iter = {};
        while (Instrument *instr = (Instrument*) MapNextVal(&instruments, &iter)) {
            bool has_error = TypeCheckInstrument(&a_tmp, instr, &components);
            if (has_error == true) {
                printf(" - ERROR");
            }
            printf("\n");

            if (dbg_print_names) {
                printf("INSTRUMENT: "); StrPrint(instr->name);
            }

            if (dbg_print_instr_details) {
                InstrumentPrint(instr, true, true, true);
            }
        }
    }
}
