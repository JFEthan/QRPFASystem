#ifndef RFABLATIONCONTENT_H
#define RFABLATIONCONTENT_H

#include <QWidget>
class CustomWidget;
namespace Ui {
class RfAblationContent;
}

class RfAblationContent : public QWidget
{
    Q_OBJECT

public:
    explicit RfAblationContent(QWidget *parent = nullptr);
    ~RfAblationContent();
private:
    CustomWidget *custom_widget = nullptr;
    Ui::RfAblationContent *ui;
};

#endif // RFABLATIONCONTENT_H
