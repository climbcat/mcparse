#ifndef __PCOMP_H__
#define __PCOMP_H__


struct Component {
    Str type;
    Str type_copy;
    Array<Parameter> setting_params;
    Array<Parameter> out_params;
    Array<Parameter> state_params;
    Array<Parameter> pol_params;

    bool flag_noacc;
    Str dependency_str;

    Str share_block;
    Str uservars_block;
    Str declare_block;
    Str initalize_block;
    Str trace_block;
    Str finally_block;
    Str display_block;

    Str share_type_copy;
    Str uservars_type_copy;
    Str declare_type_copy;
    Str initalize_type_copy;
    Str trace_type_copy;
    Str finally_type_copy;
    Str display_type_copy;

    Str share_extend;
    Str uservars_extend;
    Str declare_extend;
    Str initalize_extend;
    Str trace_extend;
    Str finally_extend;
    Str display_extend;

    bool parse_error;
};


Array<Parameter> ParseComponentParams(MArena *a_dest, Tokenizer *t, TokenType params_type, bool is_optional) {
    Token token;
    if (is_optional && Optional(t, &token, params_type) != PTR_OPTIONAL) {
        return Array<Parameter> { NULL, 0, 0 };
    }

    if (is_optional == false) {
        Required(t, &token, params_type);
    }
    Required(t, &token, TOK_MCSTAS_PARAMETERS);

    return ParseParamsBlock(a_dest, t);
}


Component *ParseComponent(MArena *a_dest, char *text) {
    TimeFunction;

    Tokenizer tokenizer = {};
    tokenizer.Init(text);
    Tokenizer *t = &tokenizer;
    Token token;
    Component *comp = (Component*) ArenaAlloc(a_dest, sizeof(Component));

    // component type
    Required(t, &token, TOK_MCSTAS_DEFINE);
    Required(t, &token, TOK_MCSTAS_COMPONENT);
    Required(t, &token, TOK_IDENTIFIER);
    comp->type = token.GetValue();
    if (Optional(t, &token, TOK_MCSTAS_COPY) == PTR_OPTIONAL) {
        Required(t, &token, TOK_IDENTIFIER);
        comp->type_copy = token.GetValue();
    }

    // parameters
    comp->setting_params = ParseComponentParams(a_dest, t, TOK_MCSTAS_SETTING, false);
    comp->out_params = ParseComponentParams(a_dest, t, TOK_MCSTAS_OUTPUT, true);
    comp->state_params = ParseComponentParams(a_dest, t, TOK_MCSTAS_STATE, true);
    comp->pol_params = ParseComponentParams(a_dest, t, TOK_MCSTAS_POLARISATION, true);

    // flags
    while (Optional(t, &token, TOK_IDENTIFIER) == PTR_OPTIONAL) {
        if (StrEqual( StrL("DEPENDENCY"), token.GetValue())) { 
            Required(t, &token, TOK_STRING);
            printf("Paresed DEP\n");
            comp->dependency_str = token.GetValue();
        }

        if (StrEqual( StrL("NOACC"), token.GetValue())) {
            printf("Paresed NOACC\n");
            comp->flag_noacc;
        }
    }

    // code blocks
    TokenType options_blocks[] = {
        TOK_MCSTAS_SHARE,
        TOK_MCSTAS_USERVARS,
        TOK_MCSTAS_DECLARE,
        TOK_MCSTAS_INITIALIZE,
        TOK_MCSTAS_TRACE,
        TOK_MCSTAS_SAVE,
        TOK_MCSTAS_FINALLY,
        TOK_MCSTAS_MCDISPLAY
    };

    ParseTokenResult result = {};
    while (result != PTR_TERMINAL) {
        Tokenizer dont_advance = *t;

        result = BranchMultiple(&dont_advance, &token, options_blocks, 8, "code block", TOK_MCSTAS_END);

        switch (token.type) {
        case TOK_ENDOFSTREAM: { } break;

        case TOK_MCSTAS_SHARE: { ParseCodeBlock(t, TOK_MCSTAS_SHARE, &comp->share_block, &comp->share_type_copy, &comp->share_extend); } break;
        case TOK_MCSTAS_USERVARS: { ParseCodeBlock(t, TOK_MCSTAS_USERVARS, &comp->uservars_block, &comp->uservars_type_copy, &comp->uservars_extend); } break;
        case TOK_MCSTAS_DECLARE: { ParseCodeBlock(t, TOK_MCSTAS_DECLARE, &comp->declare_block, &comp->declare_type_copy, &comp->declare_extend); } break;
        case TOK_MCSTAS_INITIALIZE: { ParseCodeBlock(t, TOK_MCSTAS_INITIALIZE, &comp->initalize_block, &comp->initalize_type_copy, &comp->initalize_extend); } break;
        case TOK_MCSTAS_TRACE: { ParseCodeBlock(t, TOK_MCSTAS_TRACE, &comp->trace_block, &comp->trace_type_copy, &comp->trace_extend); } break;
        case TOK_MCSTAS_SAVE: { ParseCodeBlock(t, TOK_MCSTAS_SAVE, &comp->trace_block, &comp->trace_type_copy, &comp->trace_extend); } break;
        case TOK_MCSTAS_FINALLY: { ParseCodeBlock(t, TOK_MCSTAS_FINALLY, &comp->finally_block, &comp->finally_type_copy, &comp->finally_extend); } break;
        case TOK_MCSTAS_MCDISPLAY: { ParseCodeBlock(t, TOK_MCSTAS_MCDISPLAY, &comp->display_block, &comp->display_type_copy, &comp->display_extend); } break;

        default: {  assert(token.type == TOK_MCSTAS_END); } break;
        }

        if (result == PTR_ERROR) {
            comp->parse_error = true;
            break;

            assert(1 == 0 && "DBG break");
        }
    }

    // end
    Required(t, &token, TOK_MCSTAS_END);

    return comp;
}


