#ifndef __PARSECORE_H__
#define __PARSECORE_H__


#include <cstdlib>
#include <cstring>
#include <cstdio>

#include <cstdint>
#include <ctype.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>


enum TokenType {
    TOK_UNKNOWN, // catch-all for things that aren't defined yet

    TOK_LBRACK, // (
    TOK_RBRACK,
    TOK_LBRACE, // {
    TOK_RBRACE,
    TOK_LSBRACK, // [
    TOK_RSBRACK,
    TOK_LEDGE, // <
    TOK_REDGE,
    TOK_POUND, // #
    TOK_ASTERISK,
    TOK_COMMA,
    TOK_DOT,
    TOK_SLASH, // /
    TOK_DASH, // -
    TOK_PLUS,
    TOK_COLON,
    TOK_SEMICOLON,
    TOK_ASSIGN, // =
    TOK_EXCLAMATION,
    TOK_TILDE,
    TOK_OR, // |
    TOK_AND, // &
    TOK_PERCENT,
    TOK_RPERBRACE, // %{
    TOK_LPERBRACE, // %}

    TOK_CHAR,
    TOK_STRING,
    TOK_INT, // 123
    TOK_FLOAT, // 24.5748
    TOK_SCI, // 2.4e21
    TOK_IDENTIFIER,

    TOK_MCSTAS_PARTYPE_INT,
    TOK_MCSTAS_PARTYPE_STRING,

    TOK_MCSTAS_DEFINE,
    TOK_MCSTAS_INSTRUMENT,
    TOK_MCSTAS_COMPONENT,
    TOK_MCSTAS_SETTING,
    TOK_MCSTAS_PARAMETERS,
    TOK_MCSTAS_SHARE,
    TOK_MCSTAS_DECLARE,
    TOK_MCSTAS_INITIALIZE,
    TOK_MCSTAS_TRACE,
    TOK_MCSTAS_FINALLY,
    TOK_MCSTAS_MCDISPLAY,
    TOK_MCSTAS_AT,
    TOK_MCSTAS_RELATIVE,
    TOK_MCSTAS_ROTATED,
    TOK_MCSTAS_SPLIT,
    TOK_MCSTAS_SPLITS,
    TOK_MCSTAS_USER,
    TOK_MCSTAS_END,

    TOK_ENDOFSTREAM,
};

