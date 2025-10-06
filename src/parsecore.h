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
    TOK_RBRACK, // )
    TOK_LBRACE, // {
    TOK_RBRACE, // }
    TOK_LSBRACK, // [
    TOK_RSBRACK, // ]
    TOK_LEDGE, // <
    TOK_REDGE, // >
    TOK_POUND, // #
    TOK_ASTERISK, // *
    TOK_COMMA, // ,
    TOK_DOT, // .
    TOK_SLASH, // /
    TOK_DASH, // -
    TOK_PLUS, // +
    TOK_COLON, // :
    TOK_SEMICOLON, // ;
    TOK_ASSIGN, // =
    TOK_EXCLAMATION, // !
    TOK_QUESTION, // ?
    TOK_TILDE, // ~
    TOK_OR, // |
    TOK_OR_COMPARE, // ||
    TOK_AND, // &
    TOK_AND_COMPARE, // &&
    TOK_PERCENT, // %
    TOK_RPERCENTBRACE, // %{
    TOK_LPERCENTBRACE, // %}

    TOK_CHAR,
    TOK_STRING,
    TOK_INT, // 123
    TOK_FLOAT, // 24.5748
    TOK_SCI, // 2.4e21
    TOK_IDENTIFIER,

    TOK_NULL,

    TOK_MCSTAS_DEFINE,
    TOK_MCSTAS_INSTRUMENT,
    TOK_MCSTAS_COMPONENT,
    TOK_MCSTAS_COPY,
    TOK_MCSTAS_EXTEND,
    TOK_MCSTAS_SETTING,
    TOK_MCSTAS_OUTPUT,
    TOK_MCSTAS_STATE,
    TOK_MCSTAS_POLARISATION,
    TOK_MCSTAS_PARAMETERS,
    TOK_MCSTAS_SHARE,
    TOK_MCSTAS_USERVARS,
    TOK_MCSTAS_DECLARE,
    TOK_MCSTAS_INITIALIZE,
    TOK_MCSTAS_TRACE,
    TOK_MCSTAS_SAVE,
    TOK_MCSTAS_FINALLY,
    TOK_MCSTAS_MCDISPLAY,
    TOK_MCSTAS_AT,
    TOK_MCSTAS_RELATIVE,
    TOK_MCSTAS_ABSOLUTE,
    TOK_MCSTAS_PREVIOUS,
    TOK_MCSTAS_ROTATED,
    TOK_MCSTAS_SPLIT,
    TOK_MCSTAS_REMOVABLE,
    TOK_MCSTAS_USER,
    TOK_MCSTAS_WHEN,
    TOK_MCSTAS_JUMP,
    TOK_MCSTAS_GROUP,
    TOK_MCSTAS_END,
    TOK_MCSTAS_PINCLUDE,

    TOK_MCSTAS_C_EXPRESSION,

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
        case TOK_QUESTION: return "TOK_QUESTION";
        case TOK_TILDE: return "TOK_TILDE";
        case TOK_OR: return "TOK_OR";
        case TOK_OR_COMPARE: return "TOK_OR_COMPARE";
        case TOK_AND: return "TOK_AND";
        case TOK_AND_COMPARE: return "TOK_AND_COMPARE";
        case TOK_PERCENT: return "TOK_PERCENT";
        case TOK_RPERCENTBRACE: return "TOK_RPERBRACE";
        case TOK_LPERCENTBRACE: return "TOK_LPERBRACE";

        case TOK_CHAR: return "TOK_CHAR";
        case TOK_STRING: return "TOK_STRING";
        case TOK_INT: return "TOK_INT";
        case TOK_FLOAT: return "TOK_FLOAT";
        case TOK_SCI: return "TOK_SCI";
        case TOK_IDENTIFIER: return "TOK_IDENTIFIER";

        case TOK_NULL: return "TOK_NULL";

        case TOK_MCSTAS_DEFINE: return "TOK_MCSTAS_DEFINE";
        case TOK_MCSTAS_INSTRUMENT: return "TOK_MCSTAS_INSTRUMENT";
        case TOK_MCSTAS_COMPONENT: return "TOK_MCSTAS_COMPONENT";
        case TOK_MCSTAS_COPY: return "TOK_MCSTAS_COPY";
        case TOK_MCSTAS_EXTEND: return "TOK_MCSTAS_EXTEND";
        case TOK_MCSTAS_SETTING: return "TOK_MCSTAS_SETTING";
        case TOK_MCSTAS_OUTPUT: return "TOK_MCSTAS_OUTPUT";
        case TOK_MCSTAS_STATE: return "TOK_MCSTAS_STATE";
        case TOK_MCSTAS_POLARISATION: return "TOK_MCSTAS_POLARIZATION";
        case TOK_MCSTAS_PARAMETERS: return "TOK_MCSTAS_PARAMETERS";
        case TOK_MCSTAS_SHARE: return "TOK_MCSTAS_SHARE";
        case TOK_MCSTAS_USERVARS: return "TOK_MCSTAS_USERVARS";
        case TOK_MCSTAS_DECLARE: return "TOK_MCSTAS_DECLARE";
        case TOK_MCSTAS_INITIALIZE: return "TOK_MCSTAS_INITIALIZE";
        case TOK_MCSTAS_TRACE: return "TOK_MCSTAS_TRACE";
        case TOK_MCSTAS_SAVE: return "TOK_MCSTAS_SAVE";
        case TOK_MCSTAS_FINALLY: return "TOK_MCSTAS_FINALLY";
        case TOK_MCSTAS_MCDISPLAY: return "TOK_MCSTAS_MCDISPLAY";
        case TOK_MCSTAS_AT: return "TOK_MCSTAS_AT";
        case TOK_MCSTAS_RELATIVE: return "TOK_MCSTAS_RELATIVE";
        case TOK_MCSTAS_ABSOLUTE: return "TOK_MCSTAS_ABSOLUTE";
        case TOK_MCSTAS_PREVIOUS: return "TOK_MCSTAS_PREVIOUS";
        case TOK_MCSTAS_ROTATED: return "TOK_MCSTAS_ROTATED";
        case TOK_MCSTAS_SPLIT: return "TOK_MCSTAS_SPLIT";
        case TOK_MCSTAS_REMOVABLE: return "TOK_MCSTAS_RELATIVE";
        case TOK_MCSTAS_USER: return "TOK_MCSTAS_USER";
        case TOK_MCSTAS_WHEN: return "TOK_MCSTAS_WHEN";
        case TOK_MCSTAS_JUMP: return "TOK_MCSTAS_JUMP";
        case TOK_MCSTAS_GROUP: return "TOK_MCSTAS_GROUP";
        case TOK_MCSTAS_END: return "TOK_MCSTAS_END";
        case TOK_MCSTAS_PINCLUDE: return "TOK_MCSTAS_PINCLUDE";

        case TOK_MCSTAS_C_EXPRESSION: return "TOK_MCSTAS_C_EXPRESSION";

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
        case TOK_QUESTION: return "?";
        case TOK_TILDE: return "~";
        case TOK_OR: return "|";
        case TOK_OR_COMPARE: return "||";
        case TOK_AND: return "&";
        case TOK_AND_COMPARE: return "&&";
        case TOK_PERCENT: return "%";
        case TOK_RPERCENTBRACE: return "%{";
        case TOK_LPERCENTBRACE: return "%}";

        case TOK_CHAR: return "char";
        case TOK_STRING: return "string";
        case TOK_INT: return "integer";
        case TOK_FLOAT: return "float";
        case TOK_SCI: return "float scientific";
        case TOK_IDENTIFIER: return "identifier";

        case TOK_NULL: return "NULL";

        case TOK_MCSTAS_DEFINE: return "DEFINE";
        case TOK_MCSTAS_INSTRUMENT: return "INSTRUMENT";
        case TOK_MCSTAS_COMPONENT: return "COMPONENT";
        case TOK_MCSTAS_COPY: return "COPY";
        case TOK_MCSTAS_EXTEND: return "EXTEND";
        case TOK_MCSTAS_SETTING: return "SETTING";
        case TOK_MCSTAS_OUTPUT: return "OUTPUT";
        case TOK_MCSTAS_STATE: return "STATE";
        case TOK_MCSTAS_POLARISATION: return "POLARIZATION";
        case TOK_MCSTAS_PARAMETERS: return "PARAMETERS";
        case TOK_MCSTAS_SHARE: return "SHARE";
        case TOK_MCSTAS_USERVARS: return "USERVARS";
        case TOK_MCSTAS_DECLARE: return "DECLARE";
        case TOK_MCSTAS_INITIALIZE: return "INITIALIZE";
        case TOK_MCSTAS_TRACE: return "TRACE";
        case TOK_MCSTAS_SAVE: return "SAVE";
        case TOK_MCSTAS_FINALLY: return "FINALLY";
        case TOK_MCSTAS_MCDISPLAY: return "MCDISPLAY";
        case TOK_MCSTAS_AT: return "AT";
        case TOK_MCSTAS_RELATIVE: return "RELATIVE";
        case TOK_MCSTAS_ABSOLUTE: return "ABSOLUTE";
        case TOK_MCSTAS_PREVIOUS: return "PREVIOUS";
        case TOK_MCSTAS_ROTATED: return "ROTATED";
        case TOK_MCSTAS_SPLIT: return "SPLIT";
        case TOK_MCSTAS_REMOVABLE: return "REMOVABLE";
        case TOK_MCSTAS_USER: return "USER";
        case TOK_MCSTAS_WHEN: return "WHEN";
        case TOK_MCSTAS_JUMP: return "JUMP";
        case TOK_MCSTAS_GROUP: return "GROUP";
        case TOK_MCSTAS_END: return "END";
        case TOK_MCSTAS_PINCLUDE: return "%%include";

        case TOK_MCSTAS_C_EXPRESSION: return "C_EXPRESSION";

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

