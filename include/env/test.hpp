#ifndef ENV_TEST_HPP
#define ENV_TEST_HPP


#include <env/env.hpp>
#include <env/macros.hpp>

using namespace env::syntax;


#include <gtest/gtest.h>
#include <gmock/gmock.h>


int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::InitGoogleMock(&argc, argv);

  return RUN_ALL_TESTS();
}


#endif // ENV_TEST_HPP
