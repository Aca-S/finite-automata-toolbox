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
        auto eps = FiniteAutomaton::epsilon_transition_value;

        auto ends_with_ab_e = FiniteAutomaton::construct(
            {'a', 'b'}, {0, 1, 2}, {0}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}});
        ASSERT_TRUE(ends_with_ab_e);
        ends_with_ab = new FiniteAutomaton(*ends_with_ab_e);

        auto even_num_of_a_e = FiniteAutomaton::construct(
            {'a', 'b'}, {0, 1}, {0}, {0}, {{{0, 'a'}, {1}}, {{0, 'b'}, {0}}, {{1, 'a'}, {0}}, {{1, 'b'}, {1}}});
        ASSERT_TRUE(even_num_of_a_e);
        even_num_of_a = new FiniteAutomaton(*even_num_of_a_e);

        auto empty_word_e = FiniteAutomaton::construct({}, {0}, {0}, {0}, {{{0, eps}, {0}}});
        ASSERT_TRUE(empty_word_e);
        empty_word = new FiniteAutomaton(*empty_word_e);
    }

    void TearDown() override
    {
        delete ends_with_ab;
        delete even_num_of_a;
        delete empty_word;
    }

    FiniteAutomaton *ends_with_ab;
    FiniteAutomaton *even_num_of_a;
    FiniteAutomaton *empty_word;
};

TEST_F(FiniteAutomatonTest, Accept)
{
    for (const auto &word : {"ab", "abab", "bbbabbbaaabbabaab"})
        ASSERT_TRUE(ends_with_ab->accepts(word));
        
    for (const auto &word : {"", "babababaqabab", "ba"})
        ASSERT_FALSE(ends_with_ab->accepts(word));
        
    for (const auto &word : {"", "aaaa", "baaabaaab"})
        ASSERT_TRUE(even_num_of_a->accepts(word));
        
    for (const auto &word : {"", "aaaa", "baaabaaab"})
        ASSERT_TRUE(even_num_of_a->accepts(word));
        
    for (const auto &word : {"a", "aaabbb", "bababa"})
        ASSERT_FALSE(even_num_of_a->accepts(word));
        
    ASSERT_TRUE(empty_word->accepts(""));
    ASSERT_FALSE(empty_word->accepts("10101"));
}
