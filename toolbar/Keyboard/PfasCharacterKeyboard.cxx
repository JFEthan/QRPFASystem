#include "PfasCharacterKeyboard.h"
#include "PfasWidgetKeyboard.h"
#include "ui_PfasCharacterKeyboard.h"
#include <QFile>
#include <QLineEdit>
#include <QMessageBox>
#include <QMetaType>
#include <qtextstream.h>


PfasCharacterKeyboard::PfasCharacterKeyboard(PfasWidgetKeyboard *_keyboard)
    : keyboard(_keyboard), PfasWidget(nullptr), ui(new Ui::PfasCharacterKeyboard) {
  ui->setupUi(this);
  page_multiple = 0;
  input_info.input_length = 0;
  SetKeyboardStyleCapital(keyboard_style.caps);
  SetKeyboardStylePinyin(keyboard_style.pinyin);
  chinese_btn.push_back(ui->btnCh1);
  chinese_btn.push_back(ui->btnCh2);
  chinese_btn.push_back(ui->btnCh3);
  chinese_btn.push_back(ui->btnCh4);
  chinese_btn.push_back(ui->btnCh5);
  chinese_btn.push_back(ui->btnCh6);
  chinese_btn.push_back(ui->btnCh7);
  chinese_btn.push_back(ui->btnCh8);
  chinese_btn.push_back(ui->btnCh9);
  SetChineseCharactersData();
}

PfasCharacterKeyboard::~PfasCharacterKeyboard() { delete ui; }

void PfasCharacterKeyboard::SetChineseCharactersData() {
  QFile font_library(":/ime/pinyin");

  if (!font_library.open(QIODevice::ReadOnly)) {
//    PFAS_LOG(error) << "Failed to load font library!";
    exit(-1);
  }

  QTextStream in(&font_library);
  in.setCodec("GB2312");

  QStringList lst;
  QString candidate;
  QString pinyin;
  QString portion_pinyin;
  while (!in.atEnd()) {
    QString line = in.readLine();
    lst = line.split("=");
    pinyin = lst.at(0);
    candidate = lst.at(1);
    for (int i = 0; i < candidate.length(); ++i) {
      pinyin_map.insert(pinyin, candidate.at(i));
    }
  }

  font_library.close();
}

void PfasCharacterKeyboard::KeyboardInputCharacter(char c) {
  if (c >= 97 && c <= 122 && keyboard_style.caps == true)
    c -= 32;
  input_info.input_buf = keyboard->text();
  unsigned char pos = keyboard->cursorPosition();
  input_info.input_buf.insert(pos, c);
  keyboard->setText(input_info.input_buf);
  keyboard->setCursorPosition(pos + 1);

  if (keyboard_style.pinyin == true) {
    input_pinyin_info.input_pinyin.append(c);
    if (!input_pinyin_info.input_start) {
      input_pinyin_info.input_start = true;
      input_pinyin_info.start_pos = pos;
    }
    page_multiple = 0;
    KeyboardLoadChinese(input_pinyin_info.input_pinyin);
  }
}

void PfasCharacterKeyboard::KeyboardLoadChinese(const QString &str) {
  QList<QString> pinyin_list = pinyin_map.values(str);
  short count = pinyin_list.size();

  ui->btnPageDown->setEnabled(count > 9 && !((page_multiple > 0) && (count / (9 * (page_multiple + 1) + 1) == 0)));
  ui->btnPageUp->setEnabled(page_multiple != 0);

  for (unsigned char i = 0; i < 9; i++) {
    chinese_btn.at(i)->setEnabled(false);
  }

  for (unsigned char i = 0, j = page_multiple * 9; i < 9; i++, j++) {
    chinese_btn.at(i)->setText(pinyin_list.value(count - j - 1));
    if (j < count)
      chinese_btn.at(i)->setEnabled(true);
  }

}

void PfasCharacterKeyboard::on_btnPageUp_clicked() {
  page_multiple -= 1;
  KeyboardLoadChinese(input_pinyin_info.input_pinyin);
}

void PfasCharacterKeyboard::on_btnPageDown_clicked() {
  page_multiple += 1;
  KeyboardLoadChinese(input_pinyin_info.input_pinyin);
}

