#include "numericbar.h"
#include <QStyleOption>
#include <QTextCodec>

const int  UNIT_SIZE=30;
const int  top_Button_Width=80;
const int  Right_BUTTON_SIZE=20;


Numericbar::Numericbar(QWidget *parent) :
    QWidget(parent)
{

    this->setStyleSheet("{background-color: transparent;}");

    connect(this, &Numericbar::AutoModeButtonClicked,this,&Numericbar::OnAutoModeButtonRequest);
    connect(this, &Numericbar::UpButtonClicked,this,&Numericbar::OnUpButtonRequest);
    connect(this, &Numericbar::DownButtonClicked,this,&Numericbar::OnDownButtonRequest);

}


void Numericbar::SetType(typeNumericBar type)
{
    m_type = type;
    switch(m_type)
    {
    case type_numeric_pressure:
    {
        QTextCodec* cod=QTextCodec::codecForLocale();
        qstringUnit = cod->toUnicode("g");
        color = QColor("#00B0F0");
    }break;
    case type_numeric_impedance:
    {
        //qstringUnit = "Ω";
        //QTextCodec* cod=QTextCodec::codecForLocale();
        //qstringUnit = cod->toUnicode("Ω");
        qstringUnit = u8"Ω";

        color = Qt::green;
    }break;
    case type_numeric_flow_speed:
    {

         QTextCodec* cod=QTextCodec::codecForLocale();
         qstringUnit = cod->toUnicode("mL/min");
          color = QColor("#00FFFF");
    }break;
    case type_numeric_time:
    {
         qstringUnit = "S";
         color = Qt::white;

    }break;
    case type_numeric_temperature:
    {
        //QTextCodec* cod=QTextCodec::codecForLocale();
        //qstringUnit = cod->toUnicode("℃");
         qstringUnit = u8"℃";
         color = Qt::red;

    }break;
    case type_numeric_power:
    {
        QTextCodec* cod=QTextCodec::codecForLocale();
        qstringUnit = cod->toUnicode("W");

        color = Qt::yellow;

    }break;
    case type_numeric_ablation:
    {
        color = QColor("#00FFFF");

    }break;
    case type_numeric_angle:
    default:break;

    }

    if(showThresholValue == nullptr && (m_type == type_numeric_power || m_type ==  type_numeric_temperature || m_type == type_numeric_time))
    {
        showThresholValue = new QLineEdit(this);
        QPalette palette;
        palette.setColor(QPalette::Text, color);
        showThresholValue->setPalette(palette);

        connect(showThresholValue, &QLineEdit::textChanged, this, &Numericbar::OnthresholdChanged);
        QRegExp regExp("^d");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$
        showThresholValue->setValidator(new QRegExpValidator(regExp, this));
        showThresholValue->setStyleSheet("QLineEdit{font: 20pt;background-color: #00A0E9;border:none;}");

    }
    else
    {
        if(showThresholValue)
        {
            delete showThresholValue;
            showThresholValue = nullptr;
        }

    }
    update();
}


void Numericbar::SetThresholdValue(int value)
{
    thresholdValue = value;
    if(showThresholValue)
    {
        showThresholValue->setText(QString::number(thresholdValue));
    }
}
int Numericbar::GetThresholdValue()
{
    return thresholdValue;
}

void Numericbar::SetRealtimeValue(int value)
{
    realtimeValue = value;
}
int Numericbar::GetRealtimeValue()
{
    return realtimeValue;
}
void Numericbar::SetAutoButtonMode(bool mode)
{
    isAutoMode = mode;
}
bool Numericbar::GetAutoButtonMode()
{
    return isAutoMode;
}
void Numericbar::SetfontSize(int size)
{
    fontsize = size;
}
void Numericbar::mousePressEvent(QMouseEvent *event) {
 // auto offset = QPoint(this->width(), this->height());
  auto point = event->pos();
  if (btnAutoModePath && btnAutoModePath->contains(point)) {
    emit AutoModeButtonClicked();
    return;
  } else if (btnUpPath && btnUpPath->contains(point)) {
    emit UpButtonClicked();
    return;
  } else if (btnDownPath && btnDownPath->contains(point)) {
    emit DownButtonClicked();
    return;
  }
}

void Numericbar::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    switch(m_type)
    {
    case type_numeric_pressure:
    case type_numeric_impedance:
    {
        DrawPressurebar(&painter);
    }break;
    case type_numeric_angle:
    {
        DrawAngledbar(&painter);
    }break;
    case type_numeric_flow_speed:
    {
        DrawFlowSpeedbar(&painter);
    }break;
    case type_numeric_time:
    case type_numeric_temperature:
    case type_numeric_power:
    {
        DrawTimebar(&painter);
    }break;
    case type_numeric_ablation:
    {
        DrawAblationbar(&painter);
    }break;
    default:break;
    }
}


