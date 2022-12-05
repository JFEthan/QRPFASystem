#include "rfablationcontent.h"
#include "ui_rfablationcontent.h"

RfAblationContent::RfAblationContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RfAblationContent)
{
    ui->setupUi(this);
}

RfAblationContent::~RfAblationContent()
{
    delete ui;
}