bool IsWhitespace(char c);


struct Tokenizer {
    char *at;
    s32 line;
    char *at_linestart;
    s32 line_indent;
    bool parse_error;

    void Init(char *text) {
        *this = {};

        line = 1;
        at = text;
        at_linestart = text;
    }
    void AtNewLineChar() {
        ++line;
        at_linestart = at + 1;
        line_indent = 0;

        // measure line indentation for printing error column
        char *at_indent = at_linestart;
        while (IsWhitespace(at_indent[0])) {
            ++at_indent;
            if (at_indent[0] == ' ') {
                ++line_indent;
            }
        }
    }
};

Tokenizer TokenizerInit(char *text) {
    Tokenizer t = {};
    t.Init(text);
    return t;
}

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
bool IsSpace(char c) {
    return
        c == ' ';
}

inline
bool IsNumeric(char c) {
    bool isnum = (c >= '0') && (c <= '9');
    return isnum;
}

inline
bool IsSciSymbol(char c) {
    bool issymb = ((c == '.') || (c == 'e') || (c == 'E') || (c == '-') || (c == '+'));
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
    printf("%s\n", msg);
    char lineno_tag[200];
    s32 col = (tokenizer->at - toklen) - tokenizer->at_linestart;
    sprintf(lineno_tag, "%d,%d| ", tokenizer->line, col);
    printf("%s", lineno_tag);

    // print line
    printf("%.*s\n", DistEndOfLine(tokenizer->at_linestart), tokenizer->at_linestart);

    // print marker
    s32 mark = (tokenizer->at - toklen) - tokenizer->at_linestart + strlen(lineno_tag);
    for (s32 i = 0; i < mark; i++) {
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
    char c_prev = 0;
    while (true) {
        if (c && c != ' ' && (IsAlphaOrUnderscore(c) || IsNumeric(c) || IsSciSymbol(c))) {
            if (c_prev)
                if ( ((c == '-') || (c == '+')) && (c_prev != 'e') && (c_prev != 'E') ) {
                break;
            }

            c_prev = c;
            result++;
            at++;
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
    // TODO: would be seriously needing a re-write
    //      We could probably use regex

    s32 len_was = token->len;
    token->len = ParseGetWordLen(token->text);
    tokenizer->at += token->len - len_was;

    bool has_dot = false;
    bool has_sci_e = false;
    bool has_err = false;
    bool trailing_f = false;

    for (s32 i = 0; i < token->len; ++i) {
        char c = token->text[i];

        if (IsNumeric(c)) {
            continue;
        }
        else if (((c == 'e') || (c == 'E'))) {
            has_sci_e;
            break;
        }
        else if (c == '.') {
            has_dot = true;
        }
        else if (c == 'f') {
            trailing_f = true;
            break;
        }
        else if (has_sci_e && IsSciSymbol(c)) {
            // continue
        }
        else {
            has_err = true;
            break;
        }
    }

    if (has_err) {
        token->type = TOK_UNKNOWN;
    }
    else if (has_sci_e) {
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
    if (tokenizer->parse_error) return {};

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
        Tokenizer t_prev = *tokenizer;
        Token tok_next = GetToken(tokenizer);
        *tokenizer = t_prev;

        if (tok_next.type == TOK_FLOAT || tok_next.type == TOK_SCI) {
            token.is_rval = true;

            ParseNumeric(tokenizer, &token);
        }
        else {
            token.type = TOK_DOT;
        }
    }
    break;

    case '/':
        token.type = TOK_SLASH;
        break;
    case '-':
        token.type = TOK_DASH;
        break;
    case '+':
        token.type = TOK_PLUS;
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
    case '?':
        token.type = TOK_QUESTION;
        break;
    case '~':
        token.type = TOK_TILDE;
        break;
    case '|':
    {
        if (tokenizer->at[0] == '|') {
            token.type = TOK_OR_COMPARE;
            ++tokenizer->at;
        }
        else {
            token.type = TOK_OR;
        }
    }
    break;

    case '&':
    {
        if (tokenizer->at[0] == '&') {
            token.type = TOK_AND_COMPARE;
            ++tokenizer->at;
        }
        else {
            token.type = TOK_AND;
        }
    }
    break;

    case '%':
    {
        if (tokenizer->at[0] && tokenizer->at[0] == '{')
        {
            token.type = TOK_LPERCENTBRACE;
            token.len = 2;
            ++tokenizer->at;
        }
        else if (tokenizer->at[0] && tokenizer->at[0] == '}')
        {
            token.type = TOK_RPERCENTBRACE;
            token.len = 2;
            ++tokenizer->at;
        }
        else if (tokenizer->at[0] && tokenizer->at[0] == 'i') {
            Tokenizer was = *tokenizer;
            Token nxt = GetToken(tokenizer);
            if (nxt.type == TOK_IDENTIFIER && StrEqual( StrL("include"), nxt.GetValue())) {
                token.type = TOK_MCSTAS_PINCLUDE;
            }
            else {
                *tokenizer = was;
            }
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
        // TODO: handle single quotation // no end quote before EOF -> just have a TOK_QUOTE then
        //      Probably should not parse a string outright at this level, but have a ParseStringOrQuote() function

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

            if (TokenEquals(&token, "NULL")) { token.type = TOK_NULL; }
            else if (TokenEquals(&token, "null")) { token.type = TOK_NULL; }

            else if (TokenEquals(&token, "DEFINE")) { token.type = TOK_MCSTAS_DEFINE; }
            else if (TokenEquals(&token, "INSTRUMENT")) { token.type = TOK_MCSTAS_INSTRUMENT; }
            else if (TokenEquals(&token, "COMPONENT")) { token.type = TOK_MCSTAS_COMPONENT; }
            else if (TokenEquals(&token, "COPY")) { token.type = TOK_MCSTAS_COPY; }
            else if (TokenEquals(&token, "EXTEND")) { token.type = TOK_MCSTAS_EXTEND; }
            else if (TokenEquals(&token, "SETTING")) { token.type = TOK_MCSTAS_SETTING; }
            else if (TokenEquals(&token, "OUTPUT")) { token.type = TOK_MCSTAS_OUTPUT; }
            else if (TokenEquals(&token, "STATE")) { token.type = TOK_MCSTAS_STATE; }
            else if (TokenEquals(&token, "POLARISATION")) { token.type = TOK_MCSTAS_POLARISATION; }
            else if (TokenEquals(&token, "PARAMETERS")) { token.type = TOK_MCSTAS_PARAMETERS; }
            else if (TokenEquals(&token, "SHARE")) { token.type = TOK_MCSTAS_SHARE; }
            else if (TokenEquals(&token, "USERVARS")) { token.type = TOK_MCSTAS_USERVARS; }
            else if (TokenEquals(&token, "DECLARE")) { token.type = TOK_MCSTAS_DECLARE; }
            else if (TokenEquals(&token, "INITIALIZE")) { token.type = TOK_MCSTAS_INITIALIZE; }
            else if (TokenEquals(&token, "TRACE")) { token.type = TOK_MCSTAS_TRACE; }
            else if (TokenEquals(&token, "SAVE")) { token.type = TOK_MCSTAS_SAVE; }
            else if (TokenEquals(&token, "FINALLY")) { token.type = TOK_MCSTAS_FINALLY; }
            else if (TokenEquals(&token, "MCDISPLAY")) { token.type = TOK_MCSTAS_MCDISPLAY; }
            else if (TokenEquals(&token, "AT")) { token.type = TOK_MCSTAS_AT; }
            else if (TokenEquals(&token, "RELATIVE")) { token.type = TOK_MCSTAS_RELATIVE; }
            else if (TokenEquals(&token, "ABSOLUTE")) { token.type = TOK_MCSTAS_ABSOLUTE; }
            else if (TokenEquals(&token, "PREVIOUS")) { token.type = TOK_MCSTAS_PREVIOUS; }
            else if (TokenEquals(&token, "ROTATED")) { token.type = TOK_MCSTAS_ROTATED; }
            else if (TokenEquals(&token, "SPLIT")) { token.type = TOK_MCSTAS_SPLIT; }
            else if (TokenEquals(&token, "REMOVABLE")) { token.type = TOK_MCSTAS_REMOVABLE; }
            else if (TokenEquals(&token, "USER")) { token.type = TOK_MCSTAS_USER; }
            else if (TokenEquals(&token, "WHEN")) { token.type = TOK_MCSTAS_WHEN; }
            else if (TokenEquals(&token, "JUMP")) { token.type = TOK_MCSTAS_JUMP; }
            else if (TokenEquals(&token, "GROUP")) { token.type = TOK_MCSTAS_GROUP; }
            else if (TokenEquals(&token, "END")) { token.type = TOK_MCSTAS_END; }

            else if (TokenEquals(&token, "C_EXPRESSION")) { token.type = TOK_MCSTAS_C_EXPRESSION; }
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
