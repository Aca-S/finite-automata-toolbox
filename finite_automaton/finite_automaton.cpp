#include "finite_automaton.hpp"

#include <algorithm>
#include <queue>
#include <ranges>

std::expected<FiniteAutomaton, std::string> FiniteAutomaton::construct(
    const std::set<char> &alphabet, const std::set<unsigned> &states, const std::set<unsigned> &initial_states,
    const std::set<unsigned> &final_states,
    const std::map<std::pair<unsigned, char>, std::set<unsigned>> &transition_function)
{
    const static auto is_subset = [](const auto &is, const auto &of) {
        return std::ranges::all_of(is, [&of](unsigned el) { return of.contains(el); });
    };

    const static auto is_alphabet_subset = [](const auto &is, const auto &of) {
        return std::ranges::all_of(
            is, [&of](unsigned el) { return el == epsilon_transition_value ? true : of.contains(el); });
    };

    if (alphabet.contains(epsilon_transition_value))
        return std::unexpected(
            "Alphabet cannot contain "
            "the epsilon transition "
            "value: "
            + std::string{epsilon_transition_value});

    if (!is_subset(initial_states, states))
        return std::unexpected("Initial states must be a "
                               "subset of states");

    if (!is_subset(final_states, states))
        return std::unexpected("Final states must be a "
                               "subset of states");

    if (!is_subset(std::views::keys(transition_function) | std::views::elements<0>, states))
        return std::unexpected("Transition function "
                               "states must form a subset "
                               "of states");

    if (!std::ranges::all_of(
            std::views::values(transition_function), [&states](const auto &s) { return is_subset(s, states); }))
        return std::unexpected("Transition function "
                               "states must form a subset "
                               "of states");

    if (!is_alphabet_subset(std::views::keys(transition_function) | std::views::elements<1>, alphabet))
        return std::unexpected("Transition function "
                               "symbols must form a "
                               "subset of the alphabet");

    return FiniteAutomaton(alphabet, states, initial_states, final_states, transition_function);
}

bool FiniteAutomaton::accepts(const std::string &word) const
{
    auto current_states = epsilon_closure(m_initial_states);

    for (const auto &symbol : word) {
        std::set<unsigned> after_transition_states;
        const auto ins = std::inserter(after_transition_states, after_transition_states.end());
        for (const auto &state : current_states) {
            auto it = m_transition_function.find({state, symbol});
            if (it != m_transition_function.end())
                std::ranges::set_union(after_transition_states, it->second, ins);
        }
        current_states = epsilon_closure(after_transition_states);
    }

    return std::ranges::any_of(current_states, [this](const auto &s) { return m_final_states.contains(s); });
}

std::set<unsigned> FiniteAutomaton::epsilon_closure(const std::set<unsigned> &from_states) const
{
    auto closure = from_states;

    std::queue<unsigned> state_queue;
    for (const auto &state : from_states)
        state_queue.push(state);

    while (!state_queue.empty()) {
        auto current_state = state_queue.front();
        state_queue.pop();

        auto it = m_transition_function.find({current_state, epsilon_transition_value});
        if (it == m_transition_function.end())
            continue;

        for (const auto &state : it->second) {
            if (!closure.contains(state)) {
                closure.insert(state);
                state_queue.push(state);
            }
        }
    }

    return closure;
}
