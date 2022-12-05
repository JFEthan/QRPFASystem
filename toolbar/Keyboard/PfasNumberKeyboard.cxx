#include "PfasNumberKeyboard.h"
#include "PfasWidgetKeyboard.h"
#include "ui_PfasNumberKeyboard.h"
#include <QFile>
#include <QLineEdit>
#include <QMessageBox>
#include <QMetaType>
#include <QTextCodec>
#include <qtextstream.h>

PfasNumberKeyboard::PfasNumberKeyboard(PfasWidgetKeyboard *_keyboard)
    : keyboard(_keyboard), PfasWidget(nullptr), ui(new Ui::PfasNumberKeyboard) {

  ui->setupUi(this);
}

PfasNumberKeyboard::~PfasNumberKeyboard() { delete ui; }

void PfasNumberKeyboard::changeEvent(QEvent *e) {}

void PfasNumberKeyboard::KeyboardInputCharacter(char c) {
  if (c >= 97 && c <= 122 && keyboard_style.caps == true)
    c -= 32;
  input_info.input_buf = keyboard->text();
  unsigned char pos = keyboard->cursorPosition();
  input_info.input_buf.insert(pos, c);
  keyboard->setText(input_info.input_buf);
  keyboard->setCursorPosition(pos + 1);
}

void PfasNumberKeyboard::on_btnBack_clicked() {
  input_info.input_buf = keyboard->text();
  unsigned char pos = keyboard->cursorPosition();
  if (input_info.input_buf.size() > 0 && pos > 0) {
    input_info.input_buf.remove(pos - 1, 1);
    keyboard->setText(input_info.input_buf);
    keyboard->setCursorPosition(pos - 1);
  }
}

char PfasNumberKeyboard::GetInputLength() {
  input_info.input_length = input_info.input_buf.size();
  return input_info.input_length;
}

// void PfasNumberKeyboard::SetLineEditEchoMode() { cur_edit->setEchoMode(QLineEdit::Password); }

void PfasNumberKeyboard::on_btn1_clicked() { KeyboardInputCharacter('1'); }

void PfasNumberKeyboard::on_btn2_clicked() { KeyboardInputCharacter('2'); }

void PfasNumberKeyboard::on_btn3_clicked() { KeyboardInputCharacter('3'); }

void PfasNumberKeyboard::on_btn4_clicked() { KeyboardInputCharacter('4'); }

void PfasNumberKeyboard::on_btn5_clicked() { KeyboardInputCharacter('5'); }

void PfasNumberKeyboard::on_btn6_clicked() { KeyboardInputCharacter('6'); }

void PfasNumberKeyboard::on_btn7_clicked() { KeyboardInputCharacter('7'); }

void PfasNumberKeyboard::on_btn8_clicked() { KeyboardInputCharacter('8'); }

void PfasNumberKeyboard::on_btn9_clicked() { KeyboardInputCharacter('9'); }

void PfasNumberKeyboard::on_btn0_clicked() { KeyboardInputCharacter('0'); }

void PfasNumberKeyboard::on_btnPoint_clicked() { KeyboardInputCharacter('.'); }

void PfasNumberKeyboard::on_btnSub_clicked() { KeyboardInputCharacter('_'); }

void PfasNumberKeyboard::on_symbolBtnEqual_clicked() { KeyboardInputCharacter('='); }

void PfasNumberKeyboard::on_symbolBtnDiv_clicked() { KeyboardInputCharacter('/'); }

void PfasNumberKeyboard::on_symbolBtnMul_clicked() { KeyboardInputCharacter('*'); }

void PfasNumberKeyboard::on_symbolBtnSub_clicked() { KeyboardInputCharacter('-'); }

void PfasNumberKeyboard::on_symbolBtnAdd_clicked() { KeyboardInputCharacter('+'); }

void PfasNumberKeyboard::on_btnShift_clicked() { KeyboardInputCharacter('~'); }

void PfasNumberKeyboard::on_btnShift1_clicked() { KeyboardInputCharacter('!'); }

void PfasNumberKeyboard::on_btnShift2_clicked() { KeyboardInputCharacter('@'); }

void PfasNumberKeyboard::on_btnShift3_clicked() { KeyboardInputCharacter('#'); }

void PfasNumberKeyboard::on_btnShift4_clicked() { KeyboardInputCharacter('$'); }

void PfasNumberKeyboard::on_btnShift5_clicked() { KeyboardInputCharacter('%'); }

void PfasNumberKeyboard::on_btnShift6_clicked() { KeyboardInputCharacter('^'); }

void PfasNumberKeyboard::on_btnShift7_clicked() { KeyboardInputCharacter('&'); }

void PfasNumberKeyboard::on_btnShift8_clicked() { KeyboardInputCharacter('*'); }

void PfasNumberKeyboard::on_btnShift9_clicked() { KeyboardInputCharacter('('); }

void PfasNumberKeyboard::on_btnShift0_clicked() { KeyboardInputCharacter(')'); }

void PfasNumberKeyboard::on_symbolBtn0_clicked() { KeyboardInputCharacter('{'); }

void PfasNumberKeyboard::on_symbolBtn1_clicked() { KeyboardInputCharacter('}'); }

void PfasNumberKeyboard::on_symbolBtn2_clicked() { KeyboardInputCharacter(39); }

void PfasNumberKeyboard::on_symbolBtn3_clicked() { KeyboardInputCharacter('['); }

void PfasNumberKeyboard::on_symbolBtn4_clicked() { KeyboardInputCharacter(']'); }

void PfasNumberKeyboard::on_symbolBtn5_clicked() { KeyboardInputCharacter(':'); }

void PfasNumberKeyboard::on_symbolBtn6_clicked() { KeyboardInputCharacter(';'); }

void PfasNumberKeyboard::on_symbolBtn7_clicked() { KeyboardInputCharacter('"'); }

void PfasNumberKeyboard::on_symbolBtn8_clicked() { KeyboardInputCharacter('<'); }

void PfasNumberKeyboard::on_symbolBtn9_clicked() { KeyboardInputCharacter('>'); }

void PfasNumberKeyboard::on_symbolBtn10_clicked() { KeyboardInputCharacter(','); }

void PfasNumberKeyboard::on_symbolBtn11_clicked() { KeyboardInputCharacter('?'); }

void PfasNumberKeyboard::on_symbolBtn12_clicked() { KeyboardInputCharacter('\\'); }

void PfasNumberKeyboard::on_symbolBtn13_clicked() { KeyboardInputCharacter('|'); }

void PfasNumberKeyboard::on_btnTab_clicked() { KeyboardInputCharacter(96); }

void PfasNumberKeyboard::clear() {
  input_info.input_buf.clear();
  input_info.input_length = 0;
}
