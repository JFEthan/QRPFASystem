#include "rfpuncturecontent.h"
#include "ui_rfpuncturecontent.h"

RfPunctureContent::RfPunctureContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RfPunctureContent)
{
    ui->setupUi(this);
}

RfPunctureContent::~RfPunctureContent()
{
    delete ui;
}
