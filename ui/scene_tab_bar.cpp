#include "scene_tab_bar.hpp"

SceneTabBar::SceneTabBar(QGraphicsView *main_view, QWidget *parent) : QTabBar(parent), m_main_view(main_view)
{
    this->setExpanding(false);
    this->setUsesScrollButtons(true);
    this->setDrawBase(false);
    this->setShape(QTabBar::RoundedNorth);
    this->addTab("Untitled");
}

void AutomataScene::set_scene_name(const QString &scene_name) { m_scene_name = scene_name; }

QString AutomataScene::get_scene_name() { return m_scene_name; }

bool AutomataScene::has_unsaved_changes() { return m_has_unsaved_changes; }
