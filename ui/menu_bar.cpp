#include "menu_bar.hpp"
#include "utility.hpp"

#include <QFileDialog>
#include <QMessageBox>

#include "automata_scene.hpp"
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
    m_save_action = m_file_menu->addAction("Save");
    m_save_as_action = m_file_menu->addAction("Save As");
    m_close_action = m_file_menu->addAction("Close");
}

void MenuBar::setup_file_menu()
{
    connect(m_new_action, &QAction::triggered, this, [=]() { m_scene_tab_bar->add_scene_tab(); });

    connect(m_open_action, &QAction::triggered, this, [=]() { open_with_dialog(); });

    connect(m_save_action, &QAction::triggered, this, [=]() {
        auto scene_name = m_scene_tab_bar->get_scene()->get_name();
        if (scene_name.isEmpty())
            save_with_dialog();
        else
            save_file(scene_name);
    });

    connect(m_save_as_action, &QAction::triggered, this, [=]() { save_with_dialog(); });

    connect(m_close_action, &QAction::triggered, this, [=]() { m_scene_tab_bar->remove_scene_tab(); });
}

void MenuBar::save_file(const QString &file_name)
{
    auto error = m_scene_tab_bar->get_scene()->save_to_file(file_name);
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
        m_scene_tab_bar->update_scene_tab_name();
    }
}

void MenuBar::open_with_dialog()
{
    QString file_name =
        QFileDialog::getOpenFileName(this, "Open File", "", "Finite Automata Toolbox File (*.fat);;All Files (*)");
    if (file_name.isEmpty())
        return;
    else {
        auto scene = AutomataScene::load_from_file(file_name, m_scene_tab_bar);
        if (scene)
            m_scene_tab_bar->add_scene_tab(*scene);
        else
            QMessageBox::information(this, "File error", scene.error());
    }
}
