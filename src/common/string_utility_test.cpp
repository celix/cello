#include <gtest/gtest.h>

#include "common/string_utility.h"

TEST(TestSplit, All) {
    string ss1;
    vector<string> v1;
    StringUtility::Split(ss1, ' ', &v1);
    EXPECT_TRUE(v1.empty());
    
    string ss2 = "test test";
    vector<string> v2;
    StringUtility::Split(ss2, ' ', &v2);
    EXPECT_EQ(v2.size(), static_cast<unsigned int>(2));
    
    string ss3 = "test";
    vector<string> v3;
    StringUtility::Split(ss3, ' ', &v3);
    EXPECT_EQ(v3.size(), static_cast<unsigned int>(1));

    string ss4 = "test ";
    vector<string> v4;
    StringUtility::Split(ss4, ' ', &v4);
    EXPECT_EQ(v4.size(), static_cast<unsigned int>(1));
}

TEST(ArrayTest, All) {
    vector<string> vv(4, "test");
    char ** array = StringUtility::CreateArgArray(vv);
    for (int i = 0; i < 4; ++i)
        EXPECT_STREQ("test", array[i]);
    StringUtility::DestoryArgArray(array);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