const char* TokenTypeToString(TokenType tpe) {
    switch (tpe)
    {
        case TOK_UNKNOWN: return "TOK_UNKNOWN";
        case TOK_LBRACK: return "TOK_LBRACK";
        case TOK_RBRACK: return "TOK_RBRACK";
        case TOK_LBRACE: return "TOK_LBRACE";
        case TOK_RBRACE: return "TOK_RBRACE";
        case TOK_LSBRACK: return "TOK_LSBRACK";
        case TOK_RSBRACK: return "TOK_RSBRACK";
        case TOK_LEDGE: return "TOK_LEDGE";
        case TOK_REDGE: return "TOK_REDGE";
        case TOK_POUND: return "TOK_POUND";
        case TOK_ASTERISK: return "TOK_ASTERISK";
        case TOK_COMMA: return "TOK_COMMA";
        case TOK_DOT: return "TOK_DOT";
        case TOK_SLASH: return "TOK_SLASH";
        case TOK_DASH: return "TOK_DASH";
        case TOK_PLUS: return "TOK_PLUS";
        case TOK_COLON: return "TOK_COLON";
        case TOK_SEMICOLON: return "TOK_SEMICOLON";
        case TOK_ASSIGN: return "TOK_ASSIGN";
        case TOK_EXCLAMATION: return "TOK_EXCLAMATION";
        case TOK_TILDE: return "TOK_TILDE";
        case TOK_OR: return "TOK_OR";
        case TOK_AND: return "TOK_AND";
        case TOK_PERCENT: return "TOK_PERCENT";
        case TOK_RPERBRACE: return "TOK_RPERBRACE";
        case TOK_LPERBRACE: return "TOK_LPERBRACE";

        case TOK_CHAR: return "TOK_CHAR";
        case TOK_STRING: return "TOK_STRING";
        case TOK_INT: return "TOK_INT";
        case TOK_FLOAT: return "TOK_FLOAT";
        case TOK_SCI: return "TOK_SCI";
        case TOK_IDENTIFIER: return "TOK_IDENTIFIER";

        case TOK_MCSTAS_PARTYPE_INT: return "TOK_MCSTAS_PARTYPE_INT";
        case TOK_MCSTAS_PARTYPE_STRING: return "TOK_MCSTAS_PARTYPE_STRINGs";

        case TOK_MCSTAS_DEFINE: return "TOK_MCSTAS_DEFINE";
        case TOK_MCSTAS_INSTRUMENT: return "TOK_MCSTAS_INSTRUMENT";
        case TOK_MCSTAS_COMPONENT: return "TOK_MCSTAS_COMPONENT";
        case TOK_MCSTAS_SETTING: return "TOK_MCSTAS_SETTING";
        case TOK_MCSTAS_PARAMETERS: return "TOK_MCSTAS_PARAMETERS";
        case TOK_MCSTAS_SHARE: return "TOK_MCSTAS_SHARE";
        case TOK_MCSTAS_DECLARE: return "TOK_MCSTAS_DECLARE";
        case TOK_MCSTAS_INITIALIZE: return "TOK_MCSTAS_INITIALIZE";
        case TOK_MCSTAS_TRACE: return "TOK_MCSTAS_TRACE";
        case TOK_MCSTAS_FINALLY: return "TOK_MCSTAS_FINALLY";
        case TOK_MCSTAS_MCDISPLAY: return "TOK_MCSTAS_MCDISPLAY";
        case TOK_MCSTAS_AT: return "TOK_MCSTAS_AT";
        case TOK_MCSTAS_RELATIVE: return "TOK_MCSTAS_RELATIVE";
        case TOK_MCSTAS_ROTATED: return "TOK_MCSTAS_ROTATED";
        case TOK_MCSTAS_SPLIT: return "TOK_MCSTAS_SPLIT";
        case TOK_MCSTAS_SPLITS: return "TOK_MCSTAS_SPLITS";
        case TOK_MCSTAS_USER: return "TOK_MCSTAS_USER";
        case TOK_MCSTAS_END: return "TOK_MCSTAS_END";

        case TOK_ENDOFSTREAM: return "TOK_ENDOFSTREAM";

        default: return "ReturnTokenTypeString__default";
    }
}

const char* TokenTypeToSymbol(TokenType tpe) {
    switch (tpe)
    {
        case TOK_UNKNOWN: return "(unknown)";
        case TOK_LBRACK: return "(";
        case TOK_RBRACK: return ")";
        case TOK_LBRACE: return "{";
        case TOK_RBRACE: return "}";
        case TOK_LSBRACK: return "[";
        case TOK_RSBRACK: return "]";
        case TOK_LEDGE: return "<";
        case TOK_REDGE: return ">";
        case TOK_POUND: return "#";
        case TOK_ASTERISK: return "*";
        case TOK_COMMA: return ",";
        case TOK_DOT: return ".";
        case TOK_SLASH: return "/";
        case TOK_DASH: return "-";
        case TOK_PLUS: return "+";
        case TOK_COLON: return ":";
        case TOK_SEMICOLON: return ";";
        case TOK_ASSIGN: return "=";
        case TOK_EXCLAMATION: return "!";
        case TOK_TILDE: return "~";
        case TOK_OR: return "|";
        case TOK_AND: return "&";
        case TOK_PERCENT: return "%";
        case TOK_RPERBRACE: return "%{";
        case TOK_LPERBRACE: return "%}";

        case TOK_CHAR: return "char";
        case TOK_STRING: return "string";
        case TOK_INT: return "integer";
        case TOK_FLOAT: return "float";
        case TOK_SCI: return "float scientific";
        case TOK_IDENTIFIER: return "identifier";

        case TOK_MCSTAS_DEFINE: return "DEFINE";
        case TOK_MCSTAS_INSTRUMENT: return "INSTRUMENT";
        case TOK_MCSTAS_COMPONENT: return "COMPONENT";
        case TOK_MCSTAS_SETTING: return "SETTING";
        case TOK_MCSTAS_PARAMETERS: return "PARAMETERS";
        case TOK_MCSTAS_SHARE: return "SHARE";
        case TOK_MCSTAS_DECLARE: return "DECLARE";
        case TOK_MCSTAS_INITIALIZE: return "INITIALIZE";
        case TOK_MCSTAS_TRACE: return "TRACE";
        case TOK_MCSTAS_FINALLY: return "FINALLY";
        case TOK_MCSTAS_MCDISPLAY: return "MCDISPLAY";
        case TOK_MCSTAS_AT: return "AT";
        case TOK_MCSTAS_RELATIVE: return "RELATIVE";
        case TOK_MCSTAS_ROTATED: return "ROTATED";
        case TOK_MCSTAS_SPLIT: return "SPLIT";
        case TOK_MCSTAS_SPLITS: return "SPLITS";
        case TOK_MCSTAS_USER: return "USER";
        case TOK_MCSTAS_END: return "END";

        case TOK_ENDOFSTREAM: return "[eos]";

        default: return "ReturnTokenTypeSymbol__default";
    }
}

