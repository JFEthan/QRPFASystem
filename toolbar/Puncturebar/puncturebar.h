#ifndef PUNCTUREBAR_H
#define PUNCTUREBAR_H

#include <QWidget>
#include "../toolbar/NumericBar/numericbar.h"

namespace Ui {
class Puncturebar;
}

class Puncturebar : public QWidget
{
    Q_OBJECT

public:
    explicit Puncturebar(QWidget *parent = nullptr);
    ~Puncturebar();

private:
    Ui::Puncturebar *ui;
};

#endif // PUNCTUREBAR_H
