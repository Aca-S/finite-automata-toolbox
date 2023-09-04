#ifndef UI_UTILITY_HPP
#define UI_UTILITY_HPP

#include <QGraphicsItem>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>

#include <ranges>

namespace Ui::Utility {
// Sets a line edit validator and optionally an info label which will display
// a specified message on invalid inputs or when focus has left the line edit
// while it contains a non-acceptable text.
void set_validator(
    QLineEdit *line_edit, QValidator *validator, QLabel *info_label = nullptr, const QString &info_message = "");
QFrame *create_h_line();

QPointF get_center_pos(QGraphicsItem *item);
QPointF get_viewport_center_pos(QGraphicsView *view);
void add_item_at_pos(QGraphicsItem *item, QGraphicsScene *scene, const QPointF &center_pos);

template <typename T> QList<T *> get_selected(QGraphicsScene *scene)
{
    using namespace std::views;

    auto selected = scene->selectedItems() | transform([](auto *item) { return qgraphicsitem_cast<T *>(item); })
                    | filter([](auto *item) { return item != nullptr; });
    return QList<T *>(selected.begin(), selected.end());
}

template <typename T> QList<T *> get_items(QGraphicsScene *scene)
{
    using namespace std::views;

    auto selected = scene->items() | transform([](auto *item) { return qgraphicsitem_cast<T *>(item); })
                    | filter([](auto *item) { return item != nullptr; });
    return QList<T *>(selected.begin(), selected.end());
}
} // namespace Ui::Utility

#endif // UI_UTILITY_HPP
