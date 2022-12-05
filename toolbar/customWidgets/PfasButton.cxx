#include "PfasButton.h"
#include <QEvent>
#include <QMouseEvent>
#include <QCoreApplication>
#include <iostream>
#include <QDebug>
#include <QGestureEvent>

PfasButton::PfasButton(QWidget *_parent) : QPushButton(_parent) {
  setAttribute(Qt::WA_AcceptTouchEvents);
}

bool PfasButton::event(QEvent *e) {
  switch (e->type()) {
    case QEvent::TouchBegin: {
      m_compensation = false;
      QMouseEvent mouseEvent(QEvent::MouseButtonPress, QPointF(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
      QCoreApplication::sendEvent(this, &mouseEvent);
      e->accept();
      return true;
    }
    case QEvent::TouchEnd: {
      QMouseEvent mouseEvent(QEvent::MouseButtonRelease, QPointF(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
      QCoreApplication::sendEvent(this, &mouseEvent);
      m_compensation = true;
      e->accept();
      return true;
    }
    case QEvent::MouseButtonPress:
      if (m_normal || m_compensation) {
        e->accept();
        return true;
      }
      break;
    case QEvent::MouseButtonRelease:
      if (m_normal || m_compensation) {
        m_compensation = false;
        e->accept();
        return true;
      }
      break;
    default:
      break;
  }

  return QPushButton::event(e);
}


