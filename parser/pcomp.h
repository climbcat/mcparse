#ifndef __PCOMP_H__
#define __PCOMP_H__


bool IsCompFile(char *filename)
{
    // check lenth / get expected location
    u32 len = strlen(filename);
    if (len < 6)
    {
        return false;
    }

    const char *match = ".comp";
    char cf;
    char cm;
    for (int i = 0; i < 5; i++)
    {
        cf = filename[i + len - 5];
        cm = match[i];
        if (cf != cm)
        {
            return false;
        }
    }

    return true;
}


// TODO: ParDec
// TODO: ParVal


struct Parameter {
    Str type;
    Str name;
    Str default_val;
};


struct Component {
    Str type;
    Str type_copy;
    Array<Parameter> setting_params;
    Array<Parameter> out_params;
    Array<Parameter> state_params;
    Array<Parameter> pol_params;

    bool flag_noacc;
    Str dependency_str;

    Str share_block;
    Str uservars_block;
    Str declare_block;
    Str initalize_block;
    Str trace_block;
    Str finally_block;
    Str display_block;

    Str share_type_copy;
    Str uservars_type_copy;
    Str declare_type_copy;
    Str initalize_type_copy;
    Str trace_type_copy;
    Str finally_type_copy;
    Str display_type_copy;

    Str share_extend;
    Str uservars_extend;
    Str declare_extend;
    Str initalize_extend;
    Str trace_extend;
    Str finally_extend;
    Str display_extend;

    bool parse_error;
};


enum ParseTokenResult {
    PTR_UNDEF,

    PTR_OPTIONAL,
    PTR_TERMINAL,
    PTR_ERROR,

    PTR_CNT
};

ParseTokenResult RequiredRVal(Tokenizer *t, Token *tok_out) {
    Tokenizer prev = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.is_rval) {
        return PTR_TERMINAL;
    }
    else {
        printf("\n\nERROR: Expected r-value, got '%s'\n", TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");

        assert(1 == 0 && "DBG break");

        return PTR_ERROR;
    }
}

ParseTokenResult Required(Tokenizer *t, Token *tok_out, TokenType req) {
    Tokenizer prev = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == req) {
        return PTR_TERMINAL;
    }
    else {
        printf("\n\nERROR: Expected '%s', got '%s'\n", TokenTypeToSymbol(req), TokenTypeToString(tok.type));
        PrintLineError(t, &tok, "");

        assert(1 == 0 && "DBG break");

        return PTR_ERROR;
    }
}


// NOTE: How should we cover this situations: <first options> <more options after> <terminal>, e.g. sequential optional args.


ParseTokenResult BranchMultiple(Tokenizer *t, Token *tok_out, TokenType options[], s32 options_cnt, const char *options_error, TokenType terminal) {
    Tokenizer was = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    for (s32 i = 0; i < options_cnt; ++i) {
        if (tok.type == options[i]) {
            *t = was;

            return PTR_OPTIONAL;
        }
    }

    if (tok.type == terminal) {

        return PTR_TERMINAL;
    }
    else {
        printf("\n\nERROR: Expected '%s' or '%s', got '%s'\n", options_error, TokenTypeToSymbol(terminal), TokenTypeToString(tok.type));
        PrintLineError(t, &tok, "");

        assert(1 == 0 && "DBG break");

        return PTR_ERROR;
    }
}

bool OptionOfThree(Tokenizer *t, Token *tok_out, TokenType opt0, TokenType opt1, TokenType opt2) {
    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt0 || tok.type == opt1 || tok.type == opt2) {
        return true;
    }
    else {
        printf("\n\nERROR: Expected '%s', '%s' or '%s', got '%s'\n", TokenTypeToSymbol(opt0), TokenTypeToSymbol(opt1), TokenTypeToSymbol(opt2), TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");

        assert(1 == 0 && "DBG break");

        return false;
    }
}

bool OptionOfTwo(Tokenizer *t, Token *tok_out, TokenType opt0, TokenType opt1) {
    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt0 || tok.type == opt1) {
        return true;
    }
    else {
        printf("\n\nERROR: Expected '%s' or '%s', got %s\n", TokenTypeToSymbol(opt0), TokenTypeToSymbol(opt1), TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");


        assert(1 == 0 && "DBG break");

        return false;
    }
}



ParseTokenResult Optional(Tokenizer *t, Token *tok_out, TokenType opt) {
    Tokenizer was = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt) {
        return PTR_OPTIONAL;
    }
    else {
        // rewind if not used
        *t = was;

        return PTR_UNDEF;
    }
}

// TODO: useful or not?
ParseTokenResult OptionalRequired(Tokenizer *t, Token *tok_out, TokenType opt, TokenType req) {
    Tokenizer was = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt) {
        // optional check

        return PTR_OPTIONAL;
    }
    else if (tok.type == req) {
        // terminal comes after

        return PTR_TERMINAL;
    }
    else {
        printf("\n\nERROR: Expected '%s' or '%s', got '%s'\n", TokenTypeToSymbol(opt), TokenTypeToSymbol(req), TokenTypeToString(tok.type));
        PrintLineError(t, &tok, "");

        assert(1 == 0 && "DBG break");

        return PTR_ERROR;
    }
}

