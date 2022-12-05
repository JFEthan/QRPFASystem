#include "rpfasummarycontent.h"
#include "ui_rpfasummarycontent.h"

RPFASummaryContent::RPFASummaryContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RPFASummaryContent)
{
    ui->setupUi(this);
}

RPFASummaryContent::~RPFASummaryContent()
{
    delete ui;
}
