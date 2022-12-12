#ifndef ECGCUSTOMWIDGET_H
#define ECGCUSTOMWIDGET_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class ECGCustomWidget;
}

class ECGCustomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ECGCustomWidget(QWidget *parent = nullptr);
    ~ECGCustomWidget();

private:
    void initPlot();
    void UninitPlot();
    void initWidget();
    void setBaseLayout();

private:
    Ui::ECGCustomWidget *ui;

    QCustomPlot *customPlot = nullptr;
    QComboBox * leadCombox= nullptr;
    QCheckBox *m_reviewCheckBox= nullptr;
    QCheckBox *m_autoScrollCheckBox= nullptr;

    QGridLayout *baseLayout= nullptr;
    QWidget *funcWidget =  nullptr;
    QVBoxLayout *vLayout = nullptr;

};

#endif // ECGCUSTOMWIDGET_H
