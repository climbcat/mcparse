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
    Array<Parameter> params;

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

    bool parse_error;
};


enum ParseTokenResult {
    PTR_UNDEF,

    PTR_ERROR,
    PTR_OK,
    PTR_UNUSED,

    PTR_CNT
};

ParseTokenResult RequiredRVal(Tokenizer *t, Token *tok_out) {
    Tokenizer prev = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.is_rval) {
        return PTR_OK;
    }
    else {
        printf("\n\nERROR: Expected: 'r-value' got: '%s'\n", TokenTypeToSymbol(tok.type));
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
        return PTR_OK;
    }
    else {
        printf("\n\nERROR: Expected: '%s' got: '%s'\n", TokenTypeToSymbol(req), TokenTypeToString(tok.type));
        PrintLineError(t, &tok, "");

        assert(1 == 0 && "DBG break");
        return PTR_ERROR;
    }
}

ParseTokenResult Optional(Tokenizer *t, Token *tok_out, TokenType opt) {
    Tokenizer was = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt) {
        return PTR_OK;
    }
    else {
        // rewind if not used
        *t = was;

        return PTR_UNUSED;
    }
}

ParseTokenResult OptionalDontAdvance(Tokenizer *t, TokenType opt) {
    Tokenizer was = *t;

    Token tok = GetToken(t);

    // rewind
    *t = was;

    if (tok.type == opt) {
        return PTR_OK;
    }
    else {
        return PTR_UNUSED;
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

    if (OptionalDontAdvance(t, TOK_IDENTIFIER) == PTR_OK) { // <-- initiates a parameter parse
        bool iterate = true;
        while (iterate) {
            Parameter p = {};

            Token tok_parname_or_partype;
            Required(t, &tok_parname_or_partype, TOK_IDENTIFIER);

            Token tok_parname_or_nothing;
            if (Optional(t, &tok_parname_or_nothing, TOK_IDENTIFIER) == PTR_OK) {
                // parameter type and name
                p.type = tok_parname_or_partype.GetValue();
                p.name = tok_parname_or_nothing.GetValue();
            }

            else {
                // no type, only the parameter name
                p.name = tok_parname_or_partype.GetValue();
            }

            if (Optional(t, &token, TOK_ASSIGN) == PTR_OK) {
                // = and default value

                if (p.type.len == 6 && StrEqual(p.type, "vector")) {

                    if (Optional(t, &token, TOK_NULL) == PTR_OK) {
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
                // TODO: deal with parameter type 'vector' which is inline initialization a-la {0,0,0}
            }
            params->Add(p);

            iterate = Optional(t, &token, TOK_COMMA) == PTR_OK; // <-- iterates a parameter parse
        }
    }

    // TODO: On error, can we present all optional keywords as alternatives?
    //      That way, the parser could guide users and act as a form of in-line documentation
    //      Thus merge Required() with the Optional call above.
    //      Or repeat the information - but this seems hard to control, potentially buggy

    Required(t, &token, TOK_RBRACK);

    PackArrayAllocation(a_dest, params);
}


bool ParseCodeBlock(Tokenizer *t, TokenType block_type, Str *block, Str *type_copy) {
    Token token;

    if (Optional(t, &token, block_type) == PTR_OK) {
        if (Optional(t, &token, TOK_MCSTAS_COPY) == PTR_OK) {
            Required(t, &token, TOK_IDENTIFIER);
            *type_copy = token.GetValue();
        }

        else {
            Required(t, &token, TOK_LPERCENTBRACE);

            char *block_start = t->at;
            while (true) {
                token = GetToken(t);
                if (token.type == TOK_RPERCENTBRACE) {
                    block->str = block_start;
                    block->len = (token.text - block_start);

                    break;
                }
                else if (token.type == TOK_ENDOFSTREAM) {
                    //comp->parse_error = true;
                    assert(1 == 0 && "DBG break");

                    return false;
                }
            }        
        }
    }
    return true;
}

Component *ParseComponent(MArena *a_dest, char *text) {
    TimeFunction;

    // req:
    // DEFINE COMPONENT <compname> <newline>
    
    // opt:
    // DEFINITION PARAMETERS ( ... )
    // SETTING PARAMETERS ( ... )
    // OTUPUT PARAMETERS ( ... )

    // opt:
    // SHARE %( ... %)

    // req:
    // DECLARE %( ... %)
    // INITIALIZE %( ... %)
    // TRACE %( ... %)
    // FINALLY %( ... %)
    // MCDISPLAY %( ... %)

    // req:
    // END

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
    if (Optional(t, &token, TOK_MCSTAS_COPY) == PTR_OK) {
        Required(t, &token, TOK_IDENTIFIER);
        comp->type_copy = token.GetValue();
    }

    // setting parameters
    Required(t, &token, TOK_MCSTAS_SETTING);
    Required(t, &token, TOK_MCSTAS_PARAMETERS);
    ParseParams(a_dest, t, &comp->params);

    // output parameters
    if (Optional(t, &token, TOK_MCSTAS_OUTPUT) == PTR_OK) {
        Required(t, &token, TOK_MCSTAS_PARAMETERS);
        ParseParams(a_dest, t, &comp->params);
    }

    // flags
    while (Optional(t, &token, TOK_IDENTIFIER) == PTR_OK) {
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

    // code blocks
    bool block_ok;
    block_ok = ParseCodeBlock(t, TOK_MCSTAS_SHARE, &comp->share_block, &comp->share_type_copy);
    block_ok = ParseCodeBlock(t, TOK_MCSTAS_USERVARS, &comp->uservars_block, &comp->uservars_type_copy);
    block_ok = ParseCodeBlock(t, TOK_MCSTAS_DECLARE, &comp->declare_block, &comp->declare_type_copy);
    block_ok = ParseCodeBlock(t, TOK_MCSTAS_INITIALIZE, &comp->initalize_block, &comp->initalize_type_copy);
    block_ok = ParseCodeBlock(t, TOK_MCSTAS_TRACE, &comp->trace_block, &comp->trace_type_copy);
    block_ok = ParseCodeBlock(t, TOK_MCSTAS_SAVE, &comp->trace_block, &comp->trace_type_copy);
    block_ok = ParseCodeBlock(t, TOK_MCSTAS_FINALLY, &comp->finally_block, &comp->finally_type_copy);
    block_ok = ParseCodeBlock(t, TOK_MCSTAS_MCDISPLAY, &comp->display_block, &comp->display_type_copy);

    // end
    Required(t, &token, TOK_MCSTAS_END);

    return comp;
}


void ComponentPrint(Component *comp) {

    printf("type: "); StrPrint(comp->type); printf("\n");

    for (u32 i = 0; i < comp->params.len; ++i) {
        Parameter p = comp->params.arr[i];

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


#endif
