#include "menu_bar.hpp"
#include "utility.hpp"

#include <QDataStream>
#include <QFile>

#include <iostream>

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"

using namespace Ui;
using namespace Ui::Utility;

MenuBar::MenuBar(QGraphicsView *main_view, QWidget *parent) : QMenuBar(parent), m_main_view(main_view)
{
    build_file_menu();
    setup_file_menu();
}

void MenuBar::build_file_menu()
{
    m_file_menu = this->addMenu("File");
    m_new_action = m_file_menu->addAction("New");
    m_open_action = m_file_menu->addAction("Open");
    m_save_action = m_file_menu->addAction("Save");
    m_save_as_action = m_file_menu->addAction("Save As");
    m_close_action = m_file_menu->addAction("Close");
}

namespace {
void serialize_automaton(QDataStream &out, const FiniteAutomaton &automaton)
{
    QSet<char> alphabet(automaton.get_alphabet().begin(), automaton.get_alphabet().end());
    QSet<unsigned> states(automaton.get_states().begin(), automaton.get_states().end());
    QSet<unsigned> initial_states(automaton.get_initial_states().begin(), automaton.get_initial_states().end());
    QSet<unsigned> final_states(automaton.get_final_states().begin(), automaton.get_final_states().end());
    QMap<QPair<unsigned, char>, QSet<unsigned>> transition_function;
    for (const auto &[k, v] : automaton.get_transition_function())
        transition_function.insert(k, QSet<unsigned>(v.begin(), v.end()));

    out << alphabet << states << initial_states << final_states << transition_function;
}

FiniteAutomaton deserialize_automaton(QDataStream &in)
{
    QSet<char> q_alphabet;
    QSet<unsigned> q_states, q_initial_states, q_final_states;
    QMap<QPair<unsigned, char>, QSet<unsigned>> q_transition_function;

    in >> q_alphabet >> q_states >> q_initial_states >> q_final_states >> q_transition_function;

    std::set<char> alphabet(q_alphabet.begin(), q_alphabet.end());
    std::set<unsigned> states(q_states.begin(), q_states.end());
    std::set<unsigned> initial_states(q_initial_states.begin(), q_initial_states.end());
    std::set<unsigned> final_states(q_final_states.begin(), q_final_states.end());
    std::map<std::pair<unsigned, char>, std::set<unsigned>> transition_function;
    QMapIterator<QPair<unsigned, char>, QSet<unsigned>> it(q_transition_function);
    while (it.hasNext()) {
        it.next();
        transition_function[{it.key().first, it.key().second}] =
            std::set<unsigned>(it.value().begin(), it.value().end());
    }

    return *FiniteAutomaton::construct(alphabet, states, initial_states, final_states, transition_function);
}
} // namespace

void MenuBar::setup_file_menu()
{
    connect(m_save_action, &QAction::triggered, this, [=]() {

    });
}
