#include "PfasWidgetKeyboard.h"
#include "ui_PfasWidgetKeyboard.h"
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPoint>
#include <QRegExpValidator>
#include <QTableWidget>
#include <QTextCursor>
#include <QMetaType>
#include <qtextstream.h>
#include <QDesktopWidget>

PfasWidgetKeyboard::PfasWidgetKeyboard()
  : PfasWidget(nullptr), ui(new Ui::PfasWidgetKeyboard) {
  ui->setupUi(this);
}

PfasWidgetKeyboard::~PfasWidgetKeyboard() { delete ui; }

void PfasWidgetKeyboard::OnCreate() {
  setAttribute(Qt::WA_TranslucentBackground);
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

  character_keyboard = new PfasCharacterKeyboard(this);
  number_keyboard = new PfasNumberKeyboard(this);

  ui->stackedWidget->addWidget(character_keyboard);
  ui->stackedWidget->addWidget(number_keyboard);
  ui->stackedWidget->setCurrentWidget(character_keyboard);
  installEventFilter(this);
}

bool PfasWidgetKeyboard::eventFilter(QObject *_obj, QEvent *_event) {
  if (QEvent::WindowBlocked == _event->type()) {
    Close();
  }

  return false;
}

void PfasWidgetKeyboard::Open(QWidget *_edit) {
  if (!_edit->isEnabled()) {
    return;
  }
  show();
  SetCurEdit(_edit);
}

void PfasWidgetKeyboard::Close() { hide(); }

void PfasWidgetKeyboard::SetCurEdit(QWidget *_edit) {
  number_keyboard->SetCurEdit(_edit);
  character_keyboard->SetCurEdit(_edit);
  AdjustPosition(_edit);

  if (cur_line_edit = qobject_cast<QLineEdit *>(_edit)) {
    cur_edit_type = edit_type_t::line_edit;
  } else if (cur_plain_edit = qobject_cast<QPlainTextEdit *>(_edit)) {
    cur_edit_type = edit_type_t::plain_edit;
  } else if (cur_table = qobject_cast<QTableWidget *>(_edit)) {
    cur_edit_type = edit_type_t::table;
  }
}

void PfasWidgetKeyboard::AdjustPosition(QWidget *edit) {
  if (!edit)
    return;

  QDesktopWidget* desktopWidget = QApplication::desktop();
  QRect _ui_rect = desktopWidget->availableGeometry(); //用户可用窗口大小
//  QRect _ui_rect = desktopWidget->screenGeometry(); //应用程序可用窗口大小

  int _w = _ui_rect.width();
  int _h = _ui_rect.height();
  int _x = _ui_rect.x();
  int _y = _ui_rect.y();
  int _edit_x = edit->x();
  int _edit_y = edit->y();
  QPoint _g_point = edit->parentWidget()->mapToGlobal(QPoint(_edit_x, _edit_y));
  if (_g_point.y() <= _h / 2)
    emit this->move(_x + (_w - width()) / 2, _y + _h - height());
  else
    emit this->move(_x + (_w - width()) / 2, _y);
}

QString PfasWidgetKeyboard::text() const {
  switch (cur_edit_type) {
    case edit_type_t::line_edit:
      return cur_line_edit->text();
    case edit_type_t::plain_edit:
      return cur_plain_edit->toPlainText();
    case edit_type_t::table:
      return cur_table->item(cur_row, cur_column)->text();
    default:
      return "";
  }
}

void PfasWidgetKeyboard::setText(const QString &_text) {
  switch (cur_edit_type) {
    case edit_type_t::line_edit: {
      auto _validator = cur_line_edit->validator();
      if (_validator) {
        auto _text_to_validate = _text;
        auto _pos = 0;
        auto _validate_state = _validator->validate(_text_to_validate, _pos);
        if (_validate_state == QValidator::Invalid) {
          break;
        }
      }
      if (!cur_line_edit->isReadOnly())cur_line_edit->setText(_text);
      break;
    }
    case edit_type_t::plain_edit:
      cur_plain_edit->setPlainText(_text);
      break;
    case edit_type_t::table:
      cur_table->item(cur_row, cur_column)->setText(_text);
      break;
    default:
      break;
  }
}

int PfasWidgetKeyboard::cursorPosition() const {
  QTextCursor _cursor;

  switch (cur_edit_type) {
    case edit_type_t::line_edit:
      return cur_line_edit->cursorPosition();
    case edit_type_t::plain_edit:
      _cursor = cur_plain_edit->textCursor();
      return _cursor.position();
    case edit_type_t::table:
      return cur_table->item(cur_row, cur_column)->text().length();
    default:
      return -1;
  }
}

void PfasWidgetKeyboard::setCursorPosition(int _position) {
  QTextCursor _cursor;

  switch (cur_edit_type) {
    case edit_type_t::line_edit:
      return cur_line_edit->setCursorPosition(_position);
    case edit_type_t::plain_edit:
      _cursor = cur_plain_edit->textCursor();
      _cursor.setPosition(_position);
      cur_plain_edit->setTextCursor(_cursor);
      return;
    default:
      return;
  }
}

void PfasWidgetKeyboard::clear() {
  switch (cur_edit_type) {
    case edit_type_t::line_edit:
      return cur_line_edit->clear();
    case edit_type_t::plain_edit:
      return cur_plain_edit->clear();
    case edit_type_t::table:
      return cur_table->item(cur_row, cur_column)->setText("");
    default:
      return;
  }
}

void PfasWidgetKeyboard::on_btnSwitch_clicked() {
  auto isPinyin = character_keyboard->onSwitchClicked();
  ui->btnSwitch->setText(isPinyin ? QStringLiteral("中/英") : QStringLiteral("EN/CH"));
}

void PfasWidgetKeyboard::on_btnSpace_clicked() {
  auto keyboard = ui->stackedWidget->currentWidget();
  if(keyboard == character_keyboard){
    character_keyboard->KeyboardInputCharacter(' ');
  }else{
    number_keyboard->KeyboardInputCharacter(' ');
  }
}

void PfasWidgetKeyboard::on_btnNumber_clicked() {
  auto keyboard = ui->stackedWidget->currentWidget();
  if(keyboard == character_keyboard){
    ui->btnClear->show();
    ui->btnSwitch->hide();
    character_keyboard->clear();
    ui->btnNumber->setText(tr("Return"));
    ui->stackedWidget->setCurrentWidget(number_keyboard);
  }else{
    ui->btnClear->hide();
    ui->btnSwitch->show();
    number_keyboard->clear();
    ui->btnNumber->setText(tr("123"));
    ui->stackedWidget->setCurrentWidget(character_keyboard);
  }
}

void PfasWidgetKeyboard::on_btnEsc_clicked() {
  auto keyboard = ui->stackedWidget->currentWidget();
  if(keyboard == character_keyboard){
    character_keyboard->clear();
  }else{
    number_keyboard->clear();
  }
  hide();
}

void PfasWidgetKeyboard::on_btnClear_clicked() {
  ui->stackedWidget->currentWidget() == number_keyboard ? number_keyboard->clear(), this->clear() : this->clear();
}

