#include "menu_bar.hpp"
#include "utility.hpp"

#include <QDataStream>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <expected>

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"

using namespace Ui;
using namespace Ui::Utility;

MenuBar::MenuBar(SceneTabBar *scene_tab_bar, QWidget *parent) : QMenuBar(parent), m_scene_tab_bar(scene_tab_bar)
{
    build_file_menu();
    setup_file_menu();
}

void MenuBar::build_file_menu()
{
    m_file_menu = this->addMenu("File");
    m_new_action = m_file_menu->addAction("New");
    m_open_action = m_file_menu->addAction("Open");
    m_save_as_action = m_file_menu->addAction("Save As");
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

std::expected<FiniteAutomaton, QString> deserialize_automaton(QDataStream &in)
{
    QSet<char> q_alphabet;
    QSet<unsigned> q_states, q_initial_states, q_final_states;
    QMap<QPair<unsigned, char>, QSet<unsigned>> q_transition_function;

    in >> q_alphabet >> q_states >> q_initial_states >> q_final_states >> q_transition_function;
    if (in.status() != QDataStream::Ok)
        return std::unexpected("File format error");

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

    auto automaton = FiniteAutomaton::construct(alphabet, states, initial_states, final_states, transition_function);
    if (!automaton)
        return std::unexpected(QString::fromUtf8(automaton.error().c_str()));

    return *automaton;
}

void serialize_scene(QDataStream &out, QGraphicsScene *scene)
{
    auto graphs = get_items<AutomatonGraph>(scene);
    out << graphs.size();
    for (auto *graph : graphs) {
        serialize_automaton(out, graph->get_automaton());
        out << get_center_pos(graph);
    }
}

std::expected<QGraphicsScene *, QString> deserialize_scene(QDataStream &in)
{
    QGraphicsScene *scene = new QGraphicsScene;
    qsizetype num_of_automata;
    in >> num_of_automata;
    for (qsizetype i = 0; i < num_of_automata; ++i) {
        auto automaton = deserialize_automaton(in);
        QPointF pos;
        in >> pos;
        if (in.status() != QDataStream::Ok || !automaton) {
            delete scene;
            return std::unexpected("File format error");
        }
        add_item_at_pos(new AutomatonGraph(*automaton), scene, pos);
    }

    return scene;
}
} // namespace

void MenuBar::setup_file_menu()
{
    connect(m_new_action, &QAction::triggered, this, [=]() {
        m_scene_tab_bar->add_scene_tab();
        /*delete m_main_view->scene();
        m_main_view->setScene(new QGraphicsScene(m_main_view));
        m_main_view->centerOn(0, 0);*/
    });

    connect(m_save_as_action, &QAction::triggered, this, [=]() {
        /*QString file_name =
            QFileDialog::getSaveFileName(this, "Save File", "", "Finite Automata Toolbox File (*.fat);;All Files (*)");
        if (file_name.isEmpty())
            return;
        else {
            QFile file(file_name);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, "Unable to open file", file.errorString());
                return;
            }
            QDataStream out(&file);
            serialize_scene(out, m_main_view->scene());
            file.close();
        }*/
    });

    connect(m_open_action, &QAction::triggered, this, [=]() {
        /*QString file_name =
            QFileDialog::getOpenFileName(this, "Open File", "", "Finite Automata Toolbox File (*.fat);;All Files (*)");
        if (file_name.isEmpty())
            return;
        else {
            QFile file(file_name);
            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, "Unable to open file", file.errorString());
                return;
            }
            QDataStream in(&file);
            auto scene = deserialize_scene(in);
            file.close();

            // In case the file could not be read due to an invalid format, report the
            // error and return.
            if (!scene) {
                QMessageBox::information(this, "Unable to open file", scene.error());
                return;
            }
            delete m_main_view->scene();
            scene.value()->setParent(m_main_view);
            m_main_view->setScene(*scene);
            m_main_view->centerOn(0, 0);
        }*/
    });
}
