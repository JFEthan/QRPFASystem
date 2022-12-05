#ifndef RFABLATIONCONTENT_H
#define RFABLATIONCONTENT_H

#include <QWidget>

namespace Ui {
class RfAblationContent;
}

class RfAblationContent : public QWidget
{
    Q_OBJECT

public:
    explicit RfAblationContent(QWidget *parent = nullptr);
    ~RfAblationContent();

private:
    Ui::RfAblationContent *ui;
};

#endif // RFABLATIONCONTENT_H
