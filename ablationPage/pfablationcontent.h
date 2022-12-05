#ifndef PFABLATIONCONTENT_H
#define PFABLATIONCONTENT_H

#include <QWidget>


namespace Ui {
class PfAblationContent;
}

class PfAblationContent : public QWidget
{
    Q_OBJECT

public:
    explicit PfAblationContent(QWidget *parent = nullptr);
    ~PfAblationContent();

private:
//    QGridLayout *baseLayout= nullptr;

private:
    Ui::PfAblationContent *ui;
};

#endif // PFABLATIONCONTENT_H