void PackArrayAllocation(MArena *a_src, Array<Parameter> *arr_at_tail) {
    s32 diff = arr_at_tail->max - arr_at_tail->len;
    a_src->used -= diff * sizeof(Parameter);
    arr_at_tail->max = arr_at_tail->len;
}

void ParseParams(MArena *a_dest, Tokenizer *t, Array<Parameter> *params) {
    *params = InitArray<Parameter>(a_dest, 1024);

    Token token;
    Required(t, &token, TOK_LBRACK);

    Tokenizer was = *t;
    OptionOfTwo(t, &token, TOK_IDENTIFIER, TOK_RBRACK);
    if (token.type == TOK_IDENTIFIER) {
        *t = was; // re-parse the opening identifier

        bool iterate = true;
        while (iterate) {
            Parameter p = {};

            Token tok_parname_or_partype;
            Required(t, &tok_parname_or_partype, TOK_IDENTIFIER);

            Token tok_parname_or_nothing;
            if (Optional(t, &tok_parname_or_nothing, TOK_IDENTIFIER) == PTR_OPTIONAL) {
                // parameter type and name
                p.type = tok_parname_or_partype.GetValue();
                p.name = tok_parname_or_nothing.GetValue();
            }

            else {
                // no type, only the parameter name
                p.name = tok_parname_or_partype.GetValue();
            }

            // branch
            OptionOfThree(t, &token, TOK_ASSIGN, TOK_COMMA, TOK_RBRACK);

            if (token.type == TOK_ASSIGN) {
                // parse default value
                if (p.type.len == 6 && StrEqual(p.type, "vector")) {

                    if (Optional(t, &token, TOK_NULL) == PTR_OPTIONAL) {
                        p.default_val.str = token.text;
                        p.default_val.len = token.len;
                    }
                    else {
                        // capture from '{' to '}'. Do it the lazy-pants way by ignoring everything in-between
                        Required(t, &token, TOK_LBRACE);
                        p.default_val.str = token.text;
                        do {
                            token = GetToken(t);
                        }
                        while (token.type != TOK_RBRACE);
                        p.default_val.len = token.text - p.default_val.str + 1;
                    }
                }
                else {
                    RequiredRVal(t, &token);
                    p.default_val = token.GetValue();
                }

                // branch
                OptionOfTwo(t, &token, TOK_COMMA, TOK_RBRACK);
            }
            params->Add(p);

            iterate = token.type == TOK_COMMA;
        }
    }

    PackArrayAllocation(a_dest, params);
}

bool ParseCodeBlock(Tokenizer *t, TokenType block_type, Str *block, Str *type_copy, Str *extend) {
    Token token;

    Str *block_str = block;

    if (Optional(t, &token, block_type) == PTR_OPTIONAL) {
        if (Optional(t, &token, TOK_MCSTAS_COPY) == PTR_OPTIONAL) {
            Required(t, &token, TOK_IDENTIFIER);
            *type_copy = token.GetValue();

            if (Optional(t, &token, TOK_MCSTAS_EXTEND) == PTR_OPTIONAL) {
                block_str = extend;
            }
            else {
                return true;
            }
        }

        Required(t, &token, TOK_LPERCENTBRACE);

        char *block_start = t->at;
        while (true) {
            token = GetToken(t);
            if (token.type == TOK_RPERCENTBRACE) {
                block_str->str = block_start;
                block_str->len = (token.text - block_start);

                break;
            }
            else if (token.type == TOK_ENDOFSTREAM) {
                //comp->parse_error = true;
                assert(1 == 0 && "DBG break");

                return false;
            }
        }        
    }

    return true;
}

