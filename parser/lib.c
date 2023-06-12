#include <cstdlib>
#include <cstring>
#include <cstdio>

bool ContainsArg(const char *search, int argc, char **argv, int *idx = NULL) {
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

bool ContainsArgs(const char *search_a, const char *search_b, int argc, char **argv) {
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

char *GetArgValue(const char *key, int argc, char **argv) {
    int i;
    bool error = !ContainsArg(key, argc, argv, &i) || i == argc - 1;;
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
