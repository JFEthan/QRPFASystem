#include "PfasWidget.h"
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

PfasWidget::PfasWidget(QWidget *_parent) : QWidget(_parent) {
}

void PfasWidget::paintEvent(QPaintEvent *) {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