void ComponentPrint(Component *comp, bool print_name = true, bool print_params = false, bool print_display_block = false, bool print_blocks = false) {

    printf("\n");
    if (print_name) {
        printf("COMPONENT: "); StrPrint(comp->type); printf("\n");
    }

    if (print_params) {
        for (u32 i = 0; i < comp->setting_params.len; ++i) {
            Parameter p = comp->setting_params.arr[i];

            printf("    ");
            StrPrint(p.name);
            if (p.default_val.len) {
                printf(" = ");
                StrPrint(p.default_val);
            }

            if (p.type.len) {
                printf(" (");
                StrPrint(p.type);
                printf(")");
            }
            printf("\n");
        }
    }

    if (print_blocks) {
        if (comp->share_block.len) {
            printf("\nSHARE:\n");
            StrPrint(comp->share_block);
            printf("\n");
        }

        if (comp->declare_block.len) {
            printf("\nDECLARE:\n");
            StrPrint(comp->declare_block);
            printf("\n");
        }

        if (comp->initalize_block.len) {
            printf("\nINITIALIZE:\n");
            StrPrint(comp->initalize_block);
            printf("\n");
        }

        if (comp->trace_block.len) {
            printf("\nTRACE:\n");
            StrPrint(comp->trace_block);
            printf("\n");
        }

    }

    if (print_display_block) {
        if (comp->display_block.len) {
            printf("\nMCDISPLAY:\n");
            StrPrint(comp->display_block);
        }
    }
}


void ComponentCogen(Component *comp) {
    /*
    What's the cogen gonna be: Using the example component "AComp.comp"
    - base functions: AComp.h 
    - shared library: AComp_share.h - all of the share block, which can be a lot of code
    - meta/reflection: AComp_meta.h - component type name, parameter type info/names, doc strings
            which is all of the stuff that can be provided through cogen - now that we are at it.
            With this info, we can build UI, get function pointers for the type-agnostic simulation core.

    Let's focus on AComp.h for now. We would provide the following:
    - header guard
    - include any AComp_share.h file
    - component struct: declare block (parsed for symbol mapping)
    - component struct: Transform t; Transform *parent; comp_type_name, comp_name (instance name)
    - symbols: define each declare block sumbols as #define sym comp->sym - for the entire file ! 

    - AComp AComp_Init() {} returning a component struct; runs init code and returns
    - void Trace(Acomp *comp) {}
    - Finally
    - Display

    - un-define symbols
    - end header guard

    The AComp_share.h file should contain the following:
    - header guard
    - share code block copy-pasted verbatim into the file 
    - end header guard

    The AComp_meta.h file should contain some of the following things (less specific/brain-stormy)
    - header guard
    - include the AComp.h file
    - void GetCompSignatures(HashMap dest_map) - returns function pointers to trace, finally and display
    - Wrapper functions that take and return void* arguments? 
    - ...
    */
}


#endif
