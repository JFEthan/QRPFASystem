#include "customwidget.h"
#include "ui_customwidget.h"
#include "../NumericBar/numericbar.h"

CustomWidget::CustomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomWidget)
{
    ui->setupUi(this);
    initButton();
    initPlot();
    setBaseLayoutRFAblation_H();
}

CustomWidget::~CustomWidget()
{
    UninitPlot();
    delete ui;

}
void CustomWidget::setCustomPlotMode(CustomPlotMode modeType)
{
    customPlotmode = modeType;
    switch (customPlotmode)
    {
    case Mode_RFAblation_H:
    {
        setBaseLayoutRFAblation_H();
    }break;
    case Mode_RFAblation_HV:
    {
        setBaseLayoutRFAblation_HV();
    }break;
    case Mode_RFPuncture:
    {
        setBaseLayoutRFPuncture();
    }break;
    case Mode_Unknown:
    {

    }break;

    }
}

void CustomWidget::initPlot()
{
    if(customPlot==nullptr)
    {
        customPlot = new QCustomPlot(this);
        customPlot->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//        customPlot->setGeometry(0,120,width(),height()-120);

    }
    //背景为黑色
    QLinearGradient plotGradient;
    plotGradient.setColorAt(0, QColor(0, 0, 0));
    plotGradient.setColorAt(1, QColor(0, 0, 0));
    customPlot->setBackground(plotGradient);
    customPlot->xAxis->grid()->setVisible(false);
    customPlot->yAxis->grid()->setVisible(false);

   // QLayout *layout
    // configure plot to have two right axes:
    customPlot->yAxis->setTickLabels(false);
    connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange))); // left axis only mirrors inner right axis
    customPlot->axisRect()->addAxis(QCPAxis::atLeft);
    tempAxis = customPlot->axisRect()->axis(QCPAxis::atLeft, 0);
    powerAxis = customPlot->axisRect()->axis(QCPAxis::atLeft, 1);
    //坐标轴为白色
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);

    tempAxis->setBasePen(QPen(Qt::white, 1));
    powerAxis->setBasePen(QPen(Qt::white, 1));

    powerAxis->setVisible(true);
    tempAxis->setVisible(true);
    powerAxis->setTickLabels(true);
    tempAxis->setTickLabels(true);

    tempAxis->setTickLabelSide(QCPAxis::LabelSide::lsInside);
    powerAxis->setTickLabelSide(QCPAxis::LabelSide::lsOutside);

    QColor colorPower =QColor(Qt::yellow);
    QColor colorTemp = QColor(Qt::red);
    powerAxis->setSubTickPen(colorPower);
    tempAxis->setSubTickPen(colorTemp);

    powerAxis->setTickLabelColor(colorPower);
    tempAxis->setTickLabelColor(colorTemp);
    powerAxis->setLabelColor(colorPower);
    tempAxis->setLabelColor(colorTemp);
    QString qstringTemp = u8"℃";
