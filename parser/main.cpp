#include <cstdlib>
#include <cstdio>
#include <cstddef>

#include <cstdint>
#include <cassert>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <dirent.h>

#include "../../baselayer/base.c"
#include "../../baselayer/memory.c"
#include "../../baselayer/string.c"
#include "../../baselayer/utils.c"
#include "../../baselayer/profile.c"


// TODO: devise string list iteration


StrLst GetFilesInFolderPaths(MArena *a, char *rootpath) {
    u32 rootpath_len = strlen(rootpath);
    bool needslash = rootpath[rootpath_len-1] != '/';
    StrLst *lst = NULL;
    StrLst *first = (StrLst*) ArenaAlloc(a, 0);
    
    struct dirent *dir;
    DIR *d = opendir(rootpath);
    if (d) {
        d = opendir(rootpath);
        while ((dir = readdir(d)) != NULL) {

            // next strlst node
            lst = StrLstPut(a, rootpath, lst);

            // hot catenation
            if (needslash) {
                StrCatHot(a, '/', lst);
            }
            StrCatHot(a, dir->d_name, lst);
        }
        closedir(d);
    }

    return *first;
}


void Test() {
    printf("Running tests ...\n");

    MArena arena = ArenaCreate();
    MArena *a = &arena;
    StrLst files = GetFilesInFolderPaths(a, (char*) "/home");
    StrLstPrint(files);
    exit(0);

    // TODO: print the list! Create our lovely print string list function 
    
    // templated list
    ListX<u32> lst_T;
    lst_T.Add(14);
    lst_T.Add(222);
    lst_T.At(1);
    //printf("%u\n", *lst_T.At(0));


    // TODO: move this test to baselayer
    // native-subscript malloc/realloc arraylist / stretchy buffer
    s32 *lst = NULL;
    lst_push(lst, 42);
    lst_push(lst, -15);
    for (int i = 0; i < lst_len(lst); ++i) {
        printf("%d\n", lst[i]);
    }
    lst_free(lst);
}

void ParseInstr(MArena *a, String instr) {
    StrPrint("\n%s\n", instr);
}

int main (int argc, char **argv) {
    CLAInit(argc, argv);
    if (CLAContainsArg("--help", argc, argv) || CLAContainsArg("-h", argc, argv)) {
        printf("[instr]:         instrument file\n");
        printf("--help:          display help (this text)\n");
        printf("--test:          run test function\n");
        exit(0);
    }
    if (CLAContainsArg("--test", argc, argv)) {
        Test();
        exit(0);
    }
    if (argc != 2) {
        printf("Supply instrument file (.instr) to parse.\n");
        exit(0);
    }

    // memory
    MArena arena = ArenaCreate();

    // load instr
    String instr;
    instr.str = LoadFileMMAP(argv[1]);
    instr.len = strlen(instr.str);
    printf("Parsing %s ...\n", argv[1]);

    // parse
    ParseInstr(&arena, instr);
}