void TokenTypePrint(TokenType tpe, bool newline = true) {
    printf("%s", TokenTypeToString(tpe));
    if (newline) {
        printf("\n");
    }
}

struct Tokenizer {
    char *at;
    u32 line = 1;
    char *at_linestart;

    void Init(char *text) {
        line = 1;
        at = text;
        at_linestart = text;
    }
    void AtNewLineChar() {
        ++line;
        at_linestart = at + 1;
    }
};

struct Token {
    TokenType type;
    bool is_rval;
    char* text;
    u32 len;

    void PrintValue(bool newline = true) {
        printf("%.*s", len, text);
        if (newline) {
            printf("\n");
        }
    }
    Str GetValue() {
        return Str { text, len };
    }
};

inline
bool IsEndOfLine(char c) {
    return
        c == '\n' ||
        c == '\r';
}

inline
bool IsWhitespace(char c) {
    return
        c == ' ' ||
        c == '\t' ||
        c == '\v' ||
        c == '\f' ||
        IsEndOfLine(c);
}

inline
bool IsNumeric(char c) {
    bool isnum = (c >= '0') && (c <= '9');
    return isnum;
}

inline
bool IsNumericSymbol(char c) {
    bool issymb = ((c == '.') || (c == 'e') || (c == 'E') || (c == '-') || (c == '+'));
    return issymb;
}

inline
bool IsSciSymbol(char c) {
    bool issymb = ((c == 'e') || (c == 'E'));
    return issymb;
}

inline
bool IsAlphaOrUnderscore(char c) {
    bool result =
        (c == '_') || ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z'));
    return result;
}

void EatCppStyleComment(Tokenizer* tokenizer) {
    for(;;) {
        if (tokenizer->at[0] == '\0')
            return;

        if (tokenizer->at[0] == '\n') {
            tokenizer->AtNewLineChar();
            ++tokenizer->at;
            return;
        }

        ++tokenizer->at;
    }
}

void EatCStyleComment(Tokenizer* tokenizer) {
    tokenizer->at += 2; // assuming at = "/* ... */"
    for(;;) {
        if (tokenizer->at[0] == '\0') {
            return;
        }

        if (tokenizer->at[0] == '\n') {
            tokenizer->AtNewLineChar();
        }

        if (tokenizer->at[1] == '\0') {
            ++tokenizer->at;
            return;
        }

        if (tokenizer->at[0] == '*' && tokenizer->at[1] && tokenizer->at[1] == '/') {
            tokenizer->at += 2;
            return;
        }

        ++tokenizer->at;
    }
}

