#ifndef FINITE_AUTOMATON_HPP
#define FINITE_AUTOMATON_HPP

#include <expected>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <utility>
#include <vector>

class FiniteAutomaton
{
  public:
    inline static const char epsilon_transition_value = '~';

    static std::expected<FiniteAutomaton, std::string> construct(
        const std::set<char> &alphabet, const std::set<unsigned> &states, const std::set<unsigned> &initial_states,
        const std::set<unsigned> &final_states,
        const std::map<std::pair<unsigned, char>, std::set<unsigned>> &transition_function);

    static std::expected<FiniteAutomaton, std::string> construct(const std::string &regex);

    bool accepts(const std::string &word) const;
    std::vector<std::set<unsigned>> generate_match_steps(const std::string &word) const;

    FiniteAutomaton determinize() const;
    FiniteAutomaton complete() const;
    FiniteAutomaton reverse() const;
    FiniteAutomaton minimize() const;
    FiniteAutomaton complement() const;

    FiniteAutomaton union_with(const FiniteAutomaton &other) const;
    FiniteAutomaton intersection_with(const FiniteAutomaton &other) const;
    FiniteAutomaton difference_with(const FiniteAutomaton &other) const;

    std::optional<std::string> generate_regex() const;
    std::optional<std::string> generate_valid_word() const;
    std::optional<std::string> generate_invalid_word() const;

    const std::set<char> &get_alphabet() const;
    const std::set<unsigned> &get_states() const;
    const std::set<unsigned> &get_initial_states() const;
    const std::set<unsigned> &get_final_states() const;
    const std::map<std::pair<unsigned, char>, std::set<unsigned>> &get_transition_function() const;

  private:
    FiniteAutomaton(
        const std::set<char> &alphabet, const std::set<unsigned> &states, const std::set<unsigned> &initial_states,
        const std::set<unsigned> &final_states,
        const std::map<std::pair<unsigned, char>, std::set<unsigned>> &transition_function);

    std::set<unsigned> epsilon_closure(const std::set<unsigned> &from_states) const;
    FiniteAutomaton product_operation(const FiniteAutomaton &other, const auto &operation) const;

    std::set<char> m_alphabet;
    std::set<unsigned> m_states;
    std::set<unsigned> m_initial_states;
    std::set<unsigned> m_final_states;
    std::map<std::pair<unsigned, char>, std::set<unsigned>> m_transition_function;
};

#endif // FINITE_AUTOMATON_HPP
