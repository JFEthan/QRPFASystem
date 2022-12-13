#include "ablationpage.h"
#include "ui_ablationpage.h"
#include <QPainter>
#include <QStyleOption>

#include "pfablationcontent.h"
#include "rfablationcontent.h"
#include "rfpuncturecontent.h"
#include "rpfasummarycontent.h"

AblationPage::AblationPage(QWidget *parent) :
    PfasPage(parent),
    ui(new Ui::AblationPage)
{
    ui->setupUi(this);

    OnCreate();
}

AblationPage::~AblationPage()
{
    delete ui;
    OnDestroy();
}
void AblationPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
}
void AblationPage::OnCreate()
{
    pf_ablation_content = new PfAblationContent(this);
    rf_ablation_content = new RfAblationContent(this);
    rf_puncturen_content = new RfPunctureContent(this);
    rpfa_summary_comtent = new RPFASummaryContent(this);
    ui->stackedWidget->addWidget(pf_ablation_content);
    ui->stackedWidget->addWidget(rf_ablation_content);
    ui->stackedWidget->addWidget(rf_puncturen_content);
    ui->stackedWidget->addWidget(rpfa_summary_comtent);

    ui->stackedWidget->setCurrentWidget(pf_ablation_content);


    connect(ui->pageSwitchBar, SIGNAL(MainButtonClicked()), this, SLOT(OnMainButtonClicked()));
    connect(ui->pageSwitchBar, SIGNAL(PFAblationButtonClicked()), this, SLOT(OnPFAblationButtonClicked()));
    connect(ui->pageSwitchBar, SIGNAL(RFAblationButtonClicked()), this, SLOT(OnRFAblationButtonClicked()));
    connect(ui->pageSwitchBar, SIGNAL(PFPunctureButtonClicked()), this, SLOT(OnPFPunctureButtonClicked()));
    connect(ui->pageSwitchBar, SIGNAL(PostButtonClicked()), this, SLOT(OnPostButtonClicked()));

}
void AblationPage::OnDestroy()
{
    if(pf_ablation_content != nullptr)
    {
        delete pf_ablation_content;
        pf_ablation_content = nullptr;
    }
    if(rf_ablation_content != nullptr)
    {
        delete rf_ablation_content;
        rf_ablation_content = nullptr;
    }
    if(rf_puncturen_content != nullptr)
    {
        delete rf_puncturen_content;
        rf_puncturen_content = nullptr;
    }
    if(rpfa_summary_comtent != nullptr)
    {
        delete rpfa_summary_comtent;
        rpfa_summary_comtent = nullptr;
    }
}
void AblationPage::OnInitialize()
{

}
void AblationPage::OnUnInitialize()
{

}
void AblationPage::OnEnter()
{

}
void AblationPage::OnExit()
{

}

void AblationPage::OnMainButtonClicked()
{
//    ui->pageSwitchBar->postButton->setChecked(true);
//    ui->stackedWidget->setCurrentWidget(summary_content);
}
void AblationPage::OnPFAblationButtonClicked()
{
     ui->stackedWidget->setCurrentWidget(pf_ablation_content);
}
void AblationPage::OnRFAblationButtonClicked()
{
     ui->stackedWidget->setCurrentWidget(rf_ablation_content);
}
void AblationPage::OnPFPunctureButtonClicked()
{
     ui->stackedWidget->setCurrentWidget(rf_puncturen_content);
}
void AblationPage::OnPostButtonClicked()
{
     ui->stackedWidget->setCurrentWidget(rpfa_summary_comtent);
}
