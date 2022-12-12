#ifndef PFABLATIONCONTENT_H
#define PFABLATIONCONTENT_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include "../toolbar/PFActionbar/pfactionbar.h"
#include "../toolbar/accuCustomplot/ecgcustomwidget.h"
#include "../toolbar/PFIntegratedaBar/pfintegratedabar.h"
#include "../toolbar/RingElectrodebar/ringelectrodebar.h"

namespace Ui {
class PfAblationContent;
}

class PfAblationContent : public QWidget
{
    Q_OBJECT

public:
    enum enumPFABLATION_TYPE : char
    {
        pfablation_type_unkown,
        pfablation_type_RingElectrode,
        pfablation_type_integrated
    };
public:
    explicit PfAblationContent(QWidget *parent = nullptr);
    ~PfAblationContent();
public:
    void SetECGWidgetShowStatus(bool isShow);
    void SetPFAblationType(enumPFABLATION_TYPE _type);

private:
    void initBaseLayout();
    void uninitBaseLayout();
    void SetBaseLayout();
    void removeBaseLayout();
    void setBaseLayoutElectrode();
    void setBaseLayoutIntegrated();
    void destroyMainLayoutWidget(QLayout* layout);
public slots:
  void OnStateChanged(bool);
  void OnEcgButtonClicked();
private:
    bool isEcgClicked = false;
    bool isEcgWidgetShow = false;
    enumPFABLATION_TYPE pfa_type = pfablation_type_unkown;
    Ui::PfAblationContent *ui;
    QGridLayout *baseLayout= nullptr;
    PFActionbar *pfactionbar = nullptr;
    ECGCustomWidget *ecgCustomplot = nullptr;


    QWidget * statisticsWidget = nullptr;
    QHBoxLayout *hBoxFIntegratedaBaLayoutr= nullptr;

    PFIntegratedaBar  * pfIntegratedaBar  = nullptr;
    QVBoxLayout *vIntegrateda_EcgLayout= nullptr;

    QHBoxLayout *hBoxRingElectrodeBarLayout= nullptr;
    RingElectrodeBar  * ringElectrodeBar  = nullptr;
    QVBoxLayout *vRingElectrode_EcgLayout= nullptr;

    QHBoxLayout *hBoxToolbar= nullptr;
    QPushButton *ecgButton = nullptr;
    QSpacerItem * horizontalSpacer1 = nullptr;
    QSpacerItem * horizontalSpacer2 = nullptr;
    QSpacerItem * horizontalSpacer3 = nullptr;
    QSpacerItem * horizontalSpacer4 = nullptr;

};

#endif // PFABLATIONCONTENT_H
