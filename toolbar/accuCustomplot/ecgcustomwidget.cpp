#include "ecgcustomwidget.h"
#include "ui_ecgcustomwidget.h"

ECGCustomWidget::ECGCustomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ECGCustomWidget)
{
    ui->setupUi(this);
}

ECGCustomWidget::~ECGCustomWidget()
{
    delete ui;
}



void ECGCustomWidget::initPlot()
{
    /* 画图 */
   if(customPlot == nullptr)customPlot = new QCustomPlot(this);

    customPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

}
void ECGCustomWidget::UninitPlot()
{

}
void ECGCustomWidget::initWidget()
{

}
void ECGCustomWidget::setBaseLayout()
{

}
