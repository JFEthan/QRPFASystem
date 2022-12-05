#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include "qcustomplot.h"
#include <QGridLayout>
#include "../NumericBar/numericbar.h"

//class Numericbar;
namespace Ui {
class CustomWidget;
}

class CustomWidget : public QWidget
{
    Q_OBJECT
public:
    enum CustomPlotMode
    {
        Mode_Unknown,
        Mode_RFAblation_H,
        Mode_RFAblation_HV,
        Mode_RFPuncture
    };
public:
    explicit CustomWidget(QWidget *parent = nullptr);
    ~CustomWidget();

public:
    void setCustomPlotMode(CustomPlotMode modeType);

    void clearAllGraph();
    void addGraph_pressurer();
    void removeGraph_pressurer();
    void addGraph_angle();
    void removeGraph_angle();
    void addGraph_flow_speed();
    void removeGraph_flow_speed();
    void addGraph_time();
    void removeGraph_time();
    void addGraph_temperature();
    void removeGraph_temperature();
    void addGraph_impedance();
    void removeGraph_impedance();
    void addGraph_power();
    void removeGraph_power();

    void addRealtimeValue_pressurer(double xValue,double yValue);
    void addRealtimeValue_angle(double xValue,double yValue);  
    void addRealtimeValue_flow_speed(double xValue,double yValue);
    void addRealtimeValue_time(double xValue,double yValue);
    void addRealtimeValue_temperature(double xValue,double yValue);
    void addRealtimeValue_impedance(double xValue,double yValue);
    void addRealtimeValue_power(double xValue,double yValue);

    void SetThresholdValue_pressurer(double value);
    void SetThresholdValue_angle(double value);
    void SetThresholdValue_flow_speed(double value);
    void SetThresholdValue_time(double value);
    void SetThresholdValue_impedance(double value);
    void SetThresholdValue_power(double value);

private:
    void setBaseLayoutRFAblation_H();
    void setBaseLayoutRFAblation_HV();
    void setBaseLayoutRFPuncture();
    void HideAllButton();
    void ShowAllButton();

private:
    void initPlot();
    void UninitPlot();
    void initButton();
    void removeBaseLayout();

    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);

private slots:
    void timerSlot();
private:
    Ui::CustomWidget *ui;
    CustomPlotMode customPlotmode=Mode_Unknown;
    CustomPlotMode last_customPlotmode=Mode_Unknown;
private:
    QTimer *dataTimer = nullptr;

    QGridLayout *baseLayout= nullptr;

    //RF Ablation
    QHBoxLayout *hRfAllLayout= nullptr;
    QHBoxLayout *hlayoutTypeTop = nullptr;
    QVBoxLayout *vlayoutTypeBottomRight = nullptr;
    QHBoxLayout *hlayoutTypeBottom = nullptr;

    //RF Puncture
    QHBoxLayout *hRFPuncture= nullptr;

    QCustomPlot *customPlot = nullptr;
    QCPAxis *powerAxis = nullptr;
    QCPAxis *tempAxis = nullptr;
    QPointer<QCPGraph> mPowerGraph;
    QPointer<QCPGraph> mTempGraph;

     Numericbar *pressure_button = nullptr;
     Numericbar *angle_button = nullptr;
     Numericbar *flow_speed_button = nullptr;
     Numericbar *time_button = nullptr;
     Numericbar *temperature_button = nullptr;
     Numericbar *impedance_button = nullptr;
     Numericbar *power_button = nullptr;

     QSpacerItem * horizontalSpacer1 = nullptr;
     QSpacerItem * horizontalSpacer2 = nullptr;
     QSpacerItem * horizontalSpacer3 = nullptr;
     QSpacerItem * horizontalSpacer4 = nullptr;
     QSpacerItem * horizontalSpacer5 = nullptr;
     QSpacerItem * horizontalSpacer6 = nullptr;


};

#endif // CUSTOMWIDGET_H