//    QString qstringTemp = u8"℃";
    tempAxis->setUpperEnding(QCPLineEnding(QCPLineEnding::EndingStyle::esTextBar,qstringTemp,Qt::AlignmentFlag::AlignRight));
    powerAxis->setUpperEnding(QCPLineEnding(QCPLineEnding::EndingStyle::esTextBar,"W",Qt::AlignmentFlag::AlignLeft));

    tempAxis->setTickLength(10,0);
    powerAxis->setTickLength(0,10);

    tempAxis->setSubTickLength(5,0);
    powerAxis->setSubTickLength(0,5);

    powerAxis->setPadding(2);
    tempAxis->setPadding(2);

   mPowerGraph = customPlot->addGraph(customPlot->xAxis, tempAxis);
   mTempGraph = customPlot->addGraph(customPlot->xAxis, powerAxis);
   mPowerGraph->setPen(QPen(colorPower));
   mTempGraph->setPen(QPen(colorTemp));

   dataTimer = new QTimer(this);
   connect(dataTimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
   dataTimer->start(50);
}
void CustomWidget::UninitPlot()
{
    if(dataTimer)
    {
        delete dataTimer;
    }
   if(mTempGraph) customPlot->removeGraph(mTempGraph);
   if(mPowerGraph) customPlot->removeGraph(mPowerGraph);

    if(customPlot != nullptr)
    {
        delete  customPlot;
        customPlot = nullptr;
    }
    if(baseLayout)
    {
        delete  baseLayout;
        baseLayout = nullptr;
    }
}
void CustomWidget::initButton(){

     baseLayout = new QGridLayout();
     this->setLayout(baseLayout);

   if (pressure_button == nullptr) {
     pressure_button = new Numericbar(this);
     pressure_button->setMinimumSize(QSize(150, 100));
     pressure_button->setMaximumSize(QSize(150, 100));
     pressure_button->setGeometry(QRect(0, 0, 20,50));
     pressure_button->SetType(Numericbar::type_numeric_pressure);
     pressure_button->SetRealtimeValue(235);
     pressure_button->SetThresholdValue(105);
   }
   if (angle_button == nullptr) {
     angle_button = new Numericbar(this);
     angle_button->setMinimumSize(QSize(150, 100));
     angle_button->setMaximumSize(QSize(150, 100));

     angle_button->SetType(Numericbar::type_numeric_angle);
     angle_button->SetRealtimeValue(235);
     angle_button->SetThresholdValue(105);
   }
   if (flow_speed_button == nullptr) {
     flow_speed_button = new Numericbar(this);
     flow_speed_button->setMinimumSize(QSize(150, 100));
     flow_speed_button->setMaximumSize(QSize(150, 100));

     flow_speed_button->SetType(Numericbar::type_numeric_flow_speed);
     flow_speed_button->SetRealtimeValue(235);
     flow_speed_button->SetThresholdValue(105);
   }
   if (time_button == nullptr) {
     time_button = new Numericbar(this);
     time_button->setMinimumSize(QSize(150, 100));
     time_button->setMaximumSize(QSize(150, 100));

     time_button->SetType(Numericbar::type_numeric_time);
     time_button->SetRealtimeValue(235);
     time_button->SetThresholdValue(105);
   }
   if (temperature_button == nullptr) {
     temperature_button = new Numericbar(this);
     temperature_button->setMinimumSize(QSize(150, 100));
     temperature_button->setMaximumSize(QSize(150, 100));

     temperature_button->SetType(Numericbar::type_numeric_temperature);
     temperature_button->SetRealtimeValue(235);
     temperature_button->SetThresholdValue(105);
   }
   if (impedance_button == nullptr) {
     impedance_button = new Numericbar(this);
     impedance_button->setMinimumSize(QSize(150, 100));
     impedance_button->setMaximumSize(QSize(150, 100));

     impedance_button->SetType(Numericbar::type_numeric_impedance);
     impedance_button->SetRealtimeValue(235);
     impedance_button->SetThresholdValue(105);
   }
   if (power_button == nullptr) {
     power_button = new Numericbar(this);
     power_button->setMinimumSize(QSize(150, 100));
     power_button->setMaximumSize(QSize(150, 100));

     power_button->SetType(Numericbar::type_numeric_power);
     power_button->SetRealtimeValue(235);
     power_button->SetThresholdValue(105);
   }

    horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalSpacer2= new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalSpacer3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalSpacer4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalSpacer5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalSpacer6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
}
void CustomWidget::HideAllButton()
{
    pressure_button->hide();
    angle_button->hide();
    flow_speed_button->hide();
    time_button->hide();
    temperature_button->hide();
    impedance_button->hide();
    power_button->hide();
}
void CustomWidget::ShowAllButton()
{
    pressure_button->show();
    angle_button->show();
    flow_speed_button->show();
    time_button->show();
    temperature_button->show();
    impedance_button->show();
    power_button->show();
}
void CustomWidget::setBaseLayoutRFAblation_H()
{
    removeBaseLayout();
    ShowAllButton();
    if(hRfAllLayout == nullptr)
    {
        hRfAllLayout = new QHBoxLayout();
        hRfAllLayout->setSpacing(20);
        hRfAllLayout->setObjectName(QString::fromUtf8("hRfAllLayout"));

        hRfAllLayout->addWidget(pressure_button);
        hRfAllLayout->addItem(horizontalSpacer1);

        hRfAllLayout->addWidget(angle_button);
        hRfAllLayout->addItem(horizontalSpacer2);

        hRfAllLayout->addWidget(flow_speed_button);
        hRfAllLayout->addItem(horizontalSpacer3);

        hRfAllLayout->addWidget(time_button);
        hRfAllLayout->addItem(horizontalSpacer4);

        hRfAllLayout->addWidget(temperature_button);
        hRfAllLayout->addItem(horizontalSpacer5);

        hRfAllLayout->addWidget(impedance_button);
        hRfAllLayout->addItem(horizontalSpacer6);
        hRfAllLayout->addWidget(power_button);
    }

    baseLayout->addLayout(hRfAllLayout, 0, 0);
    baseLayout->setSpacing(30);
    baseLayout->addWidget(customPlot, 1, 0);
    this->setLayout(baseLayout);
    update ();
    last_customPlotmode = Mode_RFAblation_H;
}
void CustomWidget::setBaseLayoutRFAblation_HV()
{
     removeBaseLayout();
     ShowAllButton();
    if(hlayoutTypeTop == nullptr)
    {
        hlayoutTypeTop = new QHBoxLayout();
        hlayoutTypeTop->setSpacing(20);
        hlayoutTypeTop->setObjectName(QString::fromUtf8("hlayoutTypeTop"));

        hlayoutTypeTop->addWidget(pressure_button);
        hlayoutTypeTop->addItem(horizontalSpacer1);

        hlayoutTypeTop->addWidget(angle_button);
        hlayoutTypeTop->addItem(horizontalSpacer2);

        hlayoutTypeTop->addWidget(flow_speed_button);
        hlayoutTypeTop->addItem(horizontalSpacer3);

        hlayoutTypeTop->addWidget(time_button);
        hlayoutTypeTop->addItem(horizontalSpacer4);

        hlayoutTypeTop->addWidget(temperature_button);

    }

//    hlayoutType2->addItem(horizontalSpacer5);

    if(vlayoutTypeBottomRight == nullptr)
    {
        vlayoutTypeBottomRight = new QVBoxLayout();
        vlayoutTypeBottomRight->setSpacing(20);
        vlayoutTypeBottomRight->setObjectName(QString::fromUtf8("hLayout"));

        vlayoutTypeBottomRight->addWidget(impedance_button);
    //    vlayoutType2->addItem(horizontalSpacer6);
        vlayoutTypeBottomRight->addWidget(power_button);

    }

    if(hlayoutTypeBottom == nullptr)
    {
        hlayoutTypeBottom = new QHBoxLayout();
        hlayoutTypeBottom->setObjectName(QString::fromUtf8("hLayout"));

        hlayoutTypeBottom->addWidget(customPlot);
        hlayoutTypeBottom->addLayout(vlayoutTypeBottomRight);
    }

    baseLayout->addLayout(hlayoutTypeTop, 0, 0);
    baseLayout->addLayout(hlayoutTypeBottom, 1, 0);
    baseLayout->setSpacing(60);
    this->setLayout(baseLayout);
    update ();
    last_customPlotmode = Mode_RFAblation_HV;
}

