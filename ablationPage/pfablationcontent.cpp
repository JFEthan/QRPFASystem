#include "pfablationcontent.h"
#include "ui_pfablationcontent.h"

PfAblationContent::PfAblationContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PfAblationContent)
{
    ui->setupUi(this);
    initBaseLayout();
    isEcgWidgetShow = false;
    SetPFAblationType(pfablation_type_RingElectrode);
    SetECGWidgetShowStatus(false);

}

PfAblationContent::~PfAblationContent()
{
    uninitBaseLayout();
    delete ui;

}
void PfAblationContent::SetECGWidgetShowStatus(bool isShow)
{
    if(isEcgWidgetShow != isShow)
    {
        removeBaseLayout();
        isEcgWidgetShow = isShow;
        SetBaseLayout();
    }
}

void PfAblationContent::initBaseLayout()
{
    if(baseLayout == nullptr) baseLayout = new QGridLayout(this);
    if(pfactionbar == nullptr) pfactionbar = new PFActionbar(this);
    if(ecgCustomplot == nullptr) ecgCustomplot = new ECGCustomWidget(this);

    if(horizontalSpacer3 == nullptr)  horizontalSpacer3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if(horizontalSpacer4 == nullptr)  horizontalSpacer4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if(pfIntegratedaBar == nullptr)   pfIntegratedaBar = new PFIntegratedaBar(this);
    if(hBoxFIntegratedaBaLayoutr == nullptr)
    {
        hBoxFIntegratedaBaLayoutr = new QHBoxLayout(this);
        hBoxFIntegratedaBaLayoutr->addSpacerItem(horizontalSpacer3);
        hBoxFIntegratedaBaLayoutr->addWidget(pfIntegratedaBar);
        hBoxFIntegratedaBaLayoutr->addSpacerItem(horizontalSpacer4);
        hBoxFIntegratedaBaLayoutr->setStretch(0,30);
        hBoxFIntegratedaBaLayoutr->setStretch(1,138);
        hBoxFIntegratedaBaLayoutr->setStretch(2,30);

    }
    if(vIntegrateda_EcgLayout== nullptr)
    {
        vIntegrateda_EcgLayout = new QVBoxLayout(this);
    }

    if(ringElectrodeBar == nullptr) ringElectrodeBar = new RingElectrodeBar(this);
   if(hBoxRingElectrodeBarLayout == nullptr)
    {
        hBoxRingElectrodeBarLayout = new QHBoxLayout(this);
        hBoxRingElectrodeBarLayout->addSpacerItem(horizontalSpacer3);
        hBoxRingElectrodeBarLayout->addWidget(ringElectrodeBar);
        hBoxRingElectrodeBarLayout->addSpacerItem(horizontalSpacer4);
        hBoxRingElectrodeBarLayout->setStretch(0,30);
        hBoxRingElectrodeBarLayout->setStretch(1,138);
        hBoxRingElectrodeBarLayout->setStretch(2,30);
    }

   if(vRingElectrode_EcgLayout == nullptr)
   {
       vRingElectrode_EcgLayout = new QVBoxLayout(this);
   }

    if(ecgButton == nullptr)
    {
        ecgButton = new QPushButton("ECG",this);
        connect(ecgButton, SIGNAL(clicked()), this, SLOT(OnEcgButtonClicked()));
        connect(ecgButton, &QPushButton::toggled, this, &PfAblationContent::OnStateChanged);
    }

    if(horizontalSpacer1 == nullptr)  horizontalSpacer1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if(horizontalSpacer2 == nullptr)  horizontalSpacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    if(hBoxToolbar == nullptr)
    {
        hBoxToolbar = new QHBoxLayout(this);
        hBoxToolbar->addWidget(ecgButton);
        hBoxToolbar->addSpacerItem(horizontalSpacer1);
        hBoxToolbar->addWidget(pfactionbar);
        hBoxToolbar->addItem(horizontalSpacer2);
        hBoxToolbar->setStretch(0,8);
        hBoxToolbar->setStretch(1,65);
        hBoxToolbar->setStretch(2,60);
        hBoxToolbar->setStretch(3,65);
    }
   // 8,65,60,65
}
void PfAblationContent::destroyMainLayoutWidget(QLayout* layout){
    if(layout == nullptr) return;
     QLayoutItem *child;
    int num = layout->count();
    if(num > 0 && !layout->isEmpty()){
        while ((child = layout->takeAt(0)) != nullptr) {
            if(child->widget())
            {
                child->widget()->setParent(nullptr);
            }
            else if(child->layout()){
                destroyMainLayoutWidget(child->layout());
            }
             //delete child;
        }
    }

}
void PfAblationContent::uninitBaseLayout()
{
    if(baseLayout) destroyMainLayoutWidget(baseLayout);
    if(hBoxRingElectrodeBarLayout) destroyMainLayoutWidget(hBoxRingElectrodeBarLayout);
    if(hBoxFIntegratedaBaLayoutr) destroyMainLayoutWidget(hBoxFIntegratedaBaLayoutr);
    if(hBoxToolbar) destroyMainLayoutWidget(hBoxToolbar);
    if(vIntegrateda_EcgLayout) destroyMainLayoutWidget(vIntegrateda_EcgLayout);
    if(vRingElectrode_EcgLayout) destroyMainLayoutWidget(vRingElectrode_EcgLayout);

    if(pfactionbar != nullptr)
    {
        delete pfactionbar;
        pfactionbar = nullptr;
    }
    if(ecgCustomplot != nullptr)
    {
        delete ecgCustomplot;
        ecgCustomplot = nullptr;
    }

    if(horizontalSpacer3 != nullptr)
    {
        delete horizontalSpacer3;
        horizontalSpacer3 = nullptr;
    }
    if(horizontalSpacer4 != nullptr)
    {
        delete horizontalSpacer4;
        horizontalSpacer4 = nullptr;
    }

    if(vRingElectrode_EcgLayout != nullptr)
    {
        delete vRingElectrode_EcgLayout;
        vRingElectrode_EcgLayout = nullptr;
    }
    if(hBoxRingElectrodeBarLayout != nullptr)
    {
        delete hBoxRingElectrodeBarLayout;
        hBoxRingElectrodeBarLayout = nullptr;
    }

    if(hBoxFIntegratedaBaLayoutr != nullptr)
    {
        delete hBoxFIntegratedaBaLayoutr;
        hBoxFIntegratedaBaLayoutr = nullptr;
    }

    if(vIntegrateda_EcgLayout!= nullptr)
    {
        delete vIntegrateda_EcgLayout;
        vIntegrateda_EcgLayout = nullptr;
    }

    if(ecgButton != nullptr)
    {
        delete ecgButton;
        ecgButton = nullptr;
    }

    if(horizontalSpacer1 != nullptr)
    {
        delete horizontalSpacer1;
        horizontalSpacer1 = nullptr;
    }
    if(horizontalSpacer2 != nullptr)
    {
        delete horizontalSpacer2;
        horizontalSpacer2 = nullptr;
    }
    if(hBoxToolbar != nullptr)
    {
        delete hBoxToolbar;
        hBoxToolbar = nullptr;
    }
    if(ringElectrodeBar != nullptr)
    {
        delete ringElectrodeBar;
        ringElectrodeBar = nullptr;
    }
    if(pfIntegratedaBar != nullptr)
    {
        delete pfIntegratedaBar;
        pfIntegratedaBar = nullptr;
    }
    if(baseLayout != nullptr)
    {
        delete baseLayout;
        baseLayout = nullptr;
    }
}
void PfAblationContent::SetBaseLayout()
{
    switch (pfa_type)
    {
    case pfablation_type_RingElectrode:
    {
        setBaseLayoutElectrode();
    }break;
    case pfablation_type_integrated:
    {
        setBaseLayoutIntegrated();
    }break;
    default:break;
    }
    update();
}
void PfAblationContent::SetPFAblationType(enumPFABLATION_TYPE _type)
{
    if(pfa_type != _type)
    {
        removeBaseLayout();
        pfa_type = _type;

        SetBaseLayout();
    }
}
void PfAblationContent::removeBaseLayout()
{
    switch (pfa_type)
    {
    case pfablation_type_RingElectrode:
    {
        baseLayout->removeItem(vRingElectrode_EcgLayout);
        baseLayout->removeItem(hBoxToolbar);
        if(isEcgWidgetShow)
        {
            vRingElectrode_EcgLayout->removeWidget(ecgCustomplot);
            vRingElectrode_EcgLayout->removeItem(hBoxFIntegratedaBaLayoutr);
        }
        else
        {
             vRingElectrode_EcgLayout->removeItem(hBoxFIntegratedaBaLayoutr);
        }


    }break;
    case pfablation_type_integrated:
    {
        baseLayout->removeItem(vIntegrateda_EcgLayout);
        baseLayout->removeItem(hBoxToolbar);
        if(isEcgWidgetShow)
        {
            vRingElectrode_EcgLayout->removeWidget(ecgCustomplot);
            vRingElectrode_EcgLayout->removeItem(hBoxRingElectrodeBarLayout);
        }
        else
        {
             vRingElectrode_EcgLayout->removeItem(hBoxFIntegratedaBaLayoutr);
        }


    }break;
    default:break;
    }

}
void PfAblationContent::setBaseLayoutElectrode()
{

    if(isEcgWidgetShow)
    {
    vRingElectrode_EcgLayout->addLayout(hBoxRingElectrodeBarLayout);
    vRingElectrode_EcgLayout->addWidget(ecgCustomplot);
    vRingElectrode_EcgLayout->setStretch(0,600);
    vRingElectrode_EcgLayout->setStretch(1,200);
    }
    else
    {
         vRingElectrode_EcgLayout->addLayout(hBoxRingElectrodeBarLayout);
    }


    baseLayout->addLayout(vRingElectrode_EcgLayout, 0, 0);
    baseLayout->addLayout(hBoxToolbar, 1, 0);
    baseLayout->setSpacing(2);
    baseLayout->setRowStretch(0,750);
    baseLayout->setRowStretch(1,40);
    this->setLayout(baseLayout);
    update ();
}
void PfAblationContent::setBaseLayoutIntegrated()
{
    if(isEcgWidgetShow)
    {
        vIntegrateda_EcgLayout->addLayout(hBoxFIntegratedaBaLayoutr);
        vIntegrateda_EcgLayout->addWidget(ecgCustomplot);
        vIntegrateda_EcgLayout->setStretch(0,600);
        vIntegrateda_EcgLayout->setStretch(1,200);
    }
    else
    {
         vIntegrateda_EcgLayout->addLayout(hBoxFIntegratedaBaLayoutr);
    }


    baseLayout->addLayout(vIntegrateda_EcgLayout, 0, 0);
    baseLayout->addLayout(hBoxToolbar, 1, 0);
    baseLayout->setRowStretch(0,770);
    baseLayout->setRowStretch(1,30);
    this->setLayout(baseLayout);
    update ();
}

void PfAblationContent::OnStateChanged(bool state) {
    ecgButton->setChecked(state);
    SetECGWidgetShowStatus(state);
}
void PfAblationContent::OnEcgButtonClicked()
{
    isEcgClicked = !isEcgClicked;
    ecgButton->setChecked(isEcgClicked);
    SetECGWidgetShowStatus(isEcgClicked);
}