void EatWhiteSpacesAndComments(Tokenizer* tokenizer) {
    for (;;) {
        if (tokenizer->at[0] == '\0') {
            return;
        }

        if (IsEndOfLine(tokenizer->at[0])) {
            tokenizer->AtNewLineChar();
        }

        if (IsWhitespace(tokenizer->at[0])) {
            ++tokenizer->at;
        }
        else if (tokenizer->at[0] == '/') {
            if (tokenizer->at[1]) {
                if (tokenizer->at[1] == '/') { // C++-style comment
                    EatCppStyleComment(tokenizer);
                }
                else if (tokenizer->at[1] == '*') { // C-style comment
                    EatCStyleComment(tokenizer);
                }
                else {
                    // sometimes, a slash is just a slash
                    return;
                }
            }
            else {
                // end of stream
                ++tokenizer->at;
                return;
            }
        }
        else if (tokenizer->at[0] == '#') { // pythonic | matlabish comment
            EatCppStyleComment(tokenizer);
        }
        else {
            break;
        }
    }
}

inline
bool TokenEquals(Token* token, const char* match, bool token_to_upper = false) {
    char* at = (char*) match;
    if (token->len != strlen(match)) {
        return false;
    }
    char ctok;
    char cmatch;
    for (int idx = 0; idx < token->len; idx++, at++) {
        ctok = token->text[idx];
        cmatch = *at;
        if (token_to_upper) {
            ctok = toupper(ctok);
            cmatch = toupper(cmatch);
        }
        if (ctok != cmatch) {
            return false;
        }
    }
    bool result = (*at == 0);
    return true;
}

inline
u32 Trim(char **src, u32 maxlen) { // trim whitespaces, sets *src = start and returns len (without null termination)
    u32 idx = 0;
    char *at = *src;
    while ( IsWhitespace(*at) ) {
        ++at;
        ++idx;
    }

    *src = at;
    while ( !IsWhitespace(*at) && idx < maxlen) {
        ++at;
        ++idx;
    }
    return at - *src;
}

inline
void LTrim(char **src) { // sets *src to first non-whitespace char
    u32 idx = 0;
    char *at = *src;
    while ( IsWhitespace(*at) ) {
        ++at;
        ++idx;
    }
    *src = at;
}

inline
u32 RTrimText(char *src, u32 textlen) { // returns len corresponding to last non-whitespace char
    char *at = src + textlen - 1;
    u32 steps_back = 0;
    while (at > src && IsWhitespace(*at)) {
        --at;
        ++steps_back;
    }
    return textlen - steps_back;
}

inline
int FindChar(char *text, char c) {
    u32 dist = 0;
    while (true) {
        if (*text == '\0')
            return -1;
        if (c == *text)
            return dist;
        ++dist;
        ++text;
    }
}

inline
u32 DistEndOfLine(char* text) {
    u32 dist = 0;
    while (true) {
        if ('\0' == *text || IsEndOfLine(*text))
            return dist;
        ++dist;
        ++text;
    }
}

void PrintLineError(Tokenizer *tokenizer, Token *token, const char* errmsg = NULL) {
    char* msg = (char*) errmsg;
    if (errmsg == NULL) {
        msg = (char*) "Error.";
    }

    u32 toklen = 0;
    if (token != NULL) {
        toklen = token->len;
    }

    // print message
    printf("%s\n", msg);

    // print line nb. tag
    char lineno_tag[200];
    sprintf(lineno_tag, "%d| ", tokenizer->line);
    printf("%s", lineno_tag);

    // print line
    printf("%.*s\n", DistEndOfLine(tokenizer->at_linestart), tokenizer->at_linestart);

    // print marker
    u32 mark = (tokenizer->at - toklen) - tokenizer->at_linestart + strlen(lineno_tag);
    for (u32 i = 0; i < mark; i++) {
        printf(" ");
    }
    printf("^\n");
}

Token GetToken(Tokenizer *tokenizer);

TokenType LookAheadNextTokenType(Tokenizer *tokenizer) {
    Tokenizer save = *tokenizer;
    Token token = GetToken(tokenizer);
    *tokenizer = save;
    return token.type;
}

