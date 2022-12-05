#ifndef RPFASUMMARYCONTENT_H
#define RPFASUMMARYCONTENT_H

#include <QWidget>

namespace Ui {
class RPFASummaryContent;
}

class RPFASummaryContent : public QWidget
{
    Q_OBJECT

public:
    explicit RPFASummaryContent(QWidget *parent = nullptr);
    ~RPFASummaryContent();

private:
    Ui::RPFASummaryContent *ui;
};

#endif // RPFASUMMARYCONTENT_H
