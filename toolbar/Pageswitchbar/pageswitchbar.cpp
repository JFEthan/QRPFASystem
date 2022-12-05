#include "pageswitchbar.h"
#include "ui_pageswitchbar.h"

PageSwitchBar::PageSwitchBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageSwitchBar)
{
    ui->setupUi(this);

 connect(ui->btn_main_menu, SIGNAL(clicked()), this, SIGNAL(MainButtonClicked()));
 connect(ui->btn_pf_ablation, SIGNAL(clicked()), this, SIGNAL(PFAblationButtonClicked()));
 connect(ui->btn_rf_ablation, SIGNAL(clicked()), this, SIGNAL(RFAblationButtonClicked()));
 connect(ui->btn_rf_puncture, SIGNAL(clicked()), this, SIGNAL(PFPunctureButtonClicked()));
 connect(ui->btn_post, SIGNAL(clicked()), this, SIGNAL(PostButtonClicked()));

}

PageSwitchBar::~PageSwitchBar()
{
    delete ui;
}
