#pragma once

#include "PfasCharacterKeyboard.h"
#include "PfasNumberKeyboard.h"
#include "../../public/PfasWidget.h"
#include <QStackedWidget>
#include <QString>
#include <memory>

class QLineEdit;
class QPlainTextEdit;
class QTableWidget;
class QWidget;

enum class edit_type_t {
  null,
  line_edit,
  plain_edit,
  table,
};

namespace Ui {
  class PfasWidgetKeyboard;
}

//! Special content for keyboard dialog.
class PfasWidgetKeyboard : public PfasWidget {
Q_OBJECT

public:
  PfasWidgetKeyboard();
  ~PfasWidgetKeyboard();

  void OnCreate();
  void Open(QWidget *_edit);
  void Close();

  QString text() const;
  void setText(const QString &);
  int cursorPosition() const;
  void setCursorPosition(int);
  void clear();

private:
  void SetCurEdit(QWidget *_edit);
  bool eventFilter(QObject *_obj, QEvent *_event);

  Ui::PfasWidgetKeyboard *ui;
  QWidget *cur_edit = nullptr;
  PfasCharacterKeyboard *character_keyboard = nullptr;
  PfasNumberKeyboard *number_keyboard = nullptr;

  edit_type_t cur_edit_type = edit_type_t::null;
  QLineEdit *cur_line_edit = nullptr;
  QPlainTextEdit *cur_plain_edit = nullptr;
  QTableWidget *cur_table = nullptr;
  int cur_row = 0;
  int cur_column = 0;

private slots:
  void AdjustPosition(QWidget *);
  void on_btnClear_clicked();
  void on_btnEsc_clicked();
  void on_btnNumber_clicked();
  void on_btnSpace_clicked();
  void on_btnSwitch_clicked();
};
