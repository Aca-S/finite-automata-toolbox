#include "finite_automaton.hpp"

#include <iostream>

int main(int argc, char *argv[])
{
    auto aut = FiniteAutomaton::construct(
        {'a', 'b'}, {0, 1, 2}, {0}, {2}, {{{0, 'a'}, {0, 1}}, {{0, 'b'}, {0}}, {{1, 'b'}, {2}}});

    if (aut) {
        std::cout << "aut_1" << std::endl;
        std::cout << aut.value().accepts("abababab") << std::endl;
        std::cout << aut.value().accepts("ababababb") << std::endl;
        std::cout << aut.value().accepts("abaqabab") << std::endl;
    } else
        std::cout << aut.error() << std::endl;

    auto aut_2 = FiniteAutomaton::construct("(ab|b*a+)*");
    if (aut_2) {
        std::cout << "aut_2" << std::endl;
        std::cout << aut_2.value().accepts("ababbbbbba") << std::endl;
    } else
        std::cout << aut_2.error() << std::endl;

    return 0;
}
