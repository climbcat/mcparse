#ifndef __COGENCOMP_H__
#define __COGENCOMP_H__


struct Transform {
    s32 i;
};
struct AComp {
    Transform *parent;
    Transform t;

    /*
    Declare block here
    */
};
AComp AComp_Init() {
    AComp comp = {};
    comp.t.i = 1; // identity;

    return comp;
}
void AComp_Trace(AComp *comp) {

}
void AComp_Finally(AComp *comp) {

}
void AComp_Display(AComp *comp) {

}


void PrintStdIncludes() {
    printf("// standard includes\n");
    printf("#include <cstdlib>\n");
    printf("#include <cstring>\n");
    printf("#include <cstdio>\n");
    printf("#include <cstddef>\n");
    printf("#include \"jg_baselayer.h\"\n");
    printf("#include \"jg_cbui.h\"\n");
    printf("#include \"simcore.h\"\n");
    printf("#include \"simlib.h\"\n");
}
void PrintDefines(Component *comp) {
    for (s32 i = 0; i < comp->setting_params.len; ++i) {
        Parameter p = comp->setting_params.arr[i];
        printf("    #define %.*s comp->%.*s\n", p.name.len, p.name.str, p.name.len, p.name.str);
    }
    printf("\n");
    for (s32 i = 0; i < comp->declare_members.len; ++i) {
        StructMember m = comp->declare_members.arr[i];
        printf("    #define %.*s comp->%.*s\n", m.name.len, m.name.str, m.name.len, m.name.str);
    }
    // TODO: output, state etc. params
}
void PrintUndefs(Component *comp) {
    for (s32 i = 0; i < comp->setting_params.len; ++i) {
        Parameter p = comp->setting_params.arr[i];
        printf("    #undef %.*s\n", p.name.len, p.name.str);
    }
    printf("\n");
    for (s32 i = 0; i < comp->declare_members.len; ++i) {
        StructMember m = comp->declare_members.arr[i];
        printf("    #undef %.*s\n", m.name.len, m.name.str);
    }
    // TODO: output, state etc. params
}


