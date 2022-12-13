#include "catheterpresetsetting.h"
#include "ui_catheterpresetsetting.h"

catheterpresetsetting::catheterpresetsetting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::catheterpresetsetting)
{
    ui->setupUi(this);
}

catheterpresetsetting::~catheterpresetsetting()
{
    delete ui;
}