Component *ParseComponent(MArena *a_dest, char *text) {
    TimeFunction;

    Tokenizer tokenizer = {};
    tokenizer.Init(text);
    Tokenizer *t = &tokenizer;
    Token token;
    Component *comp = (Component*) ArenaAlloc(a_dest, sizeof(Component));

    // type
    Required(t, &token, TOK_MCSTAS_DEFINE);
    Required(t, &token, TOK_MCSTAS_COMPONENT);
    Required(t, &token, TOK_IDENTIFIER);
    comp->type = token.GetValue();
    if (Optional(t, &token, TOK_MCSTAS_COPY) == PTR_OPTIONAL) {
        Required(t, &token, TOK_IDENTIFIER);
        comp->type_copy = token.GetValue();
    }

    // setting parameters
    Required(t, &token, TOK_MCSTAS_SETTING);
    Required(t, &token, TOK_MCSTAS_PARAMETERS);
    ParseParams(a_dest, t, &comp->setting_params);

    // output parameters
    if (Optional(t, &token, TOK_MCSTAS_OUTPUT) == PTR_OPTIONAL) {
        Required(t, &token, TOK_MCSTAS_PARAMETERS);
        ParseParams(a_dest, t, &comp->out_params);
    }

    // state parameters
    if (Optional(t, &token, TOK_MCSTAS_STATE) == PTR_OPTIONAL) {
        Required(t, &token, TOK_MCSTAS_PARAMETERS);
        ParseParams(a_dest, t, &comp->state_params);
    }

    // polarisation parameters
    if (Optional(t, &token, TOK_MCSTAS_POLARISATION) == PTR_OPTIONAL) {
        Required(t, &token, TOK_MCSTAS_PARAMETERS);
        ParseParams(a_dest, t, &comp->pol_params);
    }

    // flags
    while (Optional(t, &token, TOK_IDENTIFIER) == PTR_OPTIONAL) {
        if (StrEqual( StrL("DEPENDENCY"), token.GetValue())) { 
            Required(t, &token, TOK_STRING);
            printf("Paresed DEP\n");
            comp->dependency_str = token.GetValue();
        }

        if (StrEqual( StrL("NOACC"), token.GetValue())) {
            printf("Paresed NOACC\n");
            comp->flag_noacc;
        }
    }


    TokenType options[] = {
        TOK_MCSTAS_SHARE,
        TOK_MCSTAS_USERVARS,
        TOK_MCSTAS_DECLARE,
        TOK_MCSTAS_INITIALIZE,
        TOK_MCSTAS_TRACE,
        TOK_MCSTAS_SAVE,
        TOK_MCSTAS_FINALLY,
        TOK_MCSTAS_MCDISPLAY
    };

    ParseTokenResult result = {};
    while (result != PTR_TERMINAL) {
        Tokenizer dont_advance = *t;

        result = BranchMultiple(&dont_advance, &token, options, 8, "code block", TOK_MCSTAS_END);

        switch (token.type) {
        case TOK_ENDOFSTREAM: { } break;

        case TOK_MCSTAS_SHARE: { ParseCodeBlock(t, TOK_MCSTAS_SHARE, &comp->share_block, &comp->share_type_copy, &comp->share_extend); } break;
        case TOK_MCSTAS_USERVARS: { ParseCodeBlock(t, TOK_MCSTAS_USERVARS, &comp->uservars_block, &comp->uservars_type_copy, &comp->uservars_extend); } break;
        case TOK_MCSTAS_DECLARE: { ParseCodeBlock(t, TOK_MCSTAS_DECLARE, &comp->declare_block, &comp->declare_type_copy, &comp->declare_extend); } break;
        case TOK_MCSTAS_INITIALIZE: { ParseCodeBlock(t, TOK_MCSTAS_INITIALIZE, &comp->initalize_block, &comp->initalize_type_copy, &comp->initalize_extend); } break;
        case TOK_MCSTAS_TRACE: { ParseCodeBlock(t, TOK_MCSTAS_TRACE, &comp->trace_block, &comp->trace_type_copy, &comp->trace_extend); } break;
        case TOK_MCSTAS_SAVE: { ParseCodeBlock(t, TOK_MCSTAS_SAVE, &comp->trace_block, &comp->trace_type_copy, &comp->trace_extend); } break;
        case TOK_MCSTAS_FINALLY: { ParseCodeBlock(t, TOK_MCSTAS_FINALLY, &comp->finally_block, &comp->finally_type_copy, &comp->finally_extend); } break;
        case TOK_MCSTAS_MCDISPLAY: { ParseCodeBlock(t, TOK_MCSTAS_MCDISPLAY, &comp->display_block, &comp->display_type_copy, &comp->display_extend); } break;

        default: {  assert(token.type == TOK_MCSTAS_END); } break;
        }

        if (result == PTR_ERROR) {
            comp->parse_error = true;
            break;

            assert(1 == 0 && "DBG break");
        }
    }

    // end
    Required(t, &token, TOK_MCSTAS_END);

    return comp;
}


void ComponentPrint(Component *comp, bool print_blocks = false) {

    printf("type: "); StrPrint(comp->type); printf("\n");

    for (u32 i = 0; i < comp->setting_params.len; ++i) {
        Parameter p = comp->setting_params.arr[i];

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

    if (print_blocks) {
        if (comp->share_block.len) {
            printf("\nSHARE:\n");
            StrPrint(comp->share_block);
            printf("\n");
        }

        if (comp->declare_block.len) {
            printf("\nDECLARE:\n");
            StrPrint(comp->declare_block);
            printf("\n");
        }

        if (comp->initalize_block.len) {
            printf("\nINITIALIZE:\n");
            StrPrint(comp->initalize_block);
            printf("\n");
        }

        if (comp->trace_block.len) {
            printf("\nTRACE:\n");
            StrPrint(comp->trace_block);
            printf("\n");
        }

        if (comp->display_block.len) {
            printf("\nMCDISPLAY:\n");
            StrPrint(comp->display_block);
            printf("\n");
        }
    }

}


#endif
