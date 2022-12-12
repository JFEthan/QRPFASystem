#include "ringelectrodebar.h"
#include "ui_ringelectrodebar.h"
#include <QPainter>
#include <QStyleOption>
#include <QDebug>
#include <QtMath>
#include <QMouseEvent>

const qreal show_ratio = 0.9;
const qreal switch_space = 5;
const qreal border_radius = 10;
const qreal enable_threshold = 0.8;
const qreal inter_electrode_angle = 5;
const qreal start_button_angle = 32;


RingElectrodeBar::RingElectrodeBar(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::RingElectrodeBar)
  , switchTimer(new QTimer(this))
{
    ui->setupUi(this);
    switchTimer->setInterval(10);
    switchTimer->setSingleShot(false);
    connect(switchTimer, &QTimer::timeout, this, &RingElectrodeBar::updateSwitchValue);

    connect(this, &RingElectrodeBar::StartButtonClicked, this, &RingElectrodeBar::updateSwitchValue);
    connect(this, &RingElectrodeBar::UpdateButtonClicked, this, &RingElectrodeBar::OnElectrodeUpdateRequest);
    connect(this, &RingElectrodeBar::SwitchButtonClicked, this, &RingElectrodeBar::OnElectrodeSwitchRequest);
    connect(this, &RingElectrodeBar::ElectrodeSelected, this, &RingElectrodeBar::OnElectrodeSelectedRequest);


    OnElectrodeUpdateRequest();
}

RingElectrodeBar::~RingElectrodeBar()
{
    delete ui;
    if(switchTimer)
    {
        delete switchTimer;
        switchTimer = nullptr;
    }
}
void RingElectrodeBar::SetPresetType(power_preset_t  _switchValue)
{
    switchValue = _switchValue;
}
void RingElectrodeBar::SetElectrodeNumber(int nCount)
{
    electrode_number = nCount;
    electrode_item_angle = 360 / electrode_number;
}
void RingElectrodeBar::SetImpedanceInfoEnable(bool impdedanceInfoenamble_)
{
    impedanceInfoEnable = impdedanceInfoenamble_;
}


void RingElectrodeBar::resetDefaultValue() {
  auto changed = this->size().height() *0.5*0.8;
  if (ring_widget_radius != changed)
  {
      ring_widget_radius = changed;
      ballRadius = ring_widget_radius*0.12 ;

      startButtonTrackAngle = start_button_angle * 0.6;
      auto distance = GetButtonRingRadius();
      auto radius = (distance.second + distance.first) / 2;
      startButtonTrackLength = qSin(qDegreesToRadians(startButtonTrackAngle / 2)) * radius * 2;


      QFont font = this->font();
      font.setBold(true);
      font.setPixelSize(25);
      titleFont = font;
      font.setPixelSize(30);
      counterFont = font;
      font.setPixelSize(ballRadius);
      textFont = font;

      textWidth = qMax(QFontMetrics(font).width(tr("Low")), QFontMetrics(font).width(tr("High"))) * 1.8;
      switchStep = textWidth / 15;
      switchStartX = getChecked() ? 0 : textWidth;
  }
}
std::pair<qreal, qreal> RingElectrodeBar::GetButtonRingRadius() {
  return std::pair<qreal, qreal>(ring_widget_radius * (1 + 0.2), ring_widget_radius * (1 + 0.5));
}

void RingElectrodeBar::paintEvent(QPaintEvent *event)
{
    resetDefaultValue();
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width() / 2, height() / 2);

    DrawSwitch(&painter);
    DrawElectrode(&painter);
    DrawStartButton(&painter);
    DrawUpdateButton(&painter);

}
void RingElectrodeBar::mousePressEvent(QMouseEvent *event)
{
    auto offset = QPoint(this->width() / 2, this->height() / 2);
     auto point = event->pos() - offset;
     if (startPath->contains(point)) {
       if (startEnable && !startButtonChangedFlag) {
         startButtonChangedFlag = true;
         pressedPoint = event->pos();
       }
       return;
     } else if (updatePath->contains(point)) {
       emit UpdateButtonClicked();
       return;
     } else if (switchPath->contains(point)) {
       emit SwitchButtonClicked();
       return;
     }
     else {
       auto degrees = static_cast<int>(ceil(qRadiansToDegrees(qAtan2(point.y(), point.x())) + 90 + 360)) % 360;
       auto index = ceil(degrees / electrode_item_angle);
       if (index >= 1 && index <= electrode_number) {
         lock_guard_t lock_t(electrode_state_mutex);
         auto electrode_item = electrode_state[index - 1];
         if (electrode_item.ringPath->contains(point) || electrode_item.ballPath->contains(point)) {
           emit ElectrodeSelected(index);
           return;
         }
       }
     }
}
void RingElectrodeBar::mouseMoveEvent(QMouseEvent *)
{
    if (startButtonChangedFlag) {
      startButtonChangedFlag = false;
      auto percent = startButtonPositionPercent;
      OnStartButtonTrigger(startButtonChangedFlag, 0);
      if (percent > enable_threshold) {
        emit this->StartButtonClicked();
      }
    }
}
void RingElectrodeBar::OnStartButtonTrigger(bool isTrigger, float percent) {
  startButtonChangedFlag = isTrigger;
  startButtonPositionPercent = percent;
  repaint();
}

