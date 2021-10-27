#include <env/test.hpp>

TEST(env, test)
{
  ::std::vector<int> ints{10};
  EXPECT_EQ(ints.capacity(), 1);

  EXPECT_TRUE(::env::check());
}
