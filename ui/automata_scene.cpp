#include "automata_scene.hpp"

#include <QWidget>

using namespace Ui;

AutomataScene::AutomataScene(QWidget *parent) : QGraphicsScene(parent) {}

void AutomataScene::set_scene_name(const QString &scene_name) { m_scene_name = scene_name; }

QString AutomataScene::get_scene_name() { return m_scene_name; }

bool AutomataScene::has_unsaved_changes() { return m_has_unsaved_changes; }
