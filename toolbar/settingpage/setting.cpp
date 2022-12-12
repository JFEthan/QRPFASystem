#include "setting.h"
#include "ui_setting.h"
#include "QButtonGroup"

setting::setting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::setting)
{
    ui->setupUi(this);
    ui->settinglisthead->setStyleSheet("border:none;border-radius:20px;background-color:rgb(197, 228, 243)");
    ui->settingVolume->setStyleSheet("border:none;border-radius:20px;background-color:rgb(197, 228, 243)");
    ui->settingBrightness->setStyleSheet("border:none;border-radius:20px;background-color:rgb(197, 228, 243)");
    ui->settingVolumeSlider->setStyleSheet("border:none;border-radius:2px;");

    connect(ui->btnVolumeUp,SIGNAL(clicked()),this,SLOT(OnbtnVolumeUpClicked()));
    connect(ui->btnVolumeDown,SIGNAL(clicked()),this,SLOT(OnbtnVolumeDownClicked()));
    connect(ui->settingVolumeSlider,SIGNAL(valueChanged()),this,SLOT(OnVolumeValueChanged()));

    connect(ui->btnBrightnessUp,SIGNAL(clicked()),this,SLOT(OnbtnBrightnessUpClicked()));
    connect(ui->btnBrightnessDown,SIGNAL(clicked()),this,SLOT(OnbtnBrightnessDownClicked()));
    connect(ui->settingBrightnessSlider,SIGNAL(valueChanged()),this,SLOT(OnBrightnessValueChanged()));

    QButtonGroup* brightness_radiobutton;
    brightness_radiobutton = new QButtonGroup();
    brightness_radiobutton->setExclusive(true);
    brightness_radiobutton->addButton(ui->radioButton_automatic);
    brightness_radiobutton->addButton(ui->radioButton_manual);
}

setting::~setting()
{
    delete ui;
}

void setting::OnbtnVolumeUpClicked()
{
    if(ui->settingVolumeSlider->value() < ui->settingVolumeSlider->maximum())
        ui->settingVolumeSlider->setValue(ui->settingVolumeSlider->value() + 1);
}
void setting::OnbtnVolumeDownClicked()
{
   if(ui->settingVolumeSlider->value() > 0)
       ui->settingVolumeSlider->setValue(ui->settingVolumeSlider->value() - 1);
}

void setting::OnbtnBrightnessUpClicked()
{
    if(ui->settingBrightnessSlider->value() < ui->settingBrightnessSlider->maximum())
        ui->settingBrightnessSlider->setValue(ui->settingBrightnessSlider->value() + 1);
}
void setting::OnbtnBrightnessDownClicked()
{
   if(ui->settingBrightnessSlider->value() > 0)
       ui->settingBrightnessSlider->setValue(ui->settingBrightnessSlider->value() - 1);
}
