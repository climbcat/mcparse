#ifndef __PINSTR_H__
#define __PINSTR_H__


struct ComponentCall {
    ComponentCall *next;
    ComponentCall *first;

    Str name;
    Str copy_name;
    Str copy_type;
    Str type;
    Str extend;
    Str when;
    Str jump;
    Str group;

    Str split;
    bool removable;

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
    Str path;
    Str name;
    Str dependency_str;
    Array<Parameter> params;
    Array<StructMember> declare_members;
    Array<ComponentCall> comps;
    Array<Str> includes;

    Str uservars_block;
    Str declare_block;
    Str initalize_block;
    Str trace_block;
    Str finally_block;

    s32 check_idx;
    bool parse_error;
    bool type_checked;
    bool namerefs_checked;
};


Instrument *ParseInstrument(MArena *a_dest, Str text) {
    TimeFunction;

    Tokenizer tokenizer = {};
    tokenizer.Init(text.str);
    Tokenizer *t = &tokenizer;
    Token token;
    Instrument *instr = (Instrument*) ArenaAlloc(a_dest, sizeof(Instrument));
    instr->comps = InitArray<ComponentCall>(a_dest, 1000);
    instr->includes = InitArray<Str>(a_dest, 10);

    // instrument name
    Required(t, &token, TOK_MCSTAS_DEFINE);
    Required(t, &token, TOK_MCSTAS_INSTRUMENT);
    Required(t, &token, TOK_IDENTIFIER);
    instr->name = token.GetValue();

    // parameters
    instr->params = ParseParamsBlock(a_dest, t);

    // flags
    while (Optional(t, &token, TOK_IDENTIFIER)) {
        if (StrEqual( StrL("DEPENDENCY"), token.GetValue())) { 
            Required(t, &token, TOK_STRING);
            instr->dependency_str = token.GetValue();
        }
    }

    // declare block as struct members
    if (Optional(t, &token, TOK_MCSTAS_DECLARE)) {
        Required(t, &token, TOK_LPERCENTBRACE);
        instr->declare_members = ParseMembers(a_dest, t);
        Required(t, &token, TOK_RPERCENTBRACE);
    }

    // code blocks
    Str _;
    ParseCodeBlock(t, TOK_MCSTAS_USERVARS, &instr->uservars_block, &_, &_);
    ParseCodeBlock(t, TOK_MCSTAS_INITIALIZE, &instr->initalize_block, &_, &_);

    // component calls
    Required(t, &token, TOK_MCSTAS_TRACE);
    while (true) {

        while (Optional(t, &token, TOK_MCSTAS_PINCLUDE)) {
            Required(t, &token, TOK_STRING);
            instr->includes.Add(token.GetValue());
        }

        Tokenizer rewind = *t;
        if (OptionOfFive(t, &token, TOK_MCSTAS_COMPONENT, TOK_MCSTAS_SPLIT, TOK_MCSTAS_REMOVABLE, TOK_MCSTAS_FINALLY, TOK_MCSTAS_END)) {
            if (token.type == TOK_MCSTAS_END || token.type == TOK_MCSTAS_FINALLY) {
                *t = rewind;
                break;
            }

            ComponentCall c = {};

            // handle SPLIT prefix
            if (token.type == TOK_MCSTAS_SPLIT) {
                if (OptionOfTwoRewind(t, &token, TOK_INT, TOK_IDENTIFIER)) {
                    c.split = token.GetValue();
                }
                else {
                    c.split = StrL("1");
                }
                Required(t, &token, TOK_MCSTAS_COMPONENT);
            }

            if (token.type == TOK_MCSTAS_REMOVABLE) {
                // TODO: record the REMOVABLE data on the component instance 
                c.removable = true;

                Required(t, &token, TOK_MCSTAS_COMPONENT);
            }

            // comp instance name
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
                c.copy_type = token.GetValue();
                Required(t, &token, TOK_RBRACK);
            }
            else {
                c.type = token.GetValue();
            }

            // args
            Tokenizer was = *t;
            bool has_explicit_params = Optional(t, &token, TOK_LBRACK);
            *t = was;

            if (has_explicit_params) {
                c.args = ParseParamsBlock(a_dest, t, true);
            }

            // when / jump
            if (Optional(t, &token, TOK_MCSTAS_JUMP)) {
                Required(t, &token, TOK_IDENTIFIER);
                c.jump = token.GetValue();
            }
            if (Optional(t, &token, TOK_MCSTAS_WHEN)) {
                RequiredRValOrExpression(t, &token);
                c.when = token.GetValue();
            }

            // at
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

            // group
            if (Optional(t, &token, TOK_MCSTAS_GROUP)) {
                Required(t, &token, TOK_IDENTIFIER);
                c.group = token.GetValue();
            }

            // when once more
            if (Optional(t, &token, TOK_MCSTAS_JUMP)) {
                Required(t, &token, TOK_IDENTIFIER);
                c.jump = token.GetValue();
            }
            if (Optional(t, &token, TOK_MCSTAS_WHEN)) {
                RequiredRValOrExpression(t, &token);
                c.when = token.GetValue();
            }

            ParseCodeBlock(t, TOK_MCSTAS_EXTEND, &c.extend, &_, &_);
            instr->comps.Add(c);
        }
        else {
            break;
        }
    }

    ParseCodeBlock(t, TOK_MCSTAS_FINALLY, &instr->finally_block, &_, &_);
    Required(t, &token, TOK_MCSTAS_END);

    instr->parse_error = t->parse_error;
    return instr;
}


void InstrumentPrint(Instrument *instr, bool print_blocks, bool print_comps, bool print_comp_details) {

    printf("\n");
    printf("instrparse: "); StrPrint(instr->name); printf("- %u parameters, %u compnents", instr->params.len, instr->comps.len);
    printf("\n\n");

    printf("\n");
    printf("DEFINE INSTRUMENT "); StrPrint(instr->name); printf("(\n");
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
    printf(")\n");

    if (print_blocks) {
        if (instr->uservars_block.len) {
            printf("\nUSERVARS:\n");
            StrPrint(instr->uservars_block);
            printf("\n");
        }
        printf("\n");
        if (instr->declare_block.len) {
            printf("\nDECLARE:\n");
            StrPrint(instr->declare_block);
            printf("\n");
        }
        printf("\n");
        if (instr->initalize_block.len) {
            printf("\nINITIALIZE:\n");
            StrPrint(instr->initalize_block);
            printf("\n");
        }
        printf("\n");
        if (instr->finally_block.len) {
            printf("\nFINALLY:\n");
            StrPrint(instr->finally_block);
            printf("\n");
        }
    }

    if (print_comps) {
        printf("\n");

        if (print_comp_details) {
            for (s32 j = 0; j < instr->comps.len; ++j) {
                ComponentCall cc = instr->comps.arr[j];

                if (cc.split.len) {
                    StrPrint("SPLIT ", cc.split, " ");
                }


                StrPrint("COMPONENT ", cc.name," = ");
                if (cc.type.len) {
                    StrPrint("", cc.type, "");
                }
                else {
                    StrPrint("COPY (", cc.copy_name, ")");
                }

                printf("(\n");
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
                printf(")\n");

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
                    StrPrint("EXTEND \%(", cc.extend, "\n\%)\n");

                }
                printf("\n\n");
            }
        }

    }
}
#endif
