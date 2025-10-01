#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstddef>

#include "lib/jg_baselayer.h"


#include "src/parsecore.h"
#include "src/parsehelpers.h"


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

    TestParseParamsBlock();
}
