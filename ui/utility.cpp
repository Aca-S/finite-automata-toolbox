#include "utility.hpp"

#include <QApplication>

using namespace Ui::Utility;

void Ui::Utility::set_validator(
    QLineEdit *line_edit, QValidator *validator, QLabel *info_label, const QString &info_message)
{
    line_edit->setValidator(validator);
    if (info_label) {
        QObject::connect(line_edit, &QLineEdit::textChanged, info_label, [=]() { info_label->setText(""); });
        QObject::connect(
            line_edit, &QLineEdit::inputRejected, info_label, [=]() { info_label->setText(info_message); });
        QObject::connect(qApp, &QApplication::focusChanged, info_label, [=](auto *old, auto *now) {
            if ((old == line_edit || now == line_edit) && !line_edit->hasAcceptableInput())
                info_label->setText(info_message);
            else if (old == line_edit)
                info_label->setText(""); // Last input was rejected, but without it, the text would be valid.
        });
    }
}

QFrame *Ui::Utility::create_h_line()
{
    QFrame *line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

QPointF Ui::Utility::get_center_pos(QGraphicsItem *item)
{
    auto br = item->boundingRect();
    return item->pos() + QPointF(br.width(), br.height()) / 2.0;
}

QPointF Ui::Utility::get_viewport_center_pos(QGraphicsView *view) { return view->mapToScene(view->rect().center()); }

void Ui::Utility::add_item_at_pos(QGraphicsItem *item, QGraphicsScene *scene, const QPointF &center_pos)
{
    auto br = item->boundingRect();
    item->setPos(center_pos - QPointF(br.width(), br.height()) / 2.0);
    scene->addItem(item);
}
