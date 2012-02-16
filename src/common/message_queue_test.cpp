#include <string>
#include <iostream>
#include <stdint.h>

#include <gtest/gtest.h>
#include "common/message_queue.h"

using std::string;
using std::cout;
using std::endl;
using cello::MessageQueue;

MessageQueue g_queue(10001);

void* SendFunc(void* args) {
    intptr_t n = (intptr_t) args;
    while (true) {
        MessageQueue::Message data;
        snprintf(data.content, sizeof(data.content), "Thread%d", n);
        g_queue.Send(&data);
        cout << "Send: " << data.content << endl;
    }
    return NULL;
}

TEST(NormalTest, All) {
   MessageQueue queue(9999);
   MessageQueue::Message data;
   snprintf(data.content, sizeof(data.content), "TEST");
   queue.Send(&data);
   MessageQueue::Message info;
   queue.Receive(&info);
   string ss(info.content, strlen(info.content));
   EXPECT_EQ(ss, "TEST");
}

TEST(ThreadTest, All) {
    pthread_t send_t[10];
    for (int i = 0; i < 10; ++i)
        pthread_create(&send_t[i], NULL, SendFunc, (void*)i);
    while (true) {
        MessageQueue::Message info;
        g_queue.Receive(&info);
        string ss(info.content, strlen(info.content));
        ASSERT_TRUE(ss == "Thread0" || ss == "Thread1" ||
                    ss == "Thread2" || ss == "Thread3" ||
                    ss == "Thread4" || ss == "Thread5" ||
                    ss == "Thread6" || ss == "Thread7" ||
                    ss == "Thread8" || ss == "Thread9");
    }
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

