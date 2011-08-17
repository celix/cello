#ifndef _INFO_H
#define _INFO_H
const static int MAX = 100;
const static int KEY = 8888;

struct Info {
    long int type;
    char ss[MAX];
    Info(): type(1) {
        for (int i = 0; i < MAX; ++i)
            ss[i] = 0;
    }
};

#endif
