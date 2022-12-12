#include "rfablationcontent.h"
#include "ui_rfablationcontent.h"
#include "./toolbar/accuCustomplot/customwidget.h"
RfAblationContent::RfAblationContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RfAblationContent)
{
    ui->setupUi(this);
    custom_widget = new CustomWidget(this);
    ui->RFAblationwidget->addWidget(custom_widget);
    ui->RFAblationwidget->setCurrentWidget(custom_widget);
}

RfAblationContent::~RfAblationContent()
{
    delete ui;
}

