#ifndef RPFSTATUSBAR_H
#define RPFSTATUSBAR_H

#include <QWidget>
#include <QVBoxLayout>

class Numericbar;
namespace Ui {
class RPFStatusBar;
}

class RPFStatusBar : public QWidget
{
    Q_OBJECT

public:
    explicit RPFStatusBar(QWidget *parent = nullptr);
    ~RPFStatusBar();

    void init_button();

private:
    Ui::RPFStatusBar *ui;

    Numericbar *rfablatin_button = nullptr;
    Numericbar *pfablation_button = nullptr;
    QGridLayout *baseLayout= nullptr;
    QHBoxLayout* hBoxLayout= nullptr;
    QSpacerItem  *horizontalSpacer1= nullptr;
};

#endif // RPFSTATUSBAR_H