s32 ParseGetWordLen(char *at) {
    if (at == NULL) {
        return 0;
    }

    s32 result = 0;
    char c = *at;
    while (true) {
        if (c && c != ' ' && (IsAlphaOrUnderscore(c) || IsNumeric(c) || IsNumericSymbol(c))) {
            result++;
            at ++;
            c = *at;
        }
        else {
            break;
        }
    }
    return result;
}

void ParseNumeric(Tokenizer *tokenizer, Token *token)
{
    s32 len_was = token->len;
    token->len = ParseGetWordLen(token->text);
    tokenizer->at += token->len - len_was;

    bool has_dot = false;
    bool has_sci = false;
    bool has_err = false;

    for (s32 i = 0; i < token->len; ++i) {
        char c = token->text[i];

        if (IsNumeric(c)) {
            continue;
        }
        else if (IsSciSymbol(c)) {
            has_sci;
            break;
        }
        else if (c == '.') {
            has_dot = true;
        }
        else if (!IsNumericSymbol(c)) {
            has_err = true;
            break;
        }
    }

    if (has_err) {
        token->type = TOK_UNKNOWN;
    }
    else if (has_sci) {
        token->type = TOK_SCI;
    }
    else if (has_dot) {
        token->type = TOK_FLOAT;
    }
    else {
        token->type = TOK_INT;
    }
}

