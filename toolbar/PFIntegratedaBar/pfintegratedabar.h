#ifndef PFINTEGRATEDABAR_H
#define PFINTEGRATEDABAR_H

#include <QWidget>

namespace Ui {
class PFIntegratedaBar;
}

class PFIntegratedaBar : public QWidget
{
    Q_OBJECT

public:
    explicit PFIntegratedaBar(QWidget *parent = nullptr);
    ~PFIntegratedaBar();

private:
    Ui::PFIntegratedaBar *ui;
};

#endif // PFINTEGRATEDABAR_H
