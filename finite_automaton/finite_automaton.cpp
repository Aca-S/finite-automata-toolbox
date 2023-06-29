#include "finite_automaton.hpp"

#include <algorithm>
#include <ranges>

std::expected<FiniteAutomaton, std::string> FiniteAutomaton::construct(
    const std::set<char> &alphabet,
    const std::set<unsigned> &states,
    const std::set<unsigned> &initial_states,
    const std::set<unsigned> &final_states,
    const std::map<std::pair<unsigned, char>, std::set<unsigned>> &transition_function)
{
    const static auto is_subset = [](const auto &is, const auto &of) {
        return std::ranges::all_of(is, [&of](unsigned el) { return of.contains(el); });
    };

    const static auto is_alphabet_subset = [](const auto &is, const auto &of) {
        return std::ranges::all_of(is, [&of](unsigned el) { return el == epsilon_transition_value ? true : of.contains(el); });
    };

    if (alphabet.contains(epsilon_transition_value))
        return std::unexpected("Alphabet cannot contain the epsilon transition value: " + std::string{epsilon_transition_value});

    if (!is_subset(initial_states, states))
        return std::unexpected("Initial states must be a subset of states");

    if (!is_subset(final_states, states))
        return std::unexpected("Final states must be a subset of states");

    if (!is_subset(std::views::keys(transition_function) | std::views::elements<0>, states))
        return std::unexpected("Transition function states must form a subset of states");

    if (!std::ranges::all_of(std::views::values(transition_function), [&states](const auto &s) { return is_subset(s, states); }))
        return std::unexpected("Transition function states must form a subset of states");

    if (!is_alphabet_subset(std::views::keys(transition_function) | std::views::elements<1>, alphabet))
        return std::unexpected("Transition function symbols must form a subset of the alphabet");

    return FiniteAutomaton(alphabet, states, initial_states, final_states, transition_function);
}
