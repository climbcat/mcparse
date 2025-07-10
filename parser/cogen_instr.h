#ifndef __COGEN_INSTR_H__
#define __COGEN_INSTR_H__


void PrintDefines(StrBuff *b, Instrument *instr) {
    for (s32 i = 0; i < instr->params.len; ++i) {
        Parameter p = instr->params.arr[i];
        StrBuffPrint1K(b, "    #define %.*s spec->%.*s\n", 4, p.name.len, p.name.str, p.name.len, p.name.str);
    }
    StrBuffPrint1K(b, "\n", 0);
    for (s32 i = 0; i < instr->declare_members.len; ++i) {
        StructMember m = instr->declare_members.arr[i];
        StrBuffPrint1K(b, "    #define %.*s spec->%.*s\n", 4, m.name.len, m.name.str, m.name.len, m.name.str);
    }
}
void PrintUndefs(StrBuff *b, Instrument *instr) {
    for (s32 i = 0; i < instr->params.len; ++i) {
        Parameter p = instr->params.arr[i];
        StrBuffPrint1K(b, "    #undef %.*s\n", 2, p.name.len, p.name.str);
    }
    StrBuffPrint1K(b, "\n", 0);
    for (s32 i = 0; i < instr->declare_members.len; ++i) {
        StructMember m = instr->declare_members.arr[i];
        StrBuffPrint1K(b, "    #undef %.*s\n", 2, m.name.len, m.name.str);
    }
}
void AmendInstParDefaultValue(Array<Parameter> pars);


