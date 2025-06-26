#ifndef __COGENCOMP_H__
#define __COGENCOMP_H__


void PrintDefines(StrBuff *b, Component *comp) {
    for (s32 i = 0; i < comp->setting_params.len; ++i) {
        Parameter p = comp->setting_params.arr[i];
        StrBuffPrint1K(b, "    #define %.*s comp->%.*s\n", 4, p.name.len, p.name.str, p.name.len, p.name.str);
    }
    StrBuffPrint1K(b, "\n", 0);
    for (s32 i = 0; i < comp->declare_members.len; ++i) {
        StructMember m = comp->declare_members.arr[i];
        StrBuffPrint1K(b, "    #define %.*s comp->%.*s\n", 4, m.name.len, m.name.str, m.name.len, m.name.str);
    }

    // TODO: output, state, ..., params
}
void PrintUndefs(StrBuff *b, Component *comp) {
    for (s32 i = 0; i < comp->setting_params.len; ++i) {
        Parameter p = comp->setting_params.arr[i];
        StrBuffPrint1K(b, "    #undef %.*s\n", 2, p.name.len, p.name.str);
    }
    StrBuffPrint1K(b, "\n", 0);
    for (s32 i = 0; i < comp->declare_members.len; ++i) {
        StructMember m = comp->declare_members.arr[i];
        StrBuffPrint1K(b, "    #undef %.*s\n", 2, m.name.len, m.name.str);
    }

    // TODO: output, state, ..., params
}


