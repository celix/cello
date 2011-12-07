#include "common/block_queue.h"
#include <gtest/gtest.h>

#include <iostream>
#include <pthread.h>

using std::cout;
using std::endl;
using cello::BlockQueue;

BlockQueue<int> g_queue;

void* Fetch(void* unused) {
    sleep(10);
    while (1) {
        int n;
        g_queue.PopFront(&n);
        cout << n << endl;
    }
    return NULL;
}

void* Push(void* unused) {
    sleep(20);
    for (int i = 0; 1; ++i) {
        sleep(1);
        g_queue.PushBack(i);
    }
    return NULL;
}

TEST(TestQueue, Normal) {
    pthread_t fetch_t;
    pthread_t push_t;
    pthread_create(&fetch_t, NULL, Fetch, NULL);
    pthread_create(&push_t, NULL, Push, NULL);
    for (int i = 0; 1; i += 100) {
        sleep(1);
        g_queue.PushBack(i);
    }
    
    pthread_join(fetch_t, NULL);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

