#include "meta_comps.h"
#include "PSI_DMC.h"


HashMap CreateComponentExamples(MArena *a_dest) {
    s32 count = 100;
    HashMap map = InitMap(a_dest, count * 2);
    for (s32 i = 1; i < CT_CNT; ++i) {
        CompType ct = (CompType) i;
        CompMeta *cm = CreateComponent(a_dest, ct, i-1);
        MapPut(&map, ct, cm);
    }

    return map;
}


void RunProgram() {
    TimeFunction;

    Instrument instr = {};

    s32 idx = 0;

    /*
    // FAILS: because we haven't intialized all component parameters correctly
    Al_window Al_window_comp = Init_Al_window(idx++, (char*) "al_window_01", &instr);
    Arm Arm_comp = Init_Arm(idx++, (char*) "arm_01", &instr);
    Source_Maxwell_3 Source_Maxwell_3_comp = Init_Source_Maxwell_3(idx++, (char*) "Source_Maxwell_3_01", &instr);
    Bender Bender_comp = Init_Bender(idx++, (char*) "Bender_01", &instr);
    PSD_monitor PSD_monitor_comp = Init_PSD_monitor(idx++, (char*) "PSD_monitor_01", &instr);
    Progress_bar Progress_bar_comp = Init_Progress_bar(idx++, (char*) "Progress_bar_01", &instr);
    PowderN PowderN_comp = Init_PowderN(idx++, (char*) "PowderN_01", &instr);
    Slit Slit_comp = Init_Slit(idx++, (char*) "Slit_01", &instr);
    L_monitor L_monitor_comp = Init_L_monitor(idx++, (char*) "L_monitor_01", &instr);
    Beamstop Beamstop_comp = Init_Beamstop(idx++, (char*) "Beamstop_01", &instr);
    Monitor_nD Monitor_nD_comp = Init_Monitor_nD(idx++, (char*) "Monitor_nD_01", &instr);
    Guide Guide_comp = Init_Guide(idx++, (char*) "Guide_01", &instr);
    PSDlin_monitor PSDlin_monitor_comp = Init_PSDlin_monitor(idx++, (char*) "PSDlin_monitor_01", &instr);
    */


    MArena *a = GetContext()->a_life;
    HashMap comps = CreateComponentExamples(a);

    printf("Installed components:\n");
    MapIter iter = {};
    while (CompMeta *comp = (CompMeta*) MapNextVal(&comps, &iter)) {
        printf("%d", comp->type);        
        
        if (comp->type_name.len) {
            StrPrint(" -> ", comp->name, "");
            StrPrint(" (", comp->type_name, ")");
        }
        printf("\n");
    }
    
}


void Test() {
    printf("Running tests ...\n");
}


int main (int argc, char **argv) {
    TimeProgram;
    BaselayerAssertVersion(0, 2, 1);
    CbuiAssertVersion(0, 1, 0);

    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("--help:          display help (this text)\n");
        printf("--test:          run test functions\n");
        exit(0);
    }
    else if (CLAContainsArg("--test", argc, argv)) {
        Test();
    }
    else {
        InitBaselayer();
        RunProgram();
    }
}
