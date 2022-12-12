#include "pfactionbar.h"
#include "ui_pfactionbar.h"

PFActionbar::PFActionbar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PFActionbar)
{
    ui->setupUi(this);
}

PFActionbar::~PFActionbar()
{
    delete ui;
}
