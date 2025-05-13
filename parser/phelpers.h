#ifndef __PHELPERS_H__
#define __PHELPERS_H__


struct Parameter {
    Str type;
    Str name;
    Str default_val;
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

ParseTokenResult RequiredRValOrExpression(Tokenizer *t, Token *tok_out) {

    Tokenizer was;
    Token tok;
    Token token;

    s32 brack_level = 0;
    bool first = true;
    bool search = true;
    while (search) {
        was = *t;

        tok = GetToken(t);
        if (first) {
            first = false;
            if (tok.type == TOK_COMMA || tok.type == TOK_RBRACK) {
                // alarm
                *tok_out = tok;
                // TODO: line error
                assert(1 == 0 && "DBG break");

                return PTR_ERROR;
            }
            token = {};
            token.type = TOK_MCSTAS_C_EXPRESSION; // TODO: should be TOK_MCSTAS_EXPRESSION or what
            token.text = tok.text;
        }

        switch (tok.type)
        {
            case TOK_COMMA: {
                search = false;
            } break;

            case TOK_LBRACK: {
                ++brack_level;
            } break;

            case TOK_RBRACK: {
                if (brack_level == 0) {
                    search = false;
                }
                else {
                    --brack_level;
                }
            } break;

            case TOK_ENDOFSTREAM: {
                assert(1 == 0 && "DBG break");
            } break;

            default: break;
        }
    }

    token.len = tok.text - token.text;
    token.is_rval = true;
    *tok_out = token;
    *t = was;

    return PTR_TERMINAL;
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

void PackArrayAllocation(MArena *a_src, Array<Parameter> *arr_at_tail) {
    s32 diff = arr_at_tail->max - arr_at_tail->len;
    a_src->used -= diff * sizeof(Parameter);
    arr_at_tail->max = arr_at_tail->len;
}


Array<Parameter> ParseParamsBlock(MArena *a_dest, Tokenizer *t, bool allow_value_expression = false) {
    Array<Parameter> params = InitArray<Parameter>(a_dest, 1024);
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
                    if (allow_value_expression) {
                        RequiredRValOrExpression(t, &token);
                    }
                    else {
                        RequiredRVal(t, &token);
                    }
                    p.default_val = token.GetValue();
                }

                // branch
                OptionOfTwo(t, &token, TOK_COMMA, TOK_RBRACK);
            }
            params.Add(p);

            iterate = token.type == TOK_COMMA;
        }
    }

    PackArrayAllocation(a_dest, &params);
    return params;
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


#endif
