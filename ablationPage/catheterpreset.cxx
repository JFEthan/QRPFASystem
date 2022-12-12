#include "PfasWidgetPFAAblationPreset.h"
#include "PfasWidgetPFAPresetCatheter.h"
#include "PfasWidgetPFAPresetTableList.h"
#include "PfasWidgetPFAPresetButtons.h"
#include <QtCore/QVariant>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "PfasUIManager.h"

PfasWidgetPFAAblationPreset::PfasWidgetPFAAblationPreset(QWidget *parent, PfasUIManager *ui_mgr)
    : PfasUIContent(parent), ui_mgr(ui_mgr)
{
    setupUi(this);
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *Catheter;
    QVBoxLayout *CatheterLayout;
    PfasWidgetPFAPresetCatheter *presetCatheter;
    QVBoxLayout *CatheterLayout2;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *ButtonLayout;
    PfasWidgetPFAPresetButtons *presetButton;
    QVBoxLayout *Preset;
    QVBoxLayout *PresetLayout;
    PfasWidgetPFAPresetTableList *presetTableList;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *horizontalSpacer_3;

    gridLayoutWidget = new QWidget();
    gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
    gridLayoutWidget->setGeometry(QRect(0, 0, 971, 601));
    gridLayout = new QGridLayout(gridLayoutWidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);

    Catheter = new QVBoxLayout();
    Catheter->setObjectName(QString::fromUtf8("Catheter"));
    Catheter->setContentsMargins(-1, -1, -1, 0);
    CatheterLayout = new QVBoxLayout();
    CatheterLayout->setObjectName(QString::fromUtf8("CatheterLayout"));

    presetCatheter = new PfasWidgetPFAPresetCatheter(this, ui_mgr);
    presetCatheter->setObjectName(QString::fromUtf8("presetCatheter"));
    presetCatheter->setStyleSheet(QString::fromUtf8(""));
    CatheterLayout->addWidget(presetCatheter);

    Catheter->addLayout(CatheterLayout);

    CatheterLayout2 = new QVBoxLayout();
    CatheterLayout2->setObjectName(QString::fromUtf8("CatheterLayout2"));
    Catheter->addLayout(CatheterLayout2);

    verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    Catheter->addItem(verticalSpacer);

    Catheter->setStretch(0, 4);
    Catheter->setStretch(1, 4);
    Catheter->setStretch(2, 1);

    gridLayout->addLayout(Catheter, 0, 1, 1, 1);

    horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Ignored, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer_2, 0, 0, 1, 1);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Ignored, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer, 0, 5, 1, 1);

    ButtonLayout = new QHBoxLayout();
    ButtonLayout->setObjectName(QString::fromUtf8("ButtonLayout"));
    presetButton = new PfasWidgetPFAPresetButtons(this, ui_mgr);
    presetButton->setObjectName(QString::fromUtf8("presetButton"));
    presetButton->setStyleSheet(QString::fromUtf8(""));
    ButtonLayout->addWidget(presetButton);
    gridLayout->addLayout(ButtonLayout, 0, 4, 1, 1);

    Preset = new QVBoxLayout();
    Preset->setObjectName(QString::fromUtf8("Preset"));
    Preset->setContentsMargins(-1, -1, -1, 0);
    PresetLayout = new QVBoxLayout();
    PresetLayout->setObjectName(QString::fromUtf8("PresetLayout"));
    presetTableList = new PfasWidgetPFAPresetTableList(this, ui_mgr);
    presetTableList->setObjectName(QString::fromUtf8("presetTableList"));
    presetTableList->setStyleSheet(QString::fromUtf8(""));

    PresetLayout->addWidget(presetTableList);
    Preset->addLayout(PresetLayout);
    
    verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    Preset->addItem(verticalSpacer_2);

    Preset->setStretch(0, 4);
    Preset->setStretch(1, 5);

    gridLayout->addLayout(Preset, 0, 2, 1, 1);

    horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    gridLayout->addItem(horizontalSpacer_3, 0, 3, 1, 1);

    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 4);
    gridLayout->setColumnStretch(2, 9);
    gridLayout->setColumnStretch(3, 2);
    gridLayout->setColumnStretch(4, 1);
    gridLayout->setColumnStretch(5, 1);

    //this->setLayout(gridLayout);

}

void PfasWidgetPFAAblationPreset::OnCreate() {
    PfasUIContent::OnCreate();
    presetCatheter->OnCreate();
    presetTableList->OnCreate();
    presetButton->OnCreate();
}

void PfasWidgetPFAAblationPreset::OnEnter() {
    PfasUIContent::OnEnter();
    presetCatheter->OnEnter();
    presetTableList->OnEnter();
    presetButton->OnEnter();
}

