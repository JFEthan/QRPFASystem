#ifndef RINGELECTRODEBAR_H
#define RINGELECTRODEBAR_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <QVector>

namespace Ui {
class RingElectrodeBar;
}

class RingElectrodeBar : public QWidget
{
    Q_OBJECT
    using PathPtr = std::shared_ptr<QPainterPath>;
public:
   enum preset_t{
        low = 0,
        high = 1
      };
   enum class ElectrodeAttachLevel : char {
     NoneAttach,
     OpenCircuit,
     ShortCircuit,
     PoorAttach,
     GoodAttach
   };

   struct ElectrodeItem {
     PathPtr ringPath = nullptr;
     PathPtr ballPath = nullptr;
     ElectrodeAttachLevel level = ElectrodeAttachLevel::OpenCircuit;
   };
public:
    explicit RingElectrodeBar(QWidget *parent = nullptr);
    ~RingElectrodeBar();

    void SetPresetType(preset_t  _switchValue);
    void resetDefaultValue();
    void SetImpedanceInfoEnable(bool impdedanceInfoenamble);
private:
  void paintEvent(QPaintEvent *) override;
  void mousePressEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;

private:
  void DrawSwitch(QPainter *);
  void DrawElectrode(QPainter *);
  void DrawStartButton(QPainter *);
  void DrawUpdateButton(QPainter *);
  void DrawImpedanceInfo(QPainter *);
  RingElectrodeBar::PathPtr DrawSwitchBackground(QPainter *painter, qreal radius) ;
  RingElectrodeBar::PathPtr DrawRoundedRingBorder(QPainter *painter, qreal startAngle, qreal sweepLength, qreal innerRadius, qreal outerRadius);
  RingElectrodeBar::PathPtr DrawBall(QPainter *painter, qreal angle, qreal distance, qreal radius, bool isPaint = false);


  std::pair<qreal, qreal> GetButtonRingRadius();
private:

  bool CheckDischarge(int index) ;
private slots:
   void updateSwitchValue() ;
   bool getChecked() const;

private:
    Ui::RingElectrodeBar *ui;
private:
  PathPtr startPath = nullptr;
  PathPtr updatePath= nullptr;
  PathPtr switchPath= nullptr;
  PathPtr impsetStartPaht= nullptr;
  PathPtr impsetClosePaht= nullptr;
private:
  QFont textFont;
  qreal textWidth = 0;
  qreal ballRadius = 0;

  QTimer *switchTimer = nullptr;
  qreal switchStep = 0;
  int switchStartX = 0;
  preset_t switchValue = preset_t::high;

  qreal ring_widget_radius = 0;
  QVector<ElectrodeItem> electrode_state = {};
  bool impedanceInfoEnable = false;

  bool isAllSelected = true;
  bool selecting_state = false;
  int electrode_selected_end = 0;
  int electrode_selected_start = 0;
  bool warningFlag = true;
  bool startEnable = false;
  float startButtonTrackAngle = 0.0f;
  float startButtonTrackLength = 0.0f;
  float startButtonPositionPercent = 0.0f;
  bool startButtonChangedFlag = false;
};

#endif // RINGELECTRODEBAR_H
