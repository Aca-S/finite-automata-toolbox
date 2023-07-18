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

FiniteAutomaton FiniteAutomaton::determinize() const
{
    std::set<unsigned> determinized_states;
    std::set<unsigned> determinized_final_states;
    std::map<std::pair<unsigned, char>, std::set<unsigned>> determinized_transition_function;

    std::map<std::set<unsigned>, unsigned> constructed_subsets;

    unsigned state_counter = 0;
    const auto initial_subset = epsilon_closure(m_initial_states);
    constructed_subsets[initial_subset] = state_counter;

    // As an optimization, could use a bijective map structure
    // and instead of sets, store their state tags in the queue.
    std::queue<std::set<unsigned>> subset_queue;
    subset_queue.push(initial_subset);

    while (!subset_queue.empty()) {
        const auto current_subset = subset_queue.front();
        subset_queue.pop();

        const auto current_state = constructed_subsets[current_subset];
        determinized_states.insert(current_state);
        if (std::ranges::any_of(current_subset, [this](const auto &state) { return m_final_states.contains(state); }))
            determinized_final_states.insert(current_state);

        for (const auto &symbol : m_alphabet) {
            std::set<unsigned> new_subset;
            for (const auto &state : current_subset) {
                auto it = m_transition_function.find({state, symbol});
                if (it != m_transition_function.end())
                    new_subset.merge(epsilon_closure(it->second));
            }

            // This implementation supposes that deterministic
            // automata do not have to be complete, thus an error
            // state is not created in case there are no transitions
            // by a symbol in the starting automaton.
            if (new_subset.empty())
                continue;

            auto it = constructed_subsets.find(new_subset);
            if (it == constructed_subsets.end()) {
                constructed_subsets[new_subset] = ++state_counter;
                subset_queue.push(new_subset);
            }
            determinized_transition_function[{current_state, symbol}].insert(constructed_subsets[new_subset]);
        }
    }

    return FiniteAutomaton(
        m_alphabet, determinized_states, {0}, determinized_final_states, determinized_transition_function);
}

FiniteAutomaton FiniteAutomaton::complete() const
{
    auto complete_transition_function = m_transition_function;
    auto complete_states = m_states;

    // More efficient to just grab the last element since std::set is sorted,
    // but we're leaving it like this for a more seamless potential container type switch.
    const auto error_state = *std::ranges::max_element(m_states) + 1;
    bool error_state_added = false;

    for (const auto &state : m_states) {
        for (const auto &symbol : m_alphabet) {
            if (m_transition_function.find({state, symbol}) == m_transition_function.end()) {
                error_state_added = true;
                complete_transition_function[{state, symbol}].insert(error_state);
            }
        }
    }

    if (error_state_added) {
        complete_states.insert(error_state);
        for (const auto &symbol : m_alphabet)
            complete_transition_function[{error_state, symbol}].insert(error_state);
    }

    return FiniteAutomaton(m_alphabet, complete_states, m_initial_states, m_final_states, complete_transition_function);
}

FiniteAutomaton FiniteAutomaton::reverse() const
{
    std::map<std::pair<unsigned, char>, std::set<unsigned>> reverse_transition_function;

    for (const auto &[k, v] : m_transition_function) {
        for (const auto &state : v)
            reverse_transition_function[{state, k.second}].insert(k.first);
    }

    return FiniteAutomaton(m_alphabet, m_states, m_final_states, m_initial_states, reverse_transition_function);
}

FiniteAutomaton FiniteAutomaton::minimize() const
{
    // Brzozowski's minimization.
    return reverse().determinize().reverse().determinize();
}

FiniteAutomaton FiniteAutomaton::complement() const
{
    const auto complete_dfa = determinize().complete();

    std::set<unsigned> complement_final_states;
    std::ranges::set_difference(
        complete_dfa.m_states, complete_dfa.m_final_states,
        std::inserter(complement_final_states, complement_final_states.end()));

    return FiniteAutomaton(
        m_alphabet, complete_dfa.m_states, complete_dfa.m_initial_states, complement_final_states,
        complete_dfa.m_transition_function);
}

FiniteAutomaton FiniteAutomaton::union_with(const FiniteAutomaton &other) const
{
    return product_operation(other, [](bool a, bool b) { return a || b; });
}

FiniteAutomaton FiniteAutomaton::intersection_with(const FiniteAutomaton &other) const
{
    return product_operation(other, [](bool a, bool b) { return a && b; });
}

FiniteAutomaton FiniteAutomaton::difference_with(const FiniteAutomaton &other) const
{
    return product_operation(other, [](bool a, bool b) { return a && !b; });
}

namespace {

unsigned precedence(const RegexAST &ast)
{
    return std::visit(
        overloaded{
            [](const ConcatenationAST &node) { return 1; }, [](const AlternationAST &node) { return 0; },
            [](const ZeroOrOneAST &node) { return 2; }, [](const ZeroOrMoreAST &node) { return 2; },
            [](const OneOrMoreAST &node) { return 2; }, [](const SymbolAST &node) { return 3; }},
        ast);
}

std::string ast_to_string(const RegexAST &ast);

std::string node_to_string(const RegexAST &node, const RegexAST &parent)
{
    if (precedence(node) < precedence(parent))
        return "(" + ast_to_string(node) + ")";
    else
        return ast_to_string(node);
}

std::string ast_to_string(const RegexAST &ast)
{
    return std::visit(
        overloaded{
            [&ast](const ConcatenationAST &node) {
                auto left = node_to_string(node.get_left(), ast);
                auto right = node_to_string(node.get_right(), ast);
                return left + right;
            },
            [&ast](const AlternationAST &node) {
                auto left = node_to_string(node.get_left(), ast);
                auto right = node_to_string(node.get_right(), ast);
                return left + "|" + right;
            },
            [&ast](const ZeroOrOneAST &node) { return node_to_string(node.get_operand(), ast) + "?"; },
            [&ast](const ZeroOrMoreAST &node) { return node_to_string(node.get_operand(), ast) + "*"; },
            [&ast](const OneOrMoreAST &node) { return node_to_string(node.get_operand(), ast) + "+"; },
            [&ast](const SymbolAST &node) {
                if (node.get_symbol() == FiniteAutomaton::epsilon_transition_value)
                    return std::string("");
                else
                    return std::string(1, node.get_symbol());
            }},
        ast);
}
} // namespace

