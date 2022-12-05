#pragma once

#include "../../public/PfasWidget.h"
#include <QMultiMap>
#include <QPushButton>

class PfasWidgetKeyboard;

namespace Ui {
class PfasCharacterKeyboard;
}

class PfasCharacterKeyboard : public PfasWidget {
  Q_OBJECT

public:
  PfasCharacterKeyboard(PfasWidgetKeyboard *_keyboard);
  ~PfasCharacterKeyboard();

private:
  void SetChineseCharactersData();

  Ui::PfasCharacterKeyboard *ui;
  unsigned char page_multiple; // The number of pages turned when entering Chinese
  struct input_info {
    QString input_buf; // The cache content entered
    unsigned char input_length = 0;
  } input_info;

  struct input_pinyin_info {
    QString input_pinyin;
    unsigned char start_pos = 0;
    unsigned char end_pos = 0;
    bool input_start = false;

    input_pinyin_info() { clear(); }

    void clear() {
      input_pinyin.clear();
      start_pos = 0;
      end_pos = 0;
      input_start = false;
    }
  } input_pinyin_info;

  struct keyboard_style
  {
    ///< Capital flag: false is lower, true is upper
    bool caps = false;
    ///< Chinese and English flag: false is English input, true is Chinese input
    bool pinyin = false;
  } keyboard_style;
  QMultiMap<QString, QString> pinyin_map; // Store the Chinese character library

  QVector<QPushButton *> chinese_btn; // Alternative Chinese characters
  QWidget *cur_edit = nullptr;                  // Current edit

  PfasWidgetKeyboard *keyboard= nullptr;

public:
  inline QString GetInputBuf() {
    return input_info.input_buf;
  } // Returns the input result of the keyboard
  inline void SetCurEdit(QWidget *_edit) { cur_edit = _edit; }
  void SetLineEditEchoMode(); // When entering a password, make the input content a small black dot
  void KeyboardInputCharacter(char c); // Receives one character at a time entered by the keyboard
  void KeyboardLoadChinese(const QString &str); // Index Chinese characters by string (key)
  char GetInputLength();                        // Input length, reserved
  void SetKeyboardStyleCapital(bool caps);      // Switch the case style of keyboard keys
  void SetKeyboardStylePinyin(bool pinyin); // Display Chinese character options, a total of 8 options
  void InputChinese(const QString &ch); // When entering Chinese, remove English (key) from the input box
  void clear();

signals:
  void Close();

public slots:
  bool onSwitchClicked();

private slots:
  void on_symbolBtn46_clicked();
  void on_symbolBtn63_clicked();
  void on_symbolBtn44_clicked();
  void on_symbolBtn33_clicked();
  void on_btnCh9_clicked();
  void on_btnCh8_clicked();
  void on_btnCh7_clicked();
  void on_btnCh6_clicked();
  void on_btnCh5_clicked();
  void on_btnCh4_clicked();
  void on_btnCh3_clicked();
  void on_btnCh2_clicked();
  void on_btnCh1_clicked();
  void on_btnPageDown_clicked();
  void on_btnPageUp_clicked();
  void on_btnCaps_clicked();
  void on_btnM_clicked();
  void on_btnN_clicked();
  void on_btnB_clicked();
  void on_btnV_clicked();
  void on_btnC_clicked();
  void on_btnX_clicked();
  void on_btnZ_clicked();
  void on_btnL_clicked();
  void on_btnK_clicked();
  void on_btnJ_clicked();
  void on_btnH_clicked();
  void on_btnG_clicked();
  void on_btnF_clicked();
  void on_btnD_clicked();
  void on_btnS_clicked();
  void on_btnA_clicked();
  void on_btnP_clicked();
  void on_btnO_clicked();
  void on_btnI_clicked();
  void on_btnU_clicked();
  void on_btnY_clicked();
  void on_btnT_clicked();
  void on_btnR_clicked();
  void on_btnE_clicked();
  void on_btnW_clicked();
  void on_btnQ_clicked();
  void on_btnBack_clicked();
  void on_btnClear_clicked();
};
