#ifndef __PINSTR_H__
#define __PINSTR_H__


// sould be a linked list? Simply because this allows in-lining
struct ComponentCall {
    ComponentCall *next;
    ComponentCall *first;

    Str name;
    Str type;

    Str at_x;
    Str at_y;
    Str at_z;

    Str rot_x;
    Str rot_y;
    Str rot_z;

    bool at_abs;
    bool rot_abs;
    Str at_rel;
    Str rot_rel;

    Array<Parameter> args;
};

struct Instrument {
    Str name;
    Array<Parameter> params;
    Array<ComponentCall> comps;

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
    instr->comps = InitArray<ComponentCall>(a_dest, 1000);

    // component type
    Required(t, &token, TOK_MCSTAS_DEFINE);
    Required(t, &token, TOK_MCSTAS_INSTRUMENT);
    Required(t, &token, TOK_IDENTIFIER);
    instr->name = token.GetValue();

    // parameters
    instr->params = ParseParamsBlock(a_dest, t);

    // code blocks
    Str _;
    ParseCodeBlock(t, TOK_MCSTAS_DECLARE, &instr->declare_block, &_, &_);
    ParseCodeBlock(t, TOK_MCSTAS_INITIALIZE, &instr->declare_block, &_, &_);

    // component calls

    Required(t, &token, TOK_MCSTAS_TRACE);
    while (true) {

        if (Optional(t, &token, TOK_MCSTAS_COMPONENT)) {
            ComponentCall c = {};

            Required(t, &token, TOK_IDENTIFIER);
            c.name = token.GetValue();

            Required(t, &token, TOK_ASSIGN);
            Required(t, &token, TOK_IDENTIFIER);
            c.type = token.GetValue();
            c.args = ParseParamsBlock(a_dest, t, true);

            printf("args_len: %u\n", c.args.len);

            Required(t, &token, TOK_MCSTAS_AT);

            // parse AT vector:
            Required(t, &token, TOK_LBRACK);
            //OptionOfTwo(t, &token, TOK_INT, TOK_FLOAT);
            RequiredRValOrExpression(t, &token);
            c.at_x = token.GetValue();
            Required(t, &token, TOK_COMMA);
            //OptionOfTwo(t, &token, TOK_INT, TOK_FLOAT);
            RequiredRValOrExpression(t, &token);
            c.at_y = token.GetValue();
            Required(t, &token, TOK_COMMA);
            //OptionOfTwo(t, &token, TOK_INT, TOK_FLOAT);
            RequiredRValOrExpression(t, &token);
            c.at_z = token.GetValue();
            Required(t, &token, TOK_RBRACK);

            // relative / absolute
            OptionOfTwo(t, &token, TOK_MCSTAS_RELATIVE, TOK_MCSTAS_ABSOLUTE);
            if (token.type == TOK_MCSTAS_RELATIVE) {
                OptionOfTwo(t, &token, TOK_IDENTIFIER, TOK_MCSTAS_PREVIOUS);
                c.at_rel = token.GetValue();
            }
            else {
                c.at_abs = true;
            }

            if (Optional(t, &token, TOK_MCSTAS_ROTATED)) {
                // parse ROTATED vector:
                Required(t, &token, TOK_LBRACK);
                //OptionOfTwo(t, &token, TOK_INT, TOK_FLOAT);
                RequiredRValOrExpression(t, &token);
                c.rot_x = token.GetValue();
                Required(t, &token, TOK_COMMA);
                //OptionOfTwo(t, &token, TOK_INT, TOK_FLOAT);
                RequiredRValOrExpression(t, &token);
                c.rot_y = token.GetValue();
                Required(t, &token, TOK_COMMA);
                //OptionOfTwo(t, &token, TOK_INT, TOK_FLOAT);
                RequiredRValOrExpression(t, &token);
                c.rot_z = token.GetValue();
                Required(t, &token, TOK_RBRACK);

                // relative / absolute
                OptionOfTwo(t, &token, TOK_MCSTAS_RELATIVE, TOK_MCSTAS_ABSOLUTE);
                if (token.type == TOK_MCSTAS_RELATIVE) {
                    OptionOfTwo(t, &token, TOK_IDENTIFIER, TOK_MCSTAS_PREVIOUS);
                    c.rot_rel = token.GetValue();
                }
                else {
                    c.rot_abs = true;
                }
            }


            instr->comps.Add(c);
            // DBG
            break;
        }
        else {
            break;
        }


    }



    //Required(t, &token, TOK_MCSTAS_END);
    return instr;
}


void InstrumentPrint(Instrument *instr) {

    printf("name: "); StrPrint(instr->name); printf("\n");
    printf("    comps: %u\n", instr->comps.len);

    // print THE FIRST component's parameters
    ComponentCall cc = instr->comps.arr[0];

    for (u32 i = 0; i < cc.args.len; ++i) {
        Parameter p = cc.args.arr[i];

        printf("    ");
        StrPrint(p.name);
        if (p.default_val.len) {
            printf(" = ");
            StrPrint(p.default_val);
        }

        if (p.type.len) {
            printf(" (");
            StrPrint(p.type);
            printf(")");
        }
        printf("\n");
    }
}
#endif
