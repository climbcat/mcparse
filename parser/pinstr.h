#ifndef __PINSTR_H__
#define __PINSTR_H__


struct Instrument {
    Str name;
    Array<Parameter> params;

    Str uservars_block;
    Str declare_block;
    Str initalize_block;
    Str trace_block;
    Str finally_block;

    bool parse_error;
};


Instrument *ParseInstrument(MArena *a_dest, char *text) {
    TimeFunction;

    Tokenizer tokenizer = {};
    tokenizer.Init(text);
    Tokenizer *t = &tokenizer;
    Token token;
    Instrument *instr = (Instrument*) ArenaAlloc(a_dest, sizeof(Instrument));

    // component type
    Required(t, &token, TOK_MCSTAS_DEFINE);
    Required(t, &token, TOK_MCSTAS_INSTRUMENT);
    Required(t, &token, TOK_IDENTIFIER);
    instr->name = token.GetValue();

    // parameters

    instr->params = ParseParamsBlock(a_dest, t);


    return instr;
}


#endif
