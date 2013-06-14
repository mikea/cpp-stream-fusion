#include "gtest/gtest.h"
#include "vector.h"

TEST(VectorTest, SumAndLength) {
  // TODO: assert that no temps were created.
  Vector a(1024), b(1024), c(1024);
  auto d = a + b + c;
  EXPECT_EQ(0, d.length());
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

