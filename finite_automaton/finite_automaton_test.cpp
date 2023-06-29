#include "finite_automaton.hpp"

#include <gtest/gtest.h>

TEST(FiniteAutomatonConstruct, ByMember)
{
    auto eps = FiniteAutomaton::epsilon_transition_value;

    EXPECT_TRUE(FiniteAutomaton::construct({}, {}, {}, {}, {})) << "A stateless automaton is a valid automaton";

    EXPECT_TRUE(FiniteAutomaton::construct(
        {'a', 'b'}, {0, 1, 2}, {0}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}}));

    EXPECT_TRUE(FiniteAutomaton::construct(
        {'a', 'b'}, {0, 1, 2}, {0}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}, {{2, eps}, {0}}}))
        << "Epsilon transitions should be allowed in the transition function";

    EXPECT_FALSE(FiniteAutomaton::construct(
        {'a', 'b', eps}, {0, 1, 2}, {0}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}}))
        << "The epsilon transition value must not be contained in the alphabet";

    EXPECT_FALSE(FiniteAutomaton::construct(
        {'a', 'b'}, {0, 1, 2}, {0, 700}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}}))
        << "The initial states must be a subset of the automaton states";

    EXPECT_FALSE(FiniteAutomaton::construct(
        {'a', 'b'}, {0, 1, 2}, {0}, {2, 700}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}}))
        << "The final states must be a subset of the automaton states";

    EXPECT_FALSE(FiniteAutomaton::construct(
        {'a', 'b'}, {0, 1, 2}, {0}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{700, 'b'}, {2}}}))
        << "The states in the transition function must form a subset of the automaton states";

    EXPECT_FALSE(FiniteAutomaton::construct(
        {'a', 'b'}, {0, 1, 2}, {0}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {700}}, {{1, 'b'}, {2}}}))
        << "The states in the transition function must form a subset of the automaton states";

    EXPECT_FALSE(FiniteAutomaton::construct(
        {'a', 'b'}, {0, 1, 2}, {0}, {2}, {{{0, 'k'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}}))
        << "The symbols in the transition function, excluding eps, must form a subset of the alphabet";
}

class FiniteAutomatonTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        auto a = FiniteAutomaton::construct(
            {'a', 'b'}, {0, 1, 2}, {0}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}});
        ASSERT_TRUE(a);
        ends_with_ab = *a;
    }

    FiniteAutomaton ends_with_ab;
};
