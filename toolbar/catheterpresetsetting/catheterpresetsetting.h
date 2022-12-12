#ifndef CATHETERPRESETSETTING_H
#define CATHETERPRESETSETTING_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class catheterpresetsetting; }
QT_END_NAMESPACE

class catheterpresetsetting : public QWidget
{
    Q_OBJECT

public:
    catheterpresetsetting(QWidget *parent = nullptr);
    ~catheterpresetsetting();


private:
    Ui::catheterpresetsetting *ui;
};
#endif
