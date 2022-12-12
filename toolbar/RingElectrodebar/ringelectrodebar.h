#ifndef RINGELECTRODEBAR_H
#define RINGELECTRODEBAR_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QTimer>
#include <vector>
#include <mutex>

namespace Ui {
class RingElectrodeBar;
}

class RingElectrodeBar : public QWidget
{
    Q_OBJECT
    using PathPtr = std::shared_ptr<QPainterPath>;
public:
   enum power_preset_t{
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

    void SetPresetType(power_preset_t  _switchValue);
    void SetElectrodeNumber(int number);
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
  RingElectrodeBar::PathPtr DrawBall(QPainter *painter, qreal angle, qreal distance, qreal radius, bool isPaint = true);
  std::pair<qreal, qreal> GetButtonRingRadius();
private:
  bool CheckDischarge(int index) ;
  void OnStartButtonTrigger(bool isTrigger, float percent);

signals:
  void StartButtonClicked();
  void UpdateButtonClicked();
  void SwitchButtonClicked();
  void ElectrodeSelected(int);
  void PresetChanged(power_preset_t preset_t);
private slots:
  void OnElectrodeUpdateRequest();
  void OnElectrodeSwitchRequest();
  void OnElectrodeSelectedRequest(int);

   void updateSwitchValue() ;
   bool getChecked() const;
public slots:

private:
    Ui::RingElectrodeBar *ui;
private:
  PathPtr startPath = nullptr;
  PathPtr updatePath= nullptr;
  PathPtr switchPath= nullptr;
  PathPtr impsetStartPaht= nullptr;
  PathPtr impsetClosePaht= nullptr;
private:
   qreal electrode_number = 10;
   qreal electrode_item_angle = 36;

private:
   QFont textFont;
   QFont titleFont;
   QFont counterFont;
   QPoint pressedPoint;
  qreal textWidth = 0;
  qreal ballRadius = 0;

  QTimer *switchTimer = nullptr;
  qreal switchStep = 0;
  int switchStartX = 0;
  power_preset_t switchValue = power_preset_t::high;

  qreal ring_widget_radius = 0;

  using lock_guard_t = std::lock_guard<std::mutex>;
  std::mutex electrode_state_mutex;
  std::vector<ElectrodeItem> electrode_state = {};
//  QVector<ElectrodeItem> electrode_state = {};
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
