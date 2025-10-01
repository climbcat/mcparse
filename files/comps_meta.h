#ifndef __META_COMPS__
#define __META_COMPS__




enum CompType {
    CT_UNDEF,


    CT_CNT
};


enum CompCategory {
    CCAT_UNDEF,

    
    CCAT_CNT
};


Component *CreateComponent(MArena *a_dest, CompType type, s32 index, const char *name) {
    Component *comp = (Component*) ArenaAlloc(a_dest, sizeof(Component));
    comp->type = type;

    switch (type) {
        default: { } break;
    }

    return comp;
}


void InitComponent(Component *comp, Instrument *instr = NULL) {
    switch (comp->type) {

        default: { } break;
    }
}


void TraceComponent(Component *comp, Neutron *particle, Instrument *instr = NULL) {
    switch (comp->type) {

        default: { } break;
    }
}


void DisplayComponent(Component *comp) {
    switch (comp->type) {

        default: { } break;
    }
}


void FinallyComponent(Component *comp) {
    switch (comp->type) {

        default: { } break;
    }
}


#endif // __META_COMPS__