void ComponentCogen(StrBuff *b, Component *comp) {
    // header guard
    StrBuffPrint1K(b, "#ifndef __%.*s__\n", 2, comp->type.len, comp->type.str);
    StrBuffPrint1K(b, "#define __%.*s__\n", 2, comp->type.len, comp->type.str);
    StrBuffPrint1K(b, "\n", 0);

    // system header
    StrBuffPrint1K(b, "// standard includes\n", 0);
    StrBuffPrint1K(b, "#include <cstdlib>\n", 0);
    StrBuffPrint1K(b, "#include <cstring>\n", 0);
    StrBuffPrint1K(b, "#include <cstdio>\n", 0);
    StrBuffPrint1K(b, "#include <cstddef>\n", 0);
    StrBuffPrint1K(b, "#include \"jg_baselayer.h\"\n", 0);
    StrBuffPrint1K(b, "#include \"jg_cbui.h\"\n", 0);
    StrBuffPrint1K(b, "#include \"simcore.h\"\n", 0);
    StrBuffPrint1K(b, "#include \"simlib.h\"\n", 0);
    StrBuffPrint1K(b, "\n", 0);

    //
    // share block

    StrBuffPrint1K(b, "// share block\n\n", 0);
    StrBuffAppend(b, comp->share_block);
    StrBuffPrint1K(b, "\n\n", 0);

    //
    // struct

    StrBuffPrint1K(b, "struct %.*s {\n", 2, comp->type.len, comp->type.str);
    StrBuffPrint1K(b, "    Matrix4f t;\n", 0);
    StrBuffPrint1K(b, "    Matrix4f *parent;\n", 0);
    StrBuffPrint1K(b, "    int index;\n", 0);
    StrBuffPrint1K(b, "    char *name;\n", 0);
    StrBuffPrint1K(b, "    char *type;\n", 0);
    StrBuffPrint1K(b, "    Coords position_absolute;\n", 0);
    StrBuffPrint1K(b, "    Coords position_relative;\n", 0);
    StrBuffPrint1K(b, "    Rotation rotation_absolute;\n", 0);
    StrBuffPrint1K(b, "    Rotation rotation_relative;\n", 0);
    StrBuffPrint1K(b, "\n    // parameters\n", 0);

    // struct parameters
    Str string_s { (char*) "string", 6 };
    Str vector_s { (char*) "vector", 6 };
    for (s32 i = 0; i < comp->setting_params.len; ++i) {
        Parameter p = comp->setting_params.arr[i];

        if (StrEqual(p.type, string_s)) {
            StrBuffPrint1K(b, "    char *", 0);
        }
        else if (StrEqual(p.type, vector_s)) {
            StrBuffPrint1K(b, "    double ", 0);
        }
        else if (p.type.len) {
            StrBuffPrint1K(b, "    %.*s ", 2, p.type.len, p.type.str);
        }
        else {
            StrBuffPrint1K(b, "    double ", 0);
        }

        if (StrEqual(p.type, vector_s)) {
            // just scan how many commas it has
            assert(p.default_val.len > 0);
            s32 cnt = 1;
            for (s32 k = 0; k < p.default_val.len; ++k) {
                if (p.default_val.str[k] == ',') {
                    ++cnt;
                }
            }
            StrBuffPrint1K(b, "%.*s[%d]", 3, p.name.len, p.name.str, cnt);
        }
        else {
            StrBuffPrint1K(b, "%.*s", 2, p.name.len, p.name.str);
        }

        if (p.default_val.len) {
            if (StrEqual(p.type, string_s)) {
                StrBuffPrint1K(b, " = (char*) %.*s", 2, p.default_val.len, p.default_val.str);
            }
            else {
                StrBuffPrint1K(b, " = %.*s", 2, p.default_val.len, p.default_val.str);
            }
        }
        StrBuffPrint1K(b, ";\n", 0);
    }

    // declare members
    StrBuffPrint1K(b, "\n    // declares\n", 0);
    for (s32 i = 0; i < comp->declare_members.len; ++i) {
        StructMember m = comp->declare_members.arr[i];

        StrBuffPrint1K(b, "    %.*s ", 2, m.type.len, m.type.str);
        if (m.is_pointer_type) {
            StrBuffPrint1K(b, "*", 0);
        }
        StrBuffPrint1K(b, "%.*s", 2, m.name.len, m.name.str);

        if (m.is_array_type) {
            StrBuffPrint1K(b, "[%d]", 1, m.array_type_sz);
        }

        if (m.defval.len) {
            StrBuffPrint1K(b, " = %.*s", 2, m.defval.len, m.defval.str);
        }
        StrBuffPrint1K(b, ";\n", 0);
    }
    StrBuffPrint1K(b, "};\n\n", 0);

    //
    //  Init

    StrBuffPrint1K(b, "%.*s Init_%.*s(s32 index, char *name, Instrument *instrument) {\n", 4, comp->type.len, comp->type.str, comp->type.len, comp->type.str);
    StrBuffPrint1K(b, "    %.*s _comp = {};\n", 2, comp->type.len, comp->type.str);
    StrBuffPrint1K(b, "    %.*s *comp = &_comp;\n", 2, comp->type.len, comp->type.str);
    StrBuffPrint1K(b, "    comp->type = (char*) \"%.*s\";\n", 2, comp->type.len, comp->type.str);
    StrBuffPrint1K(b, "    comp->name = name;\n", 0);
    StrBuffPrint1K(b, "    comp->index = index;\n", 0);
    StrBuffPrint1K(b, "\n", 0);
    if (comp->initalize_block.len) {
        PrintDefines(b, comp);
        StrBuffPrint1K(b, "    ////////////////////////////////////////////////////////////////\n\n", 0);

        StrBuffAppend(b, comp->initalize_block);

        StrBuffPrint1K(b, "\n\n    ////////////////////////////////////////////////////////////////\n", 0);
        PrintUndefs(b, comp);
        StrBuffPrint1K(b, "\n", 0);
    }
    StrBuffPrint1K(b, "    return _comp;\n", 0);
    StrBuffPrint1K(b, "}\n\n", 0);

    //
    //  Trace

    StrBuffPrint1K(b, "void Trace_%.*s(%.*s *comp, Neutron *particle, Instrument *instrument) {\n", 4, comp->type.len, comp->type.str, comp->type.len, comp->type.str);
    if (comp->trace_block.len) {
        StrBuffPrint1K(b, "    #define x particle->x\n", 0);
        StrBuffPrint1K(b, "    #define y particle->y\n", 0);
        StrBuffPrint1K(b, "    #define z particle->z\n", 0);
        StrBuffPrint1K(b, "    #define vx particle->vx\n", 0);
        StrBuffPrint1K(b, "    #define vy particle->vy\n", 0);
        StrBuffPrint1K(b, "    #define vz particle->vz\n", 0);
        StrBuffPrint1K(b, "    #define sx particle->sx\n", 0);
        StrBuffPrint1K(b, "    #define sy particle->sy\n", 0);
        StrBuffPrint1K(b, "    #define sz particle->sz\n", 0);
        StrBuffPrint1K(b, "    #define t particle->t\n", 0);
        StrBuffPrint1K(b, "    #define p particle->p\n", 0);

        StrBuffPrint1K(b, "\n", 0);
        PrintDefines(b, comp);
        StrBuffPrint1K(b, "    ////////////////////////////////////////////////////////////////\n\n", 0);

        StrBuffAppend(b, comp->trace_block);

        StrBuffPrint1K(b, "\n\n    ////////////////////////////////////////////////////////////////\n", 0);
        PrintUndefs(b, comp);
        StrBuffPrint1K(b, "\n", 0);

        StrBuffPrint1K(b, "    #undef x\n", 0);
        StrBuffPrint1K(b, "    #undef y\n", 0);
        StrBuffPrint1K(b, "    #undef z\n", 0);
        StrBuffPrint1K(b, "    #undef vx\n", 0);
        StrBuffPrint1K(b, "    #undef vy\n", 0);
        StrBuffPrint1K(b, "    #undef vz\n", 0);
        StrBuffPrint1K(b, "    #undef sx\n", 0);
        StrBuffPrint1K(b, "    #undef sy\n", 0);
        StrBuffPrint1K(b, "    #undef sz\n", 0);
        StrBuffPrint1K(b, "    #undef t\n", 0);
        StrBuffPrint1K(b, "    #undef p\n", 0);
    }
    StrBuffPrint1K(b, "}\n\n", 0);


    //
    //  Save

    StrBuffPrint1K(b, "void Save_%.*s(%.*s *comp) {\n", 4, comp->type.len, comp->type.str, comp->type.len, comp->type.str);
    StrBuffPrint1K(b, "\n", 0);
    if (comp->save_block.len) {
        PrintDefines(b, comp);
        StrBuffPrint1K(b, "    ////////////////////////////////////////////////////////////////\n\n", 0);

        StrBuffAppend(b, comp->save_block);

        StrBuffPrint1K(b, "\n\n    ////////////////////////////////////////////////////////////////\n", 0);
        PrintUndefs(b, comp);
    }
    StrBuffPrint1K(b, "}\n\n", 0);

    //
    //  Finally

    StrBuffPrint1K(b, "void Finally_%.*s(%.*s *comp) {\n", 4, comp->type.len, comp->type.str, comp->type.len, comp->type.str);
    StrBuffPrint1K(b, "\n", 0);
    if (comp->finally_block.len) {
        PrintDefines(b, comp);
        StrBuffPrint1K(b, "    ////////////////////////////////////////////////////////////////\n\n", 0);

        StrBuffAppend(b, comp->finally_block);

        StrBuffPrint1K(b, "\n\n    ////////////////////////////////////////////////////////////////\n", 0);
        PrintUndefs(b, comp);
    }
    StrBuffPrint1K(b, "}\n\n", 0);

    //
    //  Display

    StrBuffPrint1K(b, "void Display_%.*s(%.*s *comp) {\n", 4, comp->type.len, comp->type.str, comp->type.len, comp->type.str);
    if (comp->display_block.len) {
        StrBuffPrint1K(b, "    #define magnify mcdis_magnify\n", 0);
        StrBuffPrint1K(b, "    #define line mcdis_line\n", 0);
        StrBuffPrint1K(b, "    #define dashed_line mcdis_dashed_line\n", 0);
        StrBuffPrint1K(b, "    #define multiline mcdis_multiline\n", 0);
        StrBuffPrint1K(b, "    #define rectangle mcdis_rectangle\n", 0);
        StrBuffPrint1K(b, "    #define box mcdis_box\n", 0);
        StrBuffPrint1K(b, "    #define circle mcdis_circle\n", 0);
        StrBuffPrint1K(b, "    #define Circle mcdis_Circle\n", 0);
        StrBuffPrint1K(b, "    #define cylinder mcdis_cylinder\n", 0);
        StrBuffPrint1K(b, "    #define cone mcdis_cone\n", 0);
        StrBuffPrint1K(b, "    #define sphere mcdis_sphere\n", 0);

        StrBuffPrint1K(b, "\n", 0);
        PrintDefines(b, comp);
        StrBuffPrint1K(b, "    ////////////////////////////////////////////////////////////////\n\n", 0);

        StrBuffAppend(b, comp->display_block);

        StrBuffPrint1K(b, "\n\n    ////////////////////////////////////////////////////////////////\n", 0);
        PrintUndefs(b, comp);
        StrBuffPrint1K(b, "\n", 0);

        StrBuffPrint1K(b, "    #undef magnify\n", 0);
        StrBuffPrint1K(b, "    #undef line\n", 0);
        StrBuffPrint1K(b, "    #undef dashed_line\n", 0);
        StrBuffPrint1K(b, "    #undef multiline\n", 0);
        StrBuffPrint1K(b, "    #undef rectangle\n", 0);
        StrBuffPrint1K(b, "    #undef box\n", 0);
        StrBuffPrint1K(b, "    #undef circle\n", 0);
        StrBuffPrint1K(b, "    #undef Circle\n", 0);
        StrBuffPrint1K(b, "    #undef cylinder\n", 0);
        StrBuffPrint1K(b, "    #undef cone\n", 0);
        StrBuffPrint1K(b, "    #undef sphere\n", 0);
    }
    StrBuffPrint1K(b, "}\n\n", 0);


    // close header guard
    StrBuffPrint1K(b, "#endif\n", 0);
}


void ComponentMetaCogen(StrBuff *b, HashMap *components) {

    MapIter iter = {};
    while (Component *comp = (Component*) MapNextVal(components, &iter)) {
        StrBuffPrint1K(b, "#include \"%.*s.h\"\n", 2, comp->type.len, comp->type.str);

    }
    
}



#endif
