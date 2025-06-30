#ifndef __META_COMPS__
#define __META_COMPS__


#include "Al_window.h"
#include "Arm.h"
#include "Source_Maxwell_3.h"
#include "Bender.h"
#include "PSD_monitor.h"
#include "Progress_bar.h"
#include "PowderN.h"
#include "Slit.h"
#include "L_monitor.h"
#include "Beamstop.h"
#include "Monitor_nD.h"
#include "Guide.h"
#include "PSDlin_monitor.h"


enum CompType {
    CT_UNDEF,

    CT_Al_window,
    CT_Arm,
    CT_Source_Maxwell_3,
    CT_Bender,
    CT_PSD_monitor,
    CT_Progress_bar,
    CT_PowderN,
    CT_Slit,
    CT_L_monitor,
    CT_Beamstop,
    CT_Monitor_nD,
    CT_Guide,
    CT_PSDlin_monitor,

    CT_CNT,
};


struct CompMeta {
    CompType type;
    void *comp;

    // UI fields
    Str type_name;
    Str name;

    // render fields
    Matrix4f t;
    Matrix4f *parent;
    //    box
};


Str ToStr(char *s) {
    Str result = {};
    result.str = s;
    result.len = _strlen(s);
    return result;
}


CompMeta *CreateComponentInstance(MArena *a_dest, CompType type, s32 index) {
    CompMeta *comp = (CompMeta*) ArenaAlloc(a_dest, sizeof(CompMeta));
    comp->t = Matrix4f_Identity();
    comp->type = type;

    switch (type) {
        case CT_Al_window: {

            Al_window comp_spec = Create_Al_window(index, (char*) "Al_window_default", NULL);
            comp->comp = ArenaPush(a_dest, &comp_spec, sizeof(Al_window));
            comp->type_name = ToStr(comp_spec.type);
            comp->name = ToStr(comp_spec.name);

        } break;

        case CT_Arm: {

            Arm comp_spec = Create_Arm(index, (char*) "Arm_default", NULL);
            comp->comp = ArenaPush(a_dest, &comp_spec, sizeof(Al_window));
            comp->type_name = ToStr(comp_spec.type);
            comp->name = ToStr(comp_spec.name);

        } break;


        // ...


        default: {

        } break;
    }

    return comp;
}


HashMap CreateComponentExamples(MArena *a_dest) {
    s32 count = 100;
    HashMap map = InitMap(a_dest, count * 2);
    for (s32 i = 1; i < CT_CNT; ++i) {
        CompType ct = (CompType) i;
        CompMeta *cm = CreateComponentInstance(a_dest, ct, i-1);
        MapPut(&map, ct, cm);
    }

    return map;
}

void InitComponent(CompMeta *comp, Instrument *instr = NULL) {
    switch (comp->type) {
        case CT_Al_window: { Init_Al_window((Al_window*) comp->comp, instr); } break;
        case CT_Arm: { Init_Arm((Arm*) comp->comp, instr); } break;


        // ...


        default: {

        } break;
    }
}

void TraceComponent(CompMeta *comp, Neutron *particle, Instrument *instr = NULL) {
    switch (comp->type) {
        case CT_Al_window: { Trace_Al_window((Al_window*) comp->comp, particle, instr); } break;
        case CT_Arm: { Trace_Arm((Arm*) comp->comp, particle, instr); } break;


        // ...


        default: {

        } break;
    }
}

void DisplayComponent(CompMeta *comp) {
    switch (comp->type) {
        case CT_Al_window: { Display_Al_window((Al_window*) comp->comp); } break;
        case CT_Arm: { Display_Arm((Arm*) comp->comp); } break;


        // ...


        default: {

        } break;
    }
}

void FinallyComponent(CompMeta *comp) {
    switch (comp->type) {
        case CT_Al_window: { Finally_Al_window((Al_window*) comp->comp); } break;
        case CT_Arm: { Finally_Arm((Arm*) comp->comp); } break;


        // ...


        default: {

        } break;
    }
}





#endif // __META_COMPS__
