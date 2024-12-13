#include <gtest/gtest.h>
#include "utils/math.h"
TEST(MathTest, AddTest) {
    ASSERT_EQ(add(3, 5), 8);  // Tests if 3 + 5 == 8
}

TEST(MathTest2, AddNegativeTest) {
    ASSERT_EQ(add(-3, -5), -8);  // Tests if -3 + -5 == -8
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
