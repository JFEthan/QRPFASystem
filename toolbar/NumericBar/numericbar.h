#ifndef NUMERICBAR_H
#define NUMERICBAR_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QPainter>
#include <QPainterPath>
#include <QPainterPath>
#include <QMouseEvent>



class Numericbar : public QWidget
{
    Q_OBJECT
    using PathPtr = std::shared_ptr<QPainterPath>;
public:
    enum typeNumericBar
    {
        type_numeric_pressure,
        type_numeric_angle,
        type_numeric_flow_speed,
        type_numeric_time,
        type_numeric_temperature,
        type_numeric_impedance,
        type_numeric_power,
        type_numeric_ablation
    };
public:
    Numericbar(QWidget *parent = 0);
    ~Numericbar() ;

public:
    void SetType(typeNumericBar type);
    void SetThresholdValue(int value);
    int GetThresholdValue();
    void SetRealtimeValue(int value);
    int GetRealtimeValue();
    void SetAutoButtonMode(bool mode);
    bool GetAutoButtonMode();
    void SetAblationName(QString ablationName);
    QColor GetColor(){return color;}
    void SetfontSize(int size);

private:
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *) override;

  void DrawPressurebar(QPainter *);
  void DrawAngledbar(QPainter *);
  void DrawFlowSpeedbar(QPainter *);
  void DrawTimebar(QPainter *);
  void DrawAblationbar(QPainter *);
private:
  Numericbar::PathPtr DrawRectBorder(QPainter *painter, qreal x, qreal y, qreal width, qreal height,qreal angle=0);
private:
  void DrawUnitArea(QPainter *painter,qreal x, qreal y, qreal width, qreal height,int fontsize = 25);
  void DrawRealtimeArea(QPainter *painter,qreal x, qreal y, qreal width, qreal height);
  void DrawAutoModeButtonArea(QPainter *painter, qreal x, qreal y, qreal width, qreal height);
  void DrawThresholArea(QPainter *painter, qreal x, qreal y, qreal width, qreal height);

  void DrawUpButtonArea(QPainter *painter, qreal x, qreal y, qreal width, qreal height);
  void DrawDownButtonArea(QPainter *painter, qreal x, qreal y, qreal width, qreal height);
  QFont layoutFont(QRect rectLbl, QString text);

 signals:
  void AutoModeButtonClicked();
  void UpButtonClicked();
  void DownButtonClicked();
public slots:
  void OnAutoModeButtonRequest();
  void OnUpButtonRequest();
  void OnDownButtonRequest();
  void OnthresholdChanged(QString content);

protected:
  bool isAutoMode;
  PathPtr btnAutoModePath= nullptr;
  PathPtr btnUpPath= nullptr;
  PathPtr btnDownPath= nullptr;
  QLineEdit * showThresholValue= nullptr;

protected:
    typeNumericBar m_type;
    QColor color;
    int thresholdValue = 0;
    int realtimeValue = 0;
    QString qstringUnit ="";
    QString qstringName ="";
    int fontsize = 70;

};

#endif // NUMERICBAR_H
