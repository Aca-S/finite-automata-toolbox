#include "menu_bar.hpp"
#include "utility.hpp"

#include <QFileDialog>
#include <QMessageBox>

#include "automaton_graph.hpp"
#include "finite_automaton.hpp"

using namespace Ui;
using namespace Ui::Utility;

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    build_file_menu();
    setup_file_menu();

    build_edit_menu();
    setup_edit_menu();
}

void MenuBar::set_scene(AutomataScene *scene) { m_current_scene = scene; }

void MenuBar::build_file_menu()
{
    m_file_menu = this->addMenu("File");
    m_new_action = m_file_menu->addAction("New");
    m_open_action = m_file_menu->addAction("Open");
    m_save_action = m_file_menu->addAction("Save");
    m_save_as_action = m_file_menu->addAction("Save As");
    m_close_action = m_file_menu->addAction("Close");
}

void MenuBar::setup_file_menu()
{
    connect(m_new_action, &QAction::triggered, this, [=]() { emit scene_opened(new AutomataScene); });

    connect(m_open_action, &QAction::triggered, this, [=]() { open_with_dialog(); });

    connect(m_save_action, &QAction::triggered, this, [=]() {
        auto scene_name = m_current_scene->get_name();
        if (scene_name.isEmpty())
            save_with_dialog();
        else
            save_file(scene_name);
    });

    connect(m_save_as_action, &QAction::triggered, this, [=]() { save_with_dialog(); });

    connect(m_close_action, &QAction::triggered, this, [=]() { emit scene_closed(); });
}

void MenuBar::save_file(const QString &file_name)
{
    auto error = m_current_scene->save_to_file(file_name);
    if (error)
        QMessageBox::information(this, "File error", *error);
}

void MenuBar::save_with_dialog()
{
    QString file_name =
        QFileDialog::getSaveFileName(this, "Save File", "", "Finite Automata Toolbox File (*.fat);;All Files (*)");
    if (file_name.isEmpty())
        return;
    else {
        save_file(file_name);
        emit scene_saved_as();
    }
}

void MenuBar::open_with_dialog()
{
    QString file_name =
        QFileDialog::getOpenFileName(this, "Open File", "", "Finite Automata Toolbox File (*.fat);;All Files (*)");
    if (file_name.isEmpty())
        return;
    else {
        auto scene = AutomataScene::load_from_file(file_name);
        if (scene)
            emit scene_opened(*scene);
        else
            QMessageBox::information(this, "File error", scene.error());
    }
}

void MenuBar::build_edit_menu()
{
    m_edit_menu = this->addMenu("&Edit");

    m_undo_action = m_edit_menu->addAction("&Undo");
    m_undo_action->setShortcuts(QKeySequence::Undo);

    m_redo_action = m_edit_menu->addAction("&Redo");
    m_redo_action->setShortcuts(QKeySequence::Redo);
}

void MenuBar::setup_edit_menu()
{
    connect(m_undo_action, &QAction::triggered, this, [=]() { m_current_scene->undo_action(); });

    connect(m_redo_action, &QAction::triggered, this, [=]() { m_current_scene->redo_action(); });
}
