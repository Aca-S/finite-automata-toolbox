#include "finite_automaton.hpp"

#include <gtest/gtest.h>

TEST(SetupTest, EqAssertion) { EXPECT_EQ(2 + 2, 4); }

class FiniteAutomatonTest : public ::testing::Test
{
  protected:
    void SetUp() override {}
};
