#ifndef SETTINGPAGEBUTTONS_H
#define SETTINGPAGEBUTTONS_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class settingpagebuttons; }
QT_END_NAMESPACE

class settingpagebuttons : public QWidget
{
    Q_OBJECT

public:
    settingpagebuttons(QWidget *parent = nullptr);
    ~settingpagebuttons();

private:
    Ui::settingpagebuttons *ui;
};
#endif // SETTINGPAGEBUTTONS_H
