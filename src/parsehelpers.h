#ifndef __PHELPERS_H__
#define __PHELPERS_H__


static bool dbg_print_c_expressions;


struct Parameter {
    Str type;
    Str name;
    Str default_val;
};

struct StructMember {
    Str type;
    Str name;
    Str defval;
    s32 array_type_sz;
    s32 is_array_type;
    bool is_pointer_type;
};


static bool g_parse_error_causes_exit;
void HandleParseError(Tokenizer *t) {
    t->parse_error = true;

    if (g_parse_error_causes_exit) {
        exit(0);
    }
}

bool RequiredRVal(Tokenizer *t, Token *tok_out) {
    if (t->parse_error) return false;

    Tokenizer prev = *t;

    Token tok = GetToken(t);
    *tok_out = tok;

    if (tok.is_rval) {
        return true;
    }
    else {
        printf("\nERROR: Expected r-value, got '%s'\n", TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");
        HandleParseError(t);

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

                printf("\n\nERROR: Expected '%s', got '%s'\n", TokenTypeToSymbol(TOK_MCSTAS_C_EXPRESSION), TokenTypeToSymbol(tok.type));
                PrintLineError(t, &tok, "");
                HandleParseError(t);

                return false;
            }
            token = {};
            token.type = TOK_MCSTAS_C_EXPRESSION;
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
                HandleParseError(t);
                return false;
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

    if (dbg_print_c_expressions && token.type == TOK_MCSTAS_C_EXPRESSION) {
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
        printf("\n\nERROR: Expected '%s', got '%s'\n", TokenTypeToSymbol(req), TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");
        HandleParseError(t);

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
        printf("\n\nERROR: Expected '%s' or '%s', got '%s'\n", options_error, TokenTypeToSymbol(terminal_rewind), TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");
        HandleParseError(t);

        return false;
    }
}

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
        HandleParseError(t);

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
        HandleParseError(t);

        return false;
    }
}

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
        HandleParseError(t);

        return false;
    }
}

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
        HandleParseError(t);

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
    if (t->parse_error) return {};

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
    if (t->parse_error) return false;

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
                HandleParseError(t);

                return false;
            }
        }
    }
    else {
        return false;
    }

    return true;
}

void _ParseOptionalBracketedArrayDeclaration(Tokenizer *t, StructMember *mem) {
    Token tok;
    if (Optional(t, &tok, TOK_LSBRACK)) {
        assert(mem->is_pointer_type == false);

        OptionOfThree(t, &tok, TOK_INT, TOK_IDENTIFIER, TOK_RSBRACK);
        Str s_array_size = {};
        if (tok.type == TOK_RSBRACK) {
            s_array_size = { (char*) "0", 1 };
        }
        else {
            s_array_size = tok.GetValue();

            if (tok.type == TOK_INT) {
                mem->array_type_sz = ParseInt(s_array_size.str, s_array_size.len);
            }
            mem->is_array_type = true;
            assert(mem->array_type_sz >= 0);
            Required(t, &tok, TOK_RSBRACK);
        }
    }
}

