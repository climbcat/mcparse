#ifndef __PHELPERS_H__
#define __PHELPERS_H__




u32 LookAheadTokenCountOR(Tokenizer *tokenizer, TokenType desired_type, TokenType desired_type_or = TOK_ENDOFSTREAM)
{
    Tokenizer save = *tokenizer;

    u32 steps = 0;
    Token token;
    while (true)
    {
        ++steps;
        token = GetToken(tokenizer);
        if (token.type == TOK_ENDOFSTREAM)
        {
            return 0;
        }
        else if (token.type == desired_type)
        {
            break;
        }
        else if (token.type == desired_type_or)
        {
            break;
        }
    }

    *tokenizer = save;
    return steps;
}

void BasicParsingLoopSwitch(Tokenizer *tokenizer)
{
    // template loop / switch
    bool parsing = true;
    while (parsing)
    {
        Token token = GetToken(tokenizer);

        switch (token.type)
        {
        case TOK_ENDOFSTREAM:
        {
            parsing = false;
        }
        break;

        default:
        {
        }
        break;
        }
    }
}

u32 LookAheadTokenCountNOT(Tokenizer *tokenizer, TokenType desired_type, TokenType avoid_type = TOK_ENDOFSTREAM)
{
    Tokenizer save = *tokenizer;

    u32 steps = 0;
    Token token;
    while (true)
    {
        ++steps;
        token = GetToken(tokenizer);
        if (token.type == TOK_ENDOFSTREAM || token.type == avoid_type)
        {
            return 0;
        }
        if (token.type == desired_type)
        {
            break;
        }
    }

    *tokenizer = save;
    return steps;
}

inline u32 LookAheadTokenCount(Tokenizer *tokenizer, TokenType desired_type)
{
    return LookAheadTokenCountOR(tokenizer, desired_type);
}

inline u32 LookAheadLenChars(char *at, char until)
{
    char *start = at;
    while (*at != until && *at != '\0')
    {
        ++at;
    }
    return at - start;
}

inline u32 LookAheadLenEoL(char *at)
{
    char *start = at;
    while (*at != '\0' && !IsEndOfLine(*at))
    {
        ++at;
    }
    return at - start;
}

inline u32 LookAheadLenCharsFailAtEolAndEos(char *at, char until)
{
    char *start = at;
    while (*at != until && *at != '\0' && !IsEndOfLine(*at))
    {
        ++at;
    }
    return at - start;
}

u32 LookAheadLenUntilToken(Tokenizer *tokenizer, TokenType desired_type, bool failat_eol = false)
{
    Tokenizer save = *tokenizer;

    Token token;
    while (true)
    {
        token = GetToken(tokenizer);
        if (token.type == TOK_ENDOFSTREAM)
        {
            return 0;
        }
        else if (token.type == desired_type)
        {
            break;
        }
    }

    u32 result = tokenizer->at - token.len - save.at; // TODO: use rewind feature !!
    u32 result_eol = LookAheadLenEoL(save.at);
    *tokenizer = save;

    if (failat_eol && result_eol < result)
    {
        return 0;
    }

    return result;
}

bool LookAheadNextToken(Tokenizer *tokenizer, TokenType desired_type, const char *desired_value = NULL)
{
    Tokenizer save = *tokenizer;
    Token token = GetToken(tokenizer);
    bool result = 0;

    if (token.type == TOK_ENDOFSTREAM)
    {
        result = 0;
    }
    else if (token.type == desired_type)
    {
        result = 1;
        if (desired_value != NULL && !TokenEquals(&token, desired_value))
        {
            result = 0;
        }
    }
    *tokenizer = save;
    return result;
}

void IncTokenizerUntilAtToken(Tokenizer *tokenizer, TokenType until_type)
{
    while (true)
    {
        if (LookAheadNextToken(tokenizer, until_type))
        {
            return;
        }
        if (LookAheadNextToken(tokenizer, TOK_ENDOFSTREAM))
        {
            return;
        }
        GetToken(tokenizer);
    }
}

