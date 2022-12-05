#include "rfactionbar.h"
#include "ui_rfactionbar.h"

RFActionbar::RFActionbar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RFActionbar)
{
    ui->setupUi(this);
}

RFActionbar::~RFActionbar()
{
    delete ui;
}