bool RingElectrodeBar::CheckDischarge(int index) {
  return isAllSelected || (selecting_state && index == electrode_selected_start) ||
         (!selecting_state && index >= electrode_selected_start && index <= electrode_selected_end);
}

bool RingElectrodeBar::getChecked() const { return switchValue == RingElectrodeBar::power_preset_t::low; }

void RingElectrodeBar::OnElectrodeSelectedRequest(int index) {
    if (!selecting_state) {
        selecting_state = true;
        isAllSelected = false;
        electrode_selected_start = index;
      } else {
        selecting_state = false;
        auto indexTmp = electrode_selected_start;
        if (index == electrode_selected_start) {
          electrode_selected_start = 1;
          electrode_selected_end = electrode_number;
        } else {
          electrode_selected_start = qMin(indexTmp, index);
          electrode_selected_end = qMax(indexTmp, index);
        }
        isAllSelected = (electrode_selected_start == 1 && electrode_selected_end == electrode_number);
      }
      update();
}

void RingElectrodeBar::OnElectrodeSwitchRequest() {
  switchStartX = getChecked() ? 0 : textWidth;
  switchValue = static_cast<power_preset_t>(1 ^ static_cast<int>(switchValue));
  emit PresetChanged(switchValue);
  switchTimer->start();
}
void RingElectrodeBar::DrawSwitch(QPainter *painter)
{
  painter->save();
  painter->setFont(textFont);
  painter->setPen(Qt::NoPen);
  painter->setBrush(QColor("#F6F7FC"));
  switchPath = DrawSwitchBackground(painter, ballRadius + switch_space);
  auto isChecked = getChecked();
  auto offset = (isChecked ? 1 : -1);
  QRect textRect(textWidth / 2 + offset * ballRadius, ballRadius, -textWidth, -ballRadius * 2);
  painter->setPen(QColor("#031033"));
  painter->drawText(textRect, Qt::AlignCenter, isChecked ? tr("Low") : tr("High"));
  painter->setPen(Qt::NoPen);
  QColor color = isChecked ? "#3866E6" : "#FFDB94";
  painter->setBrush(QBrush(color));
  auto difference = this->switchTimer->isActive() ? switchStartX : ((switchValue == power_preset_t::low) ? 0 : textWidth);
  QRect rect(-textWidth / 2 - ballRadius + difference, -ballRadius, ballRadius * 2, ballRadius * 2);
  painter->drawEllipse(rect);
  painter->restore();
}
void RingElectrodeBar::DrawElectrode(QPainter *painter)
{
  painter->save();
  auto angle = 360 / electrode_number;
  auto start = inter_electrode_angle / 2;
  auto innerRingRadius = ring_widget_radius * 0.6;
  painter->setFont(textFont);
  int count = 1;
  bool bWarning = false;

  for (auto &item: electrode_state) {
    auto level = this->electrode_state[count - 1].level;
    QPen pen;
    if (this->impedanceInfoEnable) {
      auto style = (level == ElectrodeAttachLevel::GoodAttach)? Qt::SolidLine : ((level == ElectrodeAttachLevel::PoorAttach)? Qt::DashLine : (warningFlag ? Qt::NoPen : Qt::DashLine)) ;
      auto color = level == ElectrodeAttachLevel::NoneAttach ?  Qt::white : QColor("#5eed05");
      pen = QPen(QBrush(color), 8, style);
      if(level == ElectrodeAttachLevel::ShortCircuit || level == ElectrodeAttachLevel::OpenCircuit)
      {
        pen = Qt::NoPen;
        bWarning =true;
      }
    } else {
      pen = Qt::NoPen;
    }
    painter->setPen(pen);
    auto dischargeColor = CheckDischarge(count) ? QColor("#FFDB94") : QColor("#F6F7FA");
    auto centreAngle = start + start_button_angle / 2;
    auto centreCosY = -qCos(qDegreesToRadians(centreAngle));
    auto centreSinX = qSin(qDegreesToRadians(centreAngle));
    QRadialGradient radial(0, 0, ring_widget_radius, innerRingRadius * centreSinX * 0.6, innerRingRadius * centreCosY * 0.6);
    radial.setColorAt(0, dischargeColor);
    radial.setColorAt(1, QColor("#3155C3"));
    painter->setBrush(radial);
    item.ringPath = DrawRoundedRingBorder(painter, start, angle - inter_electrode_angle, innerRingRadius, ring_widget_radius);
    painter->setBrush(QBrush(dischargeColor));
    painter->setPen(Qt::NoPen);
    item.ballPath = DrawBall(painter, centreAngle, ring_widget_radius * 0.65, ballRadius);
    painter->setPen(QColor("#1B1C56"));
    painter->drawText(item.ballPath->boundingRect(), Qt::AlignCenter, QString("%1").arg(count));
    start += angle;
    ++count;
  }
  painter->restore();
}
void RingElectrodeBar::DrawStartButton(QPainter *painter)
{
    painter->save();
 auto distance = GetButtonRingRadius();
 auto angle = start_button_angle;

 painter->setPen(Qt::NoPen);
 painter->setBrush(startEnable ? QBrush("#3866e6") : QBrush("#828282"));
 DrawRoundedRingBorder(painter, 90 - angle / 2, angle, distance.first, distance.second);

 painter->setPen(QPen(QBrush("#F6F7FC"), 4));
 painter->setBrush(Qt::NoBrush);
 auto lineRadius = (distance.first + distance.second) / 2;
 auto lineRect = QRectF{lineRadius, lineRadius, -lineRadius * 2, -lineRadius * 2};
 painter->drawArc(lineRect, startButtonTrackAngle * 8, -startButtonTrackAngle * 16);
 auto startSignal = startButtonChangedFlag && startButtonPositionPercent > enable_threshold;
 painter->setPen(Qt::NoPen);
 painter->setBrush(startSignal ? QBrush("#FFDB94") : QBrush("#F6F7FC"));


 auto pathWidth = (distance.second - distance.first);
 auto radius = pathWidth * 0.8 / 2;
 auto startAngle = 90 - startButtonTrackAngle / 2;
 auto startOffset = startButtonChangedFlag ? startButtonTrackAngle * startButtonPositionPercent : 0;
 auto path = DrawBall(painter, startAngle + startOffset, pathWidth / 2 + distance.first, radius);

 if (!startButtonChangedFlag) {
   startPath = path;
 }
 if (startSignal) {
   QPixmap pixmap(":/pfaPage/start.png");
   auto rect = path->boundingRect().toRect();
   painter->drawPixmap(rect, pixmap.scaled(rect.width(), rect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
 }
 painter->restore();
}
void RingElectrodeBar::DrawUpdateButton(QPainter *painter)
{
    painter->save();
 auto distance = GetButtonRingRadius();
 auto angle = start_button_angle;
 auto ringWidth = (distance.second - distance.first);
 painter->setPen(Qt::NoPen);
 painter->setBrush(QBrush("#3866e6"));
 updatePath = DrawRoundedRingBorder(painter, 270 - angle / 2, angle, distance.first, distance.second);
 painter->setPen(Qt::NoPen);
 painter->setBrush(QBrush("#f6f7fa"));
 auto radius = ringWidth * 0.8 / 2;
 auto path = DrawBall(painter, 270, ringWidth / 2 + distance.first, radius);
 QPixmap pixmap(":/pfaPage/update.png");
 auto rect = path->boundingRect().toRect();
 painter->drawPixmap(rect, pixmap.scaled(rect.width(), rect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
 painter->restore();
}
void RingElectrodeBar::DrawImpedanceInfo(QPainter *painter)
{
    painter->save();
     auto distance = GetButtonRingRadius();
     auto angle = start_button_angle;
     auto ringWidth = (distance.second - distance.first);
     painter->setPen(Qt::NoPen);
     painter->setBrush(QBrush("#3866e6"));
     updatePath = DrawRoundedRingBorder(painter, 270 - angle / 2, angle, distance.first, distance.second);
     painter->setPen(Qt::NoPen);
     painter->setBrush(QBrush("#f6f7fa"));
     auto radius = ringWidth * 0.8 / 2;
     auto path = DrawBall(painter, 270, ringWidth / 2 + distance.first, radius);
     QPixmap pixmap(":/pfaPage/update.png");
     auto rect = path->boundingRect().toRect();
     painter->drawPixmap(rect, pixmap.scaled(rect.width(), rect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
     painter->restore();
}

RingElectrodeBar::PathPtr RingElectrodeBar::DrawSwitchBackground(QPainter *painter, qreal radius) {
  painter->save();
  radius = (int) radius;
  auto path = std::make_shared<QPainterPath>();
  path->moveTo(0, radius);
  path->lineTo(textWidth / 2, radius);
  path->arcTo(QRect(textWidth / 2 + radius, radius, -radius * 2, -radius * 2), 90, 180);
  path->lineTo(-textWidth / 2, -radius);
  path->arcTo(QRect(-(textWidth / 2 + radius), -radius, radius * 2, radius * 2), 90, 180);
  path->lineTo(0, radius);
  painter->drawPath(*path);
  painter->restore();
  return path;
}

void RingElectrodeBar::updateSwitchValue() {
  switch (static_cast<int>(switchValue)) {
    case static_cast<int>(power_preset_t::low): {
      auto offset = switchStartX - switchStep;
      if (offset > 0) {
        switchStartX = offset;
      } else {
        switchStartX = 0;
        switchTimer->stop();
      }
      break;
    }
    case static_cast<int>(power_preset_t::high): {
      auto offset = switchStartX + switchStep;
      if (offset < textWidth) {
        switchStartX = offset;
      } else {
        switchStartX = textWidth;
        switchTimer->stop();
      }
      break;
    }
  }
  update();
}

RingElectrodeBar::PathPtr RingElectrodeBar::DrawRoundedRingBorder(QPainter *painter, qreal startAngle, qreal sweepLength, qreal innerRadius, qreal outerRadius) {
  painter->save();

  auto endAngle = startAngle + sweepLength;
  auto path = std::make_shared<QPainterPath>();
  auto roundedAngle = border_radius / (2 * M_PI * innerRadius) * 360;

  auto innerRect = QRectF{innerRadius, innerRadius, -innerRadius * 2, -innerRadius * 2};
  auto outerRect = QRectF{outerRadius, outerRadius, -outerRadius * 2, -outerRadius * 2};

  auto startCosY = -qCos(qDegreesToRadians(startAngle));
  auto startSinX = qSin(qDegreesToRadians(startAngle));
  auto startCosOffsetY = -qCos(qDegreesToRadians(startAngle + roundedAngle));
  auto startSinOffsetX = qSin(qDegreesToRadians(startAngle + roundedAngle));

  auto endCosY = -qCos(qDegreesToRadians(endAngle));
  auto endSinX = qSin(qDegreesToRadians(endAngle));
  auto endCosOffsetY = -qCos(qDegreesToRadians(endAngle - roundedAngle));
  auto endSinOffsetX = qSin(qDegreesToRadians(endAngle - roundedAngle));

  path->moveTo((innerRadius + border_radius) * startSinX, (innerRadius + border_radius) * startCosY);
  path->lineTo((outerRadius - border_radius) * startSinX, (outerRadius - border_radius) * startCosY);
  path->quadTo(outerRadius * startSinX, outerRadius * startCosY, outerRadius * startSinOffsetX, outerRadius * startCosOffsetY);
  path->arcTo(outerRect, 270 - (startAngle + roundedAngle), -(sweepLength - 2 * roundedAngle));
  path->quadTo(outerRadius * endSinX, outerRadius * endCosY, (outerRadius - border_radius) * endSinX, (outerRadius - border_radius) * endCosY);
  path->lineTo((innerRadius + border_radius) * endSinX, (innerRadius + border_radius) * endCosY);
  path->quadTo(innerRadius * endSinX, innerRadius * endCosY, innerRadius * endSinOffsetX, innerRadius * endCosOffsetY);
  path->arcTo(innerRect, 270 - (endAngle - roundedAngle), sweepLength - 2 * roundedAngle);
  path->quadTo(innerRadius * startSinX, innerRadius * startCosY, (innerRadius + border_radius) * startSinX, (innerRadius + border_radius) * startCosY);
  path->closeSubpath();

  painter->drawPath(*path);
  painter->restore();

  return path;
}
RingElectrodeBar::PathPtr RingElectrodeBar::DrawBall(QPainter *painter, qreal angle, qreal distance, qreal radius, bool isPaint) {
  painter->save();
  auto path = std::make_shared<QPainterPath>();
  auto centerCircle = QPointF{distance * qSin(qDegreesToRadians(angle)), distance * (-qCos(qDegreesToRadians(angle)))};
  auto rect = QRectF{centerCircle.x() - radius, centerCircle.y() - radius, radius * 2, radius * 2};
  path->addEllipse(rect);
  if (isPaint)
    painter->drawPath(*path);
  painter->restore();
  return path;
}

void RingElectrodeBar::OnElectrodeUpdateRequest() {
  isAllSelected = true;
  selecting_state = false;
  {
    lock_guard_t lock_t(electrode_state_mutex);
    electrode_state.clear();
  }

  electrode_selected_start = 1;
  electrode_selected_end = electrode_number;

  {
    lock_guard_t lock_t(electrode_state_mutex);
    electrode_state.resize(electrode_number, ElectrodeItem{});
  }

  update();
}

