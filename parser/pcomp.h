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
    Array<Parameter> params;

    /*
    DeclareDef declare;
    InitializeDef init;
    TraceDef trace;
    FinalizeDef finalize;
    */
};


void ExpectToken(Tokenizer *t, TokenType exp, bool optional) {

    Token got = GetToken(t);

    if (got.type == exp) {

    }

}


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
        printf("Error: Expected r-value, got: %s\n", TokenTypeToString(tok.type));
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
        printf("Error: Expected %s, got: %s\n", TokenTypeToString(req), TokenTypeToString(tok.type));
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


Component ComponentParse(MArena *a_dest, char *text) {
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

    /*
    Token token;
    if (!RequireToken(tokenizer, &token, TOK_IDENTIFIER, "DEFINE"))
        exit(1);
    if (!RequireToken(tokenizer, &token, TOK_IDENTIFIER, "COMPONENT"))
        exit(1);
    if (!RequireToken(tokenizer, &token, TOK_IDENTIFIER))
        exit(1);
    */

    Tokenizer tokenizer = {};
    tokenizer.Init(text);
    Tokenizer *t = &tokenizer;
    Component comp = {};
    comp.params = InitArray<Parameter>(a_dest, 100);

    Token token;
    Required(t, &token, TOK_MCSTAS_DEFINE);
    Required(t, &token, TOK_MCSTAS_COMPONENT);
    Required(t, &token, TOK_IDENTIFIER);

    // TODO: this the token values rather be copied in-line?
    comp.type = token.GetValue();

    Required(t, &token, TOK_MCSTAS_SETTING);
    Required(t, &token, TOK_MCSTAS_PARAMETERS);
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


                // TODO: deal with parameter type 'vector' which is inline initialization a-la {0,0,0}


                RequiredRVal(t, &token);
                p.default_val = token.GetValue();
            }

            comp.params.Add(p);

            iterate = Optional(t, &token, TOK_COMMA) == PTR_OK; // <-- iterates a parameter parse
        }
    }

    // TODO: On this call to Expect, we need to present "TOK_IDENTIFIER"
    //      as an alternative option to TOK_RBRACK in case of an error!
    //      So I expect to have to merge Expect with the Optional call above.
    Required(t, &token, TOK_RBRACK);

    // Done! Now we have parsed the component declaration; let's explore it by parsing all of them


    // TODO: shorten up the parameters array & allocation

    return comp;
}


void ComponentPrint(Component comp) {

    printf("type: "); StrPrint(comp.type); printf("\n");

    for (u32 i = 0; i < comp.params.len; ++i) {
        Parameter p = comp.params.arr[i];

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
