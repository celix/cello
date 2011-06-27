#include <string>

#include "gtest/gtest.h"
#include "common/message_queue.h"

using std::string;

TEST(NormalTest, All) {
   MessageQueue queue(9999);
   MessageQueue::Msg data;
   snprintf(data.msg, sizeof(data.msg), "TEST");
   queue.Send(&data);
   MessageQueue::Msg info;
   queue.Receive(&info);
   string ss(info.msg, strlen(info.msg));
   EXPECT_EQ(ss, "TEST");
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

