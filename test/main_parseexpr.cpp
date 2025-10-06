#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>

#include "../lib/jg_baselayer.h"

#include "../src/parsecore.h"
#include "../src/parsehelpers.h"


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
