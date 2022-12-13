#include "settingpage.h"
#include "ui_settingpage.h"

settingpage::settingpage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::settingpage)
{
    ui->setupUi(this);
    ui->settingPage1->setStyleSheet("border:none;border-radius:20px;background-color:rgb(255, 236, 212)");
    ui->settingPage2->setStyleSheet("border:none;border-radius:20px;background-color:rgb(195, 213, 176)");
    ui->settingPage3->setStyleSheet("border:none;border-radius:20px;background-color:rgb(255, 162, 162)");
    ui->settingPage4->setStyleSheet("border:none;border-radius:20px;background-color:rgb(245, 255, 255)");
}

settingpage::~settingpage()
{
    delete ui;
}

