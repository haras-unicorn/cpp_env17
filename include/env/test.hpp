#ifndef ENV_TEST_HPP
#define ENV_TEST_HPP


#include <env/env.hpp>
#include <env/macros.hpp>

using namespace env::syntax;


ENV_MSVC_SUPPRESS_PUSH(6326); // constant comparison

#include <gtest/gtest.h>
#include <gmock/gmock.h>

ENV_MSVC_SUPPRESS_POP;

// ignore GoogleTest unused static variable warning
#define ENV_TEST_CASE(_suite, _test) TEST(_suite, _test) // NOLINT(cert-err58-cpp)


int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);

    return RUN_ALL_TESTS();
}


#endif // ENV_TEST_HPP