Token GetToken(Tokenizer *tokenizer)
{
    EatWhiteSpacesAndComments(tokenizer);

    Token token = {};
    token.text = tokenizer->at;
    token.len = 1;

    char c = tokenizer->at[0];
    ++tokenizer->at;
    switch (c)
    {

    case '\0':
        token.type = TOK_ENDOFSTREAM;
        break;
    case '(':
        token.type = TOK_LBRACK;
        break;
    case ')':
        token.type = TOK_RBRACK;
        break;
    case '{':
        token.type = TOK_LBRACE;
        break;
    case '}':
        token.type = TOK_RBRACE;
        break;
    case '[':
        token.type = TOK_LSBRACK;
        break;
    case ']':
        token.type = TOK_RSBRACK;
        break;
    case '<':
        token.type = TOK_LEDGE;
        break;
    case '>':
        token.type = TOK_REDGE;
        break;
    case '#':
        token.type = TOK_POUND;
        break;
    case '*':
        token.type = TOK_ASTERISK;
        break;
    case ',':
        token.type = TOK_COMMA;
        break;

    case '.':
    {
        TokenType next_type = LookAheadNextTokenType(tokenizer);
        if (next_type == TOK_INT)
        {
            ParseNumeric(tokenizer, &token);
            token.is_rval = true;
        }
        else
        {
            token.type = TOK_DOT;
        }

    }
    break;

    case '/':
        token.type = TOK_SLASH;
        break;

    case '-':
    {
        TokenType next_type = LookAheadNextTokenType(tokenizer);
        if (next_type == TOK_INT || next_type == TOK_FLOAT || next_type == TOK_SCI)
        {
            ParseNumeric(tokenizer, &token);
            token.is_rval = true;
        }
        else
        {
            token.type = TOK_DASH;
        }
    }
    break;
    case '+':
    {
        TokenType next_type = LookAheadNextTokenType(tokenizer);
        if (next_type == TOK_INT || next_type == TOK_FLOAT || next_type == TOK_SCI)
        {
            ParseNumeric(tokenizer, &token);
            token.is_rval = true;
        }
        else
        {
            token.type = TOK_PLUS;
        }
    }
    break;

    case ':':
        token.type = TOK_COLON;
        break;
    case ';':
        token.type = TOK_SEMICOLON;
        break;
    case '=':
        token.type = TOK_ASSIGN;
        break;
    case '!':
        token.type = TOK_EXCLAMATION;
        break;
    case '~':
        token.type = TOK_TILDE;
        break;
    case '|':
        token.type = TOK_OR;
        break;
    case '&':
        token.type = TOK_AND;
        break;

    case '%':
    {
        if (tokenizer->at[0] && tokenizer->at[0] == '{')
        {
            token.type = TOK_LPERBRACE;
            token.len = 2;
            ++tokenizer->at;
        }
        else if (tokenizer->at[0] && tokenizer->at[0] == '}')
        {
            token.type = TOK_RPERBRACE;
            token.len = 2;
            ++tokenizer->at;
        }
        else
        {
            token.type = TOK_PERCENT;
            break;
        }
    }
    break;

    case '"':
    {
        token.type = TOK_STRING;
        token.is_rval = true;

        while (tokenizer->at[0] != '\0' && tokenizer->at[0] != '"' && !IsEndOfLine(tokenizer->at[0]))
        {
            if (tokenizer->at[0] == '\\' && tokenizer->at[1])
            {
                ++tokenizer->at;
            }
            ++tokenizer->at;
        }

        if (tokenizer->at[0] == '"')
        {
            ++tokenizer->at;
        }

        token.len = tokenizer->at - token.text;
    }
    break;

    case '\'':
    {
        token.type = TOK_CHAR;
        token.is_rval = true;

        while (tokenizer->at[0] != '\0' && tokenizer->at[0] != '\'' && !IsEndOfLine(tokenizer->at[0]))
        {
            if (tokenizer->at[0] == '\\' && tokenizer->at[1])
            {
                ++tokenizer->at;
            }
            ++tokenizer->at;
        }

        if (tokenizer->at[0] == '\'')
        {
            ++tokenizer->at;
        }

        token.len = tokenizer->at - token.text;
    }
    break;

    default:
    {
        if (IsAlphaOrUnderscore(c))
        {
            token.type = TOK_IDENTIFIER;
            token.is_rval = true;

            while ( tokenizer->at[0] != '\0' && (IsAlphaOrUnderscore(tokenizer->at[0]) || IsNumeric(tokenizer->at[0]))) {
                ++tokenizer->at;
            }
            token.len = tokenizer->at - token.text;

            if (TokenEquals(&token, "DEFINE")) { token.type = TOK_MCSTAS_DEFINE; }
            else if (TokenEquals(&token, "INSTRUMENT")) { token.type = TOK_MCSTAS_INSTRUMENT; }
            else if (TokenEquals(&token, "COMPONENT")) { token.type = TOK_MCSTAS_COMPONENT; }
            else if (TokenEquals(&token, "SETTING")) { token.type = TOK_MCSTAS_SETTING; }
            else if (TokenEquals(&token, "PARAMETERS")) { token.type = TOK_MCSTAS_PARAMETERS; }
            else if (TokenEquals(&token, "SHARE")) { token.type = TOK_MCSTAS_SHARE; }
            else if (TokenEquals(&token, "DECLARE")) { token.type = TOK_MCSTAS_DECLARE; }
            else if (TokenEquals(&token, "INITIALIZE")) { token.type = TOK_MCSTAS_INITIALIZE; }
            else if (TokenEquals(&token, "TRACE")) { token.type = TOK_MCSTAS_TRACE; }
            else if (TokenEquals(&token, "FINALLY")) { token.type = TOK_MCSTAS_FINALLY; }
            else if (TokenEquals(&token, "MCDISPLAY")) { token.type = TOK_MCSTAS_MCDISPLAY; }
            else if (TokenEquals(&token, "AT")) { token.type = TOK_MCSTAS_AT; }
            else if (TokenEquals(&token, "RELATIVE")) { token.type = TOK_MCSTAS_RELATIVE; }
            else if (TokenEquals(&token, "ROTATED")) { token.type = TOK_MCSTAS_ROTATED; }
            else if (TokenEquals(&token, "SPLIT")) { token.type = TOK_MCSTAS_SPLIT; }
            else if (TokenEquals(&token, "SPLITS")) { token.type = TOK_MCSTAS_SPLITS; }
            else if (TokenEquals(&token, "USER")) { token.type = TOK_MCSTAS_USER; }
            else if (TokenEquals(&token, "END")) { token.type = TOK_MCSTAS_END; }
        }

        else if (IsNumeric(c))
        {
            token.is_rval = true;

            ParseNumeric(tokenizer, &token);
        }

        else
        {
            token.type = TOK_UNKNOWN;
        }
    }
    }
    return token;
}

#endif