void Numericbar::DrawPressurebar(QPainter *painter)
{
  painter->save();

  painter->setPen(Qt::NoPen);



  auto path1 = DrawRectBorder(painter,0, 0,this->width(),height());
  auto rect = path1->boundingRect().toRect();

  painter->setPen(color);
  DrawUnitArea(painter,top_Button_Width, 0,width()-top_Button_Width,UNIT_SIZE,30);
  DrawRealtimeArea(painter,0, UNIT_SIZE,width(),height()-UNIT_SIZE);

  painter->restore();
}
void Numericbar::DrawAngledbar(QPainter *painter)
{
    painter->save();
    auto path1 = DrawRectBorder(painter,0, 0,this->width(),height());
    auto rect = path1->boundingRect().toRect();

    QPixmap pixmap(":/rf/angle.png");

    painter->drawPixmap(rect, pixmap.scaled(rect.width(), rect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));


    painter->restore();
}
void Numericbar::DrawFlowSpeedbar(QPainter *painter)
{
    painter->save();

    painter->setPen(Qt::NoPen);
    auto path1 = DrawRectBorder(painter,0, 0,width(),height());

   // qreal width = this->width()-UP_BUTTON_SIZE;

    DrawAutoModeButtonArea(painter,0, 0,top_Button_Width,UNIT_SIZE);
    painter->setPen(QColor(Qt::white));
    DrawUnitArea(painter,top_Button_Width, 0,width()-top_Button_Width,UNIT_SIZE,16);
    painter->setPen(color);
    DrawRealtimeArea(painter,0, UNIT_SIZE,width(),height()-UNIT_SIZE);

    painter->restore();
}

void Numericbar::DrawTimebar(QPainter *painter)
{
    painter->save();

    painter->setPen(Qt::NoPen);
    auto path1 = DrawRectBorder(painter,0, 0,width(),height());
    auto rect = path1->boundingRect().toRect();

    DrawThresholArea(painter,0, 0,top_Button_Width,UNIT_SIZE);
    DrawUpButtonArea(painter,width()-Right_BUTTON_SIZE, 0,Right_BUTTON_SIZE,Right_BUTTON_SIZE);
    DrawDownButtonArea(painter,width()-Right_BUTTON_SIZE, Right_BUTTON_SIZE+2,Right_BUTTON_SIZE,Right_BUTTON_SIZE);

    painter->setPen(color);
    DrawUnitArea(painter,top_Button_Width, 0,width()-top_Button_Width-Right_BUTTON_SIZE,UNIT_SIZE,30);

    DrawRealtimeArea(painter,0, UNIT_SIZE,width()-Right_BUTTON_SIZE,height()-UNIT_SIZE-2);

    painter->restore();
}
void Numericbar::SetAblationName(QString ablationName)
{
    qstringName = ablationName;
}
void Numericbar::DrawAblationbar(QPainter *painter)
{
    painter->save();

    painter->setPen(Qt::NoPen);
    auto path1 = DrawRectBorder(painter,0, 0,width(),height());
    auto rect = path1->boundingRect().toRect();

    painter->setPen(QColor(Qt::white));
    QRect rectUnit(0,0, width()/2,height());
    QFont font ;//layoutFont(rectUnit,qstringUnit);
    font.setPixelSize(20);
    font.setWeight(QFont::Black);
    font.setFamily(QStringLiteral("黑体"));
    //字体： 宋体 ,黑体 ,Times New Roman、微软雅黑、幼圆、仿宋
    painter->setFont(font);
    painter->drawText(rectUnit,  Qt::AlignCenter , qstringName);

    painter->setPen(color);
    DrawRealtimeArea(painter,width()/2, 0,width()/2,height());

    painter->restore();
}


void Numericbar::DrawUnitArea(QPainter *painter,qreal x, qreal y, qreal width, qreal height,int fontsize)
{

    QRect rectUnit(x,y, width,height);
    QFont font ;//layoutFont(rectUnit,qstringUnit);
    font.setPixelSize(fontsize);
    font.setWeight(QFont::Black);
    font.setFamily(QStringLiteral("黑体"));
    //字体： 宋体 ,黑体 ,Times New Roman、微软雅黑、幼圆、仿宋
    painter->setFont(font);
    painter->drawText(rectUnit,  Qt::AlignRight , qstringUnit);

}
void Numericbar::DrawRealtimeArea(QPainter *painter,qreal x, qreal y, qreal width, qreal height)
{
    painter->setPen(color);
    auto path = DrawRectBorder(painter,x,y, width,height);
    auto rectRealValue = path->boundingRect();
	QFont font  ; layoutFont(rectRealValue.toRect(), QString::number(realtimeValue));
    font.setFamily(QStringLiteral("黑体"));
    font.setPixelSize(fontsize);
    painter->setFont(font);

    painter->drawText(rectRealValue,  Qt::AlignHCenter | Qt::TextWordWrap, QString::number(realtimeValue));


}