Array<StructMember> ParseMembers(MArena *a_dest, Tokenizer *t) {
    if (t->parse_error) return {};

    Array<StructMember> mems = {};
    mems.arr = (StructMember*) ArenaAlloc(a_dest, 0);
    u32 cnt = 0;

    // ID [ASTERISK] ID [= ID] SEMICOLON

    Token tok = {};
    while (Optional(t, &tok, TOK_IDENTIFIER)) {
        cnt++;
        StructMember *mem = (StructMember*) ArenaAlloc(a_dest, sizeof(StructMember));

        if (TokenEquals(&tok, "struct")) {
            Token next;
            Required(t, &next, TOK_IDENTIFIER);
            tok.len = (next.text - tok.text) + next.len;
        }

        mem->type = tok.GetValue();
        if (Optional(t, &tok, TOK_ASTERISK)) {
            mem->is_pointer_type = true;
        }

        // clips out any inlined struct definition! Scrap this allocation
        if (Optional(t, &tok, TOK_LBRACE)) {

            while(tok.type != TOK_RBRACE) {
                tok = GetToken(t);
            }
            while(tok.type != TOK_SEMICOLON) {
                tok = GetToken(t);
            }

            // de-allocate 'mem' in a hurryt
            a_dest->used -= sizeof(StructMember);

            continue;
        }

        Required(t, &tok, TOK_IDENTIFIER);
        mem->name = tok.GetValue();

        if (Optional(t, &tok, TOK_IDENTIFIER)) {
            // this would be a 3-word type + name s.a. "long long nparticles"

            s32 diff = (mem->name.str - mem->type.str); 
            mem->type.len += diff + mem->name.len - mem->type.len;
            mem->name = tok.GetValue();
        }
        _ParseOptionalBracketedArrayDeclaration(t, mem);

        if (Optional(t, &tok, TOK_ASSIGN)) {
            if (Optional(t, &tok, TOK_LBRACE)) {
                mem->defval.str = tok.text;
                do {
                    tok = GetToken(t);
                }
                while (tok.type != TOK_RBRACE);
                mem->defval.len = tok.text - mem->defval.str + 1;
            }
            else {
                RequiredRValOrExpression(t, &tok);
                mem->defval = tok.GetValue();
            }
        }

        while (Optional(t, &tok, TOK_COMMA)) {
            // subsequent fields after the comma in e.g.: int a, b, c;
            
            StructMember *listed = (StructMember*) ArenaAlloc(a_dest, sizeof(StructMember));
            listed->type = mem->type;
            listed->is_pointer_type = Optional(t, &tok, TOK_ASTERISK);

            Required(t, &tok, TOK_IDENTIFIER);
            listed->name = tok.GetValue();

            _ParseOptionalBracketedArrayDeclaration(t, listed);
            if (Optional(t, &tok, TOK_ASSIGN)) {
                RequiredRValOrExpression(t, &tok);
                listed->defval = tok.GetValue();
            }

            cnt++;
        }

        // Required(t, &tok, TOK_SEMICOLON); // <- not enough, since there can be function declarations too

        // what we did instead:
        bool proceed_ok = OptionOfTwo(t, &tok, TOK_SEMICOLON, TOK_LBRACK);

        if ( tok.type == TOK_SEMICOLON) {
            // continue
        }
        else {
            // cut out the entire { function body }
            u32 brace_level = 0;

            while (tok.type != TOK_ENDOFSTREAM) {
                if (tok.type == TOK_LBRACE) {
                    brace_level++;
                }

                if (tok.type == TOK_RBRACE) {
                    brace_level--;
                    if (brace_level == 0) {
                        break;
                    }
                }

                if (tok.type == TOK_ENDOFSTREAM) {
                    printf("\n\nERROR: Expected '}', got 'end_of_stream'\n");
                    PrintLineError(t, &tok, "");
                    HandleParseError(t);

                    break;
                }

                tok = GetToken(t);
            }
            Optional(t, &tok, TOK_SEMICOLON);
        }

    }

    mems.len = cnt;
    mems.max = cnt;

    return mems;
}


//
//


inline
bool TokenInFilter(TokenType tpe, Array<TokenType> filter) {
    for (s32 i = 0; i < filter.len; ++i) {
        if (filter.arr[i] == tpe) {
            return true;
        }
    }

    return false;
}
static TokenType _filter_operators[] = { TOK_PLUS, TOK_DASH, TOK_SLASH, TOK_ASTERISK, TOK_AND, TOK_AND_COMPARE, TOK_OR, TOK_OR_COMPARE, TOK_LEDGE, TOK_REDGE, TOK_COLON, TOK_QUESTION, TOK_EQUALS, TOK_GREATEROREQUAL, TOK_LESSOREQUAL };
static TokenType _filter_symbols[] = { TOK_IDENTIFIER, TOK_MCSTAS_PREVIOUS, TOK_NULL, TOK_STRING, TOK_INT, TOK_FLOAT, TOK_SCI };
static TokenType _filter_seperator[] = { TOK_LBRACK, TOK_LBRACE, TOK_LSBRACK, TOK_AND, TOK_AND_COMPARE, TOK_OR, TOK_OR_COMPARE, TOK_LEDGE, TOK_REDGE, TOK_COLON, TOK_QUESTION, TOK_EQUALS, TOK_GREATEROREQUAL, TOK_LESSOREQUAL };
static Array<TokenType> g_filter_operators = { &_filter_operators[0], 15 };
static Array<TokenType> g_filter_symbols = { &_filter_symbols[0], 7 };
static Array<TokenType> g_filter_seperator = { &_filter_seperator[0], 14 };