void CustomWidget::setBaseLayoutRFPuncture()
{
    removeBaseLayout();
    impedance_button->show();
    power_button->show();

   if(hRFPuncture == nullptr)
   {
       hRFPuncture = new QHBoxLayout();
       hRFPuncture->setSpacing(20);
       hRFPuncture->setObjectName(QString::fromUtf8("hRFPuncture"));

       hRFPuncture->addWidget(pressure_button);
       hRFPuncture->addItem(horizontalSpacer1);

       hRFPuncture->addWidget(angle_button);
       hRFPuncture->addItem(horizontalSpacer2);

       hRFPuncture->addWidget(flow_speed_button);
       hRFPuncture->addItem(horizontalSpacer3);

       hRFPuncture->addWidget(time_button);
       hRFPuncture->addItem(horizontalSpacer4);

       hRFPuncture->addWidget(temperature_button);
        hRFPuncture->addItem(horizontalSpacer5);
        hRFPuncture->addWidget(impedance_button);
        hRFPuncture->addItem(horizontalSpacer6);
        hRFPuncture->addWidget(power_button);
   }
   baseLayout->addLayout(hRFPuncture, 0, 0);
   baseLayout->setSpacing(30);
   baseLayout->addWidget(customPlot, 1, 0);
   this->setLayout(baseLayout);
   update ();
   last_customPlotmode = Mode_RFPuncture;

}
void CustomWidget::removeBaseLayout()
{
    HideAllButton();
    switch (last_customPlotmode)
    {
    case Mode_RFAblation_H:
    {
        baseLayout->removeItem(hRfAllLayout);
        baseLayout->removeWidget(customPlot);

    }break;
    case Mode_RFAblation_HV:
    {

        baseLayout->removeItem(hlayoutTypeTop);
        baseLayout->removeItem(hlayoutTypeBottom);
    }break;
    case Mode_RFPuncture:
    {

        baseLayout->removeItem(hRFPuncture);
        baseLayout->removeWidget(customPlot);
    }break;
    case Mode_Unknown:
    {

    }break;

    }
}

void CustomWidget::resizeEvent(QResizeEvent *event) {
  this->QWidget::resizeEvent(event);
//  if(nLastWidth != width())
//  {
//     nLastWidth = width();
//    if(nLastWidth > 1600)
//    {
//         setBaseLayoutMode1();
//    }
//    else{
//         setBaseLayoutMode2();

//    }
//  }
}
//If you subclass from QWidget, you need to provide a paintEvent for your custom QWidget as below:
 void CustomWidget::paintEvent(QPaintEvent *event)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }

void CustomWidget::timerSlot()
{
    // calculate and add a new data point to each graph:
    int count1 = mTempGraph->dataCount();
    mTempGraph->addData(count1, qSin(count1 / 50.0) + qSin(count1 / 50.0 / 0.3843) * 0.25);
    int count2 = mPowerGraph->dataCount();
    mPowerGraph->addData(count2, qCos(count2 / 50.0) + qSin(count2 / 50.0 / 0.4364) * 0.15);

    // make key axis range scroll with the data:
    customPlot->xAxis->rescale();
    mTempGraph->rescaleValueAxis(false, true);
    mPowerGraph->rescaleValueAxis(false, true);
    customPlot->xAxis->setRange(customPlot->xAxis->range().upper, 100, Qt::AlignRight);

//     update the vertical axis tag positions and texts to match the rightmost data point of the graphs:
//    double graph1Value = mGraph1->dataMainValue(mGraph1->dataCount() - 1);
//    double graph2Value = mGraph2->dataMainValue(mGraph2->dataCount() - 1);
//    mTag1->updatePosition(graph1Value);
//    mTag2->updatePosition(graph2Value);
//    mTag1->setText(QString::number(graph1Value, 'f', 2));
//    mTag2->setText(QString::number(graph2Value, 'f', 2));

    customPlot->replot();
}
