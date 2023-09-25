#include "scene_tab_bar.hpp"

#include <QMessageBox>

using namespace Ui;

SceneTabBar::SceneTabBar(QWidget *parent) : QTabBar(parent)
{
    this->setExpanding(false);
    this->setUsesScrollButtons(true);
    this->setDrawBase(false);
    this->setShape(QTabBar::RoundedNorth);

    // Switch displayed scene on active tab change.
    connect(this, &QTabBar::currentChanged, this, [=](int new_current_index) {
        emit scene_changed(m_automata_scenes.at(new_current_index));
    });

    add_scene(new AutomataScene);
}

AutomataScene *SceneTabBar::get_current_scene() { return m_automata_scenes.at(currentIndex()); }

void SceneTabBar::add_scene(AutomataScene *scene)
{
    m_automata_scenes.append(scene);
    scene->setParent(this);

    auto scene_name = scene->get_name();
    int tab_index = scene_name.isEmpty() ? addTab("Untitled") : addTab(scene_name);

    setCurrentIndex(tab_index);
}

void SceneTabBar::remove_current_scene()
{
    int index = currentIndex();

    // We don't want to allow no tabs existing, so if
    // the last one got removed, create a new one with
    // a blank scene.
    // NOTE: Qt fails an assert upon removing the last tab,
    // so this check has to come before the removal.
    if (m_automata_scenes.size() == 1)
        add_scene(new AutomataScene);

    delete m_automata_scenes.at(index);
    m_automata_scenes.removeAt(index);
    removeTab(index);
}

void SceneTabBar::update_current_tab_label()
{
    int index = currentIndex();

    auto scene_name = m_automata_scenes.at(index)->get_name();
    scene_name.isEmpty() ? setTabText(index, "Untitled") : setTabText(index, scene_name);
}
