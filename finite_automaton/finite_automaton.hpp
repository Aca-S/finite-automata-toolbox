#ifndef FINITE_AUTOMATON_HPP
#define FINITE_AUTOMATON_HPP

#include <expected>
#include <map>
#include <set>
#include <string>
#include <utility>

class FiniteAutomaton
{
  public:
    inline static const char epsilon_transition_value = 0;

    static std::expected<FiniteAutomaton, std::string> construct(
        const std::set<char> &alphabet, const std::set<unsigned> &states, const std::set<unsigned> &initial_states,
        const std::set<unsigned> &final_states,
        const std::map<std::pair<unsigned, char>, std::set<unsigned>> &transition_function);

    static std::expected<FiniteAutomaton, std::string> construct(const std::string &regex);

    bool accepts(const std::string &word) const;

    FiniteAutomaton determinize() const;
    FiniteAutomaton complete() const;
    FiniteAutomaton reverse() const;

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

    std::set<char> m_alphabet;
    std::set<unsigned> m_states;
    std::set<unsigned> m_initial_states;
    std::set<unsigned> m_final_states;
    std::map<std::pair<unsigned, char>, std::set<unsigned>> m_transition_function;
};

#endif // FINITE_AUTOMATON_HPP
