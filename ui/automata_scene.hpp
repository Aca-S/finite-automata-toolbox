#ifndef UI_AUTOMATA_SCENE_HPP
#define UI_AUTOMATA_SCENE_HPP

#include <QGraphicsScene>

#include <expected>
#include <optional>

namespace Ui {
class AutomataScene : public QGraphicsScene
{
  public:
    AutomataScene(QWidget *parent = nullptr);

    static std::expected<AutomataScene *, QString> load_from_file(const QString &file_name, QWidget *parent = nullptr);
    std::optional<QString> save_to_file(const QString &file_name);

    QString get_name() const;

  private:
    QString m_name;
};
} // namespace Ui

#endif // UI_AUTOMATA_SCENE_HPP
