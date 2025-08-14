#ifndef __COGEN_INSTR_H__
#define __COGEN_INSTR_H__


void PrintDefines(StrBuff *b, InstrumentParse *instr) {
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

void PrintUndefs(StrBuff *b, InstrumentParse *instr) {
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

void AmendIdentifiesInRValue(Str *value) {
    Tokenizer t = {};
    t.Init(value->str);
    char *t_at0 = t.at;

    while (true) {
        Token tok = GetToken(&t);
        if (tok.type == TOK_IDENTIFIER) {
            Str identifier = tok.GetValue();
            Str amend = StrCat(StrL("spec->"), identifier);
            *value = StrInsertReplace(*value, amend, identifier);

            s32 t_advance = t.at - t_at0;
            t.at = value->str;
            t_at0 = t.at;
            t.at += t_advance + 6;
        }

        if (t.at >= (value->str + value->len)) {
            break;
        }
    }
}


void CogenInstrumentConfig(StrBuff *b, InstrumentParse *instr) {
    // header guard
    StrBuffPrint1K(b, "#ifndef __%.*s__\n", 2, instr->name.len, instr->name.str);
    StrBuffPrint1K(b, "#define __%.*s__\n", 2, instr->name.len, instr->name.str);
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
    StrBuffPrint1K(b, "Array<Component*> Config_%.*s(MArena *a_dest, %.*s *spec, Instrument *instr) {\n", 4, instr->name.len, instr->name.str, instr->name.len, instr->name.str);
    StrBuffPrint1K(b, "    Array<Component*> comp_sequence = InitArray<Component*>(a_dest, %d);\n", 1, instr->comps.len);
    StrBuffPrint1K(b, "    f32 at_x, at_y, at_z;\n", 0);
    StrBuffPrint1K(b, "    f32 phi_x, phi_y, phi_z;\n", 0);
    StrBuffPrint1K(b, "    s32 index = 0;\n\n", 0);

    for (s32 i = 0; i < instr->comps.len; ++i) {
        ComponentCall c = instr->comps.arr[i];

        StrBuffPrint1K(b, "    Component *%.*s = CreateComponent(a_dest, CT_%.*s, index++, \"%.*s\");\n", 6, c.name.len, c.name.str, c.type.len, c.type.str, c.name.len, c.name.str);
        StrBuffPrint1K(b, "    comp_sequence.Add(%.*s);\n", 2, c.name.len, c.name.str);
        StrBuffPrint1K(b, "    %.*s *%.*s_comp = (%.*s*) %.*s->comp;\n", 8, c.type.len, c.type.str, c.name.len, c.name.str, c.type.len, c.type.str, c.name.len, c.name.str);


        // TODO: move to dedicated code section
        // amend parameter asignments
        for (s32 j = 0; j < c.args.len; ++j) {
            Parameter *p = c.args.arr + j;

            // put "spec->" in front of identifiers in these rvalues
            AmendIdentifiesInRValue(&p->default_val);
        }

        for (s32 j = 0; j < c.args.len; ++j) {
            Parameter p = c.args.arr[j];

            if (p.default_val.len && p.default_val.str[0] == '"') {
                StrBuffPrint1K(b, "    %.*s_comp->%.*s = (char*) %.*s;\n", 6, c.name.len, c.name.str, p.name.len, p.name.str, p.default_val.len, p.default_val.str);
            }
            else {
                StrBuffPrint1K(b, "    %.*s_comp->%.*s = %.*s;\n", 6, c.name.len, c.name.str, p.name.len, p.name.str, p.default_val.len, p.default_val.str);
            }
        }
        StrBuffPrint1K(b, "    Init_%.*s(%.*s_comp, instr);\n", 4, c.type.len, c.type.str, c.name.len, c.name.str);

        /*
        // check at-relative vs. rot-relative
        if (c.at_relative_to.len) {
            if (c.rot_relative_to.len) {
                // NOTE: we actually don't want to deal with rot_relative being different from at_relative if we can avoid it
                //      It is a bit gnarly to program and from a usability standpoint, makes the instrument more comples neddlessly.
                //      Yet here we are.

                assert(StrEqual(c.at_relative_to, c.rot_relative_to) && "different at_relative and rot_relative");
            }
        }
        */

        if (c.at_absolute) {
            StrBuffPrint1K(b, "    // ABSOLUTE\n", 0);
            StrBuffPrint1K(b, "    %.*s->transform = SceneGraphAlloc();\n", 2, c.name.len, c.name.str);
        }
        else if (c.at_relative_to.len) {
            StrBuffPrint1K(b, "    // RELATIVE %.*s\n", 2, c.at_relative_to.len, c.at_relative_to.str);
            Str at_relative_to = c.at_relative_to;
            if ( StrEqual(c.at_relative_to, StrL("PREVIOUS")) ) {
                assert(i > 0);
                at_relative_to = instr->comps.arr[i-1].name;
            }
            StrBuffPrint1K(b, "    %.*s->transform = SceneGraphAlloc(%.*s->transform);\n", 4, c.name.len, c.name.str, at_relative_to.len, at_relative_to.str);
        }

        // TODO: move to dedicated code section
        AmendIdentifiesInRValue(&c.at_x);
        AmendIdentifiesInRValue(&c.at_y);
        AmendIdentifiesInRValue(&c.at_z);

        StrBuffPrint1K(b, "    // AT:  (%.*s, %.*s, %.*s)\n", 6, c.at_x.len, c.at_x.str, c.at_y.len, c.at_y.str, c.at_z.len, c.at_z.str);
        StrBuffPrint1K(b, "    at_x = %.*s;\n", 2, c.at_x.len, c.at_x.str);
        StrBuffPrint1K(b, "    at_y = %.*s;\n", 2, c.at_y.len, c.at_y.str);
        StrBuffPrint1K(b, "    at_z = %.*s;\n", 2, c.at_z.len, c.at_z.str);
        if (c.rot_defined) {

            // TODO: move to dedicated code section
            AmendIdentifiesInRValue(&c.rot_x);
            AmendIdentifiesInRValue(&c.rot_y);
            AmendIdentifiesInRValue(&c.rot_z);

            StrBuffPrint1K(b, "    // ROT: (%.*s, %.*s, %.*s)\n", 6, c.rot_x.len, c.rot_x.str, c.rot_y.len, c.rot_y.str, c.rot_z.len, c.rot_z.str);
            StrBuffPrint1K(b, "    phi_x = %.*s;\n", 2, c.rot_x.len, c.rot_x.str);
            StrBuffPrint1K(b, "    phi_y = %.*s;\n", 2, c.rot_y.len, c.rot_y.str);
            StrBuffPrint1K(b, "    phi_z = %.*s;\n", 2, c.rot_z.len, c.rot_z.str);
            StrBuffPrint1K(b, "    %.*s->transform->t_loc = TransformBuildRotateX( phi_z * deg2rad ) * TransformBuildRotateX( phi_y * deg2rad ) * TransformBuildRotateX( phi_x * deg2rad ) * TransformBuildTranslation( { at_x, at_y, at_z } );\n", 2, c.name.len, c.name.str);
        }
        else {
            StrBuffPrint1K(b, "    %.*s->transform->t_loc = TransformBuildTranslation( { at_x, at_y, at_z } );\n", 2, c.name.len, c.name.str);
        }
        StrBuffPrint1K(b, "\n", 0);
    }
    StrBuffPrint1K(b, "}\n\n", 0);

    // close header guard
    StrBuffPrint1K(b, "#endif // %.*s\n", 2, instr->name.len, instr->name.str);
}


#endif
