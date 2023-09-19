#include "scene_tab_bar.hpp"

AutomataScene::AutomataScene(QWidget *parent) : QGraphicsScene(parent) {}

void AutomataScene::set_scene_name(const QString &scene_name) { m_scene_name = scene_name; }

QString AutomataScene::get_scene_name() { return m_scene_name; }

bool AutomataScene::has_unsaved_changes() { return m_has_unsaved_changes; }

SceneTabBar::SceneTabBar(QGraphicsView *main_view, QWidget *parent) : QTabBar(parent), m_main_view(main_view)
{
    this->setExpanding(false);
    this->setUsesScrollButtons(true);
    this->setDrawBase(false);
    this->setShape(QTabBar::RoundedNorth);
    this->add_scene_tab();
}

int SceneTabBar::add_scene_tab(AutomataScene *scene)
{
    if (!scene)
        scene = new AutomataScene(this);

    m_automata_scenes.append(scene);
    m_main_view->setScene(scene);
    m_main_view->centerOn(0, 0);

    if (scene->get_scene_name().isEmpty())
        return addTab("Untitled");
    else
        return addTab(scene->get_scene_name());
}

void SceneTabBar::remove_scene_tab(int index) {}
