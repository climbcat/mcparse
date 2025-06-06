#ifndef __PHELPERS_H__
#define __PHELPERS_H__


static bool dbg_print;

struct Parameter {
    Str type;
    Str name;
    Str default_val;
};

bool RequiredRVal(Tokenizer *t, Token *tok_out) {
    if (t->parse_error) return false;

    Tokenizer prev = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.is_rval) {
        return true;
    }
    else {
        printf("\n\nERROR: Expected r-value, got '%s'\n", TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");

        // TODO: call hanlde error function
        assert(1 == 0 && "DBG break");

        return false;
    }
}

bool RequiredRValOrExpression(Tokenizer *t, Token *tok_out) {
    if (t->parse_error) return false;

    Tokenizer was;
    Token tok;
    Token token;

    s32 line_in = t->line;
    s32 brack_level = 0;
    bool first = true;
    bool search = true;
    while (search) {
        was = *t;

        tok = GetToken(t);
        if (first) {
            first = false;
            if (tok.type == TOK_COMMA || tok.type == TOK_RBRACK) {
                *tok_out = tok;

                // TODO: line error
                // TODO: call hanlde error function
                assert(1 == 0 && "DBG break");

                return false;
            }
            token = {};
            token.type = TOK_MCSTAS_C_EXPRESSION; // TODO: should be TOK_MCSTAS_EXPRESSION or what
            token.text = tok.text;
        }

        // TODO: If brack_level < 0 -> we actually have a parse error at this point.
        //      We need to completely re-write this part of the parser.

        switch (tok.type)
        {
            case TOK_COMMA: {
                search = brack_level > 0;
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

            case TOK_FLOAT: {
                //
            } break;

            case TOK_INT: {
                //
            } break;

            case TOK_ASSIGN: {
                //
            } break;

            case TOK_IDENTIFIER: {
                //
            } break;

            case TOK_STRING: {
                //
            } break;

            case TOK_ASTERISK: {
                //
            } break;

            case TOK_SLASH: {
                //
            } break;

            case TOK_OR: {
                //
            } break;

            case TOK_AND: {
                //
            } break;

            case TOK_DASH: {
                //
            } break;

            case TOK_PLUS: {
                //
            } break;

            case TOK_LEDGE: {
                //
            } break;

            case TOK_REDGE: {
                //
            } break;

            case TOK_QUESTION: {
                //
            } break;

            case TOK_COLON: {
                //
            } break;

            case TOK_DOT: {
                //
            } break;

            case TOK_LSBRACK: {
                //
            } break;

            case TOK_RSBRACK: {
                //
            } break;

            case TOK_UNKNOWN: {
                // tolerate TOK_UNKNOWN
            } break;

            case TOK_MCSTAS_PREVIOUS: {
                // whitelist TOK_MCSTAS_PREVIOUS
            } break;

            case TOK_EXCLAMATION: {
                //
            } break;

            case TOK_ENDOFSTREAM: {
                // TODO: call hanlde error function
                assert(1 == 0 && "DBG break");
            } break;

            default: {
                // break for non-whitelisted symbol
                search = false;
            }
        }
    }

    token.len = tok.text - token.text;
    token.is_rval = true;
    *tok_out = token;
    *t = was;

    if (dbg_print && token.type == TOK_MCSTAS_C_EXPRESSION) {
        StrPrint("", token.GetValue(), "\n");
    }
    
    return true;
}

bool Required(Tokenizer *t, Token *tok_out, TokenType req) {
    if (t->parse_error) return false;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == req) {
        return true;
    }
    else {
        printf("\n\nERROR: Expected '%s', got '%s'\n", TokenTypeToSymbol(req), TokenTypeToString(tok.type));
        PrintLineError(t, &tok, "");

        // TODO: call HandleParseError
        assert(1 == 0 && "DBG break");

        return false;
    }
}


bool BranchMultiple(Tokenizer *t, Token *tok_out, TokenType options[], s32 options_cnt, const char *options_error, TokenType terminal_rewind) {
    if (t->parse_error) return false;

    Tokenizer was = *t;
    Token tok = GetToken(t);
    *tok_out = tok;

    for (s32 i = 0; i < options_cnt; ++i) {
        if (tok.type == options[i]) {
            *t = was;

            return true;
        }
    }

    if (tok.type == terminal_rewind) {
        *t = was;

        return true;
    }
    else {
        printf("\n\nERROR: Expected '%s' or '%s', got '%s'\n", options_error, TokenTypeToSymbol(terminal_rewind), TokenTypeToString(tok.type));
        PrintLineError(t, &tok, "");

        // TODO: call HandleParseError
        assert(1 == 0 && "DBG break");

        return false;
    }
}