void PfasCharacterKeyboard::on_btnBack_clicked() {
  input_info.input_buf = keyboard->text();
  unsigned char pos = keyboard->cursorPosition();
  if (input_info.input_buf.size() > 0 && pos > 0) {
    input_info.input_buf.remove(pos - 1, 1);
    keyboard->setText(input_info.input_buf);
    keyboard->setCursorPosition(pos - 1);
  }

  if (keyboard_style.pinyin == true) {
    if (input_pinyin_info.input_pinyin.length() > 0) {
      input_pinyin_info.input_pinyin.remove(input_pinyin_info.input_pinyin.length() - 1, 1);
    }
    page_multiple = 0;
    KeyboardLoadChinese(input_pinyin_info.input_pinyin);
  }
}

char PfasCharacterKeyboard::GetInputLength() {
  input_info.input_length = input_info.input_buf.size();
  return input_info.input_length;
}

void PfasCharacterKeyboard::on_btnCaps_clicked() {
  SetKeyboardStyleCapital(!keyboard_style.caps);
}

void PfasCharacterKeyboard::SetKeyboardStyleCapital(bool caps) {
  keyboard_style.caps = caps;
  if (caps == true) {
    ui->btnQ->setText("Q");
    ui->btnW->setText("W");
    ui->btnE->setText("E");
    ui->btnR->setText("R");
    ui->btnT->setText("T");
    ui->btnY->setText("Y");
    ui->btnU->setText("U");
    ui->btnI->setText("I");
    ui->btnO->setText("O");
    ui->btnP->setText("P");
    ui->btnA->setText("A");
    ui->btnS->setText("S");
    ui->btnD->setText("D");
    ui->btnF->setText("F");
    ui->btnG->setText("G");
    ui->btnH->setText("H");
    ui->btnJ->setText("J");
    ui->btnK->setText("K");
    ui->btnL->setText("L");
    ui->btnZ->setText("Z");
    ui->btnX->setText("X");
    ui->btnC->setText("C");
    ui->btnV->setText("V");
    ui->btnB->setText("B");
    ui->btnN->setText("N");
    ui->btnM->setText("M");
  } else if (caps == false) {
    ui->btnQ->setText("q");
    ui->btnW->setText("w");
    ui->btnE->setText("e");
    ui->btnR->setText("r");
    ui->btnT->setText("t");
    ui->btnY->setText("y");
    ui->btnU->setText("u");
    ui->btnI->setText("i");
    ui->btnO->setText("o");
    ui->btnP->setText("p");
    ui->btnA->setText("a");
    ui->btnS->setText("s");
    ui->btnD->setText("d");
    ui->btnF->setText("f");
    ui->btnG->setText("g");
    ui->btnH->setText("h");
    ui->btnJ->setText("j");
    ui->btnK->setText("k");
    ui->btnL->setText("l");
    ui->btnZ->setText("z");
    ui->btnX->setText("x");
    ui->btnC->setText("c");
    ui->btnV->setText("v");
    ui->btnB->setText("b");
    ui->btnN->setText("n");
    ui->btnM->setText("m");
  }
}

bool PfasCharacterKeyboard::onSwitchClicked() {
  input_pinyin_info.clear();
  input_info.input_length = 0;
  input_info.input_buf.clear();
  SetKeyboardStyleCapital(false);
  SetKeyboardStylePinyin(!keyboard_style.pinyin);
  return keyboard_style.pinyin;
}

void PfasCharacterKeyboard::SetKeyboardStylePinyin(bool pinyin) {
  ui->btnCaps->setHidden(pinyin);
  ui->symbolBtn33->setHidden(pinyin);
  ui->btnPageUp->setHidden(!pinyin);
  ui->btnPageDown->setHidden(!pinyin);
  ui->previewWidget->setHidden(!pinyin);

  keyboard_style.pinyin = pinyin;
}

void PfasCharacterKeyboard::InputChinese(const QString &ch) {
  input_info.input_buf = keyboard->text();
  if (!input_pinyin_info.input_start) {
    input_pinyin_info.clear();
    input_pinyin_info.start_pos = keyboard->cursorPosition();
    input_pinyin_info.input_start = true;
  }

  if (input_pinyin_info.input_pinyin.length() > 0) {
    input_info.input_buf.remove(input_pinyin_info.start_pos,input_pinyin_info.input_pinyin.length());
  }
  input_info.input_buf.insert(input_pinyin_info.start_pos, ch);
  keyboard->setText(input_info.input_buf);
  keyboard->setCursorPosition(input_pinyin_info.start_pos + 1);
  input_pinyin_info.clear();
  ui->btnPageDown->setHidden(true);
  ui->btnPageUp->setHidden(true);
}


void PfasCharacterKeyboard::on_btnQ_clicked() { KeyboardInputCharacter('q'); }

