#ifndef CATHETERPRESET_H
#define CATHETERPRESET_H

#include <QWidget>
class AblationPreset;
namespace Ui {
class catheterpreset;
}

class catheterpreset : public QWidget
{
    Q_OBJECT

public:
    catheterpreset(QWidget *parent = nullptr);
    ~catheterpreset();
    void InitLauout();
    void Lauout();
private:
    AblationPreset *preset_widget = nullptr;
    Ui::catheterpreset *ui;
};
#endif // CATHETERPRESET_H
