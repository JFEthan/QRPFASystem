#ifndef PAGESWITCHBAR_H
#define PAGESWITCHBAR_H

#include <QWidget>

namespace Ui {
class PageSwitchBar;
}

class PageSwitchBar : public QWidget
{
    Q_OBJECT

public:
    explicit PageSwitchBar(QWidget *parent = nullptr);
    ~PageSwitchBar();

private:
    Ui::PageSwitchBar *ui;

signals:
  void MainButtonClicked();
  void PFAblationButtonClicked();
  void RFAblationButtonClicked();
  void PFPunctureButtonClicked();
  void PostButtonClicked();



};

#endif // PAGESWITCHBAR_H