void PfasCharacterKeyboard::on_btnW_clicked() { KeyboardInputCharacter('w'); }

void PfasCharacterKeyboard::on_btnE_clicked() { KeyboardInputCharacter('e'); }

void PfasCharacterKeyboard::on_btnR_clicked() { KeyboardInputCharacter('r'); }

void PfasCharacterKeyboard::on_btnT_clicked() { KeyboardInputCharacter('t'); }

void PfasCharacterKeyboard::on_btnY_clicked() { KeyboardInputCharacter('y'); }

void PfasCharacterKeyboard::on_btnU_clicked() { KeyboardInputCharacter('u'); }

void PfasCharacterKeyboard::on_btnI_clicked() { KeyboardInputCharacter('i'); }

void PfasCharacterKeyboard::on_btnO_clicked() { KeyboardInputCharacter('o'); }

void PfasCharacterKeyboard::on_btnP_clicked() { KeyboardInputCharacter('p'); }

void PfasCharacterKeyboard::on_btnA_clicked() { KeyboardInputCharacter('a'); }

void PfasCharacterKeyboard::on_btnS_clicked() { KeyboardInputCharacter('s'); }

void PfasCharacterKeyboard::on_btnD_clicked() { KeyboardInputCharacter('d'); }

void PfasCharacterKeyboard::on_btnF_clicked() { KeyboardInputCharacter('f'); }

void PfasCharacterKeyboard::on_btnG_clicked() { KeyboardInputCharacter('g'); }

void PfasCharacterKeyboard::on_btnH_clicked() { KeyboardInputCharacter('h'); }

void PfasCharacterKeyboard::on_btnJ_clicked() { KeyboardInputCharacter('j'); }

void PfasCharacterKeyboard::on_btnK_clicked() { KeyboardInputCharacter('k'); }

void PfasCharacterKeyboard::on_btnL_clicked() { KeyboardInputCharacter('l'); }

void PfasCharacterKeyboard::on_btnZ_clicked() { KeyboardInputCharacter('z'); }

void PfasCharacterKeyboard::on_btnX_clicked() { KeyboardInputCharacter('x'); }

void PfasCharacterKeyboard::on_btnC_clicked() { KeyboardInputCharacter('c'); }

void PfasCharacterKeyboard::on_btnV_clicked() { KeyboardInputCharacter('v'); }

void PfasCharacterKeyboard::on_btnB_clicked() { KeyboardInputCharacter('b'); }

void PfasCharacterKeyboard::on_btnN_clicked() { KeyboardInputCharacter('n'); }

void PfasCharacterKeyboard::on_btnM_clicked() { KeyboardInputCharacter('m'); }

void PfasCharacterKeyboard::on_btnCh1_clicked() { InputChinese(ui->btnCh1->text()); }

void PfasCharacterKeyboard::on_btnCh2_clicked() { InputChinese(ui->btnCh2->text()); }

void PfasCharacterKeyboard::on_btnCh3_clicked() { InputChinese(ui->btnCh3->text()); }

void PfasCharacterKeyboard::on_btnCh4_clicked() { InputChinese(ui->btnCh4->text()); }

void PfasCharacterKeyboard::on_btnCh5_clicked() { InputChinese(ui->btnCh5->text()); }

void PfasCharacterKeyboard::on_btnCh6_clicked() { InputChinese(ui->btnCh6->text()); }

void PfasCharacterKeyboard::on_btnCh7_clicked() { InputChinese(ui->btnCh7->text()); }

void PfasCharacterKeyboard::on_btnCh8_clicked() { InputChinese(ui->btnCh8->text()); }

void PfasCharacterKeyboard::on_btnCh9_clicked() { InputChinese(ui->btnCh9->text()); }

void PfasCharacterKeyboard::on_symbolBtn33_clicked() { KeyboardInputCharacter('!'); }

void PfasCharacterKeyboard::on_symbolBtn44_clicked() { KeyboardInputCharacter(','); }

void PfasCharacterKeyboard::on_symbolBtn63_clicked() { KeyboardInputCharacter('?'); }

void PfasCharacterKeyboard::on_symbolBtn46_clicked() { KeyboardInputCharacter('.'); }

void PfasCharacterKeyboard::clear() {
  input_info.input_buf.clear();
  input_info.input_length = 0;
  input_pinyin_info.clear();
}

void PfasCharacterKeyboard::on_btnClear_clicked() {
  this->clear();
  keyboard->clear();
}