void InstrumentCogen(StrBuff *b, Instrument *instr) {
    // header guard
    StrBuffPrint1K(b, "#ifndef __%.*s__\n", 2, instr->name.len, instr->name.str);
    StrBuffPrint1K(b, "#define __%.*s__\n", 2, instr->name.len, instr->name.str);
    StrBuffPrint1K(b, "\n\n", 0);

    // includes
    StrBuffPrint1K(b, "#include \"meta_comps.h\"\n", 0);
    StrBuffPrint1K(b, "\n\n", 0);

    // struct
    StrBuffPrint1K(b, "struct %.*s {\n", 2, instr->name.len, instr->name.str);
    StrBuffPrint1K(b, "\n    // parameters\n", 0);

    // parameters
    Str string_s { (char*) "string", 6 };
    Str vector_s { (char*) "vector", 6 };
    for (s32 i = 0; i < instr->params.len; ++i) {
        Parameter p = instr->params.arr[i];

        if (StrEqual(p.type, string_s)) {
            StrBuffPrint1K(b, "    char *", 0);
        }
        else if (StrEqual(p.type, vector_s)) {
            StrBuffPrint1K(b, "    double ", 0);
        }
        else if (p.type.len) {
            StrBuffPrint1K(b, "    %.*s ", 2, p.type.len, p.type.str);
        }
        else {
            StrBuffPrint1K(b, "    double ", 0);
        }

        if (StrEqual(p.type, vector_s)) {
            // just scan how many commas it has
            assert(p.default_val.len > 0);
            s32 cnt = 1;
            for (s32 k = 0; k < p.default_val.len; ++k) {
                if (p.default_val.str[k] == ',') {
                    ++cnt;
                }
            }
            StrBuffPrint1K(b, "%.*s[%d]", 3, p.name.len, p.name.str, cnt);
        }
        else {
            StrBuffPrint1K(b, "%.*s", 2, p.name.len, p.name.str);
        }

        if (p.default_val.len) {
            if (StrEqual(p.type, string_s)) {
                StrBuffPrint1K(b, " = (char*) %.*s", 2, p.default_val.len, p.default_val.str);
            }
            else {
                StrBuffPrint1K(b, " = %.*s", 2, p.default_val.len, p.default_val.str);
            }
        }
        StrBuffPrint1K(b, ";\n", 0);
    }

    // declare members
    StrBuffPrint1K(b, "\n    // declares\n", 0);
    for (s32 i = 0; i < instr->declare_members.len; ++i) {
        StructMember m = instr->declare_members.arr[i];

        StrBuffPrint1K(b, "    %.*s ", 2, m.type.len, m.type.str);
        if (m.is_pointer_type) {
            StrBuffPrint1K(b, "*", 0);
        }
        StrBuffPrint1K(b, "%.*s", 2, m.name.len, m.name.str);

        if (m.is_array_type) {
            StrBuffPrint1K(b, "[%d]", 1, m.array_type_sz);
        }

        if (m.defval.len) {
            StrBuffPrint1K(b, " = %.*s", 2, m.defval.len, m.defval.str);
        }
        StrBuffPrint1K(b, ";\n", 0);
    }
    StrBuffPrint1K(b, "};\n\n", 0);


    // init
    StrBuffPrint1K(b, "void Init_%.*s(%.*s *spec) {\n", 4, instr->name.len, instr->name.str, instr->name.len, instr->name.str);
    StrBuffPrint1K(b, "\n", 0);
    if (instr->initalize_block.len) {
        PrintDefines(b, instr);
        StrBuffPrint1K(b, "    ////////////////////////////////////////////////////////////////\n\n", 0);

        StrBuffAppend(b, instr->initalize_block);

        StrBuffPrint1K(b, "\n\n    ////////////////////////////////////////////////////////////////\n", 0);
        PrintUndefs(b, instr);
        StrBuffPrint1K(b, "\n", 0);
    }
    StrBuffPrint1K(b, "}\n\n", 0);

    // trace
    StrBuffPrint1K(b, "void Config_%.*s(%.*s *spec, Instrument *instr) {\n", 4, instr->name.len, instr->name.str, instr->name.len, instr->name.str);
    for (s32 i = 0; i < instr->comps.len; ++i) {
        ComponentCall c = instr->comps.arr[i];
        
        StrBuffPrint1K(b, "\n    %.*s %.*s = Create_%.*s(%d, (char*) \"%.*s\");\n", 9, c.type.len, c.type.str, c.name.len, c.name.str, c.type.len, c.type.str, i, c.name.len, c.name.str);
        AmendInstParDefaultValue(c.args);
        for (s32 j = 0; j < c.args.len; ++j) {
            Parameter p = c.args.arr[j];

            if (p.default_val.len && p.default_val.str[0] == '"') {
                // we need to cast to char* !
                StrBuffPrint1K(b, "    %.*s.%.*s = (char*) %.*s;\n", 6, c.name.len, c.name.str, p.name.len, p.name.str, p.default_val.len, p.default_val.str);
            }
            else {
                StrBuffPrint1K(b, "    %.*s.%.*s = %.*s;\n", 6, c.name.len, c.name.str, p.name.len, p.name.str, p.default_val.len, p.default_val.str);
            }
        }
        StrBuffPrint1K(b, "    Init_%.*s(&%.*s, instr);\n", 4, c.type.len, c.type.str, c.name.len, c.name.str);

        // TODO: set AT, ROT
        StrBuffPrint1K(b, "    // AT:  (%.*s, %.*s, %.*s)\n", 6, c.at_x.len, c.at_x.str, c.at_y.len, c.at_y.str, c.at_z.len, c.at_z.str);
        if (c.rot_defined) {
            StrBuffPrint1K(b, "    // ROT: (%.*s, %.*s, %.*s)\n", 6, c.rot_x.len, c.rot_x.str, c.rot_y.len, c.rot_y.str, c.rot_z.len, c.rot_z.str);
        }
    }
    StrBuffPrint1K(b, "}\n\n", 0);

    // close header guard
    StrBuffPrint1K(b, "#endif // %.*s\n", 2, instr->name.len, instr->name.str);
}

Str StrInsertReplace(Str src, Str amend, Str at) {
    Str before = src;
    before.len = (at.str - src.str);

    Str after = {};
    after.len = src.len - before.len - at.len;
    after.str = (src.str + src.len) - after.len;

    s32 len = src.len - at.len + amend.len;
    Str result = StrAlloc(len);
    result = StrCat(before, amend);
    result = StrCat(result, after);

    return result;
}

void AmendInstParDefaultValue(Array<Parameter> pars) {
    for (s32 j = 0; j < pars.len; ++j) {
        Parameter *p = pars.arr + j;

        // first time:
        Tokenizer t = {};
        t.Init(p->default_val.str);
        char *t_at0 = t.at;

        while (true) {
            Token tok = GetToken(&t);
            if (tok.type == TOK_IDENTIFIER) {
                Str parname = tok.GetValue();
                Str amend = StrCat(StrL("spec->"), parname);
                p->default_val = StrInsertReplace(p->default_val, amend, parname);

                s32 t_advance = t.at - t_at0;
                t.at = p->default_val.str;
                t_at0 = t.at;
                t.at += t_advance + 6;
            }
            if (t.at >= (p->default_val.str + p->default_val.len)) {
                break;
            }
        }
    }

}

#endif
