#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>

#include "../lib/jg_baselayer.h"

#include "../src/parsecore.h"
#include "../src/parsehelpers.h"



Str ParseExpression_Rec(Tokenizer *t) {

    return {};
}

Str ParseCall_Rec(Tokenizer *t) {

    return {};
}

Str ParseList_Rec(Tokenizer *t) {

    return {};
}



const char *test_lines =
  "(yheight = 0.156, xwidth = 0.126, Lmin = lambda-ldiff/2, Lmax = lambda+ldiff/2)\n"
  "a + b / c\n"
  "-21 * 17\n"
  "func(\"strarg\")\n"
  "9.2 + floor(amp) * sin(phi)\n"
  ;


bool TokenInFilter(TokenType tpe, Array<TokenType> filter) {
    for (s32 i = 0; i < filter.len; ++i) {
        if (filter.arr[i] == tpe) {
            return true;
        }
    }

    return false;
}

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
        tokenizer.Init( StrZ(lines_split->GetStr()) );
        lines_split = lines_split->next;

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
