#include "puncturebar.h"
#include "ui_puncturebar.h"

Puncturebar::Puncturebar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Puncturebar)
{
    ui->setupUi(this);
}

Puncturebar::~Puncturebar()
{
    delete ui;
}
