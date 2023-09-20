#include "scene_tab_bar.hpp"

using namespace Ui;

SceneTabBar::SceneTabBar(QGraphicsView *main_view, QWidget *parent) : QTabBar(parent), m_main_view(main_view)
{
    this->setExpanding(false);
    this->setUsesScrollButtons(true);
    this->setDrawBase(false);
    this->setShape(QTabBar::RoundedNorth);
    this->add_scene_tab();

    // Switch displayed scene on active tab change.
    connect(this, &QTabBar::currentChanged, this, [=](int new_current_index) {
        m_main_view->setScene(m_automata_scenes.at(new_current_index));
        m_main_view->centerOn(0, 0);
    });
}

int SceneTabBar::add_scene_tab(AutomataScene *scene)
{
    if (!scene)
        scene = new AutomataScene(this);

    m_automata_scenes.append(scene);
    m_main_view->setScene(scene);
    m_main_view->centerOn(0, 0);

    auto scene_name = scene->get_name();
    int tab_index = scene_name.isEmpty() ? addTab("Untitled") : addTab(scene_name);

    setCurrentIndex(tab_index);

    return tab_index;
}

void SceneTabBar::remove_scene_tab(int index)
{
    // We don't want to allow no tabs existing, so if
    // the last one got removed, create a new one with
    // a blank scene.
    // NOTE: Qt fails an assert upon removing the last tab,
    // so this check has to come before the removal.
    if (m_automata_scenes.size() == 1)
        add_scene_tab();

    delete m_automata_scenes.at(index);
    m_automata_scenes.removeAt(index);
    removeTab(index);
}

void SceneTabBar::remove_scene_tab() { remove_scene_tab(currentIndex()); }

AutomataScene *SceneTabBar::get_scene(int index) { return m_automata_scenes.at(index); }

AutomataScene *SceneTabBar::get_scene() { return get_scene(currentIndex()); }

void SceneTabBar::update_scene_tab_name(int index)
{
    auto scene_name = m_automata_scenes.at(index)->get_name();
    scene_name.isEmpty() ? setTabText(index, "Untitled") : setTabText(index, scene_name);
}

void SceneTabBar::update_scene_tab_name() { return update_scene_tab_name(currentIndex()); }
