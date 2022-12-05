#pragma once

#include "../../public/PfasWidget.h"
#include <QMultiMap>

class PfasWidgetKeyboard;

namespace Ui {
class PfasNumberKeyboard;
}

class PfasNumberKeyboard : public PfasWidget {
  Q_OBJECT

public:
  explicit PfasNumberKeyboard(PfasWidgetKeyboard *_keyboard);
  ~PfasNumberKeyboard();

protected:
  void changeEvent(QEvent *e);

private:
  Ui::PfasNumberKeyboard *ui;
  struct input_info {
    QString input_buf = "";
    unsigned char input_length = 0;
  } input_info;

  struct keyboard_style {
    bool caps;
    bool pinyin;
  } keyboard_style = {false,false};

  QWidget *cur_edit = nullptr; // Current edit
  PfasWidgetKeyboard *keyboard = nullptr;

public:
  inline QString GetInputBuf() { return input_info.input_buf; }
  inline void SetCurEdit(QWidget *_edit) { cur_edit = _edit; }
  void SetLineEditEchoMode();
  void KeyboardInputCharacter(char c);
  char GetInputLength();
  void clear();

signals:
  void Close();

private slots:
  void on_btnTab_clicked();
  void on_symbolBtn13_clicked();
  void on_symbolBtn12_clicked();
  void on_symbolBtn11_clicked();
  void on_symbolBtn10_clicked();
  void on_symbolBtn9_clicked();
  void on_symbolBtn8_clicked();
  void on_symbolBtn7_clicked();
  void on_symbolBtn6_clicked();
  void on_symbolBtn5_clicked();
  void on_symbolBtn4_clicked();
  void on_symbolBtn3_clicked();
  void on_symbolBtn2_clicked();
  void on_symbolBtn1_clicked();
  void on_symbolBtn0_clicked();
  void on_btnShift0_clicked();
  void on_btnShift9_clicked();
  void on_btnShift8_clicked();
  void on_btnShift7_clicked();
  void on_btnShift6_clicked();
  void on_btnShift5_clicked();
  void on_btnShift4_clicked();
  void on_btnShift3_clicked();
  void on_btnShift2_clicked();
  void on_btnShift1_clicked();
  void on_btnShift_clicked();
  void on_symbolBtnAdd_clicked();
  void on_symbolBtnMul_clicked();
  void on_symbolBtnDiv_clicked();
  void on_symbolBtnEqual_clicked();
  void on_symbolBtnSub_clicked();
  void on_btnSub_clicked();
  void on_btnBack_clicked();
  void on_btnPoint_clicked();
  void on_btn0_clicked();
  void on_btn9_clicked();
  void on_btn8_clicked();
  void on_btn7_clicked();
  void on_btn6_clicked();
  void on_btn5_clicked();
  void on_btn4_clicked();
  void on_btn3_clicked();
  void on_btn2_clicked();
  void on_btn1_clicked();
};