std::optional<std::string> FiniteAutomaton::generate_regex() const
{
    const auto &eps = FiniteAutomaton::epsilon_transition_value;

    // Node: Can also just be determinized instead of minimized,
    // but this way the resulting regex will be shorter.
    auto automaton = minimize().complete();

    unsigned new_initial = automaton.m_states.size();
    unsigned new_final = new_initial + 1;

    automaton.m_states.insert({new_initial, new_final});

    automaton.m_transition_function[{new_initial, eps}].insert(*automaton.m_initial_states.begin());
    for (const auto &state : automaton.m_final_states)
        automaton.m_transition_function[{state, eps}].insert(new_final);

    automaton.m_initial_states = {new_initial};
    automaton.m_final_states = {new_final};

    std::map<std::pair<unsigned, unsigned>, std::string> label_map;
    for (const auto &[k, v] : automaton.m_transition_function) {
        for (const auto &to_state : v) {
            auto symbol_node = std::string(1, k.second);
            auto it = label_map.find({k.first, to_state});
            if (it == label_map.end())
                label_map.insert({{k.first, to_state}, symbol_node});
            else
                it->second = it->second + "|" + symbol_node;
        }
    }

    auto states_copy = automaton.m_states;
    for (const auto &q_state : automaton.m_states) {
        if (q_state == new_initial || q_state == new_final)
            continue;

        states_copy.erase(q_state);
        for (const auto &p_state : states_copy) {
            for (const auto &r_state : states_copy) {
                auto pq = label_map.find({p_state, q_state});
                auto qr = label_map.find({q_state, r_state});
                if (pq == label_map.end() || qr == label_map.end())
                    continue;
                auto qq = label_map.find({q_state, q_state});
                auto pr = label_map.find({p_state, r_state});

                auto result = "(" + pq->second + ")";
                if (qq != label_map.end())
                    result = result + "(" + qq->second + ")*";
                result = result + "(" + qr->second + ")";
                if (pr != label_map.end())
                    result = result + "|" + "(" + pr->second + ")";

                label_map[{p_state, r_state}] = result;
            }
        }
    }

    // TODO: Work on getting a value based recursive variant
    // up and running for the regex AST, which would allow for
    // the direct creation of ASTs instead of string labels.
    auto it = label_map.find({new_initial, new_final});
    if (it == label_map.end())
        return std::nullopt;
    else
        return ast_to_string(*RegexDriver().parse(it->second));
}

const std::set<char> &FiniteAutomaton::get_alphabet() const { return m_alphabet; }

const std::set<unsigned> &FiniteAutomaton::get_states() const { return m_states; }

const std::set<unsigned> &FiniteAutomaton::get_initial_states() const { return m_initial_states; }

const std::set<unsigned> &FiniteAutomaton::get_final_states() const { return m_final_states; }

const std::map<std::pair<unsigned, char>, std::set<unsigned>> &FiniteAutomaton::get_transition_function() const
{
    return m_transition_function;
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

FiniteAutomaton FiniteAutomaton::product_operation(const FiniteAutomaton &other, const auto &operation) const
{
    std::set<char> alphabet_union;
    std::ranges::set_union(m_alphabet, other.m_alphabet, std::inserter(alphabet_union, alphabet_union.end()));

    const auto automaton_a =
        FiniteAutomaton(alphabet_union, m_states, m_initial_states, m_final_states, m_transition_function)
            .determinize()
            .complete();
    const auto automaton_b =
        FiniteAutomaton(
            alphabet_union, other.m_states, other.m_initial_states, other.m_final_states, other.m_transition_function)
            .determinize()
            .complete();

    // Note: In order for the product numbering to work, the states of each automaton must form a continuous
    // sequence. Luckily, that is already guaranteed with the FiniteAutomaton::determinize() method.
    const unsigned num_of_states_b = automaton_b.m_states.size();

    std::set<unsigned> product_states, product_initial_states, product_final_states;
    std::map<std::pair<unsigned, char>, std::set<unsigned>> product_transition_function;

    for (const auto &state_a : automaton_a.m_states) {
        for (const auto &state_b : automaton_b.m_states) {
            const unsigned from_state = num_of_states_b * state_a + state_b;

            product_states.insert(from_state);
            if (automaton_a.m_initial_states.contains(state_a) && automaton_b.m_initial_states.contains(state_b))
                product_initial_states.insert(from_state);
            if (operation(automaton_a.m_final_states.contains(state_a), automaton_b.m_final_states.contains(state_b)))
                product_final_states.insert(from_state);

            for (const auto &symbol : alphabet_union) {
                const auto state_a_to = *automaton_a.m_transition_function.find({state_a, symbol})->second.begin();
                const auto state_b_to = *automaton_b.m_transition_function.find({state_b, symbol})->second.begin();
                const unsigned to_state = num_of_states_b * state_a_to + state_b_to;
                product_transition_function[{from_state, symbol}].insert(to_state);
            }
        }
    }

    return FiniteAutomaton(
        alphabet_union, product_states, product_initial_states, product_final_states, product_transition_function);
}
