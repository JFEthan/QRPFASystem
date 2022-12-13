#include "settingpagebuttons.h"
#include "ui_settingpagebuttons.h"

settingpagebuttons::settingpagebuttons(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::settingpagebuttons)
{
    ui->setupUi(this);
}

settingpagebuttons::~settingpagebuttons()
{
    delete ui;
}

