#ifndef PFACTIONBAR_H
#define PFACTIONBAR_H

#include <QWidget>

namespace Ui {
class PFActionbar;
}

class PFActionbar : public QWidget
{
    Q_OBJECT

public:
    explicit PFActionbar(QWidget *parent = nullptr);
    ~PFActionbar();

private:
    Ui::PFActionbar *ui;
};

#endif // PFACTIONBAR_H
