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


void ComponentCogen(Component *comp) {

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
