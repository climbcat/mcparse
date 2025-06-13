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
    printf("struct Transform { int i; };\n\n");
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

    // share block

    printf("// share block\n\n");
    StrPrint(comp->share_block);
    printf("\n\n");

    // struct definition
    printf("struct %.*s {\n", comp->type.len, comp->type.str);
    printf("    Transform t;\n");
    printf("    Transform *parent;\n");
    printf("\n");
    printf("    // parameters\n");

    // parameters
    for (s32 i = 0; i < comp->setting_params.len; ++i) {
        Parameter p = comp->setting_params.arr[i];

        if (p.type.len) {
            printf("    %.*s ", p.type.len, p.type.str);
        }
        else {
            printf("    float ");
        }
        printf("%.*s", p.name.len, p.name.str);

        if (p.default_val.len) {
            printf(" = %.*s", p.default_val.len, p.default_val.str);
        }
        printf(";\n");
    }

    // declare members
    printf("\n    // declares\n");
    for (s32 i = 0; i < comp->declare_members.len; ++i) {
        StructMember m = comp->declare_members.arr[i];

        printf("    %.*s ", m.type.len, m.type.str);
        if (m.pointer_type) {
            printf("*");
        }
        printf("%.*s", m.name.len, m.name.str);

        if (m.defval.len) {
            printf(" = %.*s", m.defval.len, m.defval.str);
        }
        printf(";\n");
    }
    printf("};\n\n");


    printf("%.*s Init_%.*s() {\n", comp->type.len, comp->type.str, comp->type.len, comp->type.str);
    printf("    %.*s _comp = {};\n", comp->type.len, comp->type.str);
    printf("    %.*s *comp = &_comp;\n", comp->type.len, comp->type.str);
    printf("\n");
    PrintDefines(comp);
    printf("    // ---------------------------\n\n");

    StrPrint(comp->initalize_block);

    printf("\n    // ---------------------------\n");
    PrintUndefs(comp);
    printf("\n");
    printf("    return _comp;\n");
    printf("}\n");


    // TODO: print out the init code



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