static MArena *g_arena_parse_params;
static Array<Parameter> *g_parse_params;


Str ParseExpression(Tokenizer *t);


Str ParseBracketedExpressionList(Tokenizer *t) {
    if (t->parse_error) return {};
    Tokenizer t_prev = *t;
    Token tok = GetToken(t);
    Str result = {};
    result.str = tok.text;

    // '('
    if ((tok.type != TOK_LBRACK) && (tok.type != TOK_LSBRACK)) {
        // fail: expected TOK_LBRACK!

        printf("\nERROR: Expected '(', got '%s'\n", TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");
        HandleParseError(t);
    }

    else {
        while ((tok.type != TOK_RBRACK) && (tok.type != TOK_RSBRACK)) {
            Str expr = ParseExpression(t);
            if (t->parse_error) break;

            t_prev = *t;
            tok = GetToken(t);

            // ')'
            if ((tok.type == TOK_RBRACK) || (tok.type == TOK_RSBRACK)) {
                // ok, exit
                break;
            }

            // ','
            else if (expr.len > 0 && tok.type == TOK_COMMA) {
                // ok, continue
            }

            // something else
            else {
                if (expr.len == 0) {
                    // fail: expected expression

                    printf("\nERROR: Expected arithmetic expression, got '%s'\n", TokenTypeToSymbol(tok.type));
                    PrintLineError(t, &tok, "");
                    HandleParseError(t);

                    break;
                }

                else {
                    // fail: expected TOK_BRACK or TOK_COMMA

                    printf("\nERROR: Expected ',' or ')', got '%s'\n", TokenTypeToSymbol(tok.type));
                    PrintLineError(t, &tok, "");
                    HandleParseError(t);

                    break;
                }
            }
        }
    }

    result.len = t->at - result.str;
    return result;
}


Str ParseExpression(Tokenizer *t) {
    // returns as long an expression as possible (greedily)
    s32 bracket_level = 0;

    // having access to the two previous tokenizer steps is convenient
    Tokenizer t_prev = *t;
    Token tok_prev = {};
    Token tok = {};
    tok = GetToken(t);

    Str expr = {};
    expr.str = tok.text;

    while (tok.type != TOK_ENDOFSTREAM && t->parse_error == false) {
        if (tok.type == TOK_LBRACK || tok.type == TOK_LSBRACK) {
            if (tok_prev.type == TOK_IDENTIFIER || tok_prev.type == TOK_UNKNOWN) {
                *t = t_prev;

                ParseBracketedExpressionList(t);
            }
            else {
                bracket_level++;
            }
        }

        else if (tok.type == TOK_RBRACK || tok.type == TOK_RSBRACK) {
            if (bracket_level > 0) {
                bracket_level--;
            }
            else {
                // rewind back to the before the )
                *t = t_prev;
                break;
            }
        }

        else if (tok.type == TOK_LBRACE) {
            s32 brace_level = 0;
            while (tok.type != TOK_ENDOFSTREAM) {
                if (tok.type == TOK_RBRACE) {
                    break;
                }

                else if (tok.type == TOK_ENDOFSTREAM) {
                    printf("\n\nERROR: Expected '}', got 'end_of_stream'\n");
                    PrintLineError(t, &tok, "");
                    HandleParseError(t);

                    break;
                }

                else {
                    tok = GetToken(t);
                }
            }
        }

        else if (tok.type == TOK_EXCLAMATION) {
            // ignore exclaimations
        }

        else if (TokenInFilter(tok.type, g_filter_symbols)) {
            // token is a symbol

            if ((tok_prev.type != TOK_UNKNOWN) && TokenInFilter(tok_prev.type, g_filter_symbols)) {
                // exit: two symbols in a row

                *t = t_prev;
                break;
            }
        }

        else if (TokenInFilter(tok.type, g_filter_operators) || tok.type == TOK_DOT ) {
            // token is an operator

            if ((tok_prev.type != TOK_UNKNOWN) && TokenInFilter(tok_prev.type, g_filter_operators)) {
                if (TokenInFilter(tok_prev.type, g_filter_seperator) && (tok.type == TOK_DASH || tok.type == TOK_PLUS)) {
                    // the exception to the rule: (+ or (-
                }
                else {
                    // exit: two operators in a row

                    *t = t_prev;
                    break;
                }
            }
        }

        else {
            // unknown token
            *t = t_prev;
            break;
        }

        t_prev = *t;
        tok_prev = tok;
        tok = GetToken(t);
    }

    assert(t->at >= expr.str);
    expr.len = t->at - expr.str;
    return expr;
}


Str ParseBracketedParameterList(Tokenizer *t) {
    if (t->parse_error) return {};
    Tokenizer t_prev = *t;
    Token tok = GetToken(t);
    Str result = {};
    result.str = tok.text;

    // '('
    if (tok.type != TOK_LBRACK) {
        // fail: expected TOK_LBRACK!

        printf("\nERROR: Expected '(', got '%s'\n", TokenTypeToSymbol(tok.type));
        PrintLineError(t, &tok, "");
        HandleParseError(t);
    }

    else {
        while (tok.type != TOK_RBRACK) {
            Parameter p = {};

            Optional(t, &tok, TOK_IDENTIFIER);
            p.name = tok.GetValue();
            p.type = {};
            if (Optional(t, &tok, TOK_IDENTIFIER)) {
                p.type = p.name;
                p.default_val = tok.GetValue();
            }

            if (Optional(t, &tok, TOK_ASSIGN)) {

                // TODO: have a flag to turn on/off function calls 'bool allow_calls'
                p.default_val = ParseExpression(t);
                if (p.default_val.len == 0) {
                    // fail: default value must exist after the assignemnt operator

                    t_prev = *t;
                    tok = GetToken(t);

                    printf("\nERROR: Expected arithmetic expression, got '%s'\n", TokenTypeToSymbol(tok.type));
                    PrintLineError(t, &tok, "");
                    HandleParseError(t);

                    break;
                }
            }

            //
            assert(g_arena_parse_params && g_parse_params);
            ArenaAlloc(g_arena_parse_params, sizeof(Parameter));
            g_parse_params->max++;
            g_parse_params->Add(p);

            //
            if (t->parse_error) break;
            t_prev = *t;
            tok = GetToken(t);

            // ')'
            if (tok.type == TOK_RBRACK) {
                // ok, exit
                break;
            }

            // ','
            else if (p.name.len > 0 && tok.type == TOK_COMMA) {
                // ok, continue
            }

            // something else
            else {
                if (p.name.len == 0) {
                    // fail: expected expression

                    printf("\nERROR: Expected arithmetic expression, got '%s'\n", TokenTypeToSymbol(tok.type));
                    PrintLineError(t, &tok, "");
                    HandleParseError(t);

                    break;
                }

                else {
                    // fail: expected TOK_BRACK or TOK_COMMA

                    if (p.default_val.len) {
                        printf("\nERROR: Expected ',' or ')', got '%s'\n", TokenTypeToSymbol(tok.type));
                    }
                    else {
                        printf("\nERROR: Expected '=', ',' or ')', got '%s'\n", TokenTypeToSymbol(tok.type));
                    }
                    PrintLineError(t, &tok, "");
                    HandleParseError(t);

                    break;
                }
            }
        }
    }

    result.len = t->at - result.str;
    return result;
}


Array<Parameter> ParseParameterList(MArena *a_dest, Tokenizer *t) {
    Array<Parameter> params = InitArray<Parameter>(a_dest, 0);

    g_arena_parse_params = a_dest;
    g_parse_params = &params;

    ParseBracketedParameterList(t);

    g_arena_parse_params = NULL;
    g_parse_params = NULL;

    return params;
}


#endif
