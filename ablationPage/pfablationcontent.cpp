#include "pfablationcontent.h"
#include "ui_pfablationcontent.h"

PfAblationContent::PfAblationContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PfAblationContent)
{
    ui->setupUi(this);
}

PfAblationContent::~PfAblationContent()
{
    delete ui;
}
