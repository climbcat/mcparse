#ifndef __PINSTR_H__
#define __PINSTR_H__


struct Parameter2 {

};

struct Instrument {
    Str type;
    Str type_copy;
    Array<Parameter2> setting_params;

    Str uservars_block;
    Str declare_block;
    Str initalize_block;
    Str trace_block;
    Str finally_block;

    bool parse_error;
};

Instrument *ParseInstrument(MArena *a_dest, char *text) {
    Instrument *instr = (Instrument*) ArenaAlloc(a_dest, sizeof(Instrument));
    return instr;
}


#endif