// will inc tokenizer ONLY IF token requirement is satisfied
bool RequireToken(Tokenizer *tokenizer, Token *token_dest, TokenType desired_type, const char *desired_value = NULL, bool print_error = true)
{
    Tokenizer resume = *tokenizer;

    Token token = GetToken(tokenizer);
    if (token_dest != NULL)
    {
        *token_dest = token;
    }
    bool result = token.type == desired_type;
    if (desired_value != NULL)
    {
        result = result && TokenEquals(&token, desired_value, true);
    }

    if (result == false)
    {
        if (print_error)
        {
            char msg[200];
            if (desired_value == NULL)
            {
                sprintf(msg, "Expected %s", TokenTypeToString(desired_type));
            }
            else
            {
                sprintf(msg, "Expected %s", desired_value);
            }
            PrintLineError(tokenizer, &token, msg);
        }

        *tokenizer = resume;
    }
    return result;
}

typedef char **StringList;

inline void AllocTokenValue(char **dest, Token *token, MArena *stack)
{
    *dest = (char *) ArenaAlloc(stack, token->len + 1);
    strncpy(*dest, token->text, token->len);
    (*dest)[token->len] = '\0';
}

inline void AllocTokenValueAssertType(char **dest, Token *token, TokenType type_assert, MArena *stack)
{
    assert(token->type == type_assert);
    AllocTokenValue(dest, token, stack);
}

void AllocString(char **dest, const char *string, MArena *stack)
{
    *dest = (char *) ArenaAlloc(stack, strlen(string) + 1);
    strcpy(*dest, string);
}

void AllocStringField(char **dest, Token *token, MArena *stack)
{
    *dest = (char *) ArenaAlloc(stack, token->len - 1);
    memcpy(*dest, token->text + 1, token->len - 2);
    (*dest)[token->len - 2] = '\0';
}

void AllocIdentifierField(char **dest, Token *token, MArena *stack)
{
    *dest = (char *) ArenaAlloc(stack, token->len + 1);
    memcpy(*dest, token->text, token->len);
    (*dest)[token->len] = '\0';
}