void Numericbar::DrawAutoModeButtonArea(QPainter *painter, qreal x, qreal y, qreal width, qreal height)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor("#00A0E9"));
    btnAutoModePath = DrawRectBorder(painter,x, y,width,height,10);
    auto rect = btnAutoModePath->boundingRect().toRect();

    QString btmName;
    if(isAutoMode)
    {
         btmName = tr("AUTO");
    }
    else
    {
        btmName = tr("Manual");
    }

    QFont font;
    font.setPixelSize(16);
    font.setWeight(QFont::Black);
    font.setFamily(QStringLiteral("黑体"));

    painter->setFont(font);
    painter->setPen(QColor(Qt::white));
    painter->drawText(rect,  Qt::AlignHCenter | Qt::TextWordWrap, btmName);

    painter->restore();
}



Numericbar::PathPtr Numericbar::DrawRectBorder(QPainter *painter, qreal x, qreal y, qreal width, qreal height,qreal angle)
{
    painter->save();
   auto path = std::make_shared<QPainterPath>();

   auto rect = QRect(x, y,width,height);
   path->addRoundedRect(rect,angle,angle,Qt::AbsoluteSize);
   painter->drawPath(*path);
   painter->restore();
   return path;
}


void Numericbar::DrawThresholArea(QPainter *painter, qreal x, qreal y, qreal width, qreal height)
{
    if(showThresholValue)
    {
        showThresholValue->setGeometry(QRect(x, y, width, height));

    }
}


void Numericbar::DrawUpButtonArea(QPainter *painter, qreal x, qreal y, qreal width, qreal height)
{
     btnUpPath = DrawRectBorder(painter,x,y,width,height);
     QPixmap pixmap(":/rf/up.png");
     auto rectUp = btnUpPath->boundingRect().toRect();
     painter->drawPixmap(rectUp, pixmap.scaled(rectUp.width(), rectUp.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

}
void Numericbar::DrawDownButtonArea(QPainter *painter, qreal x, qreal y, qreal width, qreal height)
{
    btnDownPath = DrawRectBorder(painter,x,y,width,height);
    QPixmap pixmap(":/rf/down.png");
    auto rectDown = btnDownPath->boundingRect().toRect();
    painter->drawPixmap(rectDown, pixmap.scaled(rectDown.width(), rectDown.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}

QFont Numericbar::layoutFont(QRect rectLbl, QString text)
{
     QFont font = this->font();
     int size = font.pointSize();
	 if (size <= 0)size = 12;
     QFontMetrics fontMetrics(font);
     QRect rect = fontMetrics.boundingRect(rectLbl,
       Qt::TextWordWrap, text);
     // decide whether to increase or decrease
     int step = rect.height() > rectLbl.height() ? -1 : 1;
     // iterate until text fits best into rectangle of label
     for (;;) {
       font.setPointSize(size + step);
       QFontMetrics fontMetrics(font);
       rect = fontMetrics.boundingRect(rectLbl,
         Qt::TextWordWrap, text);
       if (size <= 1) {
         //cout << "Font cannot be made smaller!" << endl;
         break;
       }
       if (step < 0) {
         size += step;
         if (rect.height() < rectLbl.height()) break;
       } else {
         if (rect.height() > rectLbl.height()) break;
         size += step;
       }
     }
     font.setPointSize(size);
     //setFont(font);
     return  font;
}


void Numericbar::OnAutoModeButtonRequest()
{
    isAutoMode = !isAutoMode;
    update();
}
void Numericbar::OnUpButtonRequest()
{
    thresholdValue++;
    if(thresholdValue > 10000) thresholdValue = 9999;

    if(showThresholValue)
    {
        showThresholValue->setText(QString::number(thresholdValue));
    }
}
void Numericbar::OnDownButtonRequest()
{
    thresholdValue--;
    if(thresholdValue <= 0) thresholdValue = 1;

    if(showThresholValue)
    {
        showThresholValue->setText(QString::number(thresholdValue));
    }
}
 void Numericbar::OnthresholdChanged(QString content)
 {
     thresholdValue = content.toInt();
     if(showThresholValue)
     {
         showThresholValue->setText(QString::number(thresholdValue));
     }
 }

