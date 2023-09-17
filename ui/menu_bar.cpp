#include "menu_bar.hpp"

using namespace Ui;

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    build_file_menu();
    setup_file_menu();
}

void MenuBar::build_file_menu()
{
    m_file_menu = this->addMenu("File");
    m_file_menu->addAction(m_new_action = new QAction("New", this));
    m_file_menu->addAction(m_open_action = new QAction("Open", this));
    m_file_menu->addAction(m_save_action = new QAction("Save", this));
    m_file_menu->addAction(m_save_as_action = new QAction("Save As", this));
    m_file_menu->addAction(m_close_action = new QAction("Close", this));
}

void MenuBar::setup_file_menu() {}
