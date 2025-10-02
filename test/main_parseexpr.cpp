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


Str ParseExpression(Tokenizer *t, Array<TokenType> operators, Array<TokenType> symbols);


Str ParseList(Tokenizer *t, Array<TokenType> operators, Array<TokenType> symbols) {
    // ... but not the brackets around it
    Token tok = {};

    Str result = ParseExpression(t, operators, symbols);
    Tokenizer t_prev = *t;

    if (result.len) {
        while (true) {
            tok = GetToken(t);
            if (tok.type != TOK_COMMA) {
                *t = t_prev;

                break;
            }
            else {
                Str expr = ParseExpression(t, operators, symbols);
                result.len += expr.len;
            }

            t_prev = *t;
        }
    }

    result.len = t->at - result.str;
    return result;
}


Str ParseCall(Tokenizer *t, Array<TokenType> operators, Array<TokenType> symbols) {
    // wrapper for ParseList

    Str result = {};

    Tokenizer t_prev = *t;
    Token tok = {};

    // fname
    tok = GetToken(t);
    if (tok.type == TOK_IDENTIFIER) {
        result = tok.GetValue();
    }
    else {
        *t = t_prev;

        return {};
    }

    // (
    tok = GetToken(t);
    if (tok.type != TOK_LBRACK) {
        *t = t_prev;

        return {};
    }

    // a list of nested expressions and calls
    ParseList(t, operators, symbols);

    // )
    tok = GetToken(t);
    if (tok.type != TOK_RBRACK) {
        *t = t_prev;

        return {};
    }

    result.len = t->at - result.str;
    return result;
}


Str ParseExpression(Tokenizer *t, Array<TokenType> operators, Array<TokenType> symbols) {
    s32 bracket_level = 0;

    // having access to the two previous tokenizer steps is convenient
    Tokenizer t_prev = *t;
    Tokenizer t_prev2 = *t;

    Token tok_prev = {};
    Token tok_prev2 = {};
    Token tok = {};
    tok = GetToken(t);

    Str expr = {};
    expr.str = tok.text;

    while (tok.type != TOK_ENDOFSTREAM) {
        if (tok.type == TOK_LBRACK) {
            if ((tok_prev.type == TOK_UNKNOWN) || TokenInFilter(tok_prev.type, operators)) {
                // check the bracket was preceeded by an operator

                bracket_level++;
            }
            else {
                *t = t_prev2;
                if (tok_prev.type == TOK_IDENTIFIER) {
                    ParseCall(t, operators, symbols);
                }
                else {
                    break;
                }
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

        else if (TokenInFilter(tok.type, symbols)) {
            // token is a symbol

            if ((tok_prev.type != TOK_UNKNOWN) && TokenInFilter(tok_prev.type, symbols)) {
                // two symbols in a row

                // DBG
                printf("fail; two successive symbols\n");

                *t = t_prev;
                break;
            }
        }

        else if (TokenInFilter(tok.type, operators)) {
            // token is an operator

            if ((tok_prev.type != TOK_UNKNOWN) && TokenInFilter(tok_prev.type, operators)) {
                if (tok_prev.type == TOK_LBRACK && (tok.type == TOK_DASH || tok.type == TOK_PLUS)) {
                    // the exception that is fine: (+ or (-
                }
                else {
                    // two operators in a row

                    // DBG
                    printf("fail: two successive operators\n");

                    *t = t_prev;
                    break;
                }
            }
        }

        else {
            // unknown token, return/exit

            // TODO: rewind tokenizer back to last save point
            *t = t_prev;
            break;
        }

        t_prev2 = t_prev;
        t_prev = *t;
        tok_prev2 = tok_prev;
        tok_prev = tok;
        tok = GetToken(t);
    }

    assert(t->at >= expr.str);
    expr.len = t->at - expr.str;
    return expr;
}



const char *test_lines =
  "func(643.123)\n"
  "a, b/a, c + 17, -d * 1.3e-19, e\n"
  "func(\"strarg\")\n"
  "(yheight = 0.156, xwidth = 0.126, Lmin = lambda-ldiff/2, Lmax = lambda+ldiff/2)\n"
  "a + b / c\n"
  "-21 * 17\n"
  "9.2 + floor(amp) * sin(phi)\n"
  ;


void ParseNestedExpressions() {
    MContext *ctx = InitBaselayer();
    Tokenizer tokenizer = {};
    Tokenizer *t = &tokenizer;

    Str lines = StrL(test_lines);
    StrLst *lines_split = StrSplit(lines, '\n');

    StrLstPrint(lines_split);
    printf("\n");


    TokenType _filter_operators[] = { TOK_PLUS, TOK_DASH, TOK_SLASH, TOK_ASTERISK };
    Array<TokenType> filter_operators = { &_filter_operators[0], 4};

    TokenType _filter_symbols[] = { TOK_IDENTIFIER, TOK_STRING, TOK_INT, TOK_FLOAT, TOK_SCI };
    Array<TokenType> filter_symbols = { &_filter_symbols[0], 5};

    TokenType _filter_seperator[] = { TOK_LBRACK, TOK_RBRACK, TOK_COMMA };
    Array<TokenType> filter_seperator = { &_filter_seperator[0], 3};


    while (lines_split != NULL) {
        StrPrint("", lines_split->GetStr(), "\n");
        tokenizer.Init( StrZ(lines_split->GetStr()) );

        Token tok = {};
        tok = GetToken(t);
        while (tok.type != TOK_ENDOFSTREAM) {

            if (TokenInFilter(tok.type, filter_operators)) {
                printf("p");
            }
            else if (TokenInFilter(tok.type, filter_symbols)) {
                printf("y");
            }
            else if (TokenInFilter(tok.type, filter_seperator)) {
                printf("e");
            }
            else {
                printf("R");
            }

            tok = GetToken(t);
        }
        printf("\n");

        tokenizer.Init( StrZ(lines_split->GetStr()) );
        Str expr = ParseList(t, filter_operators, filter_symbols);
        StrPrint("expression: ", expr, "\n");
        printf("\n");

        lines_split = lines_split->next;
    }
}


void TestParseParamsBlock() {
    printf("\nParseParamsBlock\n");

    MContext *ctx = InitBaselayer();
    Tokenizer tokenizer = {};

    const char* expr_01 = "(yheight = 0.156, xwidth = 0.126, Lmin = lambda-ldiff/2, Lmax = lambda+ldiff/2)";
    printf("---\n1) expecting error:\n    %s\n\n", expr_01);
    tokenizer.Init( (char*) expr_01);
    ParseParamsBlock(ctx->a_tmp, &tokenizer, false);

    const char* expr_02 = "(yheight = 0.156, xwidth = 0.126, Lmin = lambda-ldiff/2, Lmax = lambda+ldiff/2)";
    printf("---\n2) expecting pass:\n    %s\n\n", expr_02);
    tokenizer.Init( (char*) expr_02);
    ParseParamsBlock(ctx->a_tmp, &tokenizer, true);

    const char* expr_03 = "(yheight = 0.156 xwidth = 0.126, Lmin = lambda-ldiff/2, Lmax = lambda+ldiff/2)";
    printf("---\n3) expecting error:\n    %s\n\n", expr_03);
    tokenizer.Init( (char*) expr_03);
    ParseParamsBlock(ctx->a_tmp, &tokenizer, true);
}


int main (int argc, char **argv) {
    TimeProgram;

    BaselayerAssertVersion(0, 2, 4);

    //TestParseParamsBlock();
    ParseNestedExpressions();
}
