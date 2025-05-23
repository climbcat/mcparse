#ifndef __PINSTR_H__
#define __PINSTR_H__


// sould be a linked list? Simply because this allows in-lining
struct ComponentCall {
    ComponentCall *next;
    ComponentCall *first;

    Str name;
    Str copy_name;
    Str type;
    Str extend;
    Str when;

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
    Str dependency_str;
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

    // instrument name
    Required(t, &token, TOK_MCSTAS_DEFINE);
    Required(t, &token, TOK_MCSTAS_INSTRUMENT);
    Required(t, &token, TOK_IDENTIFIER);
    instr->name = token.GetValue();

    // parameters
    instr->params = ParseParamsBlock(a_dest, t);

    // flags
    while (Optional(t, &token, TOK_IDENTIFIER) == PTR_OPTIONAL) {
        if (StrEqual( StrL("DEPENDENCY"), token.GetValue())) { 
            Required(t, &token, TOK_STRING);
            instr->dependency_str = token.GetValue();
        }
    }


    // TODO: code block be ordering-agnostic
    /*
    TokenType options_params[] = {
        TOK_MCSTAS_SETTING,
        TOK_MCSTAS_OUTPUT,
        TOK_MCSTAS_STATE,
        TOK_MCSTAS_POLARISATION,
    };
    */

    // code blocks
    Str _;
    ParseCodeBlock(t, TOK_MCSTAS_DECLARE, &instr->declare_block, &_, &_);
    ParseCodeBlock(t, TOK_MCSTAS_USERVARS, &instr->uservars_block, &_, &_);
    ParseCodeBlock(t, TOK_MCSTAS_INITIALIZE, &instr->initalize_block, &_, &_);

    // component calls
    Required(t, &token, TOK_MCSTAS_TRACE);
    while (true) {

        if (Optional(t, &token, TOK_MCSTAS_COMPONENT)) {
            ComponentCall c = {};

            // instance name
            OptionOfTwo(t, &token, TOK_IDENTIFIER, TOK_MCSTAS_COPY);
            if (token.type == TOK_MCSTAS_COPY) {
                Required(t, &token, TOK_LBRACK);
                Required(t, &token, TOK_IDENTIFIER);
                c.copy_name = token.GetValue();
                Required(t, &token, TOK_RBRACK);
            }
            else {
                c.name = token.GetValue();
            }
            Required(t, &token, TOK_ASSIGN);

            // type or COPY
            OptionOfTwo(t, &token, TOK_IDENTIFIER, TOK_MCSTAS_COPY);
            if (token.type == TOK_MCSTAS_COPY) {
                Required(t, &token, TOK_LBRACK);
                OptionOfTwo(t, &token, TOK_IDENTIFIER, TOK_MCSTAS_PREVIOUS);
                c.copy_name = token.GetValue();
                Required(t, &token, TOK_RBRACK);
            }
            else {
                c.type = token.GetValue();
            }

            // args
            Tokenizer was = *t;
            bool has_explicit_params = Optional(t, &token, TOK_LBRACK) == PTR_OPTIONAL;
            *t = was;

            if (has_explicit_params) {
                c.args = ParseParamsBlock(a_dest, t, true);
            }

            // when
            if (Optional(t, &token, TOK_MCSTAS_WHEN)) {
                Required(t, &token, TOK_LBRACK);
                RequiredRValOrExpression(t, &token);
                c.when = token.GetValue();
                Required(t, &token, TOK_RBRACK);
            }

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
            Optional(t, &token, TOK_MCSTAS_RELATIVE);
            if (token.type == TOK_MCSTAS_RELATIVE) {
                OptionOfThree(t, &token, TOK_IDENTIFIER, TOK_MCSTAS_PREVIOUS, TOK_MCSTAS_ABSOLUTE);
                c.at_relative_to = token.GetValue();
            }
            else {
                Required(t, &token, TOK_MCSTAS_ABSOLUTE);
                c.at_absolute = true;
            }

            // rotated
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
                    OptionOfThree(t, &token, TOK_IDENTIFIER, TOK_MCSTAS_PREVIOUS, TOK_MCSTAS_ABSOLUTE);
                    c.rot_relative_to = token.GetValue();
                }
                else {
                    c.rot_absolute = true;
                }

            }
            else {

            }

            ParseCodeBlock(t, TOK_MCSTAS_EXTEND, &c.extend, &_, &_);
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

    printf("\n");
    printf("name: "); StrPrint(instr->name); printf("\n");
    printf("params: ");
    printf("\n\n");
    Array<Parameter> ips = instr->params;
    for (u32 i = 0; i < ips.len; ++i) {
        Parameter p = ips.arr[i];

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


    printf("\n");
    if (instr->declare_block.len) {
        printf("\nDECLARE:\n");
        StrPrint(instr->declare_block);
        printf("\n");
    }


    printf("\n");
    printf("    comps: %u\n", instr->comps.len);

    return;

    // print THE FIRST component's parameters
    for (s32 j = 0; j < instr->comps.len; ++j) {
        ComponentCall cc = instr->comps.arr[j];

        StrPrint("", cc.name," (");
        if (cc.type.len) {
            StrPrint("", cc.type, ")");
        }
        else {
            StrPrint("COPY (", cc.copy_name, "))");
        }
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

        if (cc.when.len) {
            StrPrint("WHEN (", cc.when, ")\n");
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

        if (cc.extend.len) {
            printf("\n");
            StrPrint("EXTEND %%(", cc.copy_name, "\n%%)\n");

        }

        printf("\n\n");
    }
}
#endif
