#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>

#include "../lib/jg_baselayer.h"

#include "../src/parsecore.h"
#include "../src/parsehelpers.h"


inline
bool TokenInFilter(TokenType tpe, Array<TokenType> filter) {
    for (s32 i = 0; i < filter.len; ++i) {
        if (filter.arr[i] == tpe) {
            return true;
        }
    }

    return false;
}
static TokenType _filter_operators[] = { TOK_PLUS, TOK_DASH, TOK_SLASH, TOK_ASTERISK };
static TokenType _filter_symbols[] = { TOK_IDENTIFIER, TOK_STRING, TOK_INT, TOK_FLOAT, TOK_SCI };
static TokenType _filter_seperator[] = { TOK_LBRACK, TOK_RBRACK, TOK_COMMA };
static Array<TokenType> g_filter_operators = { &_filter_operators[0], 4 };
static Array<TokenType> g_filter_symbols = { &_filter_symbols[0], 5 };
static Array<TokenType> g_filter_seperator = { &_filter_seperator[0], 3 };


Str ParseExpression(Tokenizer *t);


Str ParseBracketedExpressionList(Tokenizer *t) {
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
            Str expr = ParseExpression(t);
            if (t->parse_error) break;

            t_prev = *t;
            tok = GetToken(t);

            // ')'
            if (tok.type == TOK_RBRACK) {
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
        if (tok.type == TOK_LBRACK) {
            if (tok_prev.type == TOK_IDENTIFIER || tok_prev.type == TOK_UNKNOWN) {
                *t = t_prev;

                ParseBracketedExpressionList(t);
            }
            else {
                bracket_level++;
            }
        }

        else if (tok.type == TOK_RBRACK) {
            if (bracket_level > 0) {
                bracket_level--;
            }
            else {
                // rewind back to the before the )
                *t = t_prev;
                break;
            }
        }

        else if (TokenInFilter(tok.type, g_filter_symbols)) {
            // token is a symbol

            if ((tok_prev.type != TOK_UNKNOWN) && TokenInFilter(tok_prev.type, g_filter_symbols)) {
                // exit: two symbols in a row

                // DBG
                printf("ParseExpression: fail #1\n");

                *t = t_prev;
                break;
            }
        }

        else if (TokenInFilter(tok.type, g_filter_operators)) {
            // token is an operator

            if ((tok_prev.type != TOK_UNKNOWN) && TokenInFilter(tok_prev.type, g_filter_operators)) {
                if (tok_prev.type == TOK_LBRACK && (tok.type == TOK_DASH || tok.type == TOK_PLUS)) {
                    // the exception to the rule: (+ or (-
                }
                else {
                    // exit: two operators in a row

                    // DBG
                    printf("ParseExpression: fail #2\n");

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


const char *test_lines_expressions =
  "8e-4 * BigFunc(a + b / c, ceil(floor(-21   17), myarg), 9.2 + floor(amp) * sin(phi))\n"
  "-21  17\n"
  "func(643.123)\n"
  "func(643.123, 117)\n"
  "a, b/a, c + 17, -d * 1.3e-19, e\n"
  "(a, b/a, c + 17, -d * 1.3e-19, e)\n"
  "func(\"strarg\")\n"
  "a + b / c\n"
  "-21 * 17\n"
  "9.2 + floor(amp) * sin(phi)\n"
  "8e-4 * BigFunc(a + b / c, ceil(floor(-21 * 17), myarg), 9.2 + floor(amp) * sin(phi))\n"
  "8e-4 * BigFunc(a + b / c, ceil floor(-21 * 17), myarg), 9.2 + floor(amp) * sin(phi))\n"
  "8e-4 * BigFunc(a + b / c, ceil(floor(-21   17), myarg), 9.2 + floor(amp) * sin(phi))\n"
  ;

const char *test_lines_arguments =
  "(yheight = 0.156, xwidth = 0.126, Lmin = lambda-ldiff/2, Lmax = lambda+ldiff/2)\n"
  "(yheight = 0.156  xwidth = 0.126, Lmin = lambda-ldiff/2, Lmax = lambda+ldiff/2)\n"
  "(yheight   0.156, xwidth = 0.126, Lmin = lambda-ldiff/2, Lmax = lambda+ldiff/2)\n"
  ;


void ParseNestedExpressions() {
    MContext *ctx = InitBaselayer();
    Tokenizer tokenizer = {};
    Tokenizer *t = &tokenizer;

    Str lines = StrL(test_lines_expressions);
    StrLst *lines_split = StrSplit(lines, '\n');
    Str lines_args = StrL(test_lines_arguments);
    StrLst *lines_args_split = StrSplit(lines_args, '\n');


    while (lines_split != NULL) {
        printf("------------------------\n");
        StrPrint("input:      ", lines_split->GetStr(), "\n");
        tokenizer.Init( StrZ(lines_split->GetStr()) );

        printf("tokens:     ");
        Token tok = {};
        tok = GetToken(t);
        while (tok.type != TOK_ENDOFSTREAM) {

            if (TokenInFilter(tok.type, g_filter_operators)) {
                printf("p");
            }
            else if (TokenInFilter(tok.type, g_filter_symbols)) {
                printf("y");
            }
            else if (TokenInFilter(tok.type, g_filter_seperator)) {
                printf("e");
            }
            else {
                printf("R");
            }

            tok = GetToken(t);
        }
        printf("\n");

        tokenizer.Init( StrZ(lines_split->GetStr()) );

        Str expr = ParseExpression(t);
        StrPrint("expression: ", expr, "\n\n");

        lines_split = lines_split->next;
    }


   while (lines_args_split != NULL) {
        printf("------------------------\n");
        StrPrint("input:      ", lines_args_split->GetStr(), "\n");
        tokenizer.Init( StrZ(lines_args_split->GetStr()) );

        printf("tokens:     ");
        Token tok = {};
        tok = GetToken(t);
        while (tok.type != TOK_ENDOFSTREAM) {

            if (TokenInFilter(tok.type, g_filter_operators)) {
                printf("p");
            }
            else if (TokenInFilter(tok.type, g_filter_symbols)) {
                printf("y");
            }
            else if (TokenInFilter(tok.type, g_filter_seperator)) {
                printf("e");
            }
            else {
                printf("R");
            }

            tok = GetToken(t);
        }
        printf("\n");

        tokenizer.Init( StrZ(lines_args_split->GetStr()) );

        Str expr = ParseBracketedParameterList(t);
        StrPrint("parameters: ", expr, "\n\n");

        lines_args_split = lines_args_split->next;
    }
}


int main (int argc, char **argv) {
    TimeProgram;

    BaselayerAssertVersion(0, 2, 4);

    ParseNestedExpressions();
}