// TODO: replace with Options(cnt) og OptionalMultiple
bool OptionOfThree(Tokenizer *t, Token *tok_out, TokenType opt0, TokenType opt1, TokenType opt2) {
    if (t->parse_error) return false;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt0 || tok.type == opt1 || tok.type == opt2) {
        return true;
    }
    else {
        printf("\n\nERROR: Expected '%s', '%s' or '%s', got '%s'\n", TokenTypeToSymbol(opt0), TokenTypeToSymbol(opt1), TokenTypeToSymbol(opt2), TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");

        // TODO: call HandleParseError
        assert(1 == 0 && "DBG break");

        return false;
    }
}

// TODO: replace with Options(cnt)
bool OptionOfFour(Tokenizer *t, Token *tok_out, TokenType opt0, TokenType opt1, TokenType opt2, TokenType opt3) {
    if (t->parse_error) return false;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt0 || tok.type == opt1 || tok.type == opt2 || tok.type == opt3) {
        return true;
    }
    else {
        printf("\n\nERROR: Expected '%s', '%s', '%s' or '%s', got '%s'\n", TokenTypeToSymbol(opt0), TokenTypeToSymbol(opt1), TokenTypeToSymbol(opt2), TokenTypeToSymbol(opt3), TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");

        // TODO: call HandleParseError
        assert(1 == 0 && "DBG break");

        return false;
    }
}

// TODO: replace with Options(cnt)
bool OptionOfFive(Tokenizer *t, Token *tok_out, TokenType opt0, TokenType opt1, TokenType opt2, TokenType opt3, TokenType opt4) {
    if (t->parse_error) return false;

    Tokenizer rewind = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt0 || tok.type == opt1 || tok.type == opt2 || tok.type == opt3 || tok.type == opt4) {
        return true;
    }
    else {
        printf("\n\nERROR: Expected '%s', '%s', '%s', '%s' or '%s', got '%s'\n", TokenTypeToSymbol(opt0), TokenTypeToSymbol(opt1), TokenTypeToSymbol(opt2), TokenTypeToSymbol(opt3), TokenTypeToSymbol(opt4), TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");

        // TODO: call HandleParseError
        assert(1 == 0 && "DBG break");

        return false;
    }
}

// TODO: replace with Options(cnt)
bool OptionOfTwo(Tokenizer *t, Token *tok_out, TokenType opt0, TokenType opt1) {
    if (t->parse_error) return false;

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

bool OptionOfTwoRewind(Tokenizer *t, Token *tok_out, TokenType opt0, TokenType opt1) {
    if (t->parse_error) return false;

    Tokenizer was = *t;
    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt0 || tok.type == opt1) {
        return true;
    }
    else {
        *t = was;

        return false;
    }
}

bool Optional(Tokenizer *t, Token *tok_out, TokenType opt) {
    if (t->parse_error) return false;

    Tokenizer was = *t;
    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.type == opt) {
        return true;
    }
    else {
        *t = was;

        return false;
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
            if (Optional(t, &tok_parname_or_nothing, TOK_IDENTIFIER)) {
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
                if (Optional(t, &token, TOK_LBRACE)) {
                    p.default_val.str = token.text;
                    do {
                        token = GetToken(t);
                    }
                    while (token.type != TOK_RBRACE);
                    p.default_val.len = token.text - p.default_val.str + 1;
                    p.type = StrL("vector");
                }
                else if (Optional(t, &token, TOK_NULL)) {
                    p.default_val.str = token.text;
                    p.default_val.len = token.len;
                    p.type = StrL("vector");
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
    if (t->parse_error) return false; // technically not necessary perhaps

    Token token = {};

    Str *block_str = block;

    if (Optional(t, &token, block_type)) {
        if (Optional(t, &token, TOK_MCSTAS_COPY)) {
            Required(t, &token, TOK_IDENTIFIER);
            *type_copy = token.GetValue();

            if (Optional(t, &token, TOK_MCSTAS_EXTEND)) {
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
                // TODO: do HandleParseError
                assert(1 == 0 && "DBG break");

                return false;
            }
        }
    }
    else {
        return false;
    }

    return true;
}


#endif
