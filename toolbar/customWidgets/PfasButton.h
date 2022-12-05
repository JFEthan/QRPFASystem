#pragma once
#include <QPushButton>

class QGestureEvent;
class QTapGesture;

class PfasButton : public QPushButton {
  Q_OBJECT

public:
  PfasButton(QWidget *_parent);

protected:
  bool event(QEvent *event) override;

private:
  bool m_normal = false;
  bool m_compensation = false;
};