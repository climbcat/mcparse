#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <sys/mman.h>
#include <cassert>


// four-space indentation
// zero-is-initialization
// minimize usage of cstdlib
// int/float typedefs for all sizes
// constants & macros
// length-based strings
// arena & pool allocation
// C base language, and:
// operator overloading
// easy struct definition & init
// avoid templates


//
// types


typedef u_int8_t u8;
typedef u_int16_t u16;
typedef u_int32_t u32;
typedef u_int64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef float f32;
typedef double f64;

u64 GIGABYTE = 1024*1024*1024;
u64 MEGABYTE = 1024*1024;
u64 KILOBYTE = 1024;
u64 SIXTEEN_KB = 16 * KILOBYTE;


//
// macros & constants


#define PI 3.14159
f32 deg2rad = PI / 180;
f32 rad2deg = 180 / PI;


//
// min / max


inline u8 MinU32(u8 a, u8 b) { return (a <= b) ? a : b; }
inline u16 MinU32(u16 a, u16 b) { return (a <= b) ? a : b; }
inline u32 MinU32(u32 a, u32 b) { return (a <= b) ? a : b; }
inline u64 MinU32(u64 a, u64 b) { return (a <= b) ? a : b; }

inline s8 MinS32(s8 a, s8 b) { return (a <= b) ? a : b; }
inline s16 MinS32(s16 a, s16 b) { return (a <= b) ? a : b; }
inline s32 MinS32(s32 a, s32 b) { return (a <= b) ? a : b; }
inline s64 MinS32(s64 a, s64 b) { return (a <= b) ? a : b; }

inline f32 MinF32(f32 a, f32 b) { return (a <= b) ? a : b; }
inline f64 MinF32(f64 a, f64 b) { return (a <= b) ? a : b; }

inline u8 MaxU32(u8 a, u8 b) { return (a > b) ? a : b; }
inline u16 MaxU32(u16 a, u16 b) { return (a > b) ? a : b; }
inline u32 MaxU32(u32 a, u32 b) { return (a > b) ? a : b; }
inline u64 MaxU32(u64 a, u64 b) { return (a > b) ? a : b; }

inline s8 MaxS32(s8 a, s8 b) { return (a > b) ? a : b; }
inline s16 MaxS32(s16 a, s16 b) { return (a > b) ? a : b; }
inline s32 MaxS32(s32 a, s32 b) { return (a > b) ? a : b; }
inline s64 MaxS32(s64 a, s64 b) { return (a > b) ? a : b; }

inline f32 MaxF32(f32 a, f32 b) { return (a > b) ? a : b; }
inline f64 MaxF32(f64 a, f64 b) { return (a > b) ? a : b; }


//
// parse cmd line args


s32 g_argc;
char **g_argv;
void CLAInit(s32 argc, char **argv) {
    g_argc = argc;
    g_argv = argv;
}
bool CLAContainsArg(const char *search, int argc, char **argv, int *idx = NULL) {
    for (int i = 0; i < argc; ++i) {
        char *arg = argv[i];
        if (!strcmp(argv[i], search)) {
            if (idx != NULL) {
                *idx = i;
            }
            return true;
        }
    }
    return false;
}
bool CLAContainsArgs(const char *search_a, const char *search_b, int argc, char **argv) {
    bool found_a = false;
    bool found_b = false;
    for (int i = 0; i < argc; ++i) {
        if (!strcmp(argv[i], search_a)) {
            found_a = true;
        }
        if (!strcmp(argv[i], search_b)) {
            found_b = true;
        }
    }
    return found_a && found_b;
}
char *CLAGetArgValue(const char *key, int argc, char **argv) {
    int i;
    bool error = !CLAContainsArg(key, argc, argv, &i) || i == argc - 1;;
    if (error == false) {
        char *val = argv[i+1];
        error = strlen(val) > 1 && val[0] == '-' && val[1] == '-';
    }
    if (error == true) {
        printf("KW arg %s must be followed by a value arg\n", key);
        exit(0);
    }
    return argv[i+1];
}

