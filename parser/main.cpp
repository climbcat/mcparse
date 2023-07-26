#include <cstdlib>
#include <cstdio>
#include <cstddef>

#include <cstdint>
#include <cassert>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>

#include "../../baselayer/base.c"
#include "../../baselayer/memory.c"
#include "../../baselayer/string.c"
#include "../../baselayer/utils.c"
#include "../../baselayer/profile.c"

/*
ArrayListT<char*> GetFilesInFolderPaths(char *rootpath, StackAllocator *stack) {
  ArrayListT<char*> result;
  struct dirent *dir;
  u32 count = 0;
  u32 rootpath_len = strlen(rootpath);

  DIR *d = opendir(rootpath);
  if (d) {

    while ((dir = readdir(d)) != NULL) {
      ++count;
    }
    closedir(d);
  }

  d = opendir(rootpath);
  if (d) {
    result.Init(stack->Alloc(sizeof(char*) * count));

    d = opendir(rootpath);
    while ((dir = readdir(d)) != NULL) {
      bool needslash = rootpath[rootpath_len-1] != '/';

      u32 len = rootpath_len + int(needslash) + strlen(dir->d_name) + 1;
      char* path = (char*) stack->Alloc( len );

      strcpy(path, rootpath);
      if (needslash) {
        strcat(path, "/");
      }
      strcat(path, dir->d_name);

      result.Add(&path);
    }
    closedir(d);
  }

  return result;
}
*/


void Test() {
    printf("Running tests ...\n");

    // templated list
    ListX<u32> lst_T;
    lst_T.Add(14);
    lst_T.Add(222);
    lst_T.At(1);
    //printf("%u\n", *lst_T.At(0));

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
