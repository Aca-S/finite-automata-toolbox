#include "finite_automaton.hpp"
#include "regex_driver.hpp"

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

namespace {

template <class... Ts> struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

using tf_t = std::map<std::pair<unsigned, char>, std::set<unsigned>>;

std::pair<tf_t, unsigned> compile_regex(const RegexAST &ast, unsigned start_state)
{
    const auto &eps = FiniteAutomaton::epsilon_transition_value;
    return std::visit(
        overloaded{
            [start_state](const ConcatenationAST &node) {
                auto [transition_function_left, end_state_left] = compile_regex(node.get_left(), start_state);
                auto [transition_function_right, end_state_right] = compile_regex(node.get_right(), end_state_left);
                transition_function_right.merge(transition_function_left);
                return std::make_pair(transition_function_right, end_state_right);
            },
            [start_state](const AlternationAST &node) {
                auto [transition_function_left, end_state_left] = compile_regex(node.get_left(), start_state + 1);
                auto [transition_function_right, end_state_right] = compile_regex(node.get_right(), end_state_left + 1);
                transition_function_right.merge(transition_function_left);
                transition_function_right[{start_state, eps}].insert(start_state + 1);
                transition_function_right[{start_state, eps}].insert(end_state_left + 1);
                transition_function_right[{end_state_left, eps}].insert(end_state_right + 1);
                transition_function_right[{end_state_right, eps}].insert(end_state_right + 1);
                return std::make_pair(transition_function_right, end_state_right + 1);
            },
            [start_state](const ZeroOrOneAST &node) {
                auto [transition_function, end_state] = compile_regex(node.get_operand(), start_state + 1);
                transition_function[{start_state, eps}].insert(start_state + 1);
                transition_function[{start_state, eps}].insert(end_state + 1);
                transition_function[{end_state, eps}].insert(end_state + 1);
                return std::make_pair(transition_function, end_state + 1);
            },
            [start_state](const ZeroOrMoreAST &node) {
                auto [transition_function, end_state] = compile_regex(node.get_operand(), start_state + 1);
                transition_function[{start_state, eps}].insert(start_state + 1);
                transition_function[{start_state, eps}].insert(end_state + 1);
                transition_function[{end_state, eps}].insert(start_state + 1);
                transition_function[{end_state, eps}].insert(end_state + 1);
                return std::make_pair(transition_function, end_state + 1);
            },
            [start_state](const OneOrMoreAST &node) {
                auto [transition_function, end_state] = compile_regex(node.get_operand(), start_state + 1);
                transition_function[{start_state, eps}].insert(start_state + 1);
                transition_function[{end_state, eps}].insert(start_state + 1);
                transition_function[{end_state, eps}].insert(end_state + 1);
                return std::make_pair(transition_function, end_state + 1);
            },
            [start_state](const SymbolAST &node) {
                tf_t transition_function;
                transition_function[{start_state, node.get_symbol()}].insert(start_state + 1);
                return std::make_pair(transition_function, start_state + 1);
            }},
        ast);
}
} // namespace

std::expected<FiniteAutomaton, std::string> FiniteAutomaton::construct(const std::string &regex)
{
    RegexDriver driver;
    auto ast = driver.parse(regex);

    if (!ast)
        return std::unexpected("Regex parsing error");

    auto [transition_function, end_state] = compile_regex(*ast, 0);

    auto alphabet_range = std::views::keys(transition_function) | std::views::elements<1>
                          | std::views::filter([](unsigned symbol) { return symbol != epsilon_transition_value; });
    std::set<char> alphabet(alphabet_range.begin(), alphabet_range.end());

    std::set<unsigned> states;
    for (unsigned s = 0; s <= end_state; ++s)
        states.insert(s);

    return FiniteAutomaton(alphabet, states, {0}, {end_state}, transition_function);
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
        if (!after_transition_states.empty())
            current_states = epsilon_closure(after_transition_states);
        else
            return false;
    }

    return std::ranges::any_of(current_states, [this](const auto &s) { return m_final_states.contains(s); });
}

FiniteAutomaton::FiniteAutomaton(
    const std::set<char> &alphabet, const std::set<unsigned> &states, const std::set<unsigned> &initial_states,
    const std::set<unsigned> &final_states,
    const std::map<std::pair<unsigned, char>, std::set<unsigned>> &transition_function)
    : m_alphabet(alphabet), m_states(states), m_initial_states(initial_states), m_final_states(final_states),
      m_transition_function(transition_function)
{
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