//
// linked lists


struct LList1 {
    LList1 *next;
};

struct LList2 {
    LList2 *next;
    LList2 *prev;
};

struct LList3 {
    LList3 *next;
    LList3 *prev;
    LList3 *descend;
};

void InsertBefore1(void *newlink, void *before) {
    LList1 *newlnk = (LList1*) newlink;

    newlnk->next = (LList1*) before;
}
void InsertBefore2(void *newlink, void *before) {
    LList2 *newlnk = (LList2*) newlink;
    LList2 *befre = (LList2*) before;

    newlnk->prev = befre->prev;
    newlnk->next = befre;
    if (befre->prev != NULL) {
        befre->prev->next = newlnk;
    }
    befre->prev = newlnk;
}
void InsertBelow3(void *newlink, void *below) {
    LList3 *newlnk = (LList3*) newlink;
    LList3 *belw = (LList3*) below;

    newlnk->descend = belw->descend;
    belw->descend = newlnk;
}


//
// memory allocation


struct MArena {
    u8 *mem;
    u64 mapped;
    u64 committed;
    u64 used;
    bool locked = false;
};

#define ARENA_RESERVE_SIZE GIGABYTE
#define ARENA_COMMIT_CHUNK SIXTEEN_KB

void _ArenaBumpProtected(MArena *a, u32 nbumps) {
    u64 amount = nbumps * SIXTEEN_KB;
    mprotect(a->mem + a->committed, amount, PROT_READ | PROT_WRITE);
}
MArena ArenaCreate() {
    MArena a;

    a.mem = (u8*) mmap(NULL, ARENA_RESERVE_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
    a.mapped = ARENA_RESERVE_SIZE;
    mprotect(a.mem, ARENA_COMMIT_CHUNK, PROT_READ | PROT_WRITE);
    a.committed = ARENA_COMMIT_CHUNK;
    a.used = 0;

    return a;
}
void *ArenaAlloc(MArena *a, u64 len) {
    assert(!a->locked && "ArenaAlloc: memory arena is open, use MArenaClose to allocate");

    if (a->committed < a->used + len) {
        u32 nbumps = len / ARENA_COMMIT_CHUNK + 1;
        _ArenaBumpProtected(a, nbumps);
    }
    void *result = a->mem + a->used;
    a->used += len;

    return result;
}
void ArenaPush(MArena *a, void *data, u32 len) {
    void *dest = ArenaAlloc(a, len);
    memcpy(dest, data, len);
}
void *ArenaOpen(MArena *a) {
    assert(!a->locked && "ArenaOpen: memory arena is alredy open");

    a->locked = true;
    return a->mem + a->used;
}
void ArenaClose(MArena *a, u64 len) {
    assert(a->locked && "ArenaClose: memory arena not open");

    a->locked = false;
    ArenaAlloc(a, len);
}
// TODO: de-allocation also shrinks commited memory (think; closing a large file in a model editor)
// TODO: scratch arenas
// TODO: ArenaPush(a, src, len)


struct MPool {
    u8 *mem;
    u32 block_size;
    u32 nblocks;
    LList1 *free_list;
};

#define MPOOL_CACHE_LINE_SIZE 64

MPool PoolCreate(u32 block_size_min, u32 nblocks) {
    assert(nblocks > 1);

    MPool p;
    p.block_size = MPOOL_CACHE_LINE_SIZE * (block_size_min / MPOOL_CACHE_LINE_SIZE + 1);
    p.mem = (u8*) mmap(NULL, p.block_size * nblocks, PROT_READ | PROT_WRITE, MAP_PRIVATE, -1, 0);
    p.free_list = (LList1*) p.mem;

    LList1 *current = p.free_list;
    for (u32 i = 0; i < nblocks - 1; ++i) {
        current->next = (LList1*) (p.mem + i * p.block_size);
        current = current->next;
    }
    current->next = NULL;

    return p;
}
void *PoolAlloc(MPool *p) {
    if (p->free_list == NULL) {
        return NULL;
    }
    void *retval = p->free_list;
    p->free_list = p->free_list->next;
    memset(retval, 0, MPOOL_CACHE_LINE_SIZE);

    return retval;
}
void PoolFree(MPool *p, void *element) {
    assert(element >= (void*) p->mem); // check lower bound
    u64 offset = (u8*) element -  p->mem;
    assert(offset % p->block_size == 0); // check alignment
    assert(offset < p->block_size * p->nblocks); // check upper bound

    LList1 *element_as_list = (LList1*) element;
    element_as_list->next = p->free_list;
    p->free_list = element_as_list;
}


//
// strings


struct String {
    char *str;
    u32 len;
};

struct StringList {
    StringList *next;
    String value;
};

String StrLiteral(MArena *a, const char *lit) {
    String s;
    s.len = 0;
    while (*(lit + s.len) != '\0') {
        ++s.len;
    }
    s.str = (char*) ArenaAlloc(a, s.len);
    memcpy(s.str, lit, s.len);

    return s;
}
void StrPrint(String s) {
    printf("%.*s", s.len, s.str);
}
bool StrEqual(String a, String b) {
    u32 i = 0;
    u32 len = MinU32(a.len, b.len);
    while (i < len) {
        if (a.str[i] != b.str[i]) {
            return false;
        }
        ++i;
    }

    return a.len == b.len;
}
String StrCat(MArena *arena, String a, String b) {
    String cat;
    cat.len = a.len + b.len;
    cat.str = (char*) ArenaAlloc(arena, cat.len);
    memcpy(cat.str, a.str, a.len);
    memcpy(cat.str + a.len, b.str, b.len);

    return cat;
}
void StrLstPrint(StringList *lst) {
    while (lst != NULL) {
        StrPrint(lst->value);
        printf(", ");
        lst = lst->next;
    }
}
StringList *StrSplit(MArena *arena, String base, char split_at_and_remove) {
    StringList *first;
    StringList *node;
    StringList *prev = NULL;
    u32 i = 0;

    while (true) {
        while (base.str[i] == split_at_and_remove) {
            ++i;
        }
        if (i >= base.len) {
            return first;
        }

        node = (StringList *) ArenaAlloc(arena, sizeof(StringList));
        node->value.str = (char*) ArenaOpen(arena);
        node->value.len = 0;

        if (prev != NULL) {
            prev->next = node;
        }
        else {
            first = node;
        }

        int j = 0;
        while (base.str[i] != split_at_and_remove && i < base.len) {
            ++node->value.len;
            node->value.str[j] = base.str[i];
            ++i;
            ++j;
        }

        ArenaClose(arena, node->value.len);
        prev = node;
        if (i < base.len) {
            ++i; // skip the split char
        }
        else {
            return first;
        }
    }
}
// TODO: impl. "arena push version" e.g. a version that uses ArenaPush(src, len) rather than
//     ArenaAlloc() to see which one is most readable, I expect it to be better
// StringList *StrSplit(MArena *arena, String base, char split) {}

String StrJoin(MArena *a, StringList *strs) {
    String join;
    join.str = (char*) ArenaOpen(a);
    join.len = 0;

    while (strs != NULL) {
        memcpy(join.str + join.len, strs->value.str, strs->value.len);
        join.len += strs->value.len;
        strs = strs->next;
    }

    ArenaClose(a, join.len);
    return join;
}
String StrJoinInsertChar(MArena *a, StringList *strs, char insert) {
    String join;
    join.str = (char*) ArenaOpen(a);
    join.len = 0;

    while (strs != NULL) {
        memcpy(join.str + join.len, strs->value.str, strs->value.len);
        join.len += strs->value.len;
        strs = strs->next;

        if (strs != NULL) {
            join.str[join.len] = insert;
            ++join.len;
        }
    }

    ArenaClose(a, join.len);
    return join;
}


//
// data structures



