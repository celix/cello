#include <string>

#include "gtest/gtest.h"
#include "common/message_queue.h"

using std::string;

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

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

