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

TEST(FiniteAutomatonConstruct, RegexSymbol)
{
    auto fa = FiniteAutomaton::construct("a");
    ASSERT_TRUE(fa);

    EXPECT_TRUE(fa.value().accepts("a"));

    EXPECT_FALSE(fa.value().accepts("aa"));
    EXPECT_FALSE(fa.value().accepts("b"));
    EXPECT_FALSE(fa.value().accepts(""));
}

TEST(FiniteAutomatonConstruct, RegexOneOrMore)
{
    auto fa = FiniteAutomaton::construct("a+");
    ASSERT_TRUE(fa);

    EXPECT_TRUE(fa.value().accepts("a"));
    EXPECT_TRUE(fa.value().accepts("aa"));
    EXPECT_TRUE(fa.value().accepts("aaa"));

    EXPECT_FALSE(fa.value().accepts(""));
}

TEST(FiniteAutomatonConstruct, RegexZeroOrMore)
{
    auto fa = FiniteAutomaton::construct("a*");
    ASSERT_TRUE(fa);

    EXPECT_TRUE(fa.value().accepts(""));
    EXPECT_TRUE(fa.value().accepts("a"));
    EXPECT_TRUE(fa.value().accepts("aa"));

    EXPECT_FALSE(fa.value().accepts("abababab"));
}

TEST(FiniteAutomatonConstruct, RegexZeroOrOne)
{
    auto fa = FiniteAutomaton::construct("a?");
    ASSERT_TRUE(fa);

    EXPECT_TRUE(fa.value().accepts(""));
    EXPECT_TRUE(fa.value().accepts("a"));

    EXPECT_FALSE(fa.value().accepts("aa"));
}

TEST(FiniteAutomatonConstruct, RegexAlternation)
{
    auto fa = FiniteAutomaton::construct("a|b");
    ASSERT_TRUE(fa);

    EXPECT_TRUE(fa.value().accepts("a"));
    EXPECT_TRUE(fa.value().accepts("b"));

    EXPECT_FALSE(fa.value().accepts(""));
    EXPECT_FALSE(fa.value().accepts("c"));
    EXPECT_FALSE(fa.value().accepts("ab"));
    EXPECT_FALSE(fa.value().accepts("aa"));
    EXPECT_FALSE(fa.value().accepts("bb"));
}

TEST(FiniteAutomatonConstruct, RegexConcatenation)
{
    auto fa = FiniteAutomaton::construct("ab");
    ASSERT_TRUE(fa);

    EXPECT_TRUE(fa.value().accepts("ab"));

    EXPECT_FALSE(fa.value().accepts(""));
    EXPECT_FALSE(fa.value().accepts("ba"));
    EXPECT_FALSE(fa.value().accepts("aa"));
    EXPECT_FALSE(fa.value().accepts("bb"));
}

TEST(FiniteAutomatonConstruct, RegexComplex)
{
    auto fa = FiniteAutomaton::construct("(ab|b*a+)*");
    ASSERT_TRUE(fa);

    EXPECT_TRUE(fa.value().accepts(""));
    EXPECT_TRUE(fa.value().accepts("ab"));
    EXPECT_TRUE(fa.value().accepts("ababaaaaa"));
    EXPECT_TRUE(fa.value().accepts("aaaaaaa"));
    EXPECT_TRUE(fa.value().accepts("baaaabbbbbaaa"));

    EXPECT_FALSE(fa.value().accepts("abb"));
    EXPECT_FALSE(fa.value().accepts("bbbbbbbbbb"));
}

TEST(FiniteAutomatonConstruct, RegexInvalid)
{
    EXPECT_FALSE(FiniteAutomaton::construct("")) << "Empty string is not a valid regex";
    EXPECT_FALSE(FiniteAutomaton::construct("()"));
    EXPECT_FALSE(FiniteAutomaton::construct("a+?*")) << "Chaining unary operators without parentheses is invalid";
    EXPECT_FALSE(FiniteAutomaton::construct("a|"));
}

class FiniteAutomatonTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        auto eps = FiniteAutomaton::epsilon_transition_value;

        // With the assumption that the automatons are properly constructed.
        ends_with_ab = new FiniteAutomaton(*FiniteAutomaton::construct(
            {'a', 'b'}, {0, 1, 2}, {0}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}}));
        even_num_of_a = new FiniteAutomaton(*FiniteAutomaton::construct(
            {'a', 'b'}, {0, 1}, {0}, {0}, {{{0, 'a'}, {1}}, {{0, 'b'}, {0}}, {{1, 'a'}, {0}}, {{1, 'b'}, {1}}}));
        empty_word = new FiniteAutomaton(*FiniteAutomaton::construct({}, {0}, {0}, {0}, {{{0, eps}, {0}}}));

        ends_with_aab_r = new FiniteAutomaton(*FiniteAutomaton::construct("(a|b)*aab"));
    }

    void TearDown() override
    {
        delete ends_with_ab;
        delete even_num_of_a;
        delete empty_word;
        delete ends_with_aab_r;
    }

    FiniteAutomaton *ends_with_ab;
    FiniteAutomaton *even_num_of_a;
    FiniteAutomaton *empty_word;

    FiniteAutomaton *ends_with_aab_r;
};

TEST_F(FiniteAutomatonTest, Accept)
{
    for (const auto &word : {"ab", "abab", "bbbabbbaaabbabaab"})
        EXPECT_TRUE(ends_with_ab->accepts(word));

    for (const auto &word : {"", "babababaqabab", "ba"})
        EXPECT_FALSE(ends_with_ab->accepts(word));

    for (const auto &word : {"", "aaaa", "baaabaaab"})
        EXPECT_TRUE(even_num_of_a->accepts(word));

    for (const auto &word : {"", "aaaa", "baaabaaab"})
        EXPECT_TRUE(even_num_of_a->accepts(word));

    for (const auto &word : {"a", "aaabbb", "bababa"})
        EXPECT_FALSE(even_num_of_a->accepts(word));

    EXPECT_TRUE(empty_word->accepts(""));
    EXPECT_FALSE(empty_word->accepts("10101"));
}

TEST_F(FiniteAutomatonTest, Determinize)
{
    auto eps = FiniteAutomaton::epsilon_transition_value;

    FiniteAutomaton d_ends_with_aab_r = ends_with_aab_r->determinize();

    EXPECT_TRUE(d_ends_with_aab_r.get_initial_states().size() == 1) << "DFA must have exactly one start state";
    for (const auto &[k, v] : d_ends_with_aab_r.get_transition_function()) {
        EXPECT_TRUE(k.second != eps) << "DFA must not contain epsilon values";
        EXPECT_TRUE(v.size() == 1) << "DFA must have at most one transition state per alphabet symbol";
    }

    for (const auto &word : {"aab", "bababaaaaaab", "aaaaabbbbaaaaabbbaab"})
        EXPECT_TRUE(d_ends_with_aab_r.accepts(word));

    for (const auto &word : {"", "abbabababbbbaba", "aaacabbaaab"})
        EXPECT_FALSE(d_ends_with_aab_r.accepts(word));
}

TEST_F(FiniteAutomatonTest, Complete)
{
    FiniteAutomaton c_ends_with_aab_r = ends_with_aab_r->complete();
    const auto &tf = c_ends_with_aab_r.get_transition_function();

    for (const auto &state : c_ends_with_aab_r.get_states()) {
        for (const auto &symbol : c_ends_with_aab_r.get_alphabet())
            EXPECT_TRUE(tf.find({state, symbol}) != tf.end())
                << "A complete FA must have a transition from every state by every symbol of its alphabet";
    }
}
