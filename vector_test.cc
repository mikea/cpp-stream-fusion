#include "gtest/gtest.h"

#define VECTOR_TEST
#include "vector.h"

TEST(VectorTest, SumAndLength) {
  // TODO: assert that no temps were created.
  int converts = VectorTestSupport::streamToVector;

  Vector a(1024), b(1024), c(1024);
  auto d = a + b + c;
  EXPECT_EQ(0, d.length());
  EXPECT_EQ(converts, VectorTestSupport::streamToVector);
}

int main(int argc, char** argv) {
  VectorTestSupport::streamToVector = 0;

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