u32 MinU(u32 a, u32 b)
{
    if (a <= b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

u32 MaxU(u32 a, u32 b)
{
    if (a >= b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

u32 CountNumCharsInText(char *text, char match)
{
    u32 count = 0;
    while (*text != '\0')
    {
        if (*text == match)
        {
            ++count;
        }
        ++text;
    }
    return count;
}

u32 CountTokenSeparatedStuff(char *text, TokenType tok_separator, TokenType tok_exit = TOK_ENDOFSTREAM, TokenType tok_enter = TOK_UNKNOWN, TokenType tok_separator_alt = TOK_UNKNOWN)
{
    Tokenizer tokenizer;
    tokenizer.Init(text);

    if (tok_enter != TOK_UNKNOWN)
    {
        if (!RequireToken(&tokenizer, NULL, tok_enter))
            return 0;
    }

    bool flag = true;
    u32 count = 0;
    bool parsing = true;
    while (parsing)
    {
        Token token = GetToken(&tokenizer);

        if (token.type == TOK_ENDOFSTREAM)
        {
            parsing = false;
        }
        else if (token.type == tok_separator)
        {
            flag = true;
        }
        else if (token.type == tok_separator_alt && TOK_UNKNOWN != tok_separator_alt)
        {
            flag = true;
        }
        else if (token.type == tok_exit)
        {
            parsing = false;
        }
        else
        {
            if (flag == true)
            {
                ++count;
                flag = false;
            }
        }
    }

    return count;
}

bool ParseExpression(Tokenizer *tokenizer, Token *token, const char *end_identifier = NULL)
{
    bool result = false;
    u32 bracket_level = 0;
    u32 brace_level = 0;
    u32 sbrack_level = 0;

    EatWhiteSpacesAndComments(tokenizer);
    token->text = tokenizer->at;

    bool parsing = true;
    while (parsing)
    {
        Token token = GetToken(tokenizer);
        switch (token.type)
        {
        case TOK_ENDOFSTREAM:
        {
            parsing = false;
        }
        break;

        case TOK_LBRACK:
        {
            ++bracket_level;
        }
        break;

        case TOK_RBRACK:
        {
            if (bracket_level > 0)
            {
                --bracket_level;
            }
            else
            {
                parsing = false;
            }
        }
        break;

        case TOK_LBRACE:
        {
            ++brace_level;
        }
        break;

        case TOK_RBRACE:
        {
            if (brace_level > 0)
            {
                --brace_level;
            }
        }
        break;

        case TOK_LSBRACK:
        {
            ++sbrack_level;
        }
        break;

        case TOK_RSBRACK:
        {
            if (sbrack_level > 0)
            {
                --sbrack_level;
            }
        }
        break;

        case TOK_SEMICOLON:
        {
            if (bracket_level == 0 && brace_level == 0 && sbrack_level == 0)
            {
                parsing = false;
            }
        }
        break;

        case TOK_COMMA:
        {
            if (bracket_level == 0 && brace_level == 0 && sbrack_level == 0)
            {
                parsing = false;
            }
        }
        break;

        case TOK_IDENTIFIER:
        {
            result = true;

            if (end_identifier != NULL && TokenEquals(&token, end_identifier))
            {
                tokenizer->at -= token.len;
                parsing = false;
            }
        }
        break;

        default:
        {
            result = true;
        }
        break;
        }
    }

    // exit conditions are: EOS, comma, semicolon or rbracket, all of which are 1 char long
    --tokenizer->at;

    token->len = tokenizer->at - token->text;
    token->len = RTrimText(token->text, token->len);
    return result;
}

u32 CountCommaSeparatedSequenceOfExpresions(char *text)
{
    Token token;
    Tokenizer tokenizer;
    tokenizer.Init(text);

    u32 count = 0;
    bool flag = false;
    bool parsing = true;
    while (parsing)
    {
        if (ParseExpression(&tokenizer, &token))
        {
            ++count;
        }

        if (!RequireToken(&tokenizer, &token, TOK_COMMA, NULL, false))
        {
            parsing = false;
        }
    }
    return count;
}

void ParseAllocCommaSeparatedListOfStrings(StringList *lst, Tokenizer *tokenizer, MArena *stack)
{
    Tokenizer save = *tokenizer;

    u32 list_len = 0;
    bool counting = true;
    while (counting)
    {
        Token token = GetToken(tokenizer);

        switch (token.type)
        {
        case TOK_COMMA:
        {
        }
        break;

        case TOK_IDENTIFIER:
        {
            ++list_len;
        }
        break;

        case TOK_STRING:
        {
            ++list_len;
        }
        break;

        case TOK_INT:
        {
            assert(1 == 0);
        }
        break;

        case TOK_DOT:
        {
            // TODO: extend the tokenizer - this could be a float
            assert(1 == 0);
        }
        break;

        default:
        {
            counting = false;
        }
        break;
        }
    }
    *lst = (StringList) ArenaAlloc(stack, list_len * sizeof(StringList));

    // reset
    u32 idx = 0;
    *tokenizer = save;
    while (idx < list_len)
    {
        Token token = GetToken(tokenizer);

        switch (token.type)
        {
        case TOK_COMMA:
        {
        }
        break;

        case TOK_IDENTIFIER:
        {
            AllocIdentifierField(&(*lst)[idx], &token, stack);

            ++idx;
        }
        break;

        case TOK_STRING:
        {
            AllocStringField(&(*lst)[idx], &token, stack);

            ++idx;
        }
        break;

        default:
        {
            counting = false;
        }
        break;
        }
    }
}

#endif
