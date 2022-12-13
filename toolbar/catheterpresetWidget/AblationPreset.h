#ifndef ABLATIONPRESET_H
#define ABLATIONPRESET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class AblationPreset; }
QT_END_NAMESPACE

class AblationPreset : public QWidget
{
    Q_OBJECT

public:
    AblationPreset(QWidget *parent = nullptr);
    ~AblationPreset();

private slots:
    void OnbtnPresetEditClicked();
    void OnbtnPresetSettingClicked();
    void OnbtnPresetConfirmClicked();
    void OnbtnAddCatheterClicked();
    void OnbtnDeleteCatheterClicked();
    void OnbtnAddListClicked();
    void OnbtnDeleteListClicked();
    //void OnbtnCatheterConfirmClicked();
    //void OnbtnListConfirmClicked();

private:
    void Init();
    Ui::AblationPreset *ui;
};
#endif // ABLATIONPRESET_H
