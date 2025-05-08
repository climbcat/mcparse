#ifndef __PCOMP_H__
#define __PCOMP_H__


bool IsCompFile(char *filename)
{
    // check lenth / get expected location
    u32 len = strlen(filename);
    if (len < 7)
    {
        return false;
    }

    const char *match = ".comp";
    char cf;
    char cm;
    for (int i = 0; i < 6; i++)
    {
        cf = filename[i + len - 6];
        cm = match[i];
        if (cf != cm)
        {
            return false;
        }
    }

    return true;
}


// TODO: ParamDef
// TODO: ParamValue


struct CompDef
{
    char *name;
    char *type;
    Array<CompParam> params;
    DeclareDef declare;
    InitializeDef init;
    TraceDef trace;
    FinalizeDef finalize;
};



CompDef ParseComponent(Tokenizer *tokenizer, MArena *stack) {
    Token token;

    if (!RequireToken(tokenizer, &token, TOK_IDENTIFIER, "DEFINE"))
        exit(1);
    if (!RequireToken(tokenizer, &token, TOK_IDENTIFIER, "COMPONENT"))
        exit(1);
    if (!RequireToken(tokenizer, &token, TOK_IDENTIFIER))
        exit(1);

    // instr header
    CompDef comp = {};

    return comp;
}


#endif
