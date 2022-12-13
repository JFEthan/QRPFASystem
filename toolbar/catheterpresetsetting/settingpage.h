#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class settingpage; }
QT_END_NAMESPACE

class settingpage : public QWidget
{
    Q_OBJECT

public:
    settingpage(QWidget *parent = nullptr);
    ~settingpage();

private:
    Ui::settingpage *ui;
};
#endif // SETTINGPAGE_H
