#include "catheterpreset.h"
#include "ui_catheterpreset.h"
#include "./toolbar/catheterpresetWidget/AblationPreset.h"

catheterpreset::catheterpreset(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::catheterpreset)
{
    ui->setupUi(this);
    preset_widget = new AblationPreset(this);
    ui->catheterpresetWidget->addWidget(preset_widget);
    ui->catheterpresetWidget->setCurrentWidget(preset_widget);
}

catheterpreset::~catheterpreset()
{
    delete ui;
}

void catheterpreset::InitLauout()
{


}
void catheterpreset::Lauout()
{

}
