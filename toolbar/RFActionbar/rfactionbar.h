#ifndef RFACTIONBAR_H
#define RFACTIONBAR_H

#include <QWidget>

namespace Ui {
class RFActionbar;
}

class RFActionbar : public QWidget
{
    Q_OBJECT

public:
    explicit RFActionbar(QWidget *parent = nullptr);
    ~RFActionbar();

private:
    Ui::RFActionbar *ui;
};

#endif // RFACTIONBAR_H
