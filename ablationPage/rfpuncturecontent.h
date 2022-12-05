#ifndef RFPUNCTURECONTENT_H
#define RFPUNCTURECONTENT_H

#include <QWidget>

namespace Ui {
class RfPunctureContent;
}

class RfPunctureContent : public QWidget
{
    Q_OBJECT

public:
    explicit RfPunctureContent(QWidget *parent = nullptr);
    ~RfPunctureContent();

private:
    Ui::RfPunctureContent *ui;
};

#endif // RFPUNCTURECONTENT_H
