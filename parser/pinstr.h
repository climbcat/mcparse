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

    Str at_relative_to;
    bool at_absolute;

    bool rot_defined;
    Str rot_x;
    Str rot_y;
    Str rot_z;

    Str rot_relative_to;
    bool rot_absolute;

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
    instr->comps = InitArray<ComponentCall>(a_dest, 100);

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

            Required(t, &token, TOK_MCSTAS_AT);
            // parse AT vector:
            Required(t, &token, TOK_LBRACK);
            RequiredRValOrExpression(t, &token);
            c.at_x = token.GetValue();
            Required(t, &token, TOK_COMMA);
            RequiredRValOrExpression(t, &token);
            c.at_y = token.GetValue();
            Required(t, &token, TOK_COMMA);
            RequiredRValOrExpression(t, &token);
            c.at_z = token.GetValue();
            Required(t, &token, TOK_RBRACK);

            // relative / absolute
            OptionOfTwo(t, &token, TOK_MCSTAS_RELATIVE, TOK_MCSTAS_ABSOLUTE);
            if (token.type == TOK_MCSTAS_RELATIVE) {
                OptionOfTwo(t, &token, TOK_IDENTIFIER, TOK_MCSTAS_PREVIOUS);
                c.at_relative_to = token.GetValue();
            }
            else {
                c.at_absolute = true;
            }

            if (Optional(t, &token, TOK_MCSTAS_ROTATED)) {
                c.rot_defined = true;

                // parse ROTATED vector:
                Required(t, &token, TOK_LBRACK);
                RequiredRValOrExpression(t, &token);
                c.rot_x = token.GetValue();
                Required(t, &token, TOK_COMMA);
                RequiredRValOrExpression(t, &token);
                c.rot_y = token.GetValue();
                Required(t, &token, TOK_COMMA);
                RequiredRValOrExpression(t, &token);
                c.rot_z = token.GetValue();
                Required(t, &token, TOK_RBRACK);

                // relative / absolute
                OptionOfTwo(t, &token, TOK_MCSTAS_RELATIVE, TOK_MCSTAS_ABSOLUTE);
                if (token.type == TOK_MCSTAS_RELATIVE) {
                    OptionOfTwo(t, &token, TOK_IDENTIFIER, TOK_MCSTAS_PREVIOUS);
                    c.rot_relative_to = token.GetValue();
                }
                else {
                    c.rot_absolute = true;
                }
            }
            else {

            }

            instr->comps.Add(c);
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
    for (s32 j = 0; j < instr->comps.len; ++j) {
        ComponentCall cc = instr->comps.arr[j];

        StrPrint("", cc.name," (");
        StrPrint("", cc.type, ")");
        printf("\n");
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
        printf("AT (");
        StrPrint("", cc.at_x ,", ");
        StrPrint("", cc.at_y ,", ");
        StrPrint("", cc.at_z ,")");

        if (cc.at_absolute == false) {
            StrPrint(" RELATIVE ", cc.at_relative_to, "");
        }
        else {
            printf(" ABSOLUTE ");
        }

        if (cc.rot_defined) {
            printf(" ROTATED (");
            StrPrint("", cc.rot_x ,", ");
            StrPrint("", cc.rot_y ,", ");
            StrPrint("", cc.rot_z ,")");

            if (cc.rot_absolute == false) {
                StrPrint(" RELATIVE ", cc.at_relative_to, "");
            }
            else {
                printf(" ABSOLUTE");
            }
        }

        printf("\n\n");
    }
}
#endif