void ComponentCogen(Component *comp) {

    // header guard
    printf("#ifndef __%.*s__\n", comp->type.len, comp->type.str);
    printf("#define __%.*s__\n", comp->type.len, comp->type.str);
    printf("\n");

    // system header
    PrintStdIncludes();
    printf("\n");

    //
    // share block

    printf("// share block\n\n");
    StrPrint(comp->share_block);
    printf("\n\n");

    //
    // struct

    printf("struct %.*s {\n", comp->type.len, comp->type.str);
    printf("    Matrix4f t;\n");
    printf("    Matrix4f *parent;\n");
    printf("    int index;\n");
    printf("    char *name;\n");
    printf("    char *type;\n");
    printf("    Coords position_absolute;\n");
    printf("    Coords position_relative;\n");
    printf("    Rotation rotation_absolute;\n");
    printf("    Rotation rotation_relative;\n");

    printf("\n");
    printf("    // parameters\n");

    // struct parameters
    Str string_s { (char*) "string", 6 };
    Str vector_s { (char*) "vector", 6 };
    for (s32 i = 0; i < comp->setting_params.len; ++i) {
        Parameter p = comp->setting_params.arr[i];

        if (StrEqual(p.type, string_s)) {
            printf("    char *");
        }
        else if (StrEqual(p.type, vector_s)) {
            printf("    double ");
        }
        else if (p.type.len) {
            printf("    %.*s ", p.type.len, p.type.str);
        }
        else {
            printf("    double ");
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
            printf("%.*s[%d]", p.name.len, p.name.str, cnt);
        }
        else {
            printf("%.*s", p.name.len, p.name.str);
        }

        if (p.default_val.len) {
            if (StrEqual(p.type, string_s)) {
                printf(" = (char*) %.*s", p.default_val.len, p.default_val.str);
            }
            else {
                printf(" = %.*s", p.default_val.len, p.default_val.str);
            }
        }
        printf(";\n");
    }

    // declare members
    printf("\n    // declares\n");
    for (s32 i = 0; i < comp->declare_members.len; ++i) {
        StructMember m = comp->declare_members.arr[i];

        printf("    %.*s ", m.type.len, m.type.str);
        if (m.is_pointer_type) {
            printf("*");
        }
        printf("%.*s", m.name.len, m.name.str);

        if (m.is_array_type) {
            printf("[%d]", m.array_type_sz);
        }

        if (m.defval.len) {
            printf(" = %.*s", m.defval.len, m.defval.str);
        }
        printf(";\n");
    }
    printf("};\n\n");


    //
    //  Init

    printf("%.*s Init_%.*s(s32 index, char *name, Instrument *instrument) {\n", comp->type.len, comp->type.str, comp->type.len, comp->type.str);
    printf("    %.*s _comp = {};\n", comp->type.len, comp->type.str);
    printf("    %.*s *comp = &_comp;\n", comp->type.len, comp->type.str);
    printf("    comp->type = (char*) \"%.*s\";\n", comp->type.len, comp->type.str);
    printf("    comp->name = name;\n");
    printf("\n");
    PrintDefines(comp);
    printf("    // ---------------------------\n\n");

    StrPrint(comp->initalize_block);

    printf("\n    // ---------------------------\n");
    PrintUndefs(comp);
    printf("\n");
    printf("    return _comp;\n");
    printf("}\n");



    //
    //  Trace

    printf("void Trace_%.*s(%.*s *comp, Neutron *particle, Instrument *instrument) {\n", comp->type.len, comp->type.str, comp->type.len, comp->type.str);
    printf("#define x particle->x\n");
    printf("#define y particle->y\n");
    printf("#define z particle->z\n");
    printf("#define vx particle->vx\n");
    printf("#define vy particle->vy\n");
    printf("#define vz particle->vz\n");
    printf("#define sx particle->sx\n");
    printf("#define sy particle->sy\n");
    printf("#define sz particle->sz\n");
    printf("#define t particle->t\n");
    printf("#define p particle->p\n");

    printf("\n");
    PrintDefines(comp);
    printf("    // ---------------------------\n\n");

    StrPrint(comp->trace_block);

    printf("\n    // ---------------------------\n");
    PrintUndefs(comp);
    printf("#undef x\n");
    printf("#undef y\n");
    printf("#undef z\n");
    printf("#undef vx\n");
    printf("#undef vy\n");
    printf("#undef vz\n");
    printf("#undef sx\n");
    printf("#undef sy\n");
    printf("#undef sz\n");
    printf("#undef t\n");
    printf("#undef p\n");
    printf("}\n");

    //
    //  Save

    // TODO: impl.


    //
    //  Finally

    // TODO: impl.


    //
    //  Display

    // TODO: impl.


    // close header guard
    printf("#endif\n");

    /*
    What's the cogen gonna be: Using the example component "AComp.comp"
    - base functions: AComp.h 
    - shared library: AComp_share.h - all of the share block, which can be a lot of code
    - meta/reflection: AComp_meta.h - component type name, parameter type info/names, doc strings
            which is all of the stuff that can be provided through cogen - now that we are at it.
            With this info, we can build UI, get function pointers for the type-agnostic simulation core.

    Let's focus on AComp.h for now. We would provide the following:
    - header guard
    - include any AComp_share.h file
    - component struct: declare block (parsed for symbol mapping)
    - component struct: Transform t; Transform *parent; comp_type_name, comp_name (instance name)
    - symbols: define each declare block sumbols as #define sym comp->sym - for the entire file ! 

    - AComp AComp_Init() {} returning a component struct; runs init code and returns
    - void Trace(Acomp *comp) {}
    - Finally
    - Display

    - un-define symbols
    - end header guard

    The AComp_share.h file should contain the following:
    - header guard
    - share code block copy-pasted verbatim into the file 
    - end header guard

    The AComp_meta.h file should contain some of the following things (less specific/brain-stormy)
    - header guard
    - include the AComp.h file
    - void GetCompSignatures(HashMap dest_map) - returns function pointers to trace, finally and display
    - Wrapper functions that take and return void* arguments? 
    - ...
    */
}


#endif
