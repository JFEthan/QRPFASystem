#include "pfintegratedabar.h"
#include "ui_pfintegratedabar.h"

PFIntegratedaBar::PFIntegratedaBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PFIntegratedaBar)
{
    ui->setupUi(this);
}

PFIntegratedaBar::~PFIntegratedaBar()
{
    delete ui;
}
